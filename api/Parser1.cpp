// A Bison parser, made by GNU Bison 3.8.2.

// Skeleton implementation for Bison LALR(1) parsers in C++

// Copyright (C) 2002-2015, 2018-2021 Free Software Foundation, Inc.

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

// As a special exception, you may create a larger work that contains
// part or all of the Bison parser skeleton and distribute that work
// under terms of your choice, so long as that work isn't itself a
// parser generator using the skeleton or a modified version thereof
// as a parser skeleton.  Alternatively, if you modify or redistribute
// the parser skeleton itself, you may (at your option) remove this
// special exception, which will cause the skeleton and the resulting
// Bison output files to be licensed under the GNU General Public
// License without this special exception.

// This special exception was added by the Free Software Foundation in
// version 2.2 of Bison.

// DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
// especially those whose name start with YY_ or yy_.  They are
// private implementation details that can be changed or removed.



// First part of user prologue.
#line 1 "grammar.y"

#include <iostream>
#include <string>

#include "Comandos/Mkdisk.h"
#include "Comandos/Fdisk.h"
#include "Comandos/Mount.h"
#include "Comandos/Mkfs.h"
#include "Comandos/Login.h"
#include "Comandos/Logout.h"
#include "Comandos/Mkgrp.h"
#include "Comandos/Rmgrp.h"

#line 55 "Parser1.cpp"


#include "Parser1.hpp"


// Unqualified %code blocks.
#line 119 "grammar.y"

    namespace calc 
    {
        MkdiskParams mkdiskActual;
        FdiskParams fdiskActual;
        MountParams mountActual;
        MkfsParams mkfsActual;
        LoginParams loginActual;
        MkgrpParams mkgrpActual;
        RmgrpParams rmgrpActual;
    }

#line 75 "Parser1.cpp"


#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> // FIXME: INFRINGES ON USER NAME SPACE.
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif


// Whether we are compiled with exception support.
#ifndef YY_EXCEPTIONS
# if defined __GNUC__ && !defined __EXCEPTIONS
#  define YY_EXCEPTIONS 0
# else
#  define YY_EXCEPTIONS 1
# endif
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K].location)
/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

# ifndef YYLLOC_DEFAULT
#  define YYLLOC_DEFAULT(Current, Rhs, N)                               \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).begin  = YYRHSLOC (Rhs, 1).begin;                   \
          (Current).end    = YYRHSLOC (Rhs, N).end;                     \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).begin = (Current).end = YYRHSLOC (Rhs, 0).end;      \
        }                                                               \
    while (false)
# endif


// Enable debugging if requested.
#if YYDEBUG

// A pseudo ostream that takes yydebug_ into account.
# define YYCDEBUG if (yydebug_) (*yycdebug_)

# define YY_SYMBOL_PRINT(Title, Symbol)         \
  do {                                          \
    if (yydebug_)                               \
    {                                           \
      *yycdebug_ << Title << ' ';               \
      yy_print_ (*yycdebug_, Symbol);           \
      *yycdebug_ << '\n';                       \
    }                                           \
  } while (false)

# define YY_REDUCE_PRINT(Rule)          \
  do {                                  \
    if (yydebug_)                       \
      yy_reduce_print_ (Rule);          \
  } while (false)

# define YY_STACK_PRINT()               \
  do {                                  \
    if (yydebug_)                       \
      yy_stack_print_ ();                \
  } while (false)

#else // !YYDEBUG

# define YYCDEBUG if (false) std::cerr
# define YY_SYMBOL_PRINT(Title, Symbol)  YY_USE (Symbol)
# define YY_REDUCE_PRINT(Rule)           static_cast<void> (0)
# define YY_STACK_PRINT()                static_cast<void> (0)

#endif // !YYDEBUG

#define yyerrok         (yyerrstatus_ = 0)
#define yyclearin       (yyla.clear ())

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYRECOVERING()  (!!yyerrstatus_)

#line 21 "grammar.y"
namespace calc {
#line 168 "Parser1.cpp"

  /// Build a parser object.
  Parser::Parser (yyscan_t scanner_yyarg)
#if YYDEBUG
    : yydebug_ (false),
      yycdebug_ (&std::cerr),
#else
    :
#endif
      scanner (scanner_yyarg)
  {}

  Parser::~Parser ()
  {}

  Parser::syntax_error::~syntax_error () YY_NOEXCEPT YY_NOTHROW
  {}

  /*---------.
  | symbol.  |
  `---------*/

  // basic_symbol.
  template <typename Base>
  Parser::basic_symbol<Base>::basic_symbol (const basic_symbol& that)
    : Base (that)
    , value ()
    , location (that.location)
  {
    switch (this->kind ())
    {
      case symbol_kind::S_INTEGER: // INTEGER
        value.copy< long long > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_PATH_VALUE: // PATH_VALUE
      case symbol_kind::S_ID_VALUE: // ID_VALUE
      case symbol_kind::S_PASSWORD_VALUE: // PASSWORD_VALUE
      case symbol_kind::S_MOUNT_ID: // MOUNT_ID
      case symbol_kind::S_fit_v: // fit_v
      case symbol_kind::S_unit_v: // unit_v
      case symbol_kind::S_unit_v2: // unit_v2
      case symbol_kind::S_type_v: // type_v
        value.copy< std::string > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

  }




  template <typename Base>
  Parser::symbol_kind_type
  Parser::basic_symbol<Base>::type_get () const YY_NOEXCEPT
  {
    return this->kind ();
  }


  template <typename Base>
  bool
  Parser::basic_symbol<Base>::empty () const YY_NOEXCEPT
  {
    return this->kind () == symbol_kind::S_YYEMPTY;
  }

  template <typename Base>
  void
  Parser::basic_symbol<Base>::move (basic_symbol& s)
  {
    super_type::move (s);
    switch (this->kind ())
    {
      case symbol_kind::S_INTEGER: // INTEGER
        value.move< long long > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_PATH_VALUE: // PATH_VALUE
      case symbol_kind::S_ID_VALUE: // ID_VALUE
      case symbol_kind::S_PASSWORD_VALUE: // PASSWORD_VALUE
      case symbol_kind::S_MOUNT_ID: // MOUNT_ID
      case symbol_kind::S_fit_v: // fit_v
      case symbol_kind::S_unit_v: // unit_v
      case symbol_kind::S_unit_v2: // unit_v2
      case symbol_kind::S_type_v: // type_v
        value.move< std::string > (YY_MOVE (s.value));
        break;

      default:
        break;
    }

    location = YY_MOVE (s.location);
  }

  // by_kind.
  Parser::by_kind::by_kind () YY_NOEXCEPT
    : kind_ (symbol_kind::S_YYEMPTY)
  {}

#if 201103L <= YY_CPLUSPLUS
  Parser::by_kind::by_kind (by_kind&& that) YY_NOEXCEPT
    : kind_ (that.kind_)
  {
    that.clear ();
  }
#endif

  Parser::by_kind::by_kind (const by_kind& that) YY_NOEXCEPT
    : kind_ (that.kind_)
  {}

  Parser::by_kind::by_kind (token_kind_type t) YY_NOEXCEPT
    : kind_ (yytranslate_ (t))
  {}



  void
  Parser::by_kind::clear () YY_NOEXCEPT
  {
    kind_ = symbol_kind::S_YYEMPTY;
  }

  void
  Parser::by_kind::move (by_kind& that)
  {
    kind_ = that.kind_;
    that.clear ();
  }

  Parser::symbol_kind_type
  Parser::by_kind::kind () const YY_NOEXCEPT
  {
    return kind_;
  }


  Parser::symbol_kind_type
  Parser::by_kind::type_get () const YY_NOEXCEPT
  {
    return this->kind ();
  }



  // by_state.
  Parser::by_state::by_state () YY_NOEXCEPT
    : state (empty_state)
  {}

  Parser::by_state::by_state (const by_state& that) YY_NOEXCEPT
    : state (that.state)
  {}

  void
  Parser::by_state::clear () YY_NOEXCEPT
  {
    state = empty_state;
  }

  void
  Parser::by_state::move (by_state& that)
  {
    state = that.state;
    that.clear ();
  }

  Parser::by_state::by_state (state_type s) YY_NOEXCEPT
    : state (s)
  {}

  Parser::symbol_kind_type
  Parser::by_state::kind () const YY_NOEXCEPT
  {
    if (state == empty_state)
      return symbol_kind::S_YYEMPTY;
    else
      return YY_CAST (symbol_kind_type, yystos_[+state]);
  }

  Parser::stack_symbol_type::stack_symbol_type ()
  {}

  Parser::stack_symbol_type::stack_symbol_type (YY_RVREF (stack_symbol_type) that)
    : super_type (YY_MOVE (that.state), YY_MOVE (that.location))
  {
    switch (that.kind ())
    {
      case symbol_kind::S_INTEGER: // INTEGER
        value.YY_MOVE_OR_COPY< long long > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_PATH_VALUE: // PATH_VALUE
      case symbol_kind::S_ID_VALUE: // ID_VALUE
      case symbol_kind::S_PASSWORD_VALUE: // PASSWORD_VALUE
      case symbol_kind::S_MOUNT_ID: // MOUNT_ID
      case symbol_kind::S_fit_v: // fit_v
      case symbol_kind::S_unit_v: // unit_v
      case symbol_kind::S_unit_v2: // unit_v2
      case symbol_kind::S_type_v: // type_v
        value.YY_MOVE_OR_COPY< std::string > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

#if 201103L <= YY_CPLUSPLUS
    // that is emptied.
    that.state = empty_state;
#endif
  }

  Parser::stack_symbol_type::stack_symbol_type (state_type s, YY_MOVE_REF (symbol_type) that)
    : super_type (s, YY_MOVE (that.location))
  {
    switch (that.kind ())
    {
      case symbol_kind::S_INTEGER: // INTEGER
        value.move< long long > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_PATH_VALUE: // PATH_VALUE
      case symbol_kind::S_ID_VALUE: // ID_VALUE
      case symbol_kind::S_PASSWORD_VALUE: // PASSWORD_VALUE
      case symbol_kind::S_MOUNT_ID: // MOUNT_ID
      case symbol_kind::S_fit_v: // fit_v
      case symbol_kind::S_unit_v: // unit_v
      case symbol_kind::S_unit_v2: // unit_v2
      case symbol_kind::S_type_v: // type_v
        value.move< std::string > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

    // that is emptied.
    that.kind_ = symbol_kind::S_YYEMPTY;
  }

#if YY_CPLUSPLUS < 201103L
  Parser::stack_symbol_type&
  Parser::stack_symbol_type::operator= (const stack_symbol_type& that)
  {
    state = that.state;
    switch (that.kind ())
    {
      case symbol_kind::S_INTEGER: // INTEGER
        value.copy< long long > (that.value);
        break;

      case symbol_kind::S_PATH_VALUE: // PATH_VALUE
      case symbol_kind::S_ID_VALUE: // ID_VALUE
      case symbol_kind::S_PASSWORD_VALUE: // PASSWORD_VALUE
      case symbol_kind::S_MOUNT_ID: // MOUNT_ID
      case symbol_kind::S_fit_v: // fit_v
      case symbol_kind::S_unit_v: // unit_v
      case symbol_kind::S_unit_v2: // unit_v2
      case symbol_kind::S_type_v: // type_v
        value.copy< std::string > (that.value);
        break;

      default:
        break;
    }

    location = that.location;
    return *this;
  }

  Parser::stack_symbol_type&
  Parser::stack_symbol_type::operator= (stack_symbol_type& that)
  {
    state = that.state;
    switch (that.kind ())
    {
      case symbol_kind::S_INTEGER: // INTEGER
        value.move< long long > (that.value);
        break;

      case symbol_kind::S_PATH_VALUE: // PATH_VALUE
      case symbol_kind::S_ID_VALUE: // ID_VALUE
      case symbol_kind::S_PASSWORD_VALUE: // PASSWORD_VALUE
      case symbol_kind::S_MOUNT_ID: // MOUNT_ID
      case symbol_kind::S_fit_v: // fit_v
      case symbol_kind::S_unit_v: // unit_v
      case symbol_kind::S_unit_v2: // unit_v2
      case symbol_kind::S_type_v: // type_v
        value.move< std::string > (that.value);
        break;

      default:
        break;
    }

    location = that.location;
    // that is emptied.
    that.state = empty_state;
    return *this;
  }
#endif

  template <typename Base>
  void
  Parser::yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const
  {
    if (yymsg)
      YY_SYMBOL_PRINT (yymsg, yysym);
  }

#if YYDEBUG
  template <typename Base>
  void
  Parser::yy_print_ (std::ostream& yyo, const basic_symbol<Base>& yysym) const
  {
    std::ostream& yyoutput = yyo;
    YY_USE (yyoutput);
    if (yysym.empty ())
      yyo << "empty symbol";
    else
      {
        symbol_kind_type yykind = yysym.kind ();
        yyo << (yykind < YYNTOKENS ? "token" : "nterm")
            << ' ' << yysym.name () << " ("
            << yysym.location << ": ";
        YY_USE (yykind);
        yyo << ')';
      }
  }
#endif

  void
  Parser::yypush_ (const char* m, YY_MOVE_REF (stack_symbol_type) sym)
  {
    if (m)
      YY_SYMBOL_PRINT (m, sym);
    yystack_.push (YY_MOVE (sym));
  }

  void
  Parser::yypush_ (const char* m, state_type s, YY_MOVE_REF (symbol_type) sym)
  {
#if 201103L <= YY_CPLUSPLUS
    yypush_ (m, stack_symbol_type (s, std::move (sym)));
#else
    stack_symbol_type ss (s, sym);
    yypush_ (m, ss);
#endif
  }

  void
  Parser::yypop_ (int n) YY_NOEXCEPT
  {
    yystack_.pop (n);
  }

#if YYDEBUG
  std::ostream&
  Parser::debug_stream () const
  {
    return *yycdebug_;
  }

  void
  Parser::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


  Parser::debug_level_type
  Parser::debug_level () const
  {
    return yydebug_;
  }

  void
  Parser::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }
#endif // YYDEBUG

  Parser::state_type
  Parser::yy_lr_goto_state_ (state_type yystate, int yysym)
  {
    int yyr = yypgoto_[yysym - YYNTOKENS] + yystate;
    if (0 <= yyr && yyr <= yylast_ && yycheck_[yyr] == yystate)
      return yytable_[yyr];
    else
      return yydefgoto_[yysym - YYNTOKENS];
  }

  bool
  Parser::yy_pact_value_is_default_ (int yyvalue) YY_NOEXCEPT
  {
    return yyvalue == yypact_ninf_;
  }

  bool
  Parser::yy_table_value_is_error_ (int yyvalue) YY_NOEXCEPT
  {
    return yyvalue == yytable_ninf_;
  }

  int
  Parser::operator() ()
  {
    return parse ();
  }

  int
  Parser::parse ()
  {
    int yyn;
    /// Length of the RHS of the rule being reduced.
    int yylen = 0;

    // Error handling.
    int yynerrs_ = 0;
    int yyerrstatus_ = 0;

    /// The lookahead symbol.
    symbol_type yyla;

    /// The locations where the error started and ended.
    stack_symbol_type yyerror_range[3];

    /// The return value of parse ().
    int yyresult;

#if YY_EXCEPTIONS
    try
#endif // YY_EXCEPTIONS
      {
    YYCDEBUG << "Starting parse\n";


    /* Initialize the stack.  The initial state will be set in
       yynewstate, since the latter expects the semantical and the
       location values to have been already stored, initialize these
       stacks with a primary value.  */
    yystack_.clear ();
    yypush_ (YY_NULLPTR, 0, YY_MOVE (yyla));

  /*-----------------------------------------------.
  | yynewstate -- push a new symbol on the stack.  |
  `-----------------------------------------------*/
  yynewstate:
    YYCDEBUG << "Entering state " << int (yystack_[0].state) << '\n';
    YY_STACK_PRINT ();

    // Accept?
    if (yystack_[0].state == yyfinal_)
      YYACCEPT;

    goto yybackup;


  /*-----------.
  | yybackup.  |
  `-----------*/
  yybackup:
    // Try to take a decision without lookahead.
    yyn = yypact_[+yystack_[0].state];
    if (yy_pact_value_is_default_ (yyn))
      goto yydefault;

    // Read a lookahead token.
    if (yyla.empty ())
      {
        YYCDEBUG << "Reading a token\n";
#if YY_EXCEPTIONS
        try
#endif // YY_EXCEPTIONS
          {
            yyla.kind_ = yytranslate_ (yylex (&yyla.value, &yyla.location, scanner));
          }
#if YY_EXCEPTIONS
        catch (const syntax_error& yyexc)
          {
            YYCDEBUG << "Caught exception: " << yyexc.what() << '\n';
            error (yyexc);
            goto yyerrlab1;
          }
#endif // YY_EXCEPTIONS
      }
    YY_SYMBOL_PRINT ("Next token is", yyla);

    if (yyla.kind () == symbol_kind::S_YYerror)
    {
      // The scanner already issued an error message, process directly
      // to error recovery.  But do not keep the error token as
      // lookahead, it is too special and may lead us to an endless
      // loop in error recovery. */
      yyla.kind_ = symbol_kind::S_YYUNDEF;
      goto yyerrlab1;
    }

    /* If the proper action on seeing token YYLA.TYPE is to reduce or
       to detect an error, take that action.  */
    yyn += yyla.kind ();
    if (yyn < 0 || yylast_ < yyn || yycheck_[yyn] != yyla.kind ())
      {
        goto yydefault;
      }

    // Reduce or error.
    yyn = yytable_[yyn];
    if (yyn <= 0)
      {
        if (yy_table_value_is_error_ (yyn))
          goto yyerrlab;
        yyn = -yyn;
        goto yyreduce;
      }

    // Count tokens shifted since error; after three, turn off error status.
    if (yyerrstatus_)
      --yyerrstatus_;

    // Shift the lookahead token.
    yypush_ ("Shifting", state_type (yyn), YY_MOVE (yyla));
    goto yynewstate;


  /*-----------------------------------------------------------.
  | yydefault -- do the default action for the current state.  |
  `-----------------------------------------------------------*/
  yydefault:
    yyn = yydefact_[+yystack_[0].state];
    if (yyn == 0)
      goto yyerrlab;
    goto yyreduce;


  /*-----------------------------.
  | yyreduce -- do a reduction.  |
  `-----------------------------*/
  yyreduce:
    yylen = yyr2_[yyn];
    {
      stack_symbol_type yylhs;
      yylhs.state = yy_lr_goto_state_ (yystack_[yylen].state, yyr1_[yyn]);
      /* Variants are always initialized to an empty instance of the
         correct type. The default '$$ = $1' action is NOT applied
         when using variants.  */
      switch (yyr1_[yyn])
    {
      case symbol_kind::S_INTEGER: // INTEGER
        yylhs.value.emplace< long long > ();
        break;

      case symbol_kind::S_PATH_VALUE: // PATH_VALUE
      case symbol_kind::S_ID_VALUE: // ID_VALUE
      case symbol_kind::S_PASSWORD_VALUE: // PASSWORD_VALUE
      case symbol_kind::S_MOUNT_ID: // MOUNT_ID
      case symbol_kind::S_fit_v: // fit_v
      case symbol_kind::S_unit_v: // unit_v
      case symbol_kind::S_unit_v2: // unit_v2
      case symbol_kind::S_type_v: // type_v
        yylhs.value.emplace< std::string > ();
        break;

      default:
        break;
    }


      // Default location.
      {
        stack_type::slice range (yystack_, yylen);
        YYLLOC_DEFAULT (yylhs.location, range, yylen);
        yyerror_range[1].location = yylhs.location;
      }

      // Perform the reduction.
      YY_REDUCE_PRINT (yyn);
#if YY_EXCEPTIONS
      try
#endif // YY_EXCEPTIONS
        {
          switch (yyn)
            {
  case 2: // comandos: %empty
#line 133 "grammar.y"
                    {  }
#line 762 "Parser1.cpp"
    break;

  case 4: // comando: EOL
#line 137 "grammar.y"
                  { std::cerr << "No se encontraron comandos.\n"; }
#line 768 "Parser1.cpp"
    break;

  case 5: // comando: mkdisk EOL
#line 138 "grammar.y"
                     {  }
#line 774 "Parser1.cpp"
    break;

  case 6: // comando: rmdisk EOL
#line 139 "grammar.y"
                     {  }
#line 780 "Parser1.cpp"
    break;

  case 7: // comando: fdisk EOL
#line 140 "grammar.y"
                    {  }
#line 786 "Parser1.cpp"
    break;

  case 8: // comando: mount EOL
#line 141 "grammar.y"
                    {  }
#line 792 "Parser1.cpp"
    break;

  case 9: // comando: mkfs EOL
#line 142 "grammar.y"
                   {  }
#line 798 "Parser1.cpp"
    break;

  case 10: // comando: mkusr EOL
#line 143 "grammar.y"
                    {  }
#line 804 "Parser1.cpp"
    break;

  case 11: // comando: rmusr EOL
#line 144 "grammar.y"
                    {  }
#line 810 "Parser1.cpp"
    break;

  case 12: // comando: mkfile EOL
#line 145 "grammar.y"
                     {  }
#line 816 "Parser1.cpp"
    break;

  case 13: // comando: mounted EOL
#line 147 "grammar.y"
                      {  }
#line 822 "Parser1.cpp"
    break;

  case 14: // comando: cat EOL
#line 148 "grammar.y"
                  {  }
#line 828 "Parser1.cpp"
    break;

  case 15: // comando: login EOL
#line 149 "grammar.y"
                    {  }
#line 834 "Parser1.cpp"
    break;

  case 16: // comando: logout EOL
#line 150 "grammar.y"
                     {  }
#line 840 "Parser1.cpp"
    break;

  case 17: // comando: mkgrp EOL
#line 151 "grammar.y"
                    {  }
#line 846 "Parser1.cpp"
    break;

  case 18: // comando: rmgrp EOL
#line 152 "grammar.y"
                    {  }
#line 852 "Parser1.cpp"
    break;

  case 19: // comando: chgrp EOL
#line 153 "grammar.y"
                    {  }
#line 858 "Parser1.cpp"
    break;

  case 20: // comando: mkdir EOL
#line 154 "grammar.y"
                    {  }
#line 864 "Parser1.cpp"
    break;

  case 21: // comando: rep EOL
#line 155 "grammar.y"
                  {  }
#line 870 "Parser1.cpp"
    break;

  case 22: // comando: error EOL
#line 156 "grammar.y"
                    { std::cerr << "Comando no valido.\n"; yyerrok; }
#line 876 "Parser1.cpp"
    break;

  case 23: // $@1: %empty
#line 162 "grammar.y"
        {
            std::cout 
                << ">>> INICIO MKDISK EN BISON"
                << std::endl;
            
            calc::mkdiskActual = MkdiskParams();
        }
#line 888 "Parser1.cpp"
    break;

  case 24: // mkdisk: MKDISK $@1 mkdisk_params
#line 170 "grammar.y"
        {
            std::cerr << ejecutarMkdisk(calc::mkdiskActual) << std::endl;
        }
#line 896 "Parser1.cpp"
    break;

  case 27: // mkdisk_p: SIZE EQUAL INTEGER
#line 179 "grammar.y"
                              { calc::mkdiskActual.size = yystack_[0].value.as < long long > (); }
#line 902 "Parser1.cpp"
    break;

  case 28: // mkdisk_p: FIT EQUAL fit_v
#line 180 "grammar.y"
                          { calc::mkdiskActual.fit = yystack_[0].value.as < std::string > (); }
#line 908 "Parser1.cpp"
    break;

  case 29: // mkdisk_p: UNIT EQUAL unit_v
#line 181 "grammar.y"
                            { calc::mkdiskActual.unit = yystack_[0].value.as < std::string > (); }
#line 914 "Parser1.cpp"
    break;

  case 30: // mkdisk_p: PATH EQUAL PATH_VALUE
#line 182 "grammar.y"
                                { calc::mkdiskActual.path = yystack_[0].value.as < std::string > (); }
#line 920 "Parser1.cpp"
    break;

  case 31: // fit_v: BF
#line 185 "grammar.y"
           { yylhs.value.as < std::string > () = "BF"; }
#line 926 "Parser1.cpp"
    break;

  case 32: // fit_v: FF
#line 186 "grammar.y"
             { yylhs.value.as < std::string > () = "FF"; }
#line 932 "Parser1.cpp"
    break;

  case 33: // fit_v: WF
#line 187 "grammar.y"
             { yylhs.value.as < std::string > () = "WF"; }
#line 938 "Parser1.cpp"
    break;

  case 34: // unit_v: K
#line 190 "grammar.y"
           { yylhs.value.as < std::string > () = "K"; }
#line 944 "Parser1.cpp"
    break;

  case 35: // unit_v: M
#line 191 "grammar.y"
            { yylhs.value.as < std::string > () = "M"; }
#line 950 "Parser1.cpp"
    break;

  case 37: // $@2: %empty
#line 200 "grammar.y"
    {
        calc::fdiskActual = FdiskParams();
    }
#line 958 "Parser1.cpp"
    break;

  case 38: // fdisk: FDISK $@2 fdisk_params
#line 204 "grammar.y"
    {
        std::cerr
            << ejecutarFdisk(
                calc::fdiskActual
            )
            << std::endl;
    }
#line 970 "Parser1.cpp"
    break;

  case 41: // fdisk_p: SIZE EQUAL INTEGER
#line 216 "grammar.y"
                             { calc::fdiskActual.size = yystack_[0].value.as < long long > (); }
#line 976 "Parser1.cpp"
    break;

  case 42: // fdisk_p: UNIT EQUAL unit_v2
#line 217 "grammar.y"
                             { calc::fdiskActual.unit = yystack_[0].value.as < std::string > (); }
#line 982 "Parser1.cpp"
    break;

  case 43: // fdisk_p: PATH EQUAL PATH_VALUE
#line 218 "grammar.y"
                                { calc::fdiskActual.path = yystack_[0].value.as < std::string > (); }
#line 988 "Parser1.cpp"
    break;

  case 44: // fdisk_p: TYPE EQUAL type_v
#line 219 "grammar.y"
                            { calc::fdiskActual.type = yystack_[0].value.as < std::string > (); }
#line 994 "Parser1.cpp"
    break;

  case 45: // fdisk_p: FIT EQUAL fit_v
#line 220 "grammar.y"
                          { calc::fdiskActual.fit = yystack_[0].value.as < std::string > (); }
#line 1000 "Parser1.cpp"
    break;

  case 46: // fdisk_p: NAME EQUAL ID_VALUE
#line 221 "grammar.y"
                              { calc::fdiskActual.name = yystack_[0].value.as < std::string > (); }
#line 1006 "Parser1.cpp"
    break;

  case 47: // unit_v2: B
#line 224 "grammar.y"
           { yylhs.value.as < std::string > () = "B"; }
#line 1012 "Parser1.cpp"
    break;

  case 48: // unit_v2: K
#line 225 "grammar.y"
           { yylhs.value.as < std::string > () = "K"; }
#line 1018 "Parser1.cpp"
    break;

  case 49: // unit_v2: M
#line 226 "grammar.y"
           { yylhs.value.as < std::string > () = "M"; }
#line 1024 "Parser1.cpp"
    break;

  case 50: // type_v: P
#line 229 "grammar.y"
           { yylhs.value.as < std::string > () = "P"; }
#line 1030 "Parser1.cpp"
    break;

  case 51: // type_v: E
#line 230 "grammar.y"
            { yylhs.value.as < std::string > () = "E"; }
#line 1036 "Parser1.cpp"
    break;

  case 52: // type_v: L
#line 231 "grammar.y"
            { yylhs.value.as < std::string > () = "L"; }
#line 1042 "Parser1.cpp"
    break;

  case 53: // $@3: %empty
#line 237 "grammar.y"
    {
        calc::mountActual = MountParams();
    }
#line 1050 "Parser1.cpp"
    break;

  case 54: // mount: MOUNT $@3 mount_params
#line 241 "grammar.y"
    {
        std::cerr
            << ejecutarMount(calc::mountActual)
            << std::endl;
    }
#line 1060 "Parser1.cpp"
    break;

  case 57: // mount_p: PATH EQUAL PATH_VALUE
#line 252 "grammar.y"
                                { calc::mountActual.path = yystack_[0].value.as < std::string > (); }
#line 1066 "Parser1.cpp"
    break;

  case 58: // mount_p: NAME EQUAL ID_VALUE
#line 253 "grammar.y"
                              { calc::mountActual.name = yystack_[0].value.as < std::string > (); }
#line 1072 "Parser1.cpp"
    break;

  case 59: // $@4: %empty
#line 259 "grammar.y"
    {
        calc::mkfsActual = MkfsParams();
    }
#line 1080 "Parser1.cpp"
    break;

  case 60: // mkfs: MKFS $@4 mkfs_params
#line 263 "grammar.y"
    {
        std::cerr
            << ejecutarMkfs(calc::mkfsActual)
            << std::endl;
    }
#line 1090 "Parser1.cpp"
    break;

  case 63: // mkfs_p: ID EQUAL MOUNT_ID
#line 275 "grammar.y"
                        { calc::mkfsActual.id = yystack_[0].value.as < std::string > (); }
#line 1096 "Parser1.cpp"
    break;

  case 64: // mkfs_p: TYPE EQUAL FULL
#line 276 "grammar.y"
                      { calc::mkfsActual.type = "full"; }
#line 1102 "Parser1.cpp"
    break;

  case 79: // mounted: MOUNTED
#line 313 "grammar.y"
    {
        std::cerr
            << mostrarParticionesMontadas()
            << std::endl;
    }
#line 1112 "Parser1.cpp"
    break;

  case 84: // $@5: %empty
#line 332 "grammar.y"
    {
        calc::loginActual = LoginParams();
    }
#line 1120 "Parser1.cpp"
    break;

  case 85: // login: LOGIN $@5 login_params
#line 336 "grammar.y"
    {
        std::cerr
            << ejecutarLogin(calc::loginActual)
            << std::endl;
    }
#line 1130 "Parser1.cpp"
    break;

  case 88: // login_p: USER EQUAL ID_VALUE
#line 347 "grammar.y"
                              { calc::loginActual.user = yystack_[0].value.as < std::string > (); }
#line 1136 "Parser1.cpp"
    break;

  case 89: // login_p: PASSWORD EQUAL PASSWORD_VALUE
#line 348 "grammar.y"
                                        { calc::loginActual.password = yystack_[0].value.as < std::string > (); }
#line 1142 "Parser1.cpp"
    break;

  case 90: // login_p: ID EQUAL MOUNT_ID
#line 349 "grammar.y"
                            { calc::loginActual.id = yystack_[0].value.as < std::string > (); }
#line 1148 "Parser1.cpp"
    break;

  case 91: // logout: LOGOUT
#line 355 "grammar.y"
    {
        std::cerr
            << ejecutarLogout()
            << std::endl;
    }
#line 1158 "Parser1.cpp"
    break;

  case 92: // mkgrp: MKGRP NAME EQUAL ID_VALUE
#line 365 "grammar.y"
    {
        MkgrpParams params;
        params.name = yystack_[0].value.as < std::string > ();

        std::cerr
            << ejecutarMkgrp(params)
            << std::endl;
    }
#line 1171 "Parser1.cpp"
    break;

  case 93: // rmgrp: RMGRP NAME EQUAL ID_VALUE
#line 379 "grammar.y"
    {
        RmgrpParams params;

        params.name = yystack_[0].value.as < std::string > ();

        std::cerr
            << ejecutarRmgrp(params)
            << std::endl;
    }
#line 1185 "Parser1.cpp"
    break;


#line 1189 "Parser1.cpp"

            default:
              break;
            }
        }
#if YY_EXCEPTIONS
      catch (const syntax_error& yyexc)
        {
          YYCDEBUG << "Caught exception: " << yyexc.what() << '\n';
          error (yyexc);
          YYERROR;
        }
#endif // YY_EXCEPTIONS
      YY_SYMBOL_PRINT ("-> $$ =", yylhs);
      yypop_ (yylen);
      yylen = 0;

      // Shift the result of the reduction.
      yypush_ (YY_NULLPTR, YY_MOVE (yylhs));
    }
    goto yynewstate;


  /*--------------------------------------.
  | yyerrlab -- here on detecting error.  |
  `--------------------------------------*/
  yyerrlab:
    // If not already recovering from an error, report this error.
    if (!yyerrstatus_)
      {
        ++yynerrs_;
        context yyctx (*this, yyla);
        std::string msg = yysyntax_error_ (yyctx);
        error (yyla.location, YY_MOVE (msg));
      }


    yyerror_range[1].location = yyla.location;
    if (yyerrstatus_ == 3)
      {
        /* If just tried and failed to reuse lookahead token after an
           error, discard it.  */

        // Return failure if at end of input.
        if (yyla.kind () == symbol_kind::S_YYEOF)
          YYABORT;
        else if (!yyla.empty ())
          {
            yy_destroy_ ("Error: discarding", yyla);
            yyla.clear ();
          }
      }

    // Else will try to reuse lookahead token after shifting the error token.
    goto yyerrlab1;


  /*---------------------------------------------------.
  | yyerrorlab -- error raised explicitly by YYERROR.  |
  `---------------------------------------------------*/
  yyerrorlab:
    /* Pacify compilers when the user code never invokes YYERROR and
       the label yyerrorlab therefore never appears in user code.  */
    if (false)
      YYERROR;

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYERROR.  */
    yypop_ (yylen);
    yylen = 0;
    YY_STACK_PRINT ();
    goto yyerrlab1;


  /*-------------------------------------------------------------.
  | yyerrlab1 -- common code for both syntax error and YYERROR.  |
  `-------------------------------------------------------------*/
  yyerrlab1:
    yyerrstatus_ = 3;   // Each real token shifted decrements this.
    // Pop stack until we find a state that shifts the error token.
    for (;;)
      {
        yyn = yypact_[+yystack_[0].state];
        if (!yy_pact_value_is_default_ (yyn))
          {
            yyn += symbol_kind::S_YYerror;
            if (0 <= yyn && yyn <= yylast_
                && yycheck_[yyn] == symbol_kind::S_YYerror)
              {
                yyn = yytable_[yyn];
                if (0 < yyn)
                  break;
              }
          }

        // Pop the current state because it cannot handle the error token.
        if (yystack_.size () == 1)
          YYABORT;

        yyerror_range[1].location = yystack_[0].location;
        yy_destroy_ ("Error: popping", yystack_[0]);
        yypop_ ();
        YY_STACK_PRINT ();
      }
    {
      stack_symbol_type error_token;

      yyerror_range[2].location = yyla.location;
      YYLLOC_DEFAULT (error_token.location, yyerror_range, 2);

      // Shift the error token.
      error_token.state = state_type (yyn);
      yypush_ ("Shifting", YY_MOVE (error_token));
    }
    goto yynewstate;


  /*-------------------------------------.
  | yyacceptlab -- YYACCEPT comes here.  |
  `-------------------------------------*/
  yyacceptlab:
    yyresult = 0;
    goto yyreturn;


  /*-----------------------------------.
  | yyabortlab -- YYABORT comes here.  |
  `-----------------------------------*/
  yyabortlab:
    yyresult = 1;
    goto yyreturn;


  /*-----------------------------------------------------.
  | yyreturn -- parsing is finished, return the result.  |
  `-----------------------------------------------------*/
  yyreturn:
    if (!yyla.empty ())
      yy_destroy_ ("Cleanup: discarding lookahead", yyla);

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYABORT or YYACCEPT.  */
    yypop_ (yylen);
    YY_STACK_PRINT ();
    while (1 < yystack_.size ())
      {
        yy_destroy_ ("Cleanup: popping", yystack_[0]);
        yypop_ ();
      }

    return yyresult;
  }
#if YY_EXCEPTIONS
    catch (...)
      {
        YYCDEBUG << "Exception caught: cleaning lookahead and stack\n";
        // Do not try to display the values of the reclaimed symbols,
        // as their printers might throw an exception.
        if (!yyla.empty ())
          yy_destroy_ (YY_NULLPTR, yyla);

        while (1 < yystack_.size ())
          {
            yy_destroy_ (YY_NULLPTR, yystack_[0]);
            yypop_ ();
          }
        throw;
      }
#endif // YY_EXCEPTIONS
  }

  void
  Parser::error (const syntax_error& yyexc)
  {
    error (yyexc.location, yyexc.what ());
  }

  const char *
  Parser::symbol_name (symbol_kind_type yysymbol)
  {
    static const char *const yy_sname[] =
    {
    "end of file", "error", "invalid token", "SIZE", "FIT", "BF", "FF",
  "WF", "UNIT", "B", "K", "M", "PATH", "TYPE", "P", "E", "L", "NAME", "ID",
  "USER", "GROUP", "R", "COUNT", "FULL", "MKDISK", "RMDISK", "FDISK",
  "MOUNT", "MKFS", "MKUSR", "RMUSR", "MKFILE", "MOUNTED", "CAT", "FILEN",
  "LOGIN", "LOGOUT", "MKGRP", "RMGRP", "CHGRP", "MKDIR", "REP",
  "PATH_FILE_LIST", "PASSWORD", "INTEGER", "PATH_VALUE", "ID_VALUE",
  "PASSWORD_VALUE", "MOUNT_ID", "EQUAL", "EOL", "$accept", "comandos",
  "comando", "mkdisk", "$@1", "mkdisk_params", "mkdisk_p", "fit_v",
  "unit_v", "rmdisk", "fdisk", "$@2", "fdisk_params", "fdisk_p", "unit_v2",
  "type_v", "mount", "$@3", "mount_params", "mount_p", "mkfs", "$@4",
  "mkfs_params", "mkfs_p", "mkusr", "mkusr_params", "mkusr_p", "rmusr",
  "mkfile", "mkfile_params", "mkfile_p", "mounted", "cat", "cat_params",
  "cat_p", "login", "$@5", "login_params", "login_p", "logout", "mkgrp",
  "rmgrp", "chgrp", "chgrp_params", "chgrp_p", "mkdir", "mkdir_params",
  "mkdir_p", "rep", "rep_params", "rep_p", YY_NULLPTR
    };
    return yy_sname[yysymbol];
  }



  // Parser::context.
  Parser::context::context (const Parser& yyparser, const symbol_type& yyla)
    : yyparser_ (yyparser)
    , yyla_ (yyla)
  {}

  int
  Parser::context::expected_tokens (symbol_kind_type yyarg[], int yyargn) const
  {
    // Actual number of expected tokens
    int yycount = 0;

    const int yyn = yypact_[+yyparser_.yystack_[0].state];
    if (!yy_pact_value_is_default_ (yyn))
      {
        /* Start YYX at -YYN if negative to avoid negative indexes in
           YYCHECK.  In other words, skip the first -YYN actions for
           this state because they are default actions.  */
        const int yyxbegin = yyn < 0 ? -yyn : 0;
        // Stay within bounds of both yycheck and yytname.
        const int yychecklim = yylast_ - yyn + 1;
        const int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
        for (int yyx = yyxbegin; yyx < yyxend; ++yyx)
          if (yycheck_[yyx + yyn] == yyx && yyx != symbol_kind::S_YYerror
              && !yy_table_value_is_error_ (yytable_[yyx + yyn]))
            {
              if (!yyarg)
                ++yycount;
              else if (yycount == yyargn)
                return 0;
              else
                yyarg[yycount++] = YY_CAST (symbol_kind_type, yyx);
            }
      }

    if (yyarg && yycount == 0 && 0 < yyargn)
      yyarg[0] = symbol_kind::S_YYEMPTY;
    return yycount;
  }






  int
  Parser::yy_syntax_error_arguments_ (const context& yyctx,
                                                 symbol_kind_type yyarg[], int yyargn) const
  {
    /* There are many possibilities here to consider:
       - If this state is a consistent state with a default action, then
         the only way this function was invoked is if the default action
         is an error action.  In that case, don't check for expected
         tokens because there are none.
       - The only way there can be no lookahead present (in yyla) is
         if this state is a consistent state with a default action.
         Thus, detecting the absence of a lookahead is sufficient to
         determine that there is no unexpected or expected token to
         report.  In that case, just report a simple "syntax error".
       - Don't assume there isn't a lookahead just because this state is
         a consistent state with a default action.  There might have
         been a previous inconsistent state, consistent state with a
         non-default action, or user semantic action that manipulated
         yyla.  (However, yyla is currently not documented for users.)
       - Of course, the expected token list depends on states to have
         correct lookahead information, and it depends on the parser not
         to perform extra reductions after fetching a lookahead from the
         scanner and before detecting a syntax error.  Thus, state merging
         (from LALR or IELR) and default reductions corrupt the expected
         token list.  However, the list is correct for canonical LR with
         one exception: it will still contain any token that will not be
         accepted due to an error action in a later state.
    */

    if (!yyctx.lookahead ().empty ())
      {
        if (yyarg)
          yyarg[0] = yyctx.token ();
        int yyn = yyctx.expected_tokens (yyarg ? yyarg + 1 : yyarg, yyargn - 1);
        return yyn + 1;
      }
    return 0;
  }

  // Generate an error message.
  std::string
  Parser::yysyntax_error_ (const context& yyctx) const
  {
    // Its maximum.
    enum { YYARGS_MAX = 5 };
    // Arguments of yyformat.
    symbol_kind_type yyarg[YYARGS_MAX];
    int yycount = yy_syntax_error_arguments_ (yyctx, yyarg, YYARGS_MAX);

    char const* yyformat = YY_NULLPTR;
    switch (yycount)
      {
#define YYCASE_(N, S)                         \
        case N:                               \
          yyformat = S;                       \
        break
      default: // Avoid compiler warnings.
        YYCASE_ (0, YY_("syntax error"));
        YYCASE_ (1, YY_("syntax error, unexpected %s"));
        YYCASE_ (2, YY_("syntax error, unexpected %s, expecting %s"));
        YYCASE_ (3, YY_("syntax error, unexpected %s, expecting %s or %s"));
        YYCASE_ (4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
        YYCASE_ (5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
#undef YYCASE_
      }

    std::string yyres;
    // Argument number.
    std::ptrdiff_t yyi = 0;
    for (char const* yyp = yyformat; *yyp; ++yyp)
      if (yyp[0] == '%' && yyp[1] == 's' && yyi < yycount)
        {
          yyres += symbol_name (yyarg[yyi++]);
          ++yyp;
        }
      else
        yyres += *yyp;
    return yyres;
  }


  const signed char Parser::yypact_ninf_ = -72;

  const signed char Parser::yytable_ninf_ = -1;

  const signed char
  Parser::yypact_[] =
  {
     -72,     0,   -72,   -48,   -72,    -7,   -72,   -72,   -72,     1,
      -3,    40,   -72,   -15,   -72,   -72,    28,    32,    50,    54,
      -8,   -72,   -72,     4,    17,    21,    22,    23,    24,    25,
      26,    27,    29,    30,    31,    33,    34,    36,    37,    38,
     -72,     3,    41,    43,    -9,    35,    42,    44,    45,     1,
     -72,    46,    47,    51,   -72,    52,    40,   -72,    53,   -15,
     -72,    -1,    55,    56,    57,    58,    50,   -72,    59,   -72,
      54,   -72,    60,    61,    62,    63,    -8,   -72,   -72,   -72,
     -72,   -72,   -72,   -72,   -72,   -72,   -72,   -72,   -72,   -72,
     -72,   -72,   -72,   -72,   -72,    64,    65,    66,    67,     3,
     -72,    72,    69,    70,    71,    73,    74,    75,    43,   -72,
      76,    77,    -9,   -72,    78,    79,    35,   -72,    39,    83,
      84,   -72,    86,    89,    85,    90,   -72,    91,   -72,    88,
      92,    93,    -1,   -72,    94,    97,    98,    99,   -72,   101,
     -72,   102,   103,   104,   106,   -72,    95,     7,    12,   107,
     -72,   -72,   109,     7,    48,   110,    49,   108,   -72,   111,
     112,   -72,    80,   100,   -72,   -72,   -72,   -72,   -72,   -72,
     -72,   -72,   -72,   113,   114,    87,   -72,   -72,   -72,   -72,
     -72,   -72,   -72,   -72,   -72,   -72,   -72,   -72,   -72,   -72,
     -72,   -72,   -72,   -72,   -72,   -72,   -72,   -72,   -72,   -72,
     -72,   -72,   -72,   -72,   -72,   -72,   -72,   -72,   -72,   -72,
     -72,   -72,   -72,   -72
  };

  const signed char
  Parser::yydefact_[] =
  {
       2,     0,     1,     0,    23,     0,    37,    53,    59,     0,
       0,     0,    79,     0,    84,    91,     0,     0,     0,     0,
       0,     4,     3,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      22,     0,     0,     0,     0,     0,     0,     0,     0,    65,
      67,     0,     0,     0,    76,     0,    72,    74,     0,    80,
      82,     0,     0,     0,     0,     0,    94,    96,     0,   103,
      99,   101,     0,     0,     0,     0,   104,   106,     5,     6,
       7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,     0,     0,     0,     0,    24,
      26,     0,     0,     0,     0,     0,     0,     0,    38,    40,
       0,     0,    54,    56,     0,     0,    60,    62,     0,     0,
       0,    66,     0,     0,     0,     0,    73,     0,    81,     0,
       0,     0,    85,    87,     0,     0,     0,     0,    95,     0,
     100,     0,     0,     0,     0,   105,     0,     0,     0,     0,
      25,    36,     0,     0,     0,     0,     0,     0,    39,     0,
       0,    55,     0,     0,    61,    68,    70,    69,    71,    77,
      75,    78,    83,     0,     0,     0,    86,    92,    93,    97,
      98,   102,   108,   107,   109,   110,    27,    31,    32,    33,
      28,    34,    35,    29,    30,    41,    45,    47,    48,    49,
      42,    43,    50,    51,    52,    44,    46,    57,    58,    64,
      63,    90,    88,    89
  };

  const signed char
  Parser::yypgoto_[] =
  {
     -72,   -72,   -72,   -72,   -72,   -72,   -21,   -71,   -72,   -72,
     -72,   -72,   -72,   -19,   -72,   -72,   -72,   -72,   -72,   -20,
     -72,   -72,   -72,   -18,   -72,   -72,   115,   -72,   -72,   -72,
      82,   -72,   -72,   -72,   116,   -72,   -72,   -72,   -35,   -72,
     -72,   -72,   -72,   -72,    96,   -72,   -72,   117,   -72,   -72,
      81
  };

  const unsigned char
  Parser::yydefgoto_[] =
  {
       0,     1,    22,    23,    41,    99,   100,   190,   193,    24,
      25,    43,   108,   109,   200,   205,    26,    44,   112,   113,
      27,    45,   116,   117,    28,    49,    50,    29,    30,    56,
      57,    31,    32,    59,    60,    33,    61,   132,   133,    34,
      35,    36,    37,    66,    67,    38,    70,    71,    39,    76,
      77
  };

  const unsigned char
  Parser::yytable_[] =
  {
       2,     3,    40,   110,    72,    42,    95,    96,   111,    73,
      74,    97,   187,   188,   189,    98,    51,   129,   130,    58,
      46,    47,   191,   192,     4,     5,     6,     7,     8,     9,
      10,    11,    12,    13,    75,    14,    15,    16,    17,    18,
      19,    20,   131,    52,    48,    62,   102,   103,   114,    63,
      21,   104,    53,   115,    78,   105,   106,   197,   198,   199,
     107,    54,    55,   202,   203,   204,    68,    79,    69,    64,
      65,    80,    81,    82,    83,    84,    85,    86,   150,    87,
      88,    89,   196,    90,    91,   165,    92,    93,    94,   158,
     101,   118,   161,   119,   120,   122,   123,   176,   164,     0,
     124,   125,   127,   209,   134,   135,   136,   137,   139,   141,
     142,   143,   144,   146,   147,   148,   149,   151,   152,   153,
     154,     0,   155,   156,   157,   159,   160,   162,   163,   166,
     170,   167,   168,   169,   213,   171,   172,   173,   126,   186,
     177,   174,   175,   178,   179,   180,   181,   182,   210,   183,
     184,   185,   194,   195,   206,   201,   207,   145,   208,     0,
     212,   211,   138,     0,   121,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   128,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   140
  };

  const short
  Parser::yycheck_[] =
  {
       0,     1,    50,    12,    12,    12,     3,     4,    17,    17,
      18,     8,     5,     6,     7,    12,    19,    18,    19,    34,
      19,    20,    10,    11,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    42,    35,    36,    37,    38,    39,
      40,    41,    43,     3,    43,    17,     3,     4,    13,    17,
      50,     8,    12,    18,    50,    12,    13,     9,    10,    11,
      17,    21,    22,    14,    15,    16,    12,    50,    14,    19,
      20,    50,    50,    50,    50,    50,    50,    50,    99,    50,
      50,    50,   153,    50,    50,    46,    50,    50,    50,   108,
      49,    49,   112,    49,    49,    49,    49,   132,   116,    -1,
      49,    49,    49,    23,    49,    49,    49,    49,    49,    49,
      49,    49,    49,    49,    49,    49,    49,    45,    49,    49,
      49,    -1,    49,    49,    49,    49,    49,    49,    49,    46,
      45,    47,    46,    44,    47,    45,    45,    49,    56,    44,
      46,    49,    49,    46,    46,    46,    45,    45,    48,    46,
      46,    45,    45,    44,    46,    45,    45,    76,    46,    -1,
      46,    48,    66,    -1,    49,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    59,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    70
  };

  const signed char
  Parser::yystos_[] =
  {
       0,    52,     0,     1,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    35,    36,    37,    38,    39,    40,
      41,    50,    53,    54,    60,    61,    67,    71,    75,    78,
      79,    82,    83,    86,    90,    91,    92,    93,    96,    99,
      50,    55,    12,    62,    68,    72,    19,    20,    43,    76,
      77,    19,     3,    12,    21,    22,    80,    81,    34,    84,
      85,    87,    17,    17,    19,    20,    94,    95,    12,    14,
      97,    98,    12,    17,    18,    42,   100,   101,    50,    50,
      50,    50,    50,    50,    50,    50,    50,    50,    50,    50,
      50,    50,    50,    50,    50,     3,     4,     8,    12,    56,
      57,    49,     3,     4,     8,    12,    13,    17,    63,    64,
      12,    17,    69,    70,    13,    18,    73,    74,    49,    49,
      49,    77,    49,    49,    49,    49,    81,    49,    85,    18,
      19,    43,    88,    89,    49,    49,    49,    49,    95,    49,
      98,    49,    49,    49,    49,   101,    49,    49,    49,    49,
      57,    45,    49,    49,    49,    49,    49,    49,    64,    49,
      49,    70,    49,    49,    74,    46,    46,    47,    46,    44,
      45,    45,    45,    49,    49,    49,    89,    46,    46,    46,
      46,    45,    45,    46,    46,    45,    44,     5,     6,     7,
      58,    10,    11,    59,    45,    44,    58,     9,    10,    11,
      65,    45,    14,    15,    16,    66,    46,    45,    46,    23,
      48,    48,    46,    47
  };

  const signed char
  Parser::yyr1_[] =
  {
       0,    51,    52,    52,    53,    53,    53,    53,    53,    53,
      53,    53,    53,    53,    53,    53,    53,    53,    53,    53,
      53,    53,    53,    55,    54,    56,    56,    57,    57,    57,
      57,    58,    58,    58,    59,    59,    60,    62,    61,    63,
      63,    64,    64,    64,    64,    64,    64,    65,    65,    65,
      66,    66,    66,    68,    67,    69,    69,    70,    70,    72,
      71,    73,    73,    74,    74,    75,    76,    76,    77,    77,
      77,    78,    79,    80,    80,    81,    81,    81,    81,    82,
      83,    84,    84,    85,    87,    86,    88,    88,    89,    89,
      89,    90,    91,    92,    93,    94,    94,    95,    95,    96,
      97,    97,    98,    98,    99,   100,   100,   101,   101,   101,
     101
  };

  const signed char
  Parser::yyr2_[] =
  {
       0,     2,     0,     2,     1,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     0,     3,     2,     1,     3,     3,     3,
       3,     1,     1,     1,     1,     1,     4,     0,     3,     2,
       1,     3,     3,     3,     3,     3,     3,     1,     1,     1,
       1,     1,     1,     0,     3,     2,     1,     3,     3,     0,
       3,     2,     1,     3,     3,     2,     2,     1,     3,     3,
       3,     4,     2,     2,     1,     3,     1,     3,     3,     1,
       2,     2,     1,     3,     0,     3,     2,     1,     3,     3,
       3,     1,     4,     4,     2,     2,     1,     3,     3,     2,
       2,     1,     3,     1,     2,     2,     1,     3,     3,     3,
       3
  };




#if YYDEBUG
  const short
  Parser::yyrline_[] =
  {
       0,   133,   133,   134,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   147,   148,   149,   150,   151,   152,   153,
     154,   155,   156,   162,   161,   175,   176,   179,   180,   181,
     182,   185,   186,   187,   190,   191,   195,   200,   199,   212,
     213,   216,   217,   218,   219,   220,   221,   224,   225,   226,
     229,   230,   231,   237,   236,   248,   249,   252,   253,   259,
     258,   270,   271,   275,   276,   280,   283,   284,   287,   288,
     289,   293,   297,   300,   301,   304,   305,   306,   307,   312,
     320,   322,   323,   326,   332,   331,   343,   344,   347,   348,
     349,   354,   364,   378,   391,   393,   394,   397,   398,   402,
     404,   405,   408,   409,   413,   415,   416,   419,   420,   421,
     422
  };

  void
  Parser::yy_stack_print_ () const
  {
    *yycdebug_ << "Stack now";
    for (stack_type::const_iterator
           i = yystack_.begin (),
           i_end = yystack_.end ();
         i != i_end; ++i)
      *yycdebug_ << ' ' << int (i->state);
    *yycdebug_ << '\n';
  }

  void
  Parser::yy_reduce_print_ (int yyrule) const
  {
    int yylno = yyrline_[yyrule];
    int yynrhs = yyr2_[yyrule];
    // Print the symbols being reduced, and their result.
    *yycdebug_ << "Reducing stack by rule " << yyrule - 1
               << " (line " << yylno << "):\n";
    // The symbols being reduced.
    for (int yyi = 0; yyi < yynrhs; yyi++)
      YY_SYMBOL_PRINT ("   $" << yyi + 1 << " =",
                       yystack_[(yynrhs) - (yyi + 1)]);
  }
#endif // YYDEBUG

  Parser::symbol_kind_type
  Parser::yytranslate_ (int t) YY_NOEXCEPT
  {
    // YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to
    // TOKEN-NUM as returned by yylex.
    static
    const signed char
    translate_table[] =
    {
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50
    };
    // Last valid token kind.
    const int code_max = 305;

    if (t <= 0)
      return symbol_kind::S_YYEOF;
    else if (t <= code_max)
      return static_cast <symbol_kind_type> (translate_table[t]);
    else
      return symbol_kind::S_YYUNDEF;
  }

#line 21 "grammar.y"
} // calc
#line 1810 "Parser1.cpp"

#line 425 "grammar.y"


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
