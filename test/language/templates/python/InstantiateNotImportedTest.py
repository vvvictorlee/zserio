import unittest
import zserio

from testutils import getZserioApi

class InstantiateNotImportedTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.api = getZserioApi(__file__, "templates.zs").instantiate_not_imported

    def testReadWrite(self):
        instantiateNotImported = self.api.InstantiateNotImported.fromFields(
            self.api.pkg.Test_uint32.fromFields(13),
            self.api.pkg.Test_string.fromFields("test")
        )

        writer = zserio.BitStreamWriter()
        instantiateNotImported.write(writer)
        reader = zserio.BitStreamReader(writer.getByteArray())
        readInstantiateNotImported = self.api.InstantiateNotImported()
        readInstantiateNotImported.read(reader)
        self.assertEqual(instantiateNotImported, readInstantiateNotImported)
