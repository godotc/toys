#include <cctype>
#include <cerrno>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <ios>
#include <iostream>
#include <istream>
#include <iterator>
#include <ostream>
#include <random>
#include <ranges>
#include <string>
#include <sys/types.h>
#include <type_traits>
#include <vector>

#include <cxxopts.hpp>

using namespace std;



// void hexdump(std::string const &s)
//{
//     for (uint32_t addr = 0; addr < s.size(); ++addr) {
//         char c = s[addr];
//         std::cout << std::hex << std::setw(2) << std::setfill('0')
//                   << std::right // text align to the right
//                   << (int)c;
//         std::cout << ((addr % 8) == 7 ? '\n' : ' ');
//     }
//     std::cout << std::endl;
// }


template <std::ranges::input_range Range> // requires {ranges::range<typename T>}
void hexdump(Range const &sequence, std::size_t width = 16)
{
    using type = // typename Range::value_type;
                 // std::decay_t<decltype(sequence[0])>;
        typename std::ranges::range_value_t<decltype(sequence)>;

    const int   byte_width = 2;
    std::size_t addr       = 0;


    std::vector<char> cached;

    for (auto chunk : sequence | std::ranges::views::chunk(width))
    {

        // address
        std::cout << std::hex << std::setw(8) << std::setfill('0') << std::right << addr << ' ';

        // bin byte
        for (auto c : chunk)
        {
            std::cout << ' '
                      << std::hex
                      << std::setw(byte_width) // the width of text
                      << std::setfill('0')
                      << std::right // text align to the right
                                    // << (int)c; if c <0 will be fffffxxx , fill wiht the signed
                      << (unsigned long long)(std::make_unsigned_t<type>)c;


            ++addr;

            if constexpr (sizeof(type) == sizeof(char) && std::is_convertible_v<type, char>) {
                cached.push_back((char)c);
            }
        }

        // text dump
        if constexpr (sizeof(type) == sizeof(char) && std::is_convertible_v<type, char>)
        // (std::is_same_v<type, char> || std::is_same_v<type, int8_t>)
        {
            std::size_t mod = addr % width;
            // printf("\n%d\n", mod);

            if (mod != 0) {
                for (std::size_t i = 0; i < (width - mod) * (byte_width + 1); ++i) {
                    std::cout << ' ';
                }
            }

            std::cout << "\t|";
            {
                // print char text
                for (auto c : cached)
                {
                    if (!std::isprint(c)) {
                        c = '.'; // \n\t....
                    }
                    std::cout << c;
                }

                if (mod != 0) {
                    for (int i = 0; i < (width - mod); ++i) {
                        std::cout << ' ';
                    }
                }
            }
            std::cout << "|";

            cached.clear();
        }


        std ::cout << '\n';
    }
}


template <typename It>
struct Range {
    It b, e;
    It begin() const { return b; }
    It end() const { return e; }
};
template <typename It>
Range(It, It) -> Range<It>;


struct IStreamRange : public Range<std::istreambuf_iterator<char>> {

    using It = std::istreambuf_iterator<char>;

    It b, e;

    IStreamRange(std::istream &is)
        : b(std::istreambuf_iterator<char>(is)),
          e(std::istreambuf_iterator<char>())
    {
    }

    It begin() const { return b; }
    It end() const { return e; }
};


int dump_file(std::string const &filepath)
{
    std::ifstream ifs(filepath);
    if (!ifs.good()) {
        std::cerr << std::strerror(errno) << "(" << errno << ")"
                  << ": " << filepath << '\n';
        return 1;
    }
    std::istream_iterator<char> b{ifs}, e{};

    hexdump(Range{b, e});
    return 0;
}


void test()
{

    // std::string          str = "hello world!";
    std::vector<uint8_t> vec;

    std::mt19937                           gen(3);
    std::uniform_int_distribution<uint8_t> distra;
    for (int i = 0; i < 103; ++i) {
        auto r = distra(gen);
        //    str.push_back();
        vec.push_back(r);
    }

    // hexdump(str);
    hexdump(vec);
}

int main(int argc, char **argv)
{
    // test();

    cxxopts::Options options("hexdump", "---");
    options.add_options()("f,file", "Input File", cxxopts::value<std::string>()->default_value("-"));
    options.add_options()("h,help", "print help");

    auto args = options.parse(argc, argv);

    if (args.count("help")) {
        std::cout << options.help() << "\n";
        return 0;
    }


    auto p = args["file"].as<std::string>();

    if (p == "-") {
        hexdump(IStreamRange(std::cin));
        return 0;
    }

    else {
        auto path = std::filesystem::path(p);
        // printf("path: %s\n", path.string().c_str());
        return dump_file(path);
    }


    return 0;
}
