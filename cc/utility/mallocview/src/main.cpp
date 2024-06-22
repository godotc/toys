#include "ut.hpp"
#include <climits>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <ios>
#include <iostream>
#include <map>
#include <new>
#include <ostream>
#include <string>


#if defined(__GNUC__)
    #include <execinfo.h>
#endif

#if defined(__GNUC__)
std::string address_to_symbol(void *addr)
{
    char **strings = backtrace_symbols(&addr, 1);
    if (strings == nullptr) {
        return "";
    }

    std::string ret = strings[0];
    free(strings);
    return ret;

    size_t pos = ret.find("( ");
    if (pos == std::string::npos) {
        return ret;
    }
    return ret.substr(pos + 1, ret.size() - pos - 2);
}
#endif

struct AllocInfo {
    void  *ptr;
    size_t size;
    void  *caller;
};
std::ostream &operator<<(std::ostream &os, AllocInfo &info)
{
    os << "\n{"
       << "ptr = " << info.ptr << ", "
       << "size = " << info.size << ", "
       << "\ncaller = [" << address_to_symbol(info.caller).c_str() << "], "
       << "}\n";
    return os;
}

std::map<void *, AllocInfo> allocated;

bool enable = false;


void *operator new(size_t size)
{
    bool was_enable = enable;
    enable          = false;
    void *ptr       = malloc(size);
    if (was_enable)
    {
        void *caller = __builtin_return_address(0);

        if (ptr) {
            debug(), "new: ptr = ", ptr, ", size = ", size;
            // map insert also will call new
            allocated[ptr] = {ptr, size, caller};
        }
    }
    enable = was_enable;
    if (nullptr == ptr) {
        throw std::bad_alloc();
    }
    return ptr;
}

void operator delete(void *ptr) noexcept
{
    bool was_enable = enable;
    enable          = false;
    if (was_enable) {
        if (ptr) {
            debug(), "delete: ptr = ", ptr;
            allocated.erase(ptr);
        }
    }
    free(ptr);
    enable = was_enable;
}


void abc()
{
    auto p2 = new short;
    // delete p2;
}


struct AloocateIndicator {
} AI;

int main(int argc, char **argv)
{
    enable = true;

    auto p1 = new int;
    delete p1;
    abc();

    for (auto [ptr, info] : allocated) {
        debug(), "Memory leak: ", info;
    }
    return 0;
}
