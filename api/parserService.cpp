#include <iostream>
#include <string>
#include <sstream>

#include "httplib.h"

#include "Parser1.hpp"
#include "Scanner1.hpp"

using namespace std;
using namespace calc;


string ejecutarParser(const string& comando)
{
    // Crear scanner
    yyscan_t scanner;

    if (yylex_init(&scanner) != 0) {
        return "Error: no se pudo inicializar el scanner.";
    }

    // El parser necesita EOL al final
    string entrada = comando;

    if (entrada.empty() || entrada.back() != '\n') {
        entrada += '\n';
    }

    // Pasar el string al scanner
    YY_BUFFER_STATE buffer = yy_scan_string(
        entrada.c_str(),
        scanner
    );

    if (buffer == nullptr) {
        yylex_destroy(scanner);
        return "Error: no se pudo crear el buffer del scanner.";
    }


    // Capturar lo que actualmente imprime el parser por cerr
    ostringstream salida;

    streambuf* cerrOriginal = cerr.rdbuf(
        salida.rdbuf()
    );


    // Crear parser
    Parser parser(scanner);

    // Ejecutar parser
    int resultado = parser.parse();


    // Restaurar cerr
    cerr.rdbuf(cerrOriginal);


    // Liberar buffer
    yy_delete_buffer(buffer, scanner);

    // Destruir scanner
    yylex_destroy(scanner);


    // Si no hubo ningún mensaje
    string respuesta = salida.str();

    if (respuesta.empty()) {

        if (resultado == 0) {
            respuesta = "Comando ejecutado correctamente.";
        }
        else {
            respuesta = "Error al ejecutar el comando.";
        }
    }


    return respuesta;
}