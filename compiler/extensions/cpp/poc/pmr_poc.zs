package pmr_poc;

struct ChildStruct
{
    uint16 uint16Array[];
    string stringArray[];
    extern externArray[];
};

struct SampleStruct
{
    uint16 uint16Field;
    string stringField;
    extern externField;
    ChildStruct childField;
};
