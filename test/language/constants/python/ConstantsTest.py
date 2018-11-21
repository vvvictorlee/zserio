import unittest

import zserio.limits
from testutils import getZserioApi

class BitfieldEnumTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.api = getZserioApi(__file__, "constants.zs")

    def testUInt8MinConstant(self):
        self.assertEqual(zserio.limits.UINT8_MIN, self.api.UINT8_MIN_CONSTANT)

    def testUInt8MaxConstant(self):
        self.assertEqual(zserio.limits.UINT8_MAX, self.api.UINT8_MAX_CONSTANT)

    def testUInt16MinConstant(self):
        self.assertEqual(zserio.limits.UINT16_MIN, self.api.UINT16_MIN_CONSTANT)

    def testUInt16MaxConstant(self):
        self.assertEqual(zserio.limits.UINT16_MAX, self.api.UINT16_MAX_CONSTANT)

    def testUInt32MinConstant(self):
        self.assertEqual(zserio.limits.UINT32_MIN, self.api.UINT32_MIN_CONSTANT)

    def testUInt32MaxConstant(self):
        self.assertEqual(zserio.limits.UINT32_MAX, self.api.UINT32_MAX_CONSTANT)

    def testUInt64MinConstant(self):
        self.assertEqual(zserio.limits.UINT64_MIN, self.api.UINT64_MIN_CONSTANT)

    def testUInt64MaxConstant(self):
        self.assertEqual(zserio.limits.UINT64_MAX, self.api.UINT64_MAX_CONSTANT)

    def testInt8MinConstant(self):
        self.assertEqual(zserio.limits.INT8_MIN, self.api.INT8_MIN_CONSTANT)

    def testInt8MaxConstant(self):
        self.assertEqual(zserio.limits.INT8_MAX, self.api.INT8_MAX_CONSTANT)

    def testInt16MinConstant(self):
        self.assertEqual(zserio.limits.INT16_MIN, self.api.INT16_MIN_CONSTANT)

    def testInt16MaxConstant(self):
        self.assertEqual(zserio.limits.INT16_MAX, self.api.INT16_MAX_CONSTANT)

    def testInt32MinConstant(self):
        self.assertEqual(zserio.limits.INT32_MIN, self.api.INT32_MIN_CONSTANT)

    def testInt32MaxConstant(self):
        self.assertEqual(zserio.limits.INT32_MAX, self.api.INT32_MAX_CONSTANT)

    def testInt64MinConstant(self):
        self.assertEqual(zserio.limits.INT64_MIN, self.api.INT64_MIN_CONSTANT)

    def testInt64MaxConstant(self):
        self.assertEqual(zserio.limits.INT64_MAX, self.api.INT64_MAX_CONSTANT)

    def testBitfield8MinConstant(self):
        self.assertEqual(zserio.limits.UINT8_MIN, self.api.BITFIELD8_MIN_CONSTANT)

    def testBitfield8MaxConstant(self):
        self.assertEqual(zserio.limits.UINT8_MAX, self.api.BITFIELD8_MAX_CONSTANT)

    def testVariableBitfield8Constant(self):
        self.assertEqual(0xAB, self.api.VARIABLE_BITFIELD_CONSTANT)

    def testIntfield8MinConstant(self):
        self.assertEqual(zserio.limits.INT8_MIN, self.api.INTFIELD8_MIN_CONSTANT)

    def testIntfield8MaxConstant(self):
        self.assertEqual(zserio.limits.INT8_MAX, self.api.INTFIELD8_MAX_CONSTANT)

    def testVariableIntfield8Constant(self):
        self.assertEqual(0x12, self.api.VARIABLE_INTFIELD_CONSTANT)

    def testFloat16Constant(self):
        self.assertEqual(3.13, self.api.FLOAT16_CONSTANT)

    def testFloat32Constant(self):
        self.assertEqual(3.131, self.api.FLOAT32_CONSTANT)

    def testFloat64Constant(self):
        self.assertEqual(3.1314, self.api.FLOAT64_CONSTANT)

    def testVaruint16MinConstant(self):
        self.assertEqual(zserio.limits.VARUINT16_MIN, self.api.VARUINT16_MIN_CONSTANT)

    def testVaruint16MaxConstant(self):
        self.assertEqual(zserio.limits.VARUINT16_MAX, self.api.VARUINT16_MAX_CONSTANT)

    def testVaruint32MinConstant(self):
        self.assertEqual(zserio.limits.VARUINT32_MIN, self.api.VARUINT32_MIN_CONSTANT)

    def testVaruint32MaxConstant(self):
        self.assertEqual(zserio.limits.VARUINT32_MAX, self.api.VARUINT32_MAX_CONSTANT)

    def testVaruint64MinConstant(self):
        self.assertEqual(zserio.limits.VARUINT64_MIN, self.api.VARUINT64_MIN_CONSTANT)

    def testVaruint64MaxConstant(self):
        self.assertEqual(zserio.limits.VARUINT64_MAX, self.api.VARUINT64_MAX_CONSTANT)

    def testVaruintMinConstant(self):
        self.assertEqual(zserio.limits.VARUINT_MIN, self.api.VARUINT_MIN_CONSTANT)

    def testVaruintMaxConstant(self):
        self.assertEqual(zserio.limits.VARUINT_MAX, self.api.VARUINT_MAX_CONSTANT)

    def testVarint16MinConstant(self):
        self.assertEqual(zserio.limits.VARINT16_MIN, self.api.VARINT16_MIN_CONSTANT)

    def testVarint16MaxConstant(self):
        self.assertEqual(zserio.limits.VARINT16_MAX, self.api.VARINT16_MAX_CONSTANT)

    def testVarint32MinConstant(self):
        self.assertEqual(zserio.limits.VARINT32_MIN, self.api.VARINT32_MIN_CONSTANT)

    def testVarint32MaxConstant(self):
        self.assertEqual(zserio.limits.VARINT32_MAX, self.api.VARINT32_MAX_CONSTANT)

    def testVarint64MinConstant(self):
        self.assertEqual(zserio.limits.VARINT64_MIN, self.api.VARINT64_MIN_CONSTANT)

    def testVarint64MaxConstant(self):
        self.assertEqual(zserio.limits.VARINT64_MAX, self.api.VARINT64_MAX_CONSTANT)

    def testVarintMinConstant(self):
        self.assertEqual(zserio.limits.VARINT_MIN, self.api.VARINT_MIN_CONSTANT)

    def testVarintMaxConstant(self):
        self.assertEqual(zserio.limits.VARINT_MAX, self.api.VARINT_MAX_CONSTANT)

    def testBoolTrueConstant(self):
        self.assertTrue(self.api.BOOL_TRUE_CONSTANT)

    def testBoolFalseConstant(self):
        self.assertFalse(self.api.BOOL_FALSE_CONSTANT)

    def testStringConstant(self):
        self.assertEqual('Test "Quated" String', self.api.STRING_CONSTANT)

    def testUnicodeEscStringConstant(self):
        self.assertEqual("Test string with unicode escape \x19", self.api.UNICODE_ESC_STRING_CONSTANT)

    def testHexEscStringConstant(self):
        self.assertEqual("Test string with hexadecimal escape \x19", self.api.HEX_ESC_STRING_CONSTANT)

    def testOctalEscStringConstant(self):
        self.assertEqual("Test string with octal escape \031", self.api.OCTAL_ESC_STRING_CONSTANT)

    def testEnumerationConstant(self):
        self.assertEqual(self.api.Colors.BLACK, self.api.DEFAULT_PEN_COLOR)

    def testConstantDefinedByConstant(self):
        self.assertEqual(self.api.UINT32_FULL_MASK, self.api.UINT32_MAX_CONSTANT)
