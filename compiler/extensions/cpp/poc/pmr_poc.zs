package pmr_poc;

struct ChildStruct
{
    uint64 uint64Field;
    uint16 uint16Array[];
};

struct SampleStruct
{
    uint8 uint8Field;
    string stringField;
    ChildStruct childField;
};
