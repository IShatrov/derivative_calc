#include "derivative_calc.h"

int main(int argc, const char *argv[])
{
    const char *src_filename = "in.txt";
    if(argc > 1) src_filename = argv[1];

    my_tree initial_expression = parse_expression(src_filename);

    //tree_dump(&initial_expression);

    my_tree derivative = get_derivative(&initial_expression);

    //out_latex(&initial_expression);
    out_latex(&derivative);

    //tree_dump(&initial_expression);
    //tree_dump(&derivative);

    tree_dtor(&initial_expression);
    tree_dtor(&derivative);

    return 0;
}
