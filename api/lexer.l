%top {
#include "Parser1.hpp"

#define YYSTYPE calc::Parser::semantic_type
#define YYLTYPE calc::Parser::location_type
}

%{
#include <string>
#include <cstdlib>
#include <iostream>

#include "Parser1.hpp"

using namespace calc;

static std::string ultimo_token;

#define YY_USER_ACTION                                      \
    yylloc_param->begin.line = yyget_lineno(yyscanner);    \
    yylloc_param->begin.column = yyget_column(yyscanner);  \
    yylloc_param->end.line = yyget_lineno(yyscanner);      \
    yylloc_param->end.column = yyget_column(yyscanner) + yyleng; \
    yyset_column(yyget_column(yyscanner) + yyleng, yyscanner);
%}

%option reentrant
%option bison-bridge
%option bison-locations
%option noyywrap
%option yylineno
%option nodefault
%option outfile="Scanner1.cpp"
%option header-file="Scanner1.hpp"
 
/*------------EXPRESIONES REGULARES--------------*/
INTEGER          [0-9]+
PATH_VALUE       \"\/[^"\r\n]*\"|\/[^ \t\r\n]+
PASSWORD_VALUE   [a-zA-Z0-9]{1,10}
ID_VALUE         [a-zA-Z_][a-zA-Z0-9_]*

%x ESPERA_PASSWORD
 
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
"="         return Parser::token::EQUAL;

"-pass" {
    ultimo_token = yytext;
    BEGIN(ESPERA_PASSWORD);
    return Parser::token::PASSWORD;
}

"-grp"  return Parser::token::GROUP;

"-r"        return Parser::token::R; 
"-count"    return Parser::token::COUNT;

"-filen"|"-file"[0-9]+|"-file" return Parser::token::FILEN;
"-path_file_list"              return Parser::token::PATH_FILE_LIST;

"full"  return Parser::token::FULL;

<ESPERA_PASSWORD>"=" {
    ultimo_token = yytext;
    return Parser::token::EQUAL;
}

<ESPERA_PASSWORD>[a-zA-Z0-9]{1,10} {
    ultimo_token = yytext;

    yylval_param->emplace<std::string>(yytext);

    BEGIN(INITIAL);

    return Parser::token::PASSWORD_VALUE;
}

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

[a-zA-Z_][a-zA-Z0-9_]* {
    ultimo_token = yytext;
    yylval_param->emplace<std::string>(yytext);
    return Parser::token::ID_VALUE;
}

{ID_VALUE} {
    yylval_param->emplace<std::string>(yytext);
    return Parser::token::ID_VALUE;
}

    /*------------------- FIN DE LINEA -----------------*/

\n {
    yyset_column(1, yyscanner);
    return Parser::token::EOL;
}


    /*------------------- OTROS CARACTERES -------------*/

. {
    std::cerr << "Caracter no reconocido: " << yytext << std::endl;
}
 
%%

std::string obtenerUltimoToken()
{
    return ultimo_token;
}

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
