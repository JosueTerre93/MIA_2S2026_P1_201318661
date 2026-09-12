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

INTEGER          [0-9]+
PATH_VALUE       \"\/[^\"\r\n]+\"|\/[^ \t\r\n]+
PASSWORD_VALUE   [a-zA-Z0-9]{1,10}
ID_VALUE         [a-zA-Z_][a-zA-Z0-9_]*

%x ESPERA_PASSWORD

%%

[ \t\r]+        ;

"mkdisk"    { ultimo_token = yytext; return Parser::token::MKDISK; }
"rmdisk"    { ultimo_token = yytext; return Parser::token::RMDISK; }
"fdisk"     { ultimo_token = yytext; return Parser::token::FDISK; }
"mount"     { ultimo_token = yytext; return Parser::token::MOUNT; }
"mkfs"      { ultimo_token = yytext; return Parser::token::MKFS; }
"mkusr"     { ultimo_token = yytext; return Parser::token::MKUSR; }
"rmusr"     { ultimo_token = yytext; return Parser::token::RMUSR; }
"mkfile"    { ultimo_token = yytext; return Parser::token::MKFILE; }

"mounted"   { ultimo_token = yytext; return Parser::token::MOUNTED; }
"cat"       { ultimo_token = yytext; return Parser::token::CAT; }
"login"     { ultimo_token = yytext; return Parser::token::LOGIN; }
"logout"    { ultimo_token = yytext; return Parser::token::LOGOUT; }
"mkgrp"     { ultimo_token = yytext; return Parser::token::MKGRP; }
"rmgrp"     { ultimo_token = yytext; return Parser::token::RMGRP; }
"chgrp"     { ultimo_token = yytext; return Parser::token::CHGRP; }
"mkdir"     { ultimo_token = yytext; return Parser::token::MKDIR; }
"rep"       { ultimo_token = yytext; return Parser::token::REP; }

"-size" { ultimo_token = yytext; return Parser::token::SIZE; }
"-fit"  { ultimo_token = yytext; return Parser::token::FIT; }

"BF"    { ultimo_token = yytext; return Parser::token::BF; }
"FF"    { ultimo_token = yytext; return Parser::token::FF; }
"WF"    { ultimo_token = yytext; return Parser::token::WF; }

"-unit" { ultimo_token = yytext; return Parser::token::UNIT; }

"B"     { ultimo_token = yytext; return Parser::token::B; }
"K"     { ultimo_token = yytext; return Parser::token::K; }
"M"     { ultimo_token = yytext; return Parser::token::M; }

"-path" { ultimo_token = yytext; return Parser::token::PATH; }
"-type" { ultimo_token = yytext; return Parser::token::TYPE; }

"P"|"-p" { ultimo_token = yytext; return Parser::token::P; }
"E"       { ultimo_token = yytext; return Parser::token::E; }
"L"       { ultimo_token = yytext; return Parser::token::L; }

"-name" { ultimo_token = yytext; return Parser::token::NAME; }
"-id"   { ultimo_token = yytext; return Parser::token::ID; }

"-user"|"-usr" { ultimo_token = yytext; return Parser::token::USER; }

"=" { ultimo_token = yytext; return Parser::token::EQUAL; }

"-pass" {
    ultimo_token = yytext;
    BEGIN(ESPERA_PASSWORD);
    return Parser::token::PASSWORD;
}

"-grp" { ultimo_token = yytext; return Parser::token::GROUP; }

"-r"     { ultimo_token = yytext; return Parser::token::R; }
"-count" { ultimo_token = yytext; return Parser::token::COUNT; }

"-filen"|"-file"[0-9]+|"-file" {
    ultimo_token = yytext;
    return Parser::token::FILEN;
}

"-path_file_list" {
    ultimo_token = yytext;
    return Parser::token::PATH_FILE_LIST;
}

"full" {
    ultimo_token = yytext;
    return Parser::token::FULL;
}

<ESPERA_PASSWORD>[ \t\r]+ ;

<ESPERA_PASSWORD>"=" {
    ultimo_token = yytext;
    return Parser::token::EQUAL;
}

<ESPERA_PASSWORD>{PASSWORD_VALUE} {
    ultimo_token = yytext;
    yylval_param->emplace<std::string>(yytext);
    BEGIN(INITIAL);
    return Parser::token::PASSWORD_VALUE;
}

<ESPERA_PASSWORD>\n {
    ultimo_token = "\\n";
    BEGIN(INITIAL);
    yyset_column(1, yyscanner);
    return Parser::token::EOL;
}

<ESPERA_PASSWORD>. {
    ultimo_token = yytext;
    std::cerr
        << "Caracter no valido en password: "
        << yytext
        << std::endl;
    BEGIN(INITIAL);
}

[0-9]{3}[A-Z] {
    ultimo_token = yytext;
    yylval_param->emplace<std::string>(yytext);
    return Parser::token::MOUNT_ID;
}

{INTEGER} {
    ultimo_token = yytext;
    yylval_param->emplace<long long>(
        strtoll(yytext, nullptr, 10)
    );
    return Parser::token::INTEGER;
}

{PATH_VALUE} {
    ultimo_token = yytext;
    yylval_param->emplace<std::string>(yytext);
    return Parser::token::PATH_VALUE;
}

{ID_VALUE} {
    ultimo_token = yytext;
    yylval_param->emplace<std::string>(yytext);
    return Parser::token::ID_VALUE;
}

\n {
    ultimo_token = "\\n";
    yyset_column(1, yyscanner);
    return Parser::token::EOL;
}

. {
    ultimo_token = yytext;
    std::cerr
        << "Caracter no reconocido: "
        << yytext
        << std::endl;
}

%%

std::string obtenerUltimoToken()
{
    return ultimo_token;
}