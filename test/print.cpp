#include <sf/sformat.hpp>

using namespace sf;
using namespace std;

int main()
{
    ostringstream oss;
    print(oss, "Test\n");
    print(oss, "{0}", '\n');
    println(oss, "0x{:x8,u}", 4276215469);
    print(oss, "{}\n", "Hello, world!");
    print(oss, "{0:b,l8}", true);
    println<char>(oss);
    oss << sprint("{0}{{{1}}}{0}\n", "123", static_cast<const char*>("321"));
    if (oss.str() == "Test\n\n0xFEE1DEAD\nHello, world!\ntrue    \n123{321}123\n")
    {
        println("Success.");
    }
    return 0;
}
