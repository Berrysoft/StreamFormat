#include <sf/sformat.hpp>

using namespace sf;
using namespace std;

int main()
{
    ostringstream oss;
    print(oss, "Test\n");
    print(oss, "{0}", '\n');
    print(oss, "0x{0:x8}\n", 4276215469);//0xfee1dead
    print(oss, "{0}\n", "Hello, world!");
    print(oss, "{0:b}\n", true);
    oss << sprint("{0}{{{1}}}{0}\n", "123", static_cast<const char*>("321"));
    if (oss.str() == "Test\n\n0xfee1dead\nHello, world!\ntrue\n123{321}123\n")
    {
        print("Success.\n");
    }
    return 0;
}
