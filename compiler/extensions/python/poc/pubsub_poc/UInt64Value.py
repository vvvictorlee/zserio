"""
Automatically generated by Zserio Python extension version 1.4.0-pre2.
"""

import zserio

class UInt64Value():
    def __init__(self):
        self._value_ = None

    @classmethod
    def fromReader(cls, reader):
        instance = cls()
        instance.read(reader)

        return instance

    @classmethod
    def fromFields(cls, value_):
        instance = cls()
        instance.setValue(value_)

        return instance

    def __eq__(self, other):
        if isinstance(other, UInt64Value):
            return self._value_ == other._value_

        return False

    def __hash__(self):
        result = zserio.hashcode.HASH_SEED
        result = zserio.hashcode.calcHashCode(result, hash(self._value_))

        return result

    def getValue(self):
        return self._value_

    def setValue(self, value_):
        self._value_ = value_

    def bitSizeOf(self, bitPosition=0):
        endBitPosition = bitPosition
        endBitPosition += 64

        return endBitPosition - bitPosition

    def initializeOffsets(self, bitPosition):
        endBitPosition = bitPosition
        endBitPosition += 64

        return endBitPosition

    def read(self, reader):
        self._value_ = reader.readBits(64)

    def write(self, writer, *, callInitializeOffsets=True):
        del callInitializeOffsets

        writer.writeBits(self._value_, 64)
