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
                node->priority = PRIO_NUM;
            }
            else
            {
                node->type = NODE_VAR;
                node->var = target->var;
                node->priority = PRIO_VAR;
            }
            break;
        case NODE_OP:
            tree_node *new_node;
            node->type = NODE_OP;
            switch(target->op)
            {
                #include "binary_operators.h"
                #include "unary_operators.h"
                default:
                printf("ERROR: unknown operator\n");
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
    dest->priority = src->priority;

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

char tree_find(tree_node *node, const char target)
{
    assert(node);

    if(node->type == NODE_VAR && node->var == target) return 1;

    if(node->l_child && tree_find(node->l_child, target)) return 1;

    if(node->r_child && tree_find(node->r_child, target)) return 1;

    return 0;
}
