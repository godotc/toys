#include "log.h"
#include <filesystem>
#include <fstream>
#include <stdexcept>
#include <stdio.h>
#include <string>

#if !_NDEBUG
#include <iostream>
#endif

#if _WIN32
#include <windows.h>
#endif


using path = std::filesystem::path;


static path GetCurrentExePath()
{
    // just not support the non-endlish path
    std::string path_str(4096, '\0');
#if _WIN32
    int len = GetModuleFileName(NULL, path_str.data(), path_str.size());
    if (len <= 0)
        return {};
#elif __LINUX__
    ssize_t n = readlink("/proc/self/exe", path_str.data(), path_str.size());
    if (n < 0)
        return {};
#elif __APPLE__
    int pid = GetCurrentProcessId();
    if (proc_pidpath(pid, path_str, path_str.size()) <= 0)
        return {};
#endif // ZEN_PLATFORM_WINDOWS
    return path_str;
}

inline namespace Path {


static std::string project_root_symbol = ".project-root-symbol";

static void SetProjectRootSymbol(std::string symbol) { project_root_symbol = symbol; }

// I will find `bin`'s  parent dir as the root dir;
static path FindDirectoryBySymbolFile(std::string target_symbol)
{
    auto is_dir_contain_root_symbol =
        [](std::filesystem::path &the_path, std::string &target_symbol) {
            for (auto &file : std::filesystem::directory_iterator(the_path)) {
                // std::cerr << "file entry: " << file.path().filename().string() << std::endl;
                if (file.path().filename() == target_symbol) {
                    return true;
                }
            }
            return false;
        };

    static path exe_path = GetCurrentExePath();
    LOG_DEBUG("executable path: {}", exe_path.filename());
    // auto        status   = std::filesystem::status(exe_path);

    if (!exe_path.empty() &&
#if _WIN32
        exe_path.extension().compare(".exe") && // not work on linux
#endif
        exe_path.has_parent_path())

    {
        LOG_DEBUG("start iterating parent...");
        path directory = exe_path;
        while (directory.has_parent_path())
        {
            directory = directory.parent_path();
            if (is_dir_contain_root_symbol(directory, target_symbol)) {
                return directory;
            }
        };
    }

    throw std::runtime_error("The direcory mark by symbol file not found! Symbol file: " + target_symbol);
}

static path ProjectRoot()
{
    return FindDirectoryBySymbolFile(project_root_symbol);
}


struct FPath {
    FPath(std::string path)
    {
        m_path = ProjectRoot().c_str() + path;
    }

    operator std::string() { return m_path; }
    operator char const *() { return m_path.c_str(); }

    std::string m_path;
};

}; // namespace Path
   //


using Path::FPath;
