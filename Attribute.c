#include "Attribute.h"
#include "Value.h"
#include <stdlib.h>
#include <string.h>
#include "helper.h"
#include <stdio.h>
#include "array.h"

Attribute *init_attribute(char *name, AttrType type)
{
    Attribute *attribute = calloc(1, sizeof(Attribute));
    char *buf = calloc(strlen(name) + 1, sizeof(char));
    strcpy(buf, name);
    attribute->name = buf;
    attribute->type = type;
    return attribute;
}
void attribute_free(Attribute *attr)
{
    free(attr->name);
    free(attr);
}
size_t attributes_size(Attributes *attributes)
{
    size_t t = 0;
    for (size_t i = 0; i < attributes->count; i++)
    {
        Attribute *attr = array_at(attributes, i);
        t += attribute_size(attr->type);
    }
    return t;
}
size_t attribute_size(AttrType type)
{
    switch (type)
    {
    case ATTR_INT:
        return sizeof(DB_INT);
    case ATTR_CHAR:
        // CHAR_PREFIX_LEN is prefixed length for db_varchar field;
        return CHAR_PREFIX_LEN + sizeof(DB_CHAR) * DB_VARHCAR_LEN;
    default:
        NOTREACHABLE;
    }
}

char *attribute_type_to_str(AttrType type)
{
    switch (type)
    {
    case ATTR_INT:
        return "INT";
    case ATTR_CHAR:
        return "CHAR";
    default:
        return "UNKNOWN";
    }
}