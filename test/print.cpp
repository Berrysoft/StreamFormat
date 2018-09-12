#include <sf/sformat.hpp>

using namespace sf;

int main()
{
    print("{0}", '\n');
    print("0x{0:x8}\n", 4276215469);//0xfee1dead
    print(L"{0}\n", L"Hello, world!");
    std::cout << sprint("{0}{{{1}}}{0}\n", "123", static_cast<const char*>("321"));
    return 0;
}
