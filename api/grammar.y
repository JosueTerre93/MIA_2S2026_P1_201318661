%{
#include <iostream>
#include <string>

#include "Comandos/Mkdisk.h"
#include "Comandos/Fdisk.h"
#include "Comandos/Mount.h"
%}
 
%require "3.7.4"
%language "C++"
%defines "Parser1.hpp"
%output "Parser1.cpp"
 
%define api.parser.class {Parser}
%define api.namespace {calc}
%define api.value.type variant
%define parse.error detailed

%locations

%param {yyscan_t scanner}

%code requires
{
  typedef void* yyscan_t;
  #include "Comandos/Mkdisk.h"
  #include "Comandos/Fdisk.h"
}
 
%code provides
{
    #define YY_DECL \
    int yylex(calc::Parser::semantic_type *yylval_param, \
              calc::Parser::location_type *yylloc_param, \
              yyscan_t yyscanner)

    YY_DECL;

    std::string obtenerUltimoToken();
}

/*
GRAMATICA NUEVA A IMPLEMENTAR:

MOUNTED (no lleva parametros)

CAT -filen (enlaza archivos)

LOGIN -user(cadena) -pass(password) -id(cadena)

LOGOUT (no lleva parametros)

- Los siguientes comandos necesitan una sesion iniciada

MKGRP -name(cadena)

RMGRP -name(cadena)

MKUSR -usr(cadena) -pass(password) -grp(cadena)

RMUSR -usr(cadena)

CHGRP -usr(cadena) -grp(cadena)

- Administracion de carpetas, archivos y permisos

MKDIR -path(ruta) -p

Reportes

REP -name(cadena) -path(ruta) -id(cadena) -path_file_list(ruta)




*/
%token SIZE
%token FIT

%token BF
%token FF
%token WF
%token UNIT

%token B
%token K
%token M

%token PATH
%token TYPE

%token P
%token E
%token L
%token NAME

%token ID

%token USER
%token GROUP

%token R
%token COUNT
%token FULL

%token MKDISK
%token RMDISK
%token FDISK
%token MOUNT
%token MKFS
%token MKUSR
%token RMUSR
%token MKFILE

%token MOUNTED
%token CAT
%token FILEN
%token LOGIN
%token LOGOUT
%token MKGRP
%token RMGRP
%token CHGRP
%token MKDIR
%token REP
%token PATH_FILE_LIST

%token PASSWORD

%token <long long> INTEGER
%token <std::string> PATH_VALUE
%token <std::string> ID_VALUE
%token <std::string> PASSWORD_VALUE

%token EQUAL
%token EOL

%type <std::string> fit_v
%type <std::string> unit_v
%type <std::string> unit_v2
%type <std::string> type_v


%code
{
    namespace calc 
    {
        MkdiskParams mkdiskActual;
        FdiskParams fdiskActual;
        MountParams mountActual;
    }
} // %code
 
%%
comandos   : %empty {  }
        | comandos comando
        ;
 
comando    : EOL  { std::cerr << "No se encontraron comandos.\n"; }
        | mkdisk EOL { std::cerr << "MKDISK ejecutado.\n"; }
        | rmdisk EOL { std::cerr << "RMDISK ejecutado.\n"; }
        | fdisk EOL { std::cerr << "FDISK ejecutado.\n"; }
        | mount EOL { std::cerr << "MOUNT ejecutado.\n"; }
        | mkfs EOL { std::cerr << "MKFS ejecutado.\n"; }
        | mkusr EOL { std::cerr << "MKUSR ejecutado.\n"; }
        | rmusr EOL { std::cerr << "RMUSR ejecutado.\n"; }
        | mkfile EOL { std::cerr << "MKFILE ejecutado.\n"; }

        | mounted EOL { std::cerr << "MOUNTED ejecutado.\n"; }
        | cat EOL { std::cerr << "CAT ejecutado.\n"; }
        | login EOL { std::cerr << "LOGIN ejecutado.\n"; }
        | logout EOL { std::cerr << "LOGOUT ejecutado.\n"; }
        | mkgrp EOL { std::cerr << "MKGRP ejecutado.\n"; }
        | rmgrp EOL { std::cerr << "RMGRP ejecutado.\n"; }
        | chgrp EOL { std::cerr << "CHGRP ejecutado.\n"; }
        | mkdir EOL { std::cerr << "MKDIR ejecutado.\n"; }
        | rep EOL { std::cerr << "REP ejecutado.\n"; }
        | error EOL { std::cerr << "Comando no valido.\n"; yyerrok; }
        ;

/*----------------------------------------*/
mkdisk 
        : MKDISK 
        {
            std::cout 
                << ">>> INICIO MKDISK EN BISON"
                << std::endl;
            
            calc::mkdiskActual = MkdiskParams();
        }
        mkdisk_params
        {
            std::cerr << ejecutarMkdisk(calc::mkdiskActual) << std::endl;
        }
;

mkdisk_params : mkdisk_params mkdisk_p
        |mkdisk_p
        ;

mkdisk_p : SIZE EQUAL INTEGER { calc::mkdiskActual.size = $3; }
        | FIT EQUAL fit_v { calc::mkdiskActual.fit = $3; }
        | UNIT EQUAL unit_v { calc::mkdiskActual.unit = $3; }
        | PATH EQUAL PATH_VALUE { calc::mkdiskActual.path = $3; }
        ;

fit_v : BF { $$ = "BF"; }
        | FF { $$ = "FF"; }
        | WF { $$ = "WF"; }
        ;

unit_v : K { $$ = "K"; }
        | M { $$ = "M"; }
        ;

/*--------------------------------------*/
rmdisk : RMDISK PATH EQUAL PATH_VALUE;

/*--------------------------------------*/
fdisk
    : FDISK
    {
        calc::fdiskActual = FdiskParams();
    }
    fdisk_params
    {
        std::cerr
            << ejecutarFdisk(
                calc::fdiskActual
            )
            << std::endl;
    }
;
fdisk_params : fdisk_params fdisk_p
        |fdisk_p
        ;

fdisk_p : SIZE EQUAL INTEGER { calc::fdiskActual.size = $3; }
        | UNIT EQUAL unit_v2 { calc::fdiskActual.unit = $3; }
        | PATH EQUAL PATH_VALUE { calc::fdiskActual.path = $3; }
        | TYPE EQUAL type_v { calc::fdiskActual.type = $3; }
        | FIT EQUAL fit_v { calc::fdiskActual.fit = $3; }
        | NAME EQUAL ID_VALUE { calc::fdiskActual.name = $3; }  
        ;

unit_v2 : B{ $$ = "B"; }
        | K{ $$ = "K"; }
        | M{ $$ = "M"; }
        ;

type_v : P { $$ = "P"; }
        | E { $$ = "E"; }
        | L { $$ = "L"; }
        ;

/*-------------------------------------*/
mount
    : MOUNT
    {
        calc::mountActual = MountParams();
    }
    mount_params
    {
        std::cerr
            << ejecutarMount(calc::mountActual)
            << std::endl;
    }
;

mount_params : mount_params mount_p
        | mount_p
        ;

mount_p : PATH EQUAL PATH_VALUE { calc::mountActual.path = $3; }
        | NAME EQUAL ID_VALUE { calc::mountActual.name = $3; }
        ;

/*------------------------------------*/
mkfs : MKFS mkfs_params
;

mkfs_params : mkfs_params mkfs_p
        | mkfs_p
        ;

mkfs_p : ID EQUAL ID_VALUE
        | TYPE EQUAL FULL
        ;

/*------------------------------------*/
mkusr : MKUSR mkusr_params
;

mkusr_params : mkusr_params mkusr_p
        | mkusr_p
        ;

mkusr_p : USER EQUAL ID_VALUE
        | PASSWORD EQUAL PASSWORD_VALUE
        | GROUP EQUAL ID_VALUE
        ;

/*-------------------------------------*/
rmusr : RMUSR USER EQUAL ID_VALUE
;

/*--------------------------------------*/
mkfile : MKFILE mkfile_params
;

mkfile_params : mkfile_params mkfile_p
        | mkfile_p
        ;

mkfile_p : PATH EQUAL PATH_VALUE
        | R
        | SIZE EQUAL INTEGER
        | COUNT EQUAL PATH_VALUE
        ;

/*--------------------------------------*/
mounted
    : MOUNTED
    {
        std::cerr
            << mostrarParticionesMontadas()
            << std::endl;
    }
;
/*--------------------------------------*/
cat : CAT cat_params;

cat_params : cat_params cat_p
        | cat_p
        ;

cat_p : FILEN EQUAL PATH_VALUE
        ;

/*--------------------------------------*/
login : LOGIN login_params;

login_params : login_params login_p
        | login_p
        ;

login_p : USER EQUAL ID_VALUE
        | PASSWORD EQUAL PASSWORD_VALUE
        | ID EQUAL ID_VALUE
        ;

/*--------------------------------------*/
logout : LOGOUT;

/*--------------------------------------*/
mkgrp : MKGRP NAME EQUAL ID_VALUE
        ;

/*--------------------------------------*/
rmgrp : RMGRP NAME EQUAL ID_VALUE
        ;

/*--------------------------------------*/
chgrp : CHGRP chgrp_params;

chgrp_params : chgrp_params chgrp_p
        | chgrp_p
        ;

chgrp_p : USER EQUAL ID_VALUE
        | GROUP EQUAL ID_VALUE
        ;

/*--------------------------------------*/
mkdir : MKDIR mkdir_params;

mkdir_params : mkdir_params mkdir_p
        | mkdir_p
        ;

mkdir_p : PATH EQUAL PATH_VALUE
        | P
        ;

/*--------------------------------------*/
rep : REP rep_params;

rep_params : rep_params rep_p
        | rep_p
        ;

rep_p : NAME EQUAL ID_VALUE
        | PATH EQUAL PATH_VALUE
        | ID EQUAL ID_VALUE
        | PATH_FILE_LIST EQUAL PATH_VALUE
        ;

%%

#include "Scanner1.hpp"
 
void calc::Parser::error(
    const location_type& loc,
    const std::string& msg
)
{
    std::cerr
        << "Error sintáctico en línea "
        << loc.begin.line
        << ", columna "
        << loc.begin.column
        << std::endl;

    std::cerr
        << "Token encontrado: \""
        << obtenerUltimoToken()
        << "\""
        << std::endl;

    std::cerr
        << "Detalle: "
        << msg
        << std::endl;
}