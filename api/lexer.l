%top {
#include "Parser1.hpp"
#define YYSTYPE calc::Parser::semantic_type
}

%{
#include <string>
#include <cstdlib>
#include <iostream>

#include "Parser1.hpp"

using namespace calc;
%}

%option reentrant
%option bison-bridge
%option noyywrap
%option noyylineno
%option nodefault
%option outfile="Scanner1.cpp"
%option header-file="Scanner1.hpp"


/*

COMANDOS A IMPLEMENTAR:

MKDISK
    *-size ENTERO
    -fit BF | FF | WF
    -unit K | M
    *-path RUTA

RMDISK
    *-path RUTA

FDISK
    *-size ENTERO
    -unit B | K | M
    *-path RUTA
    -type P | E | L
    -fit BF | FF | WF
    *-name IDENTIFICADOR

MOUNT
    *-path RUTA
    *-name IDENTIFICADOR

MKFS
    *-id ID
    -type = full

MKUSR
    *-user USER (maximo 10)
    *-pass PASSWORD (maximo 10)
    *-grp GROUP (maixmo 10)

RMUSR
    *-user USER (maximo 10)

MKFILE
    *-path RUTA
    -r 
    -size ENTERO
    -count 
*/
 
/*------------EXPRESIONES REGULARES--------------*/
INTEGER          [0-9]+
PATH_VALUE       \"\/[^"\r\n]*\"|\/[^ \t\r\n]+
PASSWORD_VALUE   [a-zA-Z0-9]{1,10}
ID_VALUE         [a-zA-Z_][a-zA-Z0-9_]*

 
%%

    /*------------ESPACIOS EN BLANCO------------------*/
[ \t\r]+        ;

    /*------------------COMANDOS----------------------*/

"mkdisk"    return Parser::token::MKDISK;
"rmdisk"    return Parser::token::RMDISK;
"fdisk"     return Parser::token::FDISK;
"mount"     return Parser::token::MOUNT;
"mkfs"      return Parser::token::MKFS;
"mkusr"     return Parser::token::MKUSR;
"rmusr"     return Parser::token::RMUSR;
"mkfile"    return Parser::token::MKFILE;

"mounted"   return Parser::token::MOUNTED;
"cat"       return Parser::token::CAT;
"login"     return Parser::token::LOGIN;
"logout"    return Parser::token::LOGOUT;
"mkgrp"     return Parser::token::MKGRP;
"rmgrp"     return Parser::token::RMGRP;
"chgrp"     return Parser::token::CHGRP;
"mkdir"     return Parser::token::MKDIR;
"rep"       return Parser::token::REP;

    /*-------------------PARAMETROS-------------------*/
"-size" return Parser::token::SIZE;

"-fit"  return Parser::token::FIT;

"BF"    return Parser::token::BF;
"FF"    return Parser::token::FF;
"WF"    return Parser::token::WF;

"-unit" return Parser::token::UNIT;

"B"     return Parser::token::B;
"K"     return Parser::token::K;
"M"     return Parser::token::M;

"-path" return Parser::token::PATH;

"-type" return Parser::token::TYPE;

"P"|"-p" return Parser::token::P;
"E"     return Parser::token::E;
"L"     return Parser::token::L;

"-name" return Parser::token::NAME;

"-id"           return Parser::token::ID;

"-user"|"-usr" return Parser::token::USER;
"-pass" return Parser::token::PASSWORD;
"-grp"  return Parser::token::GROUP;

"-r"        return Parser::token::R; 
"-count"    return Parser::token::COUNT;

"-filen"|"-file"[0-9]+|"-file" return Parser::token::FILEN;
"-path_file_list"              return Parser::token::PATH_FILE_LIST;

"full"  return Parser::token::FULL;

"="         return Parser::token::EQUAL;

{INTEGER} {
    yylval_param->emplace<long long>(
        strtoll(yytext, nullptr, 10)
    );

    return Parser::token::INTEGER;
}

{PATH_VALUE} {
    yylval_param->emplace<std::string>(yytext);
    return Parser::token::PATH_VALUE;
}

{ID_VALUE} {
    yylval_param->emplace<std::string>(yytext);
    return Parser::token::ID_VALUE;
}

{PASSWORD_VALUE} {
    yylval_param->emplace<std::string>(yytext);
    return Parser::token::PASSWORD_VALUE;
}

    /*------------------- FIN DE LINEA -----------------*/

\n              return Parser::token::EOL;


    /*------------------- OTROS CARACTERES -------------*/

. {
    std::cerr << "Caracter no reconocido: " << yytext << std::endl;
}
 
%%

/*
int main() {
    yyscan_t scanner;
    yylex_init(&scanner);
    calc::Parser parser{ scanner };
    std::cout.precision(10);
    parser.parse();
    yylex_destroy(scanner);
}
 */
