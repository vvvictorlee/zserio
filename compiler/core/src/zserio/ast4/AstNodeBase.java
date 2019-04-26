package zserio.ast4;

import org.antlr.v4.runtime.Token;

public abstract class AstNodeBase implements AstNode
{
    /**
     * Constructor from ANTLR4 token.
     *
     * @param token Token to construct from.
     */
    public AstNodeBase(Token token)
    {
        this.location = new AstNodeLocation(token);
    }

    @Override
    public AstNodeLocation getLocation()
    {
        return location;
    }

    @Override
    public void visitChildren(ZserioAstVisitor visitor)
    {}

    private final AstNodeLocation location;
};
