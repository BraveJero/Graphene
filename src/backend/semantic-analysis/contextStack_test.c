#include "contextStack.h"
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

struct point {
    int x, y;
};

int main() {
    struct point p1 = (struct point) {
        .x = 1,
        .y = 2,
    };
    struct point p2 = (struct point) {
        .x = 3,
        .y = 4,
    };
    struct point p3 = (struct point) {
        .x = 5,
        .y = 6,
    };
    struct point p4 = (struct point) {
        .x = 7,
        .y = 8,
    };
    struct point *p1_cpy;
    struct point *p2_cpy;
    struct point *p3_cpy;

    contextStack *cs = newContextStack(sizeof(struct point));

    pushContext(cs);
    assert(cs != NULL);

    assert(addToContext(cs, "p1", &p1) == 0);
    assert(addToContext(cs, "p2", &p2) == 0);
    assert(addToContext(cs, "p3", &p3) == 0);

    p1_cpy = (struct point *) retrieveData(cs, "p1");
    p2_cpy = (struct point *) retrieveData(cs, "p2");
    p3_cpy = (struct point *) retrieveData(cs, "p3");

    assert(p1_cpy != NULL);
    assert(p2_cpy != NULL);
    assert(p3_cpy != NULL);
    assert(p1_cpy->x == p1.x);
    assert(p1_cpy->y == p1.y);
    assert(p2_cpy->x == p2.x);
    assert(p2_cpy->y == p2.y);
    assert(p3_cpy->x == p3.x);
    assert(p3_cpy->y == p3.y);

    free(p1_cpy);
    free(p2_cpy);
    free(p3_cpy);

    assert(pushContext(cs) == 0);

    assert(addToContext(cs, "p1", &p4) == 0);
    p1_cpy = (struct point *) retrieveData(cs, "p1");
    p2_cpy = (struct point *) retrieveData(cs, "p2");
    p3_cpy = (struct point *) retrieveData(cs, "p3");

    assert(p1_cpy != NULL);
    assert(p2_cpy != NULL);
    assert(p3_cpy != NULL);
    assert(p1_cpy->x == p4.x);
    assert(p1_cpy->y == p4.y);
    assert(p2_cpy->x == p2.x);
    assert(p2_cpy->y == p2.y);
    assert(p3_cpy->x == p3.x);
    assert(p3_cpy->y == p3.y);

    free(p1_cpy);
    free(p2_cpy);
    free(p3_cpy);

    assert(popContext(cs) == 0);

    assert(addToContext(cs, "p1", &p1) != 0);
    assert(addToContext(cs, "p2", &p2) != 0);
    assert(addToContext(cs, "p3", &p3) != 0);

    p1_cpy = (struct point *) retrieveData(cs, "p1");
    p2_cpy = (struct point *) retrieveData(cs, "p2");
    p3_cpy = (struct point *) retrieveData(cs, "p3");

    assert(p1_cpy != NULL);
    assert(p2_cpy != NULL);
    assert(p3_cpy != NULL);
    assert(p1_cpy->x == p1.x);
    assert(p1_cpy->y == p1.y);
    assert(p2_cpy->x == p2.x);
    assert(p2_cpy->y == p2.y);
    assert(p3_cpy->x == p3.x);
    assert(p3_cpy->y == p3.y);

    free(p1_cpy);
    free(p2_cpy);
    free(p3_cpy);

    freeStack(cs);
    puts("OK!");
    return 0;
}

