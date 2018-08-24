#include <azuik/test/unit_test.hpp>
#include <vector>
AZUIK_TEST_SUIT(simple)
{
    AZUIK_TEST_CASE(case1)
    {
        std::vector<int> v{10, 20, 30};
        AZUIK_TEST(v.size() == 3);
        AZUIK_TEST(v.empty() == false);
        AZUIK_TEST(v[0] == 10);
        AZUIK_TEST(v[1] == 20);
        AZUIK_TEST(v[2] == 30);
    }
}
int main(int argc, char *argv[])
{
    azuik::test::execute(argc, argv);
}
