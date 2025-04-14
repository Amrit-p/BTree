#ifndef ATTRIBUTE_H
#define ATTRIBUTE_H
#include <stddef.h>

typedef enum
{
    ATTR_INT,
    ATTR_CHAR
} AttrType;

typedef struct
{
    AttrType type;
    char *name;
} Attribute;

typedef struct
{
    Attribute **items;
    size_t count;
    size_t capacity;
} Attributes;

Attribute *init_attribute(char *name, AttrType type);
char *attribute_type_to_str(AttrType type);
size_t attribute_size(AttrType type);
size_t attributes_size(Attributes *attributes);
void attribute_free(Attribute *attr);
#endif