#ifndef HELPER_H
#define HELPER_H
#include <stdlib.h>
#include <stdio.h>

#define NOTIMPLEMENTED(message)                                                                               \
    do                                                                                                        \
    {                                                                                                         \
        fprintf(stderr, "NOT IMPLEMENTD: %s:%d in %s\nmessage: %s\n", __FILE__, __LINE__, __func__, message); \
        exit(1);                                                                                              \
    } while (0)

#define NOTREACHABLE                                                                   \
    do                                                                                 \
    {                                                                                  \
        fprintf(stderr, "NOT REACHABLE: %s:%d in %s\n", __FILE__, __LINE__, __func__); \
        exit(1);                                                                       \
    } while (0)
#endif