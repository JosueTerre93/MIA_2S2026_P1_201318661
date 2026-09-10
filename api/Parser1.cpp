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

#line 46 "Parser1.cpp"


#include "Parser1.hpp"


// Unqualified %code blocks.
#line 191 "grammar.y"

    namespace calc 
    {
    
    }

#line 60 "Parser1.cpp"


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

#line 12 "grammar.y"
namespace calc {
#line 153 "Parser1.cpp"

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
#line 199 "grammar.y"
                    {  }
#line 712 "Parser1.cpp"
    break;

  case 4: // comando: EOL
#line 203 "grammar.y"
                  { std::cerr << "No se encontraron comandos.\n"; }
#line 718 "Parser1.cpp"
    break;

  case 5: // comando: mkdisk EOL
#line 204 "grammar.y"
                     { std::cerr << "Comando mkdisk ejecutado.\n"; }
#line 724 "Parser1.cpp"
    break;

  case 6: // comando: rmdisk EOL
#line 205 "grammar.y"
                     { std::cerr << "Comando rmdisk ejecutado.\n"; }
#line 730 "Parser1.cpp"
    break;

  case 7: // comando: fdisk EOL
#line 206 "grammar.y"
                    { std::cerr << "Comando fdisk ejecutado.\n"; }
#line 736 "Parser1.cpp"
    break;

  case 8: // comando: mount EOL
#line 207 "grammar.y"
                    { std::cerr << "Comando mount ejecutado.\n"; }
#line 742 "Parser1.cpp"
    break;

  case 9: // comando: mkfs EOL
#line 208 "grammar.y"
                   { std::cerr << "Comando mkfs ejecutado.\n"; }
#line 748 "Parser1.cpp"
    break;

  case 10: // comando: mkusr EOL
#line 209 "grammar.y"
                    { std::cerr << "Comando mkusr ejecutado.\n"; }
#line 754 "Parser1.cpp"
    break;

  case 11: // comando: rmusr EOL
#line 210 "grammar.y"
                    { std::cerr << "Comando rmusr ejecutado.\n"; }
#line 760 "Parser1.cpp"
    break;

  case 12: // comando: mkfile EOL
#line 211 "grammar.y"
                     { std::cerr << "Comando mkfile ejecutado.\n"; }
#line 766 "Parser1.cpp"
    break;

  case 13: // comando: mounted EOL
#line 213 "grammar.y"
                      { std::cerr << "Comando mounted ejecutado.\n"; }
#line 772 "Parser1.cpp"
    break;

  case 14: // comando: cat EOL
#line 214 "grammar.y"
                  { std::cerr << "Comando cat ejecutado.\n"; }
#line 778 "Parser1.cpp"
    break;

  case 15: // comando: login EOL
#line 215 "grammar.y"
                    { std::cerr << "Comando login ejecutado.\n"; }
#line 784 "Parser1.cpp"
    break;

  case 16: // comando: logout EOL
#line 216 "grammar.y"
                     { std::cerr << "Comando logout ejecutado.\n"; }
#line 790 "Parser1.cpp"
    break;

  case 17: // comando: mkgrp EOL
#line 217 "grammar.y"
                    { std::cerr << "Comando mkgrp ejecutado.\n"; }
#line 796 "Parser1.cpp"
    break;

  case 18: // comando: rmgrp EOL
#line 218 "grammar.y"
                    { std::cerr << "Comando rmgrp ejecutado.\n"; }
#line 802 "Parser1.cpp"
    break;

  case 19: // comando: chgrp EOL
#line 219 "grammar.y"
                    { std::cerr << "Comando chgrp ejecutado.\n"; }
#line 808 "Parser1.cpp"
    break;

  case 20: // comando: mkdir EOL
#line 220 "grammar.y"
                    { std::cerr << "Comando mkdir ejecutado.\n"; }
#line 814 "Parser1.cpp"
    break;

  case 21: // comando: rep EOL
#line 221 "grammar.y"
                  { std::cerr << "Comando rep ejecutado.\n"; }
#line 820 "Parser1.cpp"
    break;


#line 824 "Parser1.cpp"

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
  "PASSWORD_VALUE", "EQUAL", "EOL", "$accept", "comandos", "comando",
  "mkdisk", "mkdisk_params", "mkdisk_p", "fit_v", "unit_v", "rmdisk",
  "fdisk", "fdisk_params", "fdisk_p", "unit_v2", "type_v", "mount",
  "mount_params", "mount_p", "mkfs", "mkfs_params", "mkfs_p", "mkusr",
  "mkusr_params", "mkusr_p", "rmusr", "mkfile", "mkfile_params",
  "mkfile_p", "mounted", "cat", "cat_params", "cat_p", "login",
  "login_params", "login_p", "logout", "mkgrp", "rmgrp", "chgrp",
  "chgrp_params", "chgrp_p", "mkdir", "mkdir_params", "mkdir_p", "rep",
  "rep_params", "rep_p", YY_NULLPTR
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


  const signed char Parser::yypact_ninf_ = -44;

  const signed char Parser::yytable_ninf_ = -1;

  const signed char
  Parser::yypact_[] =
  {
     -44,     0,   -44,     3,    -7,    42,    -9,    35,     1,    -3,
      40,   -44,   -15,    -1,   -44,    30,    34,   -18,    54,    -8,
     -44,   -44,    11,    18,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    31,    32,    33,    36,    37,    39,
      41,    43,    44,     3,   -44,    45,    46,    48,    50,    51,
      52,    53,    42,   -44,    55,    56,    -9,   -44,    57,    58,
      35,   -44,    59,    60,    61,     1,   -44,    62,    63,    64,
     -44,   -44,    40,   -44,    65,   -15,   -44,    66,    67,    68,
      -1,   -44,    69,    70,    71,    72,   -18,   -44,    73,   -44,
      54,   -44,    74,    75,    76,    77,    -8,   -44,   -44,   -44,
     -44,   -44,   -44,   -44,   -44,   -44,   -44,   -44,   -44,   -44,
     -44,   -44,   -44,   -44,   -44,    82,     7,    12,    38,   -44,
      83,    85,     7,    47,    86,    49,    81,   -44,    87,    84,
     -44,    79,    88,   -44,    89,    90,    91,   -44,    93,    96,
      92,   -44,    97,   -44,    95,    98,    99,   -44,   101,   102,
     103,   104,   -44,   100,   -44,   106,   107,   108,   110,   -44,
     -44,   -44,   -44,   -44,   -44,   -44,   -44,   -44,   -44,   -44,
     -44,   -44,   -44,   -44,   -44,   -44,   -44,   -44,   -44,   -44,
     -44,   -44,   -44,   -44,   -44,   -44,   -44,   -44,   -44,   -44,
     -44,   -44,   -44,   -44,   -44,   -44,   -44,   -44,   -44,   -44,
     -44,   -44,   -44,   -44,   -44
  };

  const signed char
  Parser::yydefact_[] =
  {
       2,     0,     1,     0,     0,     0,     0,     0,     0,     0,
       0,    74,     0,     0,    85,     0,     0,     0,     0,     0,
       4,     3,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    22,    24,     0,     0,     0,     0,     0,
       0,     0,    35,    37,     0,     0,    50,    52,     0,     0,
      55,    57,     0,     0,     0,    60,    62,     0,     0,     0,
      71,    73,    67,    69,     0,    75,    77,     0,     0,     0,
      79,    81,     0,     0,     0,     0,    88,    90,     0,    97,
      93,    95,     0,     0,     0,     0,    98,   100,     5,     6,
       7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,     0,     0,     0,     0,    23,
       0,     0,     0,     0,     0,     0,     0,    36,     0,     0,
      51,     0,     0,    56,     0,     0,     0,    61,     0,     0,
       0,    68,     0,    76,     0,     0,     0,    80,     0,     0,
       0,     0,    89,     0,    94,     0,     0,     0,     0,    99,
      25,    29,    30,    31,    26,    32,    33,    27,    28,    34,
      38,    42,    44,    45,    46,    39,    40,    47,    48,    49,
      41,    43,    53,    54,    59,    58,    63,    65,    64,    66,
      72,    70,    78,    84,    82,    83,    86,    87,    91,    92,
      96,   102,   101,   103,   104
  };

  const signed char
  Parser::yypgoto_[] =
  {
     -44,   -44,   -44,   -44,   -44,   109,   -43,   -44,   -44,   -44,
     -44,   105,   -44,   -44,   -44,   -44,   111,   -44,   -44,   112,
     -44,   -44,    19,   -44,   -44,   -44,    16,   -44,   -44,   -44,
      15,   -44,   -44,    17,   -44,   -44,   -44,   -44,   -44,     9,
     -44,   -44,    78,   -44,   -44,    80
  };

  const unsigned char
  Parser::yydefgoto_[] =
  {
       0,     1,    21,    22,    43,    44,   164,   167,    23,    24,
      52,    53,   175,   180,    25,    56,    57,    26,    60,    61,
      27,    65,    66,    28,    29,    72,    73,    30,    31,    75,
      76,    32,    80,    81,    33,    34,    35,    36,    86,    87,
      37,    90,    91,    38,    96,    97
  };

  const unsigned char
  Parser::yytable_[] =
  {
       2,    84,    85,    54,    92,    45,    39,    40,    55,    93,
      94,    41,   161,   162,   163,    42,    67,    77,    78,    74,
      62,    63,   165,   166,     3,     4,     5,     6,     7,     8,
       9,    10,    11,    12,    95,    13,    14,    15,    16,    17,
      18,    19,    79,    68,    64,    46,    47,    82,    58,    20,
      48,    83,    69,    59,    49,    50,   172,   173,   174,    51,
      98,    70,    71,   177,   178,   179,    88,    99,    89,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   109,   171,
     110,   111,   112,   168,   137,   113,   114,   115,   141,   116,
     143,   117,   118,   120,   121,   152,   122,   147,   123,   124,
     125,   126,   184,   128,   129,   131,   132,   134,   135,   136,
     138,   139,   140,   142,   144,   145,   146,   148,   149,   150,
     151,   153,   155,   156,   157,   158,   160,   181,   169,   170,
     183,   176,   182,     0,   185,   186,   187,   191,   188,   189,
     190,   193,   192,     0,   194,   200,   195,   196,   197,   198,
     199,   201,   119,   202,   203,   204,     0,   127,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   130,   154,     0,
       0,     0,   133,     0,     0,     0,   159
  };

  const signed char
  Parser::yycheck_[] =
  {
       0,    19,    20,    12,    12,    12,     3,     4,    17,    17,
      18,     8,     5,     6,     7,    12,    19,    18,    19,    34,
      19,    20,    10,    11,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    42,    35,    36,    37,    38,    39,
      40,    41,    43,     3,    43,     3,     4,    17,    13,    49,
       8,    17,    12,    18,    12,    13,     9,    10,    11,    17,
      49,    21,    22,    14,    15,    16,    12,    49,    14,    49,
      49,    49,    49,    49,    49,    49,    49,    49,    49,   122,
      49,    49,    49,    45,    65,    49,    49,    48,    72,    48,
      75,    48,    48,    48,    48,    86,    48,    80,    48,    48,
      48,    48,    23,    48,    48,    48,    48,    48,    48,    48,
      48,    48,    48,    48,    48,    48,    48,    48,    48,    48,
      48,    48,    48,    48,    48,    48,    44,    46,    45,    44,
      46,    45,    45,    -1,    46,    46,    46,    45,    47,    46,
      44,    46,    45,    -1,    46,    45,    47,    46,    46,    46,
      46,    45,    43,    46,    46,    45,    -1,    52,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    56,    90,    -1,
      -1,    -1,    60,    -1,    -1,    -1,    96
  };

  const signed char
  Parser::yystos_[] =
  {
       0,    51,     0,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    35,    36,    37,    38,    39,    40,    41,
      49,    52,    53,    58,    59,    64,    67,    70,    73,    74,
      77,    78,    81,    84,    85,    86,    87,    90,    93,     3,
       4,     8,    12,    54,    55,    12,     3,     4,     8,    12,
      13,    17,    60,    61,    12,    17,    65,    66,    13,    18,
      68,    69,    19,    20,    43,    71,    72,    19,     3,    12,
      21,    22,    75,    76,    34,    79,    80,    18,    19,    43,
      82,    83,    17,    17,    19,    20,    88,    89,    12,    14,
      91,    92,    12,    17,    18,    42,    94,    95,    49,    49,
      49,    49,    49,    49,    49,    49,    49,    49,    49,    49,
      49,    49,    49,    49,    49,    48,    48,    48,    48,    55,
      48,    48,    48,    48,    48,    48,    48,    61,    48,    48,
      66,    48,    48,    69,    48,    48,    48,    72,    48,    48,
      48,    76,    48,    80,    48,    48,    48,    83,    48,    48,
      48,    48,    89,    48,    92,    48,    48,    48,    48,    95,
      44,     5,     6,     7,    56,    10,    11,    57,    45,    45,
      44,    56,     9,    10,    11,    62,    45,    14,    15,    16,
      63,    46,    45,    46,    23,    46,    46,    46,    47,    46,
      44,    45,    45,    46,    46,    47,    46,    46,    46,    46,
      45,    45,    46,    46,    45
  };

  const signed char
  Parser::yyr1_[] =
  {
       0,    50,    51,    51,    52,    52,    52,    52,    52,    52,
      52,    52,    52,    52,    52,    52,    52,    52,    52,    52,
      52,    52,    53,    54,    54,    55,    55,    55,    55,    56,
      56,    56,    57,    57,    58,    59,    60,    60,    61,    61,
      61,    61,    61,    61,    62,    62,    62,    63,    63,    63,
      64,    65,    65,    66,    66,    67,    68,    68,    69,    69,
      70,    71,    71,    72,    72,    72,    73,    74,    75,    75,
      76,    76,    76,    76,    77,    78,    79,    79,    80,    81,
      82,    82,    83,    83,    83,    84,    85,    86,    87,    88,
      88,    89,    89,    90,    91,    91,    92,    92,    93,    94,
      94,    95,    95,    95,    95
  };

  const signed char
  Parser::yyr2_[] =
  {
       0,     2,     0,     2,     1,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     1,     3,     3,     3,     3,     1,
       1,     1,     1,     1,     4,     2,     2,     1,     3,     3,
       3,     3,     3,     3,     1,     1,     1,     1,     1,     1,
       2,     2,     1,     3,     3,     2,     2,     1,     3,     3,
       2,     2,     1,     3,     3,     3,     4,     2,     2,     1,
       3,     1,     3,     1,     1,     2,     2,     1,     3,     2,
       2,     1,     3,     3,     3,     1,     4,     4,     2,     2,
       1,     3,     3,     2,     2,     1,     3,     1,     2,     2,
       1,     3,     3,     3,     3
  };




#if YYDEBUG
  const short
  Parser::yyrline_[] =
  {
       0,   199,   199,   200,   203,   204,   205,   206,   207,   208,
     209,   210,   211,   213,   214,   215,   216,   217,   218,   219,
     220,   221,   226,   229,   230,   233,   234,   235,   236,   239,
     240,   241,   244,   245,   249,   252,   255,   256,   259,   260,
     261,   262,   263,   264,   267,   268,   269,   272,   273,   274,
     278,   281,   282,   285,   286,   290,   293,   294,   297,   298,
     302,   305,   306,   309,   310,   311,   315,   319,   322,   323,
     326,   327,   328,   329,   333,   336,   338,   339,   342,   346,
     348,   349,   352,   353,   354,   358,   361,   365,   369,   371,
     372,   375,   376,   380,   382,   383,   386,   387,   391,   393,
     394,   397,   398,   399,   400
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
      45,    46,    47,    48,    49
    };
    // Last valid token kind.
    const int code_max = 304;

    if (t <= 0)
      return symbol_kind::S_YYEOF;
    else if (t <= code_max)
      return static_cast <symbol_kind_type> (translate_table[t]);
    else
      return symbol_kind::S_YYUNDEF;
  }

#line 12 "grammar.y"
} // calc
#line 1435 "Parser1.cpp"

#line 403 "grammar.y"


#include "Scanner1.hpp"
 
void calc::Parser::error(const location_type& loc, const std::string& msg)
{
    std::cerr
        << "Error sintáctico en línea "
        << loc.begin.line
        << ", columna "
        << loc.begin.column
        << ": "
        << msg
        << std::endl;
}
