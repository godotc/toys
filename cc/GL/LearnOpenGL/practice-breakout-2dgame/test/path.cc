#include <cstdlib>
#include <filesystem>
#include <gtest/gtest.h>
#include <iostream>
#include <ostream>
#include <unistd.h>

using namespace std;


TEST(DISABLED_TestPath, should_print_the_path)
{

    char bf[256]{};

    getcwd(bf, 256);

    std::cout << "getcwd:\t ";
    std::cout << bf << "\n";

    std::cout << "pwd:\t ";
    std::flush(std::cout);
    system("pwd");

    readlink("/proc/self/exe", bf, 256);
    std::cout << "readlink:\t ";
    std::cout << bf << "\n";

    std::cout << "filesystem:\t ";
    std::cout << std::filesystem::current_path() << '\n';

    std::cout << __FILE_NAME__ << '\n'
              << __FILE__ << '\n';


    std::cout << PROJECT_ROOT_DIR << endl;
}
