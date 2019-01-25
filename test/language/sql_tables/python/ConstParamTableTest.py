import unittest
import os

from testutils import getZserioApi, getApiDir

class ConstParamTableTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.api = getZserioApi(__file__, "sql_tables.zs")
        cls._fileName = os.path.join(getApiDir(os.path.dirname(__file__)), "const_param_table_test.sqlite")

    def setUp(self):
        if os.path.exists(self._fileName):
            os.remove(self._fileName)
        self._database = self.api.TestDb.fromFile(self._fileName)
        self._database.createSchema()

    def testDeleteTable(self):
        self.assertTrue(self._isTableInDb())

        testTable = self._database.getConstParamTable()
        testTable.deleteTable()
        self.assertFalse(self._isTableInDb())

        testTable.createTable()
        self.assertTrue(self._isTableInDb())

    def testReadWithoutCondition(self):
        testTable = self._database.getConstParamTable()

        writtenRows = self._createConstParamTableRows()
        testTable.write(writtenRows)

        readRows = testTable.read()
        numReadRows = 0
        for readRow in readRows:
            self.assertEqual(writtenRows[numReadRows], readRow)
            numReadRows += 1
        self.assertTrue(len(writtenRows), numReadRows)

    def testReadWithCondition(self):
        testTable = self._database.getConstParamTable()

        writtenRows = self._createConstParamTableRows()
        testTable.write(writtenRows)

        condition = "name='Name1'"
        readRows = testTable.read(condition)
        expectedRowNum = 1
        for readRow in readRows:
            self.assertEqual(writtenRows[expectedRowNum], readRow)

    def testUpdate(self):
        testTable = self._database.getConstParamTable()

        writtenRows = self._createConstParamTableRows()
        testTable.write(writtenRows)

        updateRowId = 3
        updateRow = self._createConstParamTableRow(updateRowId, "UpdatedName")
        updateCondition = "blobId=" + str(updateRowId)
        testTable.update(updateRow, updateCondition)

        readRows = testTable.read(updateCondition)
        for readRow in readRows:
            self.assertEqual(updateRow, readRow)

    def _createConstParamTableRows(self):
        rows = []
        for blobId in range(self.NUM_CONST_PARAM_TABLE_ROWS):
            rows.append(self._createConstParamTableRow(blobId, "Name" + str(blobId)))

        return rows

    def _createConstParamTableRow(self, blobId, name):
        blob = self.api.const_param_table.ParameterizedBlob.fromFields(self.PARAMETERIZED_BLOB_PARAM,
                                                                       self.PARAMETERIZED_BLOB_VALUE)

        return (blobId, name, blob)

    def _isTableInDb(self):
        # check if database does contain table
        sqlQuery = "SELECT name FROM sqlite_master WHERE type='table' AND name='" + self.TABLE_NAME + "'"
        for row in self._database.executeQuery(sqlQuery):
            if len(row) == 1 and row[0] == self.TABLE_NAME:
                return True

        return False

    TABLE_NAME = "constParamTable"

    PARAMETERIZED_BLOB_VALUE = 0xABCD
    PARAMETERIZED_BLOB_PARAM = 2
    NUM_CONST_PARAM_TABLE_ROWS = 5
