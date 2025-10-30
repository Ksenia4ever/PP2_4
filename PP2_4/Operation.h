#pragma once

#include <list>

struct Operation
{
    enum Action
    {
        Write,
        Read,
        String
    };

    Action Do = Action::Write;
    int FieldIndex = 0;
    int FieldValue = 0;
};

using TOperations = std::list<Operation>;