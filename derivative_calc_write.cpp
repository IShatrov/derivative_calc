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
                    ERR_N_OP(node->op);
            }
            break;
        default:
            ERR_UNDEFINED;
    }

    return;
}
#undef DEF_BIN_OP
#undef DEF_UN_OP

#define DEF_BIN_OP(name, symbol, derivative, out, ...)  \
case OP_##name:                                         \
    out;                                                \
    break;

#define DEF_UN_OP(name, symbol, derivative, out, ...)   \
case OP_##name:                                         \
    out;                                                \
    break;

void tex_print_math_brackets(FILE *tex, const tree_node *node)
{
    assert(tex);
    assert(node);

    TEX_PRINT("(");

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
                    ERR_N_OP(node->op);
            }
            break;
        default:
            ERR_UNDEFINED;
    }

    TEX_PRINT(")");

    return;
}
#undef DEF_BIN_OP
#undef DEF_UN_OP

FILE* degenerator_ctor()
{
    FILE *tex = fopen("degenerator_out.tex", "w");
    assert(tex);

    TEX_PRINT("\\documentclass[a4paper, 12pt]{article}\n\n"
              "\\usepackage{amsmath,amsfonts,amssymb,amsthm,mathtools, breqn}\n\n"
              "\\begin{document}\n\n");

    return tex;
}

void degenerator_dtor(FILE *tex)
{
    assert(tex);

    TEX_PRINT("\\end{document}");

    fclose(tex);

    system("degenerator_out.tex");

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
void tex_print_derivative(FILE *tex, const tree_node *node)
{
    assert(tex);
    assert(node);

    TEX_PRINT("(");
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
                    ERR_N_OP(node->op);
            }
            break;
        default:
            ERR_UNDEFINED;
    }
    TEX_PRINT(")'");

    return;
}
#undef DEF_BIN_OP
#undef DEF_UN_OP

void degenerator_print(FILE *tex, const tree_node *node)
{
    assert(tex);
    assert(node);

    TEX_PRINT("\\begin{dmath}\n");

    tex_print_derivative(tex, node);
    TEX_PRINT("=");

    char is_l_func = 0, is_r_func = 0;

    switch(node->type)
    {
        case NODE_NUM:
            TEX_PRINT("0");
            break;
        case NODE_VAR:
            if(node->var == 'x')
            {
                TEX_PRINT("1");
            }
            else
            {
                TEX_PRINT("0");
            }
            break;
        case NODE_OP:
            switch(node->op)
            {
                    case OP_ADD:
                        tex_print_derivative(tex, LEFT(node));
                        TEX_PRINT("+");
                        tex_print_derivative(tex, RIGHT(node));
                        break;
                    case OP_SUB:
                        tex_print_derivative(tex, LEFT(node));
                        TEX_PRINT("-");
                        tex_print_derivative(tex, RIGHT(node));
                        break;
                    case OP_MULT:
                        tex_print_derivative(tex, LEFT(node));
                        TEX_PRINT("\\cdot ");
                        tex_print_math_brackets(tex, RIGHT(node));

                        TEX_PRINT("+");

                        tex_print_derivative(tex, RIGHT(node));
                        TEX_PRINT("\\cdot ");
                        tex_print_math_brackets(tex, LEFT(node));
                        break;
                    case OP_DIV:
                        TEX_PRINT("\\frac{");

                        tex_print_derivative(tex, LEFT(node));
                        TEX_PRINT("\\cdot ");
                        tex_print_math_brackets(tex, RIGHT(node));

                        TEX_PRINT("-");

                        tex_print_derivative(tex, RIGHT(node));
                        TEX_PRINT("\\cdot ");
                        tex_print_math_brackets(tex, LEFT(node));

                        TEX_PRINT("}{");

                        tex_print_math_brackets(tex, RIGHT(node));
                        TEX_PRINT("^2}");
                        break;
                    case OP_POW:
                        is_l_func = FIND(LEFT(node), 'x');
                        is_r_func = FIND(RIGHT(node), 'x');

                        if(is_l_func)
                        {
                            if(is_r_func) printf("I am not doing this\n");
                            else
                            {
                                tex_print_math(tex, RIGHT(node));
                                TEX_PRINT("\\cdot");

                                tex_print_math_brackets(tex, LEFT(node));
                                TEX_PRINT("^{");
                                tex_print_math(tex, RIGHT(node));
                                TEX_PRINT("-1.000}\\cdot ");

                                tex_print_derivative(tex, LEFT(node));
                            }
                        }
                        else
                        {
                            if(is_r_func)
                            {
                                TEX_PRINT("ln(");

                                tex_print_math(tex, LEFT(node));

                                TEX_PRINT(")\\cdot ");

                                tex_print_math_brackets(tex, node);
                                TEX_PRINT("\\cdot");
                                tex_print_derivative(tex, RIGHT(node));
                            }
                            else
                            {
                                TEX_PRINT("0");
                            }
                        }
                        break;
                    case OP_SIN:
                        TEX_PRINT("cos(");
                        tex_print_math(tex, RIGHT(node));
                        TEX_PRINT(")\\cdot ");

                        tex_print_derivative(tex, RIGHT(node));
                        break;
                    case OP_COS:
                        TEX_PRINT("-sin(");
                        tex_print_math(tex, RIGHT(node));
                        TEX_PRINT(")\\cdot ");

                        tex_print_derivative(tex, RIGHT(node));
                        break;
                    case OP_LN:
                        TEX_PRINT("\\frac{1}{");
                        tex_print_math(tex, RIGHT(node));
                        TEX_PRINT("}\\cdot ");

                        tex_print_derivative(tex, RIGHT(node));
                        break;
                default:
                    ERR_N_OP(node->op);
            }
            break;
        default:
            ERR_TYPE(node->type);
    }

    TEX_PRINT("\\end{dmath}\n\n");

    return;
}
