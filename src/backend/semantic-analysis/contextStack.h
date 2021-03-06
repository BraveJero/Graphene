#pragma once

#include "../domain-specific/ast.h"
#include <stddef.h>

typedef struct contextStack contextStack;

// Retorna un nuevo stack vacío. ¿Habría que definir algún máximo?
contextStack *newContextStack(size_t size);

// Pushea un nuevo contexto. Retorna negativo si hubo error, 0 si salió bien.
int pushContext(contextStack *cs);

// Popea un contexto. Si pudo popear algo retorna 0, caso contrario -1.
int popContext(contextStack *cs);

// Agrega el par (id, data) al contexto. Negativo si hubo error, 0 si no.
int addToContext(contextStack *cs, const char *id, void *data);

// Retorna el data type asociado a id. NULL si no lo encuentra
void *retrieveData(contextStack *cs, const char *id);

//
void freeStack(contextStack *cs);

