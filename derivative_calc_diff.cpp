#include "derivative_calc.h"

my_tree get_derivative(const my_tree *init_tree, FILE *degenerator)
{
    assert(init_tree);

    my_tree tree;
    tree_ctor(&tree, DEFAULT_TREE_CAP);

    diff(&tree, tree.root, init_tree->root, degenerator);

    return tree;
}

#define DEF_BIN_OP(name, symbol, derivative, ...)   \
{                                                   \
    case OP_##name:                                 \
        derivative;                                 \
        break;                                      \
}

#define DEF_UN_OP(name, symbol, derivative, ...)    \
{                                                   \
    case OP_##name:                                 \
        derivative;                                 \
        break;                                      \
}
tree_node* diff(my_tree *tree, tree_node *node, const tree_node *target, FILE *degenerator)
{
    assert(tree);
    assert(node);
    assert(target);

    if(degenerator) degenerator_print(degenerator, target);

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
                    ERR_N_OP(target->op);
            }
            break;
        default:
            ERR_TYPE(target->type);
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

int are_doubles_equal(double n1, double n2)
{
    return (fabs(n1 - n2) <= EPSILON);
}

char simplify(my_tree *tree, tree_node *node)
{
    assert(tree);
    assert(node);

    //printf("simplify entered %d\n", node->op);

    if(TYPE(node) != NODE_OP) return 0;

    char has_changed = 0, ans = 0;

    if(LEFT(node) && TYPE(LEFT(node)) == NODE_OP)
    {
        do
        {
            has_changed = simplify(tree, LEFT(node));
            //printf("%d", node->op);
            if(has_changed) ans = 1;
        } while(has_changed);
    }

    has_changed = 0;

    if(RIGHT(node) && TYPE(RIGHT(node)) == NODE_OP)
    {
        do
        {
            has_changed = simplify(tree, RIGHT(node));

            if(has_changed) ans = 1;
        } while(has_changed);
    }

    ans += fold_consts(node);
    ans += fold_neutral(tree, node);

    return ans;
}

char fold_consts(tree_node *node)
{
    assert(node);

    if(TYPE(node) != NODE_OP) return 0;

    if(LEFT(node) && TYPE(LEFT(node)) != NODE_NUM) return 0;
    if(RIGHT(node) && TYPE(RIGHT(node)) != NODE_NUM) return 0;

    switch(node->op)
    {
        case(OP_ADD):
            VAL(node) = VAL(LEFT(node)) + VAL(RIGHT(node));
            break;
        case(OP_SUB):
            VAL(node) = VAL(LEFT(node)) - VAL(RIGHT(node));
            break;
        case(OP_MULT):
            VAL(node) = VAL(LEFT(node)) * VAL(RIGHT(node));
            break;
        case(OP_DIV):
            VAL(node) = VAL(LEFT(node)) / VAL(RIGHT(node));
            break;
        case(OP_POW):
            VAL(node) = pow(VAL(LEFT(node)), VAL(RIGHT(node)));
            break;
        case(OP_SIN):
            VAL(node) = sin(VAL(RIGHT(node)));
            break;
        case(OP_COS):
            VAL(node) = cos(VAL(RIGHT(node)));
            break;
        case(OP_LN):
            VAL(node) = log(VAL(RIGHT(node)));
            break;
        default:
            return 0;
    }

    TYPE(node) = NODE_NUM;
    node->priority = PRIO_NUM;
    CUT(node);

    return 1;
}

char fold_neutral(my_tree *tree, tree_node *node)
{
    assert(tree);
    assert(node);

    if(TYPE(node) != NODE_OP) return 0;

    if(!LEFT(node) || !RIGHT(node)) return 0;

    if(TYPE(LEFT(node)) == NODE_OP && TYPE(RIGHT(node)) == NODE_OP) return 0;

   // tree_node *new_node;

    switch(node->op)
    {
        case OP_ADD:
            if(IS_NUM(LEFT(node)) && EQUALS(0, VAL(LEFT(node))))
            {
                NODE_COPY(RIGHT(node));
                return 1;
            }
            else if(IS_NUM(RIGHT(node)) && EQUALS(0, VAL(RIGHT(node))))
            {
                NODE_COPY(LEFT(node));
                return 1;
            }
            break;
        case OP_SUB:
            if(IS_NUM(RIGHT(node)) && EQUALS(0, VAL(RIGHT(node))))
            {
                NODE_COPY(LEFT(node));
                return 1;
            }
            break;
        case OP_MULT:
            if(IS_NUM(LEFT(node)) && EQUALS(0, VAL(LEFT(node))))
            {
                FILL_NODE(0);
                return 1;
            }
            else if(IS_NUM(RIGHT(node)) && EQUALS(0, VAL(RIGHT(node))))
            {
                FILL_NODE(0);
                return 1;
            }
            else if(IS_NUM(LEFT(node)) && EQUALS(1, VAL(LEFT(node))))
            {
                NODE_COPY(RIGHT(node));
                return 1;
            }
            else if(IS_NUM(RIGHT(node)) && EQUALS(1, VAL(RIGHT(node))))
            {
                NODE_COPY(LEFT(node));
                return 1;
            }
            break;
        case OP_DIV:
            if(IS_NUM(RIGHT(node)) && EQUALS(1, VAL(RIGHT(node))))
            {
                NODE_COPY(LEFT(node));
                return 1;
            }
            else if(IS_NUM(LEFT(node)) && EQUALS(0, VAL(LEFT(node))))
            {
                FILL_NODE(0);
                return 1;
            }
            break;
        case OP_POW:
            if(IS_NUM(RIGHT(node)) && EQUALS(1, VAL(RIGHT(node))))
            {
                NODE_COPY(LEFT(node));
                return 1;
            }
            else if(IS_NUM(RIGHT(node)) && EQUALS(0, VAL(RIGHT(node))))
            {
                FILL_NODE(1);
                return 1;
            }
            else if(IS_NUM(LEFT(node)) && EQUALS(0, VAL(LEFT(node))))
            {
                FILL_NODE(0);
                return 1;
            }
            else if(IS_NUM(LEFT(node)) && EQUALS(1, VAL(LEFT(node))))
            {
                FILL_NODE(1);
                return 1;
            }
            break;
        default:
            return 0;
    }

    return 0;
}
