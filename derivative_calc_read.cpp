#include "derivative_calc.h"

#define SKIP_SPACES(text) while(isspace(*text)) text++
#define SKIP_NUM(text) while(isdigit(*text) || (*text) == '.') text++

size_t get_file_size(FILE *stream)
{
    assert(stream != NULL);

    size_t ans = 0;

    assert(!fseek(stream, 0, SEEK_END));   //fseek returns zero upon success and non-zero otherwise
    ans = ftell(stream);
    assert(!ferror(stream));

    assert(!fseek(stream, 0, SEEK_SET));

    return ans;
}

char* read_text(FILE *stream)
{
    assert(stream != NULL);

    size_t len = get_file_size(stream);
    assert(len != 0);

    char* text = (char*)calloc(len + 1, sizeof(char)); //+1 for null terminator
    assert(text != NULL);

    fread(text, sizeof(char), len, stream);
    assert(!ferror(stream));

    return text;
}

my_tree parse_expression(const char *src_filename)
{
    assert(src_filename);

    FILE *src = fopen(src_filename, "r");
    assert(src);

    char *text = read_text(src);
    char *ptr = text;

    my_tree tree;
    tree_ctor(&tree, DEFAULT_TREE_CAP);

    SKIP_SPACES(ptr);

    parse_node(&ptr, &tree, tree.root);

    fclose(src);
    free(text);

    return tree;
}


#define DEF_BIN_OP(name, symbol, ...)                \
else if(!(strncmp(#symbol, *text, strlen(#symbol)))) \
{                                                    \
    node->op = OP_##name;                            \
    node->priority = PRIO_##name;                    \
                                                     \
    *text += strlen(#symbol);                        \
                                                     \
    SKIP_SPACES((*text));                            \
}

#define DEF_UN_OP(name, symbol, ...)                    \
else if(!(strncmp(#symbol, *text, strlen(#symbol))))    \
{                                                       \
    node->type = NODE_OP;                               \
    node->op = OP_##name;                               \
    node->priority = PRIO_##name;                       \
                                                        \
    *text += strlen(#symbol);                           \
                                                        \
    SKIP_SPACES((*text));                               \
                                                        \
    tree_node *new_node = node_ctor(tree);              \
    node->r_child = new_node;                           \
    parse_node(text, tree, new_node);                   \
}
void parse_node(char **text, my_tree *tree, tree_node *node)
{
    assert(text);
    assert(*text);
    assert(tree);
    assert(node);

    if(**text == '(')
    {
        (*text)++;
        SKIP_SPACES((*text));

        if(**text == '(')        //binary operators
        {
            node->type = NODE_OP;

            tree_node *new_node;

            NEW_NODE;
            ADD_L(new_node);
            parse_node(text, tree, new_node);

            if(0); //this is required for else if in macro definition
            #include "binary_operators.h"
            else ERR_OP_TEXT;

            NEW_NODE;
            ADD_R(new_node);
            parse_node(text, tree, new_node);

            if(**text == ')')
            {
                (*text)++;
                SKIP_SPACES((*text));
            }
            else ERR_MISSING_CLOSE_BRACKET;
        }
        else            //unary operators, vars or nums
        {
            double num = 0;
            char var = 0;

            if(sscanf(*text, "%lf", &num))
            {
                node->type = NODE_NUM;
                node->priority = PRIO_NUM;

                node->val = num;

                SKIP_NUM((*text));
            }
            #include "unary_operators.h"
            else if(sscanf(*text, "%c", &var))
            {
                node->type = NODE_VAR;
                node->priority = PRIO_VAR;

                node->var = var;

                (*text)++;
            }
            else ERR_UNDEFINED;

            if(**text == ')') (*text)++;
            else ERR_MISSING_CLOSE_BRACKET;

            SKIP_SPACES((*text));
        }
    }
    else ERR_MISSING_OPEN_BRACKET;

    return;
}
#undef DEF_BIN_OP
#undef DEF_UN_OP

#undef SKIP_SPACES
