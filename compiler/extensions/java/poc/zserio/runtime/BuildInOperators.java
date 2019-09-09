package zserio.runtime;

import java.math.BigInteger;

/**
 * The implementation of all Zserio build-in operators called from code generated by Zserio.
 */
public class BuildInOperators
{
    /**
     * Gets the minimum number of bits required to encode <code>numValues</code> different values.
     * <p>
     * This method implements Zserio build-in operator <code>numbits</code> for Zserio uint8, uint16
     * and uint32 types.</p>
     * <p>
     * <b>Note:</b> Please note that this method returns 0 if value is zero or negative value.</p>
     * <p>
     * Examples:</p>
     * <code>numbits(0) = 0</code><br>
     * <code>numbits(1) = 1</code><br>
     * <code>numbits(2) = 1</code><br>
     * <code>numbits(3) = 2</code><br>
     * <code>numbits(4) = 2</code><br>
     * <code>numbits(8) = 3</code><br>
     * <code>numbits(16) = 4</code>
     *
     * @param numValues The number of different values from which to calculate number of bits.
     *
     * @return Number of bis required to encode <code>numValues</code> different values.
     */
    public static short getNumBits(long numValues)
    {
        if (numValues <= 0)
            return 0;

        short result = 1;
        long current = (numValues - 1) >> 1;
        while (current > 0)
        {
            result++;
            current >>= 1;
        }

        return result;
    }

    /**
     * Gets the minimum number of bits required to encode <code>numValues</code> different values.
     * <p>
     * This method implements Zserio build-in operator <code>numbits</code> for Zserio uint64 type
     * which is mapped to Java BigInteger type.</p>
     * <p>
     * Examples:</p>
     * <code>numbits(0) = 0</code><br>
     * <code>numbits(1) = 1</code><br>
     * <code>numbits(2) = 1</code><br>
     * <code>numbits(3) = 2</code><br>
     * <code>numbits(4) = 2</code><br>
     * <code>numbits(8) = 3</code><br>
     * <code>numbits(16) = 4</code>
     *
     * @param numValues The number of different values from which to calculate number of bits.
     *
     * @return Number of bis required to encode <code>numValues</code> different values.
     */
    public static short getNumBits(BigInteger numValues)
    {
        if (numValues.doubleValue() <= 0.0)
            return 0;
        if (numValues.doubleValue() == 1.0)
            return 1;

        final BigInteger calcValue = numValues.subtract(BigInteger.ONE);

        return (short)calcValue.bitLength();
    }
}
