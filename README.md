[![✗](https://img.shields.io/badge/Release-v0.1.0-ffb600.svg?style=for-the-badge)](https://github.com/agustin-golmar/Flex-Bison-Compiler/releases)

# Compilador Flex/Bison

Un compilador vacío construido con Flex y Bison.

## Requerimientos

Para construir el compilador, se requieren las siguientes dependencias:

* [Bison v3.8.2](https://www.gnu.org/software/bison/)
* [CMake v3.22.2](https://cmake.org/)
* [Flex v2.6.4](https://github.com/westes/flex)
* [GCC v11.1.0](https://gcc.gnu.org/)
* [Make v4.3](https://www.gnu.org/software/make/)

Si en lugar de trabajar con un entorno _Linux_, se está construyendo el proyecto sobre un entorno _Microsoft Windows_, se debe instalar _Microsoft Visual Studio 2022_ con las extensiones para desarrollar aplicaciones en _C/C++_, así como también las herramientas requeridas, con excepción del compilador _GCC_ y la herramienta _Make_.

## Construcción

Para construir el proyecto por completo, ejecute en la raíz del repositorio el siguiente comando:

```bash
user@machine:path/ $ chmod u+x build.sh
user@machine:path/ $ ./build.sh
```

En un entorno _Microsoft Windows_, en lugar de ejecutar el comando `make`, se deberá abrir la solución generada `bin/Compiler.sln` con el IDE _Microsoft Visual Studio 2022_. Los ejecutables que este sistema construye se depositan dentro del directorio `bin/Debug` y `bin/Release` según corresponda.

## Ejecución

Como ejemplo, puede compilar el archivo de ejemplo denominado `program`.

Ejecute el compilador desde el directorio raíz del proyecto, inyectando el programa desde la entrada estándar:

```bash
user@machine:path/ $ cat program | bin/Compiler
```

Deberia obtener el resultado correcto de evaluar el programa anterior: `Program.java`.

Tambien se puede realizar lo mismo con uno de los "usecases" o escribir su propio programa.

Para testear todos los usecases, recomendamos correr> `./test.sh`

Finalmente, habiendo generado su codigo java preferido, puede correrlo de la siguiente forma:
`mv Program.java out; cd out; javac *.java; java Program; rm *.class`

Por ultimo, de tener instalado DOT Graphviz, recomendamos correr el programa de prueba y luego correr: 'dot -Tsvg graph.out > output.svg' para ver el resultado final.