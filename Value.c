#include "Value.h"
#include <stdio.h>
#include "helper.h"
#include <string.h>

size_t value_size(Value value)
{

    switch (value.type)
    {
    case ATTR_INT:
        return attribute_size(value.type);
    case ATTR_CHAR:
        return strlen(AS_CHAR(value)) * sizeof(DB_CHAR);
    default:
        NOTREACHABLE;
    }
}
void value_dump(Value value)
{
    switch (value.type)
    {
    case ATTR_INT:
    {
        printf("%d", AS_INT(value));
    }
    break;
    case ATTR_CHAR:
    {
        printf("%s", AS_CHAR(value));
    }
    break;
    default:
        printf("UNKNOWN value type.\n");
    }
}