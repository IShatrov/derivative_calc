DEF_UN_OP(SIN, sin,
{
    node->op = OP_MULT;

    RIGHT(node) = DIFF(RIGHT(target));

    LEFT(node) = NEW_OP_NODE(OP_COS);
    RIGHT(LEFT(node)) = COPY(RIGHT(target));
},
{
    TEX_PRINT("\\sin(");
    PRINT(RIGHT(node));
    TEX_PRINT(")");
})

DEF_UN_OP(COS, cos,
{
    node->op = OP_MULT;

    RIGHT(node) = DIFF(RIGHT(target));

    LEFT(node) = NEW_OP_NODE(OP_MULT);
    LEFT(LEFT(node)) = NEW_NUM_NODE(-1);       //(cos(t))' = -sin(t)t'
    RIGHT(LEFT(node)) = NEW_OP_NODE(OP_SIN);

    RIGHT(RIGHT(LEFT(node))) = COPY(RIGHT(target));
},
{
    TEX_PRINT("\\cos(");
    PRINT(RIGHT(node));
    TEX_PRINT(")");
})

DEF_UN_OP(LN, ln,
{
    node->op = OP_MULT;

    RIGHT(node) = DIFF(RIGHT(target));

    LEFT(node) = NEW_OP_NODE(OP_DIV);
    RIGHT(LEFT(node)) = COPY(RIGHT(target));
    LEFT(LEFT(node)) = NEW_NUM_NODE(1);
},
{
    TEX_PRINT("\\ln(");
    PRINT(RIGHT(node));
    TEX_PRINT(")");
})
