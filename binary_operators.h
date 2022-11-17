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
})

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
})

DEF_BIN_OP(MULT, *,
{
    node->op = OP_ADD;

    LEFT(node) = NEW_OP_NODE(OP_MULT);
    LEFT(LEFT(node)) = DIFF(LEFT(target));
    RIGHT(LEFT(node)) = COPY(RIGHT(target));

    RIGHT(node) = NEW_OP_NODE(OP_MULT);
    LEFT(RIGHT(node)) = DIFF(RIGHT(target));
    RIGHT(RIGHT(node)) = COPY(LEFT(target));
},
{
    PRINT(LEFT(node));
    TEX_PRINT(" \\cdot ");
    PRINT(RIGHT(node));
})

DEF_BIN_OP(DIV, /,
{
    node->op = OP_DIV;

    RIGHT(node) = NEW_OP_NODE(OP_POW);
    LEFT(RIGHT(node)) = COPY(RIGHT(target));
    RIGHT(RIGHT(node)) = NEW_NUM_NODE(2); //denominator is squared, see derivative formulas

    LEFT(node) = NEW_OP_NODE(OP_SUB);

    LEFT(LEFT(node)) = NEW_OP_NODE(OP_MULT);
    LEFT(LEFT(LEFT(node))) = DIFF(LEFT(target));
    RIGHT(LEFT(LEFT(node))) = COPY(RIGHT(target));

    RIGHT(LEFT(node)) = NEW_OP_NODE(OP_MULT);
    LEFT(RIGHT(LEFT(node))) = DIFF(RIGHT(target));
    RIGHT(RIGHT(LEFT(node))) = COPY(LEFT(target));
},
{
    TEX_PRINT("\\frac{");
    PRINT(LEFT(node));
    TEX_PRINT("}{");
    PRINT(RIGHT(node));
    TEX_PRINT("}");
})

DEF_BIN_OP(POW, ^,
{
    ;
},
{
    PRINT(LEFT(node));
    TEX_PRINT("^{");
    PRINT(RIGHT(node));
    TEX_PRINT("}");
})
