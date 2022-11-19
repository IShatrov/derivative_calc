#include "derivative_calc.h"

#define TEX_PRINT(...) fprintf(tex, __VA_ARGS__)

void out_latex(const my_tree *tree)
{
    assert(tree);

    FILE *tex = fopen("out.tex", "w");
    assert(tex);

    TEX_PRINT("\\documentclass[a4paper, 12pt]{article}\n\n"
              "\\usepackage{amsmath,amsfonts,amssymb,amsthm,mathtools}\n\n"
              "\\begin{document}\n\n");

    TEX_PRINT("\\[");
    tex_print_math(tex, tree->root);
    TEX_PRINT("\\]\n\n");

    TEX_PRINT("\\end{document}");

    fclose(tex);

    system("out.tex");

    return;
}

#define DEF_BIN_OP(name, symbol, derivative, out, ...)  \
case OP_##name:                                         \
    out;                                                \
    break;

#define DEF_UN_OP(name, symbol, derivative, out, ...)   \
case OP_##name:                                         \
    out;                                                \
    break;
void tex_print_math(FILE *tex, const tree_node *node)
{
    assert(tex);
    assert(node);

    switch(node->type)
    {
        case NODE_NUM:
            TEX_PRINT("%.3lf", node->val);
            break;
        case NODE_VAR:
            TEX_PRINT("%c", node->var);
            break;
        case NODE_OP:
            switch(node->op)
            {
                #include "binary_operators.h"
                #include "unary_operators.h"
                default:
                    printf("ERROR: unknown operator\n");
            }
            break;
        default:
            printf("ERROR: invalid node content\n");
    }

    return;
}
#undef DEF_BIN_OP
#undef DEF_UN_OP
