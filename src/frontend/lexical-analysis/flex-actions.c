#include <stdlib.h>
#include <string.h>

#include "../../backend/support/logger.h"
#include "flex-actions.h"

/**
 * Implementación de "flex-rules.h".
 */

void IgnoredPatternAction(const char * lexeme) {
	LogDebug("IgnoredPatternAction: '%s'.\n", lexeme);
}

TokenID UnknownPatternAction(const char * lexeme) {
	LogDebug("UnknownPatternAction: '%s' que es %d.\n", lexeme, *lexeme);
	yylval.token = YYUNDEF;
	return YYUNDEF;
}

TokenID KeywordPatternAction(const char * lexeme, TokenID token) {
	LogDebug("KeywordPatternAction: '%s' with %d.\n", lexeme, token);
	yylval.token = token;
	return token;
}

TokenID BooleanPatternAction(const char * lexeme, char value) {
	LogDebug("BooleanPatternAction: '%s'.\n", lexeme);
	// yylval = value;
	yylval.bool = value;
	return BOOLEAN;
}

TokenID CharPatternAction(const char * lexeme) {
	LogDebug("CharPatternAction: '%s'.\n", lexeme);
	// yylval = *lexeme;
	yylval.letter = *lexeme;
	return CHAR;
}

TokenID IntegerPatternAction(const char * lexeme) {
	LogDebug("IntegerPatternAction: '%s'.\n", lexeme);
	// yylval = atoi(lexeme);
	yylval.integer = atoi(lexeme);
	return INTEGER;
}

TokenID StringPatternAction(const char * lexeme) {
	LogDebug("StringPatternAction: '%s'.\n", lexeme);
	yylval.string = strdup(lexeme);
	return STRING;
}

TokenID DecimalPatternAction(const char * lexeme) {
	LogDebug("DecimalPatternAction: '%s'.\n", lexeme);
	yylval.decimal = atof(lexeme);
	return DECIMAL;
}

TokenID IdentifierPatternAction(const char * lexeme) {
	LogDebug("IdentifierPatternAction: '%s'.\n", lexeme);
	yylval.string = strdup(lexeme);
	return IDENTIFIER;
}

TokenID IndentationPatternAction(TokenID token) {
	LogDebug("IndentationPatternAction: '%s'.\n", (token == INDENT) ? "INDENT" : "DEDENT");
	yylval.token = token;
	return token;
}

TokenID EndOfLinePatternAction(TokenID token) {
	LogDebug("EndOfLinePatternAction.\n");
	yylval.token = token;
	return token;
}

void CommentPatternAction(const char * lexeme) {
	LogDebug("CommentPatternAction: '%s'.\n", lexeme);
}