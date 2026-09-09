%{
#include <iostream>
#include <string>
%}
 
%require "3.7.4"
%language "C++"
%defines "Parser1.hpp"
%output "Parser1.cpp"
 
%define api.parser.class {Parser}
%define api.namespace {calc}
%define api.value.type variant
%param {yyscan_t scanner}

%code requires
{
  typedef void* yyscan_t;
}
 
%code provides
{
        #define YY_DECL \
        int yylex(calc::Parser::semantic_type *yylval_param, yyscan_t yyscanner)

    YY_DECL;
}

/*
GRAMATICA A IMPLEMENTAR:

comandos -> comandos comando
        | comando

comando -> mkdisk
        | rmdisk
        | fdisk
        | mount
        | mkfs
        | mkusr
        | rmusr
        | mkfile

----------------------------------------
mkdisk -> MKDISK mkdisk_p

mkdisk_p -> SIZE EQUAL INTEGER
        | FIT EQUAL fit_V
        | UNIT EQUAL unit_v
        | PATH EQUAL PATH_V

fit_v -> BF
        | FF
        | WF

unit_v -> K
        | M

--------------------------------------
rmdisk -> RMDISK PATH EQUAL PATH_V

--------------------------------------
fdisk -> FDISK fdisk_p

fdisk_p -> SIZE EQUAL INTEGER
        | UNIT EQUAL unit_v2
        | PATH EQUAL PATH_V
        | TYPE EQUAL type_v
        | FIT EQUAL fit_v
        | NAME EQUAL ID_V

unit_v2 -> B
        | K
        | M

type_v -> P
        | E
        | L

-------------------------------------
mount -> MOUNT mount_p

mount_p -> PATH EQUAL PATH_V
        | NAME EQUAL ID_V

------------------------------------
mkfs -> MKFS mkfs_p

mkfs_p -> ID EQUAL ID_V
        TYPE EQUAL FULL

------------------------------------
mkusr -> MKUSER mkusr_p

mkusr_p -> USER EQUAL USER_V
        | PASSWORD EQUAL PASSWORD_V
        | GROUP GROUP_V

-------------------------------------
rmusr -> RMUSR USER EQUAL USER_V

--------------------------------------
mkfile -> MKFILE mkfile_p

mkfile_p -> PATH EQUAL PATH_V
        | R
        | SIZE EQUAL INTEGER
        | COUNT

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

%token PASSWORD

%token <long long> INTEGER
%token <std::string> PATH_VALUE
%token <std::string> ID_VALUE
%token <std::string> PASSWORD_VALUE

%token EQUAL
%token EOL

/*
%nterm <std::string> comandos
%nterm <std::string> comando
%nterm <std::string> mkdisk
%nterm <std::string> mkdisk_params
%nterm <std::string> mkdisk_p
%nterm <std::string> fit_v
%nterm <std::string> unit_v
%nterm <std::string> rmdisk
%nterm <std::string> fdisk
%nterm <std::string> fdisk_params
%nterm <std::string> fdisk_p
%nterm <std::string> unit_v2
%nterm <std::string> type_v
%nterm <std::string> mount
%nterm <std::string> mount_params
%nterm <std::string> mount_p
%nterm <std::string> mkfs
%nterm <std::string> mkfs_params
%nterm <std::string> mkfs_p
%nterm <std::string> mkusr
%nterm <std::string> mkusr_params
%nterm <std::string> mkusr_p
%nterm <std::string> rmusr
%nterm <std::string> mkfile_p
%nterm <std::string> mkfile
%nterm <std::string> mkfile_params
*/

%code
{
    namespace calc 
    {
    
    }
} // %code
 
%%
comandos   : %empty {  }
        | comandos comando
        ;
 
comando    : EOL  { std::cerr << "No se encontraron comandos.\n"; }
        | mkdisk EOL { std::cerr << "Comando mkdisk ejecutado.\n"; }
        | rmdisk EOL { std::cerr << "Comando rmdisk ejecutado.\n"; }
        | fdisk EOL { std::cerr << "Comando fdisk ejecutado.\n"; }
        | mount EOL { std::cerr << "Comando mount ejecutado.\n"; }
        | mkfs EOL { std::cerr << "Comando mkfs ejecutado.\n"; }
        | mkusr EOL { std::cerr << "Comando mkusr ejecutado.\n"; }
        | rmusr EOL { std::cerr << "Comando rmusr ejecutado.\n"; }
        | mkfile EOL { std::cerr << "Comando mkfile ejecutado.\n"; }
        /*| error EOL { std::cerr << "Comando no valido, revise que los parametros y los comandos esten bien escritos.\n"; yyerrok; }*/
        ;

/*----------------------------------------*/
mkdisk : MKDISK mkdisk_params
;

mkdisk_params : mkdisk_params mkdisk_p
        |mkdisk_p
        ;

mkdisk_p : SIZE EQUAL INTEGER
        | FIT EQUAL fit_v
        | UNIT EQUAL unit_v
        | PATH EQUAL PATH_VALUE
        ;

fit_v : BF
        | FF
        | WF
        ;

unit_v : K
        | M
        ;

/*--------------------------------------*/
rmdisk : RMDISK PATH EQUAL PATH_VALUE;

/*--------------------------------------*/
fdisk : FDISK fdisk_params
;

fdisk_params : fdisk_params fdisk_p
        |fdisk_p
        ;

fdisk_p : SIZE EQUAL INTEGER
        | UNIT EQUAL unit_v2
        | PATH EQUAL PATH_VALUE
        | TYPE EQUAL type_v
        | FIT EQUAL fit_v
        | NAME EQUAL ID_VALUE
        ;

unit_v2 : B
        | K
        | M
        ;

type_v : P
        | E
        | L
        ;

/*-------------------------------------*/
mount : MOUNT mount_params
;

mount_params : mount_params mount_p
        | mount_p
        ;

mount_p : PATH EQUAL PATH_VALUE 
        | NAME EQUAL ID_VALUE
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
        | COUNT
        ;

%%
 
void calc::Parser::error(const std::string& msg) {
    std::cerr << "Error: " << msg << '\n';
}