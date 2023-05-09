#include "set.h"

#include <stdlib.h>

candidate_function set_function(void (*f1)(void *), void (*f2)(void *))
{
    return (f1 != NULL) ? f1 : f2;
}
