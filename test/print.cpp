#include <sf/sformat.hpp>

using namespace sf;
using namespace std;

int main()
{
    ostringstream oss;
    print(oss, "Test\n");
    char c = '\n';
    print(oss, c);
    println(oss, "{:x8,s}", 4276215469);
    print(oss, "{}\n", "Hello, world!");
    print(oss, "{0:b,l8}", true);
    println(oss, "");
    oss << sprint("{0}{{{1}}}{0}\n", "123", "321");
    oss << sprint("{}", 123.456) << endl;
    if (oss.str() == "Test\n\n0xfee1dead\nHello, world!\ntrue    \n123{321}123\n123.456\n")
    {
        println("Success.");
    }
    return 0;
}
