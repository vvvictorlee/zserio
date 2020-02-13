package without_service_code;

import static org.junit.Assert.*;

import java.io.File;

import org.junit.Test;

public class WithoutServiceCodeTest
{
    @Test
    public void checkService()
    {
        assertFalse(isFilePresent("../gen/without_service_code/Service.java"));
    }

    @Test
    public void checkRequest()
    {
        assertTrue(isFilePresent("../gen/without_service_code/Request.java"));
    }

    @Test
    public void checkResponse()
    {
        assertTrue(isFilePresent("../gen/without_service_code/Response.java"));
    }

    private boolean isFilePresent(String fileName)
    {
        File file = new File(fileName);
        return file.exists();
    }
};
