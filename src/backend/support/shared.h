#ifndef SHARED_HEADER
#define SHARED_HEADER

#include <stdio.h>

#include "../domain-specific/ast.h"
#include "../semantic-analysis/contextStack.h"

// Descriptor del archivo de entrada que utiliza Bison.
extern FILE * yyin;

// Descriptor del archivo de salida que utiliza Bison.
extern FILE * yyout;

// Variable global que contiene el número escaneado.
// extern int yylval;

// Variable global que contiene el número de la línea analizada.
extern int yylineno;

// Token actual en el tope de la pila del analizador Bison.
extern char * yytext;

// Función global de manejo de errores en Bison.
extern void yyerror(const char * string);

// Función global del analizador léxico Flex.
extern int yylex(void);

// Función global del analizador sintáctico Bison.
extern int yyparse(void);

// Estado global de toda la aplicación.
typedef struct {

	// Indica si la compilación tuvo problemas hasta el momento:
	boolean succeed;

	// Indica el resultado de la compilación:
	int result;

	// Agregar un nodo hacia la raíz del árbol de sintaxis abstracta.
    struct Program* program;
	// Agregar una pila para almacenar tokens/nodos.
	// Agregar una tabla de símbolos.
    struct contextStack* stfv; // Symbol Table For Variables
	struct contextStack* stfr; // Symbol Table For Return
	struct contextStack* stfa; // Symbol Table For Arguments

	// ...


} CompilerState;

// El estado se define e inicializa en el archivo "main.c":
extern CompilerState state;

#endif
