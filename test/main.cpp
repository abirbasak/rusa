#include <azuik/tool/unit_test.hpp>

#include <vector>
#include <iostream>

AZUIK_TEST_SUIT(command_line)
{
    using azuik::tool::command_line;
    AZUIK_TEST_CASE(arg_check)
    {
        std::vector<char const*> args{"--test=true", "--include=abc", "build"};
        command_line cmd{(int)args.size(), args.data()};
        AZUIK_TEST(cmd.contains("--test") == true);
        AZUIK_TEST(cmd.contains("--include") == true);
        AZUIK_TEST(cmd.contains("build") == true);
        AZUIK_TEST(cmd.contains("--link") == false);
        AZUIK_TEST(cmd.contains("--link") == false);
        std::cout << cmd.value("build").data() << std::endl;
        AZUIK_TEST(std::strcmp(cmd.value("--include").data(), "abc") == 0);
    }
}
int main(int argc, char* argv[])
{
    azuik::tool::run_test(argc, argv);
}
