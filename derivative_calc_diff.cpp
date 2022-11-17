#include "derivative_calc.h"

my_tree get_derivative(const my_tree *init_tree)
{
    assert(init_tree);

    my_tree tree;
    tree_ctor(&tree, DEFAULT_TREE_CAP);

    diff(&tree, tree.root, init_tree->root);

    return tree;
}

#define DEF_BIN_OP(name, symbol, derivative, ...)   \
{                                                   \
    case OP_##name:                                 \
        derivative;                                 \
        break;                                      \
}

#define DEF_UN_OP(name, symbol, derivative, ...)   \
{                                                   \
    case OP_##name:                                 \
        derivative;                                 \
        break;                                      \
}
tree_node* diff(my_tree *tree, tree_node *node, const tree_node *target)
{
    assert(tree);
    assert(node);
    assert(target);

    switch(target->type)
    {
        case NODE_NUM:
            node->type = NODE_NUM;
            node->val = 0;
            break;
        case NODE_VAR:
            if(target->var == 'x')
            {
                node->type = NODE_NUM;
                node->val = 1;
            }
            else
            {
                node->type = NODE_VAR;
                node->var = target->var;
            }
            break;
        case NODE_OP:
            tree_node *new_node;
            node->type = NODE_OP;
            switch(target->op)
            {
                #include "binary_operators.h"
                #include "unary_operators.h"
            }
            break;
        default:
            printf("ERROR: invalid node type\n");
    }

    return node;
}
#undef DEF_BIN_OP
#undef DEF_UN_OP

tree_node* subtree_cpy(my_tree *tree, tree_node *dest, const tree_node *src)
{
    assert(src);
    assert(dest);

    dest->type = src->type;

    memcpy(&(dest->val), &(src->val), sizeof(double));

    tree_node *new_node;

    if(src->l_child)
    {
        NEW_NODE;

        subtree_cpy(tree, new_node, src->l_child);

        dest->l_child = new_node;
    }

    if(src->r_child)
    {
        NEW_NODE;

        subtree_cpy(tree, new_node, src->r_child);

        dest->r_child = new_node;
    }

    return dest;
}
