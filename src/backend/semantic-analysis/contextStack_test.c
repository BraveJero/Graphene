#include "contextStack.h"
#include <stdio.h>
#include <assert.h>

int main() {
    DataType integer = INT_TYPE, floaty = FLOAT_TYPE, string = STRING_TYPE;
    contextStack *cs = newContextStack();
    assert(cs != NULL);

    assert(pushContext(cs) == 0);

    assert(addToContext(cs, "integer1", &integer) == 0);
    assert(addToContext(cs, "float1", &floaty) == 0);
    assert(addToContext(cs, "string1", &string) == 0);
    assert(addToContext(cs, "integer1", &string) < 0);

    assert(*retrieveType(cs, "integer1") == INT_TYPE);
    assert(*retrieveType(cs, "float1") == FLOAT_TYPE);
    assert(*retrieveType(cs, "string1") == STRING_TYPE);

    assert(pushContext(cs) == 0);

    assert(addToContext(cs, "integer1", &string) == 0);
    assert(addToContext(cs, "integer2", &integer) == 0);
    assert(addToContext(cs, "integer2", &integer) < 0);

    assert(*retrieveType(cs, "integer1") == STRING_TYPE);
    assert(*retrieveType(cs, "integer2") == INT_TYPE);

    assert(popContext(cs) == 0);

    assert(*retrieveType(cs, "integer1") == INT_TYPE);
    assert(retrieveType(cs, "integer2") == NULL);

    freeStack(cs);
    puts("OK!");
    return 0;
}

