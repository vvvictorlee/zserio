package zserio.emit.java.types;

public class NativeShortArrayType extends NativeArrayType
{
    public NativeShortArrayType()
    {
        super("ShortArray");
    }

    @Override
    public boolean requiresElementBitSize()
    {
        return true;
    }
}
