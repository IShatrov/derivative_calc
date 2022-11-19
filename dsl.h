#define NEW_NODE (new_node = node_ctor(tree))

#define ADD_L(new_node) node->l_child = new_node

#define ADD_R(new_node) node->r_child = new_node

#define LEFT(node) ((node)->l_child)

#define RIGHT(node) ((node)->r_child)

#define DIFF(target) diff(tree, NEW_NODE, target)

#define COPY(src) subtree_cpy(tree, NEW_NODE, src)

#define PRINT(arg) tex_print_math(tex, arg)

#define FIND(node, var) tree_find(node, var)

#define NEW_OP_NODE(oper)              \
    NEW_NODE;                          \
    new_node->type = NODE_OP;          \
    new_node->op = oper;               \

#define NEW_NUM_NODE(arg)              \
    NEW_NODE;                          \
    new_node->type = NODE_NUM;         \
    new_node->val = arg                \

