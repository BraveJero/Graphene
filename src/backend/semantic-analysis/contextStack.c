#include "contextStack.h"
#include <stdlib.h>
#include <string.h>

typedef struct elementNode {
    char *id;
    DataType *type;
    struct elementNode *next;
}elementNode;


typedef struct contextNode {
    elementNode *head;
    struct contextNode *next;
} contextNode;

struct contextStack {
    contextNode *current;
};

static void freeContext(contextNode *cn);
static void freeList(elementNode *en);
static elementNode *addToList(elementNode *list, const char *id, DataType *type, int *added);


contextStack *newContextStack() {
    return calloc(1, sizeof(struct contextStack));
}

int pushContext(contextStack *cs) {
    contextNode *newContext = malloc(sizeof(struct contextNode));
    if(newContext == NULL) return -1;
    newContext->head = NULL;
    newContext->next = cs->current;
    cs->current = newContext;
    return 0;
}

int popContext(contextStack *cs) {
    contextNode *aux = cs->current;
    cs->current = aux->next;
    freeContext(aux);
    return 0;
}

int addToContext(contextStack *cs, const char *id, DataType *type) {
    int error = 10;
    cs->current->head = addToList(cs->current->head, id, type, &error);
    return error;
}

static DataType *retrieveTypeFromList(elementNode *list, const char *id) {
    elementNode *iter = list;
    while(iter != NULL) {
        if(strcmp(iter->id, id) == 0) {
            return iter->type;
        }
        iter = iter->next;
    }
    return NULL;
}

DataType *retrieveType(contextStack *cs, const char *id) {
    contextNode *iter = cs->current;
    DataType *ans = NULL;
    while(iter != NULL) {
        ans = retrieveTypeFromList(iter->head, id);
        if(ans != NULL) break;
        iter = iter->next;
    }
    return ans;
}


void freeStack(contextStack *cs) {
    contextNode *iter = cs->current;

    while(iter != NULL) {
        contextNode *aux = iter;
        iter = iter->next;
        freeContext(aux);
    }
    free(cs);
}

static void freeContext(contextNode *cn) {
    if(cn == NULL) return;
    freeList(cn->head);
    free(cn);
}


static void freeList(elementNode *en) {
    if(en == NULL) return;
    freeList(en->next);
    free(en->id);
    free(en);
}

static elementNode *addToList(elementNode *list, const char *id, DataType *type, int *error) {
    int c;
    if(list == NULL || (c = strcmp(id, list->id)) < 0) {
        elementNode *newNode = malloc(sizeof(struct elementNode));
        if(newNode == NULL) {
            *error = -1;
            return NULL;
        }
        size_t len = strlen(id);
        newNode->id = malloc(len + 1);
        if(newNode->id == NULL) {
            *error = -1;
            return NULL;
        }
        strcpy(newNode->id, id);
        newNode->type = type;
        newNode->next = list;
        *error = 0;
        return newNode;
    } else if(c == 0) {
        *error = -1;
        return list;
    } else {
        list->next = addToList(list->next, id, type, error);
        return list;
    }
}
