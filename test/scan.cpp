#include <sf/sformat.hpp>

using namespace sf;
using namespace std;

int main(int argc, char** argv)
{
    if (argc > 2)
    {
        int a, b;
        ostringstream oss;
        auto pos = sscan(argv[1], "{0}{1}", a, b);
        if (pos != decltype(pos)(-1))
        {
            print(oss, argv[1] + pos);
        }
        else
        {
            print(oss, "{0} + {1} = {2}", a, b, a + b);
        }
        if (oss.str() == argv[2])
        {
            print("Success.\n");
        }
    }
    return 0;
}
