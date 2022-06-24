#include "contextStack.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct elementNode {
    char *id;
    void *data;
    struct elementNode *next;
}elementNode;


typedef struct contextNode {
    elementNode *head;
    struct contextNode *next;
} contextNode;

struct contextStack {
    contextNode *current;
    size_t size;
};

static void freeContext(contextNode *cn);
static void freeList(elementNode *en);
static elementNode *addToList(elementNode *list, const char *id, void *data, size_t size, int *error);


contextStack *newContextStack(size_t size) {
    contextStack *cs = malloc(sizeof(struct contextStack));
    cs->current = NULL;
    cs->size = size;
    return cs;
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

int addToContext(contextStack *cs, const char *id, void *type) {
    int error = 10;
    if(cs->current == NULL)
        pushContext(cs);
    elementNode* en = addToList(cs->current->head, id, type, cs->size, &error);
    cs->current->head = en;
    return error;
}

static void *retrieveTypeFromList(elementNode *list, const char *id, size_t size) {
    elementNode *iter = list;
    while(iter != NULL) {
        if(strcmp(iter->id, id) == 0) {
            void *ans = malloc(size);
            if(ans == NULL) goto finally;
            memcpy(ans, iter->data, size);
            return ans;
        }
        iter = iter->next;
    }
    finally:
    return NULL;
}

void *retrieveData(contextStack *cs, const char *id) {
    contextNode *iter = cs->current;
    void *ans = NULL;
    while(iter != NULL) {
        ans = retrieveTypeFromList(iter->head, id, cs->size);
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
    free(en->data);
    free(en->id);
    free(en);
}

static elementNode *addToList(elementNode *list, const char *id, void *data, size_t size, int *error) {
    int c;
    if(list == NULL || (c = strcmp(id, list->id)) < 0) {
        elementNode *newNode = malloc(sizeof(struct elementNode));
        if(newNode == NULL) {
            goto finally;
        }

        size_t len = strlen(id);
        newNode->id = malloc(len + 1);
        if(newNode->id == NULL) {
            goto finally;
        }
        strcpy(newNode->id, id);

        newNode->data = malloc(size);
        if(newNode->data == NULL) {
            goto finally;
        }
        memcpy(newNode->data, data, size);

        newNode->next = list;
        *error = 0;
        return newNode;

        finally:
        *error = -1;
    } else if(c == 0) {
        *error = -1;
    } else {
        list->next = addToList(list->next, id, data, size, error);
    }
    return list;
}
