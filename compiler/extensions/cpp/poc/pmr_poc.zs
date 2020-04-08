package pmr_poc;

struct ChildStruct
{
    uint16 uint16Array[];
    string stringArray[];
    extern externArray[];
    // optional array
};

struct BigStruct
{
    uint64 field1;
    uint64 field2;
    uint64 field3;
    uint64 field4;
    uint64 field5;
    uint64 field6;
    uint64 field7;
    uint64 field8;
};

struct StringStruct
{
    string stringField;
};

struct SampleStruct
{
    uint16 uint16Field;
    string stringField;
    extern externField;
    optional BigStruct inplaceOptionalField;
    optional StringStruct heapOptionalField;
    // choice which allocates dynamically
    ChildStruct childField[];
};
