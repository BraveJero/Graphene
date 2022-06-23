#ifndef GENERATOR_HEADER
#define GENERATOR_HEADER

#include "../domain-specific/ast.h"
#include "../support/shared.h"

#define TEMP_FILENAME   "__temp__"

boolean Generator(CompilerState* state, const char * outputFileName);

#endif
