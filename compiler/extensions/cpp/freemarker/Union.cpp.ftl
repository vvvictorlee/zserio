<#include "FileHeader.inc.ftl">
<#include "CompoundConstructor.inc.ftl">
<#include "CompoundParameter.inc.ftl">
<#include "CompoundField.inc.ftl">
<#include "CompoundFunction.inc.ftl">
<@file_header generatorDescription/>

#include <zserio/StringConvertUtil.h>
#include <zserio/CppRuntimeException.h>
#include <zserio/HashCodeUtil.h>
#include <zserio/BitPositionUtil.h>
#include <zserio/BitSizeOfCalculator.h>
#include <zserio/BitFieldUtil.h>
#include <zserio/VarUInt64Util.h>
<#if has_field_with_constraint(fieldList)>
#include <zserio/ConstraintException.h>
</#if>
<@system_includes cppSystemIncludes, false/>

#include "<@include_path package.path, "${name}.h"/>"
<@user_includes cppUserIncludes, false/>

<@namespace_begin package.path/>

<@define_inner_classes fieldList/>
<#-- TODO empty ctor must be only if withWriterCode if reader ctor won't need empty ctors -->
${name}::${name}() noexcept :
    <#assign constructorMembersInitialization><@compound_constructor_members_initialization compoundConstructorsData/></#assign>
    <#if constructorMembersInitialization?has_content>
        ${constructorMembersInitialization}, m_choiceTag(UNDEFINED_CHOICE)
    <#else>
        m_choiceTag(UNDEFINED_CHOICE)
    </#if>
{
}

<@compound_read_constructor_definition compoundConstructorsData/>

<#if needs_compound_initialization(compoundConstructorsData) || has_field_with_initialization(fieldList)>
${name}::${name}(<#rt>
    <#lt>const ${name}& other) :
        m_choiceTag(other.m_choiceTag)<#rt>
    <#if fieldList?has_content>
        <#lt>,
        <#list fieldList as field>
        <@compound_copy_constructor_initializer_field field, field?has_next, 2/>
            <#if field.usesAnyHolder>
                <#break>
            </#if>
        </#list>
    <#else>

    </#if>
{
    <@compound_copy_initialization compoundConstructorsData/>
}

${name}& ${name}::operator=(const ${name}& other)
{
    m_choiceTag = other.m_choiceTag;
    <#list fieldList as field>
    <@compound_assignment_field field, 1/>
        <#if field.usesAnyHolder>
            <#break>
        </#if>
    </#list>
    <@compound_copy_initialization compoundConstructorsData/>

    return *this;
}

${name}::${name}(${name}&& other) :
        m_choiceTag(other.m_choiceTag)<#rt>
    <#if fieldList?has_content>
        <#lt>,
        <#list fieldList as field>
        <@compound_move_constructor_initializer_field field, field?has_next, 2/>
            <#if field.usesAnyHolder>
                <#break>
            </#if>
        </#list>
    <#else>

    </#if>
{
    <@compound_copy_initialization compoundConstructorsData/>
}

${name}& ${name}::operator=(${name}&& other)
{
    m_choiceTag = other.m_choiceTag;
    <#list fieldList as field>
    <@compound_move_assignment_field field, 1/>
        <#if field.usesAnyHolder>
            <#break>
        </#if>
    </#list>
    <@compound_copy_initialization compoundConstructorsData/>

    return *this;
}

</#if>
<#if needs_compound_initialization(compoundConstructorsData)>
    <@compound_initialize_definition compoundConstructorsData, needsChildrenInitialization/>

</#if>
<#if needsChildrenInitialization>
void ${name}::initializeChildren()
{
    <#if fieldList?has_content>
    switch (m_choiceTag)
    {
        <#list fieldList as field>
    case <@choice_tag_name field/>:
        <@compound_initialize_children_field field, name, 2, true/>
        break;
        </#list>
    default:
        throw zserio::CppRuntimeException("No match in union ${name}!");
    }
    </#if>
    <@compound_initialize_children_epilog_definition compoundConstructorsData/>
}

</#if>
${name}::ChoiceTag ${name}::choiceTag() const
{
    return m_choiceTag;
}

<@compound_parameter_accessors_definition name, compoundParametersData/>
<#list fieldList as field>
    <#assign fieldOrOptional=field.optional!field>
    <#if needs_field_getter(field)>
${fieldOrOptional.cppTypeName}& ${name}::${field.getterName}()
{
    return m_objectChoice.get<${field.cppTypeName}>();
}

    </#if>
${fieldOrOptional.cppArgumentTypeName} ${name}::${field.getterName}() const
{
    return m_objectChoice.get<${field.cppTypeName}>();
}

    <#if needs_field_setter(field)>
void ${name}::${field.setterName}(${fieldOrOptional.cppArgumentTypeName} <@field_argument_name field.name/>)
{
    m_choiceTag = <@choice_tag_name field/>;
    m_objectChoice = <@field_argument_name field.name/>;
}

    </#if>
    <#if needs_field_rvalue_setter(field)>
void ${name}::${field.setterName}(${fieldOrOptional.cppTypeName}&& <@field_argument_name field.name/>)
{
    m_choiceTag = <@choice_tag_name field/>;
    m_objectChoice = std::move(<@field_argument_name field.name/>);
}

    </#if>
</#list>
<@compound_functions_definition name, compoundFunctionsData/>
size_t ${name}::bitSizeOf(size_t<#if fieldList?has_content> bitPosition</#if>) const
{
<#if fieldList?has_content>
    size_t endBitPosition = bitPosition;

    endBitPosition += zserio::bitSizeOfVarUInt64(static_cast<uint64_t>(m_choiceTag));

    switch (m_choiceTag)
    {
    <#list fieldList as field>
    case <@choice_tag_name field/>:
        <@compound_bitsizeof_field field, 2/>
        break;
    </#list>
    default:
        throw zserio::CppRuntimeException("No match in union ${name}!");
    }

    return endBitPosition - bitPosition;
<#else>
    return 0;
</#if>
}

bool ${name}::operator==(const ${name}& other) const
{
    if (this == &other)
        return true;

    <@compound_parameter_comparison_with_any_holder compoundParametersData/>
    if (m_choiceTag != other.m_choiceTag)
        return false;

<#if fieldList?has_content>
    if (m_objectChoice.hasValue() != other.m_objectChoice.hasValue())
        return false;

    if (!m_objectChoice.hasValue())
        return true;

    switch (m_choiceTag)
    {
    <#list fieldList as field>
    case <@choice_tag_name field/>:
        return m_objectChoice.get<${field.cppTypeName}>() == other.m_objectChoice.get<${field.cppTypeName}>();
    </#list>
    default:
        return true; // UNDEFINED_CHOICE
    }
<#else>
    return true;
</#if>
}

int ${name}::hashCode() const
{
    int result = zserio::HASH_SEED;

    <@compound_parameter_hash_code compoundParametersData/>
    result = zserio::calcHashCode(result, static_cast<uint32_t>(m_choiceTag));
<#if fieldList?has_content>
    if (m_objectChoice.hasValue())
    {
        switch (m_choiceTag)
        {
        <#list fieldList as field>
        case <@choice_tag_name field/>:
            result = zserio::calcHashCode(result, m_objectChoice.get<${field.cppTypeName}>());
            break;
        </#list>
        default:
            // UNDEFINED_CHOICE
            break;
        }
    }
</#if>

    return result;
}

void ${name}::read(zserio::BitStreamReader&<#if fieldList?has_content> in</#if>)
{
<#if fieldList?has_content>
    m_choiceTag = static_cast<ChoiceTag>(zserio::convertVarUInt64ToInt32(in.readVarUInt64()));

    switch (m_choiceTag)
    {
    <#list fieldList as field>
    case <@choice_tag_name field/>:
        <@compound_read_field field, name, 2/>
        <@compound_check_constraint_field field, name, 2/>
        break;
    </#list>
    default:
        throw zserio::CppRuntimeException("No match in union ${name}!");
    }
<#else>
    m_choiceTag = UNDEFINED_CHOICE;
</#if>
}
<#assign needsRangeCheck=withRangeCheckCode && has_field_with_range_check(fieldList)/>
<#if withWriterCode>

size_t ${name}::initializeOffsets(size_t bitPosition)
{
    <#if fieldList?has_content>
    size_t endBitPosition = bitPosition;

    endBitPosition += zserio::bitSizeOfVarUInt64(static_cast<uint64_t>(m_choiceTag));

    switch (m_choiceTag)
    {
        <#list fieldList as field>
    case <@choice_tag_name field/>:
        <@compound_initialize_offsets_field field, 2/>
        break;
        </#list>
    default:
        throw zserio::CppRuntimeException("No match in union ${name}!");
    }

    return endBitPosition;
    <#else>
    return bitPosition;
    </#if>
}

<#assign hasPreWriteAction=needsRangeCheck || needsChildrenInitialization || hasFieldWithOffset/>
void ${name}::write(zserio::BitStreamWriter&<#if fieldList?has_content> out</#if>, <#rt>
        zserio::PreWriteAction<#if hasPreWriteAction> preWriteAction</#if>)<#lt>
{
    <#if fieldList?has_content>
        <#if hasPreWriteAction>
    <@compound_pre_write_actions needsRangeCheck, needsChildrenInitialization, hasFieldWithOffset/>

        </#if>
    out.writeVarUInt64(static_cast<uint64_t>(m_choiceTag));

    switch (m_choiceTag)
    {
        <#list fieldList as field>
    case <@choice_tag_name field/>:
        <@compound_check_constraint_field field, name, 2/>
        <@compound_write_field field, name, 2/>
        break;
        </#list>
    default:
        throw zserio::CppRuntimeException("No match in union ${name}!");
    }
    </#if>
}
</#if>
<#if needsRangeCheck>

void ${name}::checkRanges()
{
    <#if fieldList?has_content>
    switch (m_choiceTag)
    {
        <#list fieldList as field>
    case <@choice_tag_name field/>:
            <#if needs_field_range_check(field)>
        {
            <@compound_check_range_field field, name, 3, true/>
        }
            </#if>
        break;
        </#list>
    default:
        throw zserio::CppRuntimeException("No match in union ${name}!");
    }
    </#if>
}
</#if>

<@namespace_end package.path/>
