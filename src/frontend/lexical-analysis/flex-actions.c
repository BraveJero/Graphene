#include "../../backend/support/logger.h"
#include "flex-actions.h"
#include <stdlib.h>

/**
 * Implementación de "flex-rules.h".
 */

void IgnoredPatternAction(const char * lexeme) {
	LogDebug("IgnoredPatternAction: '%s'.\n", lexeme);
}

TokenID UnknownPatternAction(const char * lexeme) {
	LogDebug("UnknownPatternAction: '%s' que es %d.\n", lexeme, *lexeme);
	return YYUNDEF;
}

TokenID KeywordPatternAction(const char * lexeme, TokenID token) {
	LogDebug("KeywordPatternAction: '%s' with %d.\n", lexeme, token);
	return token;
}

TokenID BooleanPatternAction(const char * lexeme, char value) {
	LogDebug("BooleanPatternAction: '%s'.\n", lexeme);
	yylval = value;
	return BOOLEAN;
}

TokenID CharPatternAction(const char * lexeme) {
	LogDebug("CharPatternAction: '%s'.\n", lexeme);
	yylval = *lexeme;
	return CHAR;
}

TokenID IntegerPatternAction(const char * lexeme) {
	LogDebug("IntegerPatternAction: '%s'.\n", lexeme);
	yylval = atoi(lexeme);
	return INTEGER;
}

TokenID StringPatternAction(const char * lexeme) {
	LogDebug("StringPatternAction: '%s'.\n", lexeme);
	return STRING;
}

TokenID DecimalPatternAction(const char * lexeme) {
	LogDebug("DecimalPatternAction: '%s'.\n", lexeme);
	return DECIMAL;
}

TokenID IdentifierPatternAction(const char * lexeme) {
	LogDebug("IdentifierPatternAction: '%s'.\n", lexeme);
	return IDENTIFIER;
}

TokenID IndentationPatternAction(TokenID token) {
	LogDebug("IndentationPatternAction: '%s'.\n", (token == INDENT) ? "INDENT" : "DEDENT");
	return token;
}

TokenID EndOfLinePatternAction(TokenID token) {
	LogDebug("EndOfLinePatternAction.\n");
	return token;
}