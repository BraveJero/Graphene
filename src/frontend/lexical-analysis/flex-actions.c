#include "../../backend/support/logger.h"
#include "flex-actions.h"
#include <stdlib.h>

/**
 * Implementación de "flex-rules.h".
 */

TokenID IntegerPatternAction(const char * lexeme) {
	printf("IntegerPatternAction: '%s'.", lexeme);
	yylval = atoi(lexeme);
	return INTEGER;
}

void IgnoredPatternAction(const char * lexeme) {
	printf("IgnoredPatternAction: '%s'.", lexeme);
}

TokenID UnknownPatternAction(const char * lexeme) {
	printf("UnknownPatternAction: '%s'.", lexeme);
	return YYUNDEF;
}

TokenID StringPatternAction(const char * lexeme) {
	printf("StringPatternAction: '%s'.", lexeme);
	return STRING;
}

TokenID DecimalPatternAction(const char * lexeme) {
	printf("DecimalPatternAction: '%s'.", lexeme);
	return DECIMAL;
}
