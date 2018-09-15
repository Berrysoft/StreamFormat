#include <sf/format.hpp>
#include <sf/ansi.hpp>

using namespace sf;

int main()
{
    print("{0}, {1}!\n", make_color_arg("Hello", yellow), make_color_arg("world", bright_cyan, blue));
    return 0;
}
