#include <sf/sformat.hpp>
#include <sf/ansi.hpp>

using namespace sf;
using namespace std;

int main()
{
    string s = sprint("{0}, {1}!\n", make_color_arg("Hello", yellow), make_color_arg("world", bright_cyan, blue));
    if (s == "\033[33;49mHello\033[0m, \033[96;44mworld\033[0m!\n")
    {
        print("Success.\n");
    }
    return 0;
}
