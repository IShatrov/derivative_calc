#include "derivative_calc.h"

#define SKIP_SPACES(text) while(isspace(*text)) text++

const char *s = NULL;

my_tree get_grammar(const char **str)
{
    SKIP_SPACES((*str));

    my_tree ans;
    tree_ctor(&ans, 10);

    ans.root = get_expr(&ans, str);

    SKIP_SPACES((*str));

    assert(**str == '\0');

    return ans;
}

tree_node* get_num(my_tree *tree, const char **str)
{
    SKIP_SPACES((*str));

    const char *s_old = *str;
    char *end = NULL;

    double val = strtod(*str, &end);
    *str = end;

    if(*str != s_old)
    {
        tree_node *new_node;
        NEW_NUM_NODE(val);

        SKIP_SPACES((*str));

        return new_node;
    }
    else
    {
        tree_node *new_node;
        NEW_NODE;

        new_node->type = NODE_VAR;
        new_node->var = **str;
        (*str)++;
        new_node->priority = PRIO_VAR;

        SKIP_SPACES((*str));

        return new_node;
    }

    ERR_UNDEFINED;
    return NULL;
}

tree_node* get_expr(my_tree *tree, const char **str)
{
    tree_node* ans = get_turn(tree, str);

    SKIP_SPACES((*str));

    tree_node* new_node;

    if(**str == '+' || **str == '-')
    {
        char op = **str;
        (*str)++;

        tree_node *r_expr = get_expr(tree, str);

        if(op == '+')
        {
            NEW_OP_NODE(ADD);

            LEFT(new_node) = ans;
            RIGHT(new_node) = r_expr;
            ans = new_node;
        }
        else
        {
            NEW_OP_NODE(SUB);

            LEFT(new_node) = ans;
            RIGHT(new_node) = r_expr;
            ans = new_node;
        }
    }

    SKIP_SPACES((*str));

    return ans;
}

tree_node* get_turn(my_tree* tree, const char **str)
{
    tree_node* ans = get_pow(tree, str);

    SKIP_SPACES((*str));

    tree_node* new_node;

    if(**str == '*' || **str == '/')
    {
        char op = **str;
        (*str)++;

        tree_node *r_expr = get_turn(tree, str);

        if(op == '*')
        {
            NEW_OP_NODE(MULT);

            LEFT(new_node) = ans;
            RIGHT(new_node) = r_expr;
            ans = new_node;
        }
        else
        {
            NEW_OP_NODE(DIV);

            LEFT(new_node) = ans;
            RIGHT(new_node) = r_expr;
            ans = new_node;
        }
    }

    SKIP_SPACES((*str));

    return ans;
}

tree_node* get_prim(my_tree *tree, const char **str)
{
    tree_node *ans;

    SKIP_SPACES((*str));

    if(**str == '(')
    {
        (*str)++;

        ans = get_expr(tree, str);

        if(**str != ')')
        {
            ERR_MISSING_CLOSE_BRACKET;
            return NULL;
        }
        (*str)++;
    }
    else ans = get_unary(tree, str);

    SKIP_SPACES((*str));

    return ans;
}

tree_node* get_pow(my_tree *tree, const char **str)
{
    tree_node* ans = get_prim(tree, str);

    SKIP_SPACES((*str));

    tree_node* new_node;

    if(**str == '^')
    {
        (*str)++;

        tree_node *r_expr = get_prim(tree, str);

        NEW_OP_NODE(POW);

        LEFT(new_node) = ans;
        RIGHT(new_node) = r_expr;
        ans = new_node;
    }

    SKIP_SPACES((*str));

    return ans;
}

tree_node* get_unary(my_tree *tree, const char **str)
{
    tree_node* ans;

    SKIP_SPACES((*str));

    tree_node *new_node;

    if(!strncmp("sin", *str, strlen("sin")))
    {
        *str += strlen("sin");

        tree_node* arg = get_arg(tree, str);

        NEW_OP_NODE(SIN);
        ans = new_node;

        RIGHT(ans) = arg;
    }
    else if(!strncmp("cos", *str, strlen("cos")))
    {
        *str += strlen("cos");

        tree_node* arg = get_arg(tree, str);

        NEW_OP_NODE(COS);
        ans = new_node;

        RIGHT(ans) = arg;
    }
    else if(!strncmp("ln", *str, strlen("ln")))
    {
        *str += strlen("ln");

        tree_node* arg = get_arg(tree, str);

        NEW_OP_NODE(LN);
        ans = new_node;

        RIGHT(ans) = arg;
    }
    else ans = get_num(tree, str);

    SKIP_SPACES((*str));

    return ans;
}

tree_node* get_arg(my_tree *tree, const char **str)
{
    SKIP_SPACES((*str));

    assert(**str == '(');
    (*str)++;

    tree_node* ans = get_expr(tree, str);

    assert(**str == ')');
    (*str)++;

    SKIP_SPACES((*str));

    return ans;
}
