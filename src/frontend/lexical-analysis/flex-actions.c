#include "../../backend/support/logger.h"
#include "flex-actions.h"
#include <stdlib.h>

/**
 * Implementación de "flex-rules.h".
 */

void IgnoredPatternAction(const char * lexeme) {
	printf("IgnoredPatternAction: '%s'.\n", lexeme);
}

TokenID UnknownPatternAction(const char * lexeme) {
	printf("UnknownPatternAction: '%s' que es %d.\n", lexeme, *lexeme);
	return YYUNDEF;
}

TokenID KeywordPatternAction(const char * lexeme, TokenID token) {
	printf("KeywordPatternAction: '%s'.\n", lexeme);
	return token;
}

TokenID BooleanPatternAction(const char * lexeme, char value) {
	printf("BooleanPatternAction: '%s'.\n", lexeme);
	// yylval = value;
	return BOOLEAN;
}

TokenID CharPatternAction(const char * lexeme) {
	printf("CharPatternAction: '%s'.\n", lexeme);
	// yylval = *lexeme;
	return CHAR;
}

TokenID IntegerPatternAction(const char * lexeme) {
	printf("IntegerPatternAction: '%s'.\n", lexeme);
	// yylval = atoi(lexeme);
	return INTEGER;
}

TokenID StringPatternAction(const char * lexeme) {
	printf("StringPatternAction: '%s'.\n", lexeme);
	return STRING;
}

TokenID DecimalPatternAction(const char * lexeme) {
	printf("DecimalPatternAction: '%s'.\n", lexeme);
	return DECIMAL;
}

TokenID IndentationPatternAction(TokenID token) {
	printf("IndentationPatternAction: '%s'.\n", (token == INDENT) ? "INDENT" : "DEDENT");
	return DECIMAL;
}

TokenID EndOfLinePatternAction(TokenID token) {
	printf("EndOfLinePatternAction.\n");
	return token;
}