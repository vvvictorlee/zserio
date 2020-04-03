package pmr_poc;

struct ChildStruct
{
    uint16 uint16Array[];
    string stringArray[];
    extern externArray[];
    // optional array
};

struct SampleStruct
{
    uint16 uint16Field;
    string stringField;
    extern externField;
    // optional which allocates dynamically
    // choice which allocates dynamically
    ChildStruct childField[];
};
