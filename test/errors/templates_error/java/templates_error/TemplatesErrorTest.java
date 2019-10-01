package templates_error;

import static org.junit.Assert.*;

import org.junit.BeforeClass;
import org.junit.Test;

import java.io.IOException;

import test_utils.ZserioErrors;

public class TemplatesErrorTest
{
    @BeforeClass
    public static void readZserioErrors() throws IOException
    {
        zserioErrors = new ZserioErrors();
    }

    @Test
    public void constUsedAsType()
    {
        String error = "constant_used_as_type_error.zs:12:5: " +
                "In instantiation of 'TestStruct' required from here";
        assertTrue(zserioErrors.isPresent(error));

        error = "constant_used_as_type_error.zs:5:5: Invalid usage of constant 'CONST' as a type!";
        assertTrue(zserioErrors.isPresent(error));
    }

    @Test
    public void constraintExpressionsExpectsConstant()
    {
        String error = "constraint_expression_expects_constant_error.zs:10:5: " +
                "In instantiation of 'TestStruct' required from here";
        assertTrue(zserioErrors.isPresent(error));

        error = "constraint_expression_expects_constant_error.zs:5:27: " +
                "Unresolved symbol 'uint32' within expression scope!";
        assertTrue(zserioErrors.isPresent(error));
    }

    @Test
    public void enumNotATemplate()
    {
        final String error = "enum_not_a_template_error.zs:11:5: 'Enumeration' is not a template!";
        assertTrue(zserioErrors.isPresent(error));
    }

    @Test
    public void instantiationNameClash()
    {
        String error = "instantiation_name_clash_error.zs:32:5: " +
                "In instantiation of 'Template' required from here";
        assertTrue(zserioErrors.isPresent(error));

        error = "instantiation_name_clash_error.zs:31:5: First instantiated from here";
        assertTrue(zserioErrors.isPresent(error));

        error = "instantiation_name_clash_error.zs:23:8: " +
                "Instantiation name 'Template_A_B_C' already exits!";
        assertTrue(zserioErrors.isPresent(error));
    }

    @Test
    public void missingTypeParameters()
    {
        String error = "missing_type_parameters_error.zs:15:5: " +
                "In instantiation of 'TestStruct' required from here";
        assertTrue(zserioErrors.isPresent(error));

        error = "missing_type_parameters_error.zs:5:5: " +
                "Referenced type 'Parameterized' is defined as parameterized type!";
        assertTrue(zserioErrors.isPresent(error));
    }

    @Test
    public void parameterizedBuiltinType()
    {
        final String error = "parameterized_builtin_type_error.zs:11:16: " +
                "uint32 cannot be used as a parameterized type!";
        assertTrue(zserioErrors.isPresent(error));
    }

    @Test
    public void parameterizedCompoundType()
    {
        String error = "parameterized_compound_type_error.zs:16:5: " +
                "In instantiation of 'TestStruct' required from here";
        assertTrue(zserioErrors.isPresent(error));

        error = "parameterized_compound_type_error.zs:11:5: " +
                "Parameterized type instantiation 'Compound()' does not refer to a parameterized type!";
        assertTrue(zserioErrors.isPresent(error));
    }

    @Test
    public void templatableNotATemplate()
    {
        final String error = "templatable_not_a_template_error.zs:10:5: 'Templatable' is not a template!";
        assertTrue(zserioErrors.isPresent(error));
    }

    @Test
    public void templatedTemplateParameter()
    {
        final String error = "templated_template_parameter_error.zs:5:5: " +
                "Template parameter cannot be used as a template!";
        assertTrue(zserioErrors.isPresent(error));
    }

    @Test
    public void unresolvedReference()
    {
        final String error = "unresolved_reference_error.zs:5:5: Unresolved referenced type 'TemplatedStruct'!";
        assertTrue(zserioErrors.isPresent(error));
    }

    @Test
    public void wrongNumberOfArguments()
    {
        final String error = "wrong_number_of_arguments_error.zs:3:8: " +
                "Wrong number of template arguments for template 'TestStruct'! Expecting 2, got 1!";
        assertTrue(zserioErrors.isPresent(error));
    }

    private static ZserioErrors zserioErrors;
}
