#ifndef FLEX_ACTIONS_HEADER
#define FLEX_ACTIONS_HEADER

#include "../../backend/support/shared.h"

/**
 * Se definen los diferentes IDs de cada token disponible para el scanner Flex.
 */
typedef enum TokenID {

	// Por defecto, el valor "0" hace fallar el analizador sintáctico.
	UNKNOWN = 0,

	// Código de error de Bison, que permite abortar el escaneo de lexemas cuando
	// se presente un patrón desconocido. El número "257" coincide con el valor
	// que Bison le otorga por defecto, pero además permite que el resto de
	// tokens continúen desde el valor "258" lo que permite proteger los IDs
	// internos que Bison reserva para crear "tokens literales":
	YYUNDEF = 257,

	// Operadores aritméticos.
	ADD,
	SUB,
	MUL,
	DIV,

	// Paréntesis.
	OPEN_PARENTHESIS,
	CLOSE_PARENTHESIS,

	// Tipos de dato.
	INTEGER,
	US_ASCII_TEXT,
	START,
	CREATE,
	INSERT,
	INTO,
	FOR,
	WHILE,
	CHAR,
	DECIMAL,
	STRING,
	BOOLEAN,
	NODE,
	EDGE,
	GRAPH,
	FROM,
	IF,
	ELSE,
	TO,
	INCLUSIVE,
	EXCLUSIVE,
	WITH,
	VALUE,
	DFS,
	BFS,
	IS,
	NOT,
	EMPTY_TYPE,
	POP,
	ENTRY,
	PRINT,
	DEF,
	LABEL,
	GET,
	IN,
	EQ,
	LT,
	GT,
	GEQ,
	LEQ,
	AND,
	OR,
	RETURN,
	DUMP,
	GRAPHVIZ_DOT,
	LET,
	BE,
	QUEUE,
	STACK,
	SET,
	DIGRAPH,
	INTEGER_TYPE,
	ANY_TYPE,
	BOOLEAN_TYPE,
	DECIMAL_TYPE,
	CHAR_TYPE,
	STRING_TYPE,
	OPEN_ANTILAMBDA,
	CLOSE_ANTILAMBDA,
	COMMA,
	COLON,
	DOT,
	SEMICOLON,
	LEFT_ARROW,
	RIGHT_ARROW,
	IDENTIFIER,
	EOL,
	INDENT,
	DEDENT,
} TokenID;

/**
 * Se definen las acciones a ejecutar sobre cada patrón hallado mediante el
 * analizador léxico Flex. Este analizador solo puede identificar
 * construcciones regulares, ya que utiliza un autómata finito determinístico
 * (a.k.a. DFA), como mecanismo de escaneo y reconocimiento.
 */

void IgnoredPatternAction(const char * lexeme);
TokenID KeywordPatternAction(const char * lexeme, TokenID token);
TokenID UnknownPatternAction(const char * lexeme);
TokenID BooleanPatternAction(const char * lexeme, char value);
TokenID CharPatternAction(const char * lexeme);
TokenID StringPatternAction(const char * lexeme);
TokenID IntegerPatternAction(const char * lexeme);
TokenID DecimalPatternAction(const char * lexeme);
TokenID IndentationPatternAction(TokenID token);
TokenID EndOfLinePatternAction(TokenID token);

#endif
