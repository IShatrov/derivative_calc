DEF_BIN_OP(ADD, +,
{
    node->op = OP_ADD;

    LEFT(node) = DIFF(LEFT(target));

    RIGHT(node) = DIFF(RIGHT(target));
},
{
    PRINT(LEFT(node));
    TEX_PRINT(" + ");
    PRINT(RIGHT(node));
}, 0)

DEF_BIN_OP(SUB, -,
{
    node->op = OP_SUB;

    LEFT(node) = DIFF(LEFT(target));

    RIGHT(node) = DIFF(RIGHT(target));
},
{
    PRINT(LEFT(node));
    TEX_PRINT(" - ");
    PRINT(RIGHT(node));
}, 0)

DEF_BIN_OP(MULT, *,
{
    node->op = OP_ADD;

    LEFT(node) = NEW_OP_NODE(MULT);
    LEFT(LEFT(node)) = DIFF(LEFT(target));
    RIGHT(LEFT(node)) = COPY(RIGHT(target));

    RIGHT(node) = NEW_OP_NODE(MULT);
    LEFT(RIGHT(node)) = DIFF(RIGHT(target));
    RIGHT(RIGHT(node)) = COPY(LEFT(target));
},
{
    PRINT(LEFT(node));
    TEX_PRINT(" \\cdot ");
    PRINT(RIGHT(node));
}, 10)

DEF_BIN_OP(DIV, /,
{
    node->op = OP_DIV;

    RIGHT(node) = NEW_OP_NODE(POW);
    LEFT(RIGHT(node)) = COPY(RIGHT(target));
    RIGHT(RIGHT(node)) = NEW_NUM_NODE(2); //denominator is squared, see derivative formulas

    LEFT(node) = NEW_OP_NODE(SUB);

    LEFT(LEFT(node)) = NEW_OP_NODE(MULT);
    LEFT(LEFT(LEFT(node))) = DIFF(LEFT(target));
    RIGHT(LEFT(LEFT(node))) = COPY(RIGHT(target));

    RIGHT(LEFT(node)) = NEW_OP_NODE(MULT);
    LEFT(RIGHT(LEFT(node))) = DIFF(RIGHT(target));
    RIGHT(RIGHT(LEFT(node))) = COPY(LEFT(target));
},
{
    TEX_PRINT("\\frac{");
    PRINT(LEFT(node));
    TEX_PRINT("}{");
    PRINT(RIGHT(node));
    TEX_PRINT("}");
}, 10)

DEF_BIN_OP(POW, ^,
{
    char is_l_func = FIND(LEFT(target), 'x');
    char is_r_func = FIND(RIGHT(target), 'x');

    if(is_l_func)
    {
        if(is_r_func) printf("I am not doing this\n");
        else
        {
            node->op = OP_MULT;

            RIGHT(node) = DIFF(LEFT(target));

            LEFT(node) = NEW_OP_NODE(MULT);

            LEFT(LEFT(node)) = NEW_NUM_NODE(RIGHT(target)->val);

            RIGHT(LEFT(node)) = NEW_OP_NODE(POW);
            LEFT(RIGHT(LEFT(node))) = COPY(LEFT(target));
            RIGHT(RIGHT(LEFT(node))) = NEW_NUM_NODE(RIGHT(target)->val - 1);
        }
    }
    else
    {
        if(is_r_func)
        {
            node->op = OP_MULT;

            LEFT(node) = COPY(target);

            RIGHT(node) = NEW_OP_NODE(MULT);

            LEFT(RIGHT(node)) = NEW_OP_NODE(LN);
            RIGHT(LEFT(RIGHT(node))) = NEW_NUM_NODE(LEFT(target)->val);

            RIGHT(RIGHT(node)) = DIFF(RIGHT(target));
        }
        else
        {
            node->type = NODE_NUM;
            node->val = 0;
        }
    }
},
{
    PRINT(LEFT(node));
    TEX_PRINT("^{");
    PRINT(RIGHT(node));
    TEX_PRINT("}");
}, 20)
