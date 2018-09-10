#include <sf/format.hpp>

using namespace sf;

int main()
{
    print("{0}", '\n');
    print("{0}\n", 3342);
    print(L"{0}\n", L"Hello, world!");
    print("{0}{{{1}}}{0}\n", "123", "321", "123");
    return 0;
}
