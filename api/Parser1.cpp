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
#line 190 "grammar.y"

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
#line 134 "Parser1.cpp"

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
    : super_type (YY_MOVE (that.state))
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
    : super_type (s)
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
            << ' ' << yysym.name () << " (";
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
            yyla.kind_ = yytranslate_ (yylex (&yyla.value, scanner));
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



      // Perform the reduction.
      YY_REDUCE_PRINT (yyn);
#if YY_EXCEPTIONS
      try
#endif // YY_EXCEPTIONS
        {
          switch (yyn)
            {
  case 2: // comandos: %empty
#line 198 "grammar.y"
                    {  }
#line 679 "Parser1.cpp"
    break;

  case 4: // comando: EOL
#line 202 "grammar.y"
                  { std::cerr << "No se encontraron comandos.\n"; }
#line 685 "Parser1.cpp"
    break;

  case 5: // comando: mkdisk EOL
#line 203 "grammar.y"
                     { std::cerr << "Comando mkdisk ejecutado.\n"; }
#line 691 "Parser1.cpp"
    break;

  case 6: // comando: rmdisk EOL
#line 204 "grammar.y"
                     { std::cerr << "Comando rmdisk ejecutado.\n"; }
#line 697 "Parser1.cpp"
    break;

  case 7: // comando: fdisk EOL
#line 205 "grammar.y"
                    { std::cerr << "Comando fdisk ejecutado.\n"; }
#line 703 "Parser1.cpp"
    break;

  case 8: // comando: mount EOL
#line 206 "grammar.y"
                    { std::cerr << "Comando mount ejecutado.\n"; }
#line 709 "Parser1.cpp"
    break;

  case 9: // comando: mkfs EOL
#line 207 "grammar.y"
                   { std::cerr << "Comando mkfs ejecutado.\n"; }
#line 715 "Parser1.cpp"
    break;

  case 10: // comando: mkusr EOL
#line 208 "grammar.y"
                    { std::cerr << "Comando mkusr ejecutado.\n"; }
#line 721 "Parser1.cpp"
    break;

  case 11: // comando: rmusr EOL
#line 209 "grammar.y"
                    { std::cerr << "Comando rmusr ejecutado.\n"; }
#line 727 "Parser1.cpp"
    break;

  case 12: // comando: mkfile EOL
#line 210 "grammar.y"
                     { std::cerr << "Comando mkfile ejecutado.\n"; }
#line 733 "Parser1.cpp"
    break;


#line 737 "Parser1.cpp"

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
        std::string msg = YY_("syntax error");
        error (YY_MOVE (msg));
      }


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

        yy_destroy_ ("Error: popping", yystack_[0]);
        yypop_ ();
        YY_STACK_PRINT ();
      }
    {
      stack_symbol_type error_token;


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
    error (yyexc.what ());
  }

#if YYDEBUG || 0
  const char *
  Parser::symbol_name (symbol_kind_type yysymbol)
  {
    return yytname_[yysymbol];
  }
#endif // #if YYDEBUG || 0









  const signed char Parser::yypact_ninf_ = -30;

  const signed char Parser::yytable_ninf_ = -1;

  const signed char
  Parser::yypact_[] =
  {
     -30,     0,   -30,    10,    -6,    -1,    20,    21,   -15,   -11,
      -2,   -30,   -30,   -29,   -23,   -17,    -5,    11,    12,    13,
      14,   -14,    16,    17,    18,    10,   -30,    19,    22,    23,
      24,    26,    27,    28,    -1,   -30,    29,    30,    20,   -30,
      31,    33,    21,   -30,    36,    37,    38,   -15,   -30,    39,
      40,    41,   -30,   -30,    -2,   -30,   -30,   -30,   -30,   -30,
     -30,   -30,   -30,   -30,    46,    35,    25,    47,   -30,    48,
      50,    35,    34,    51,    32,    45,   -30,    52,    49,   -30,
      64,    53,   -30,    54,    55,    56,   -30,    58,    61,    57,
     -30,   -30,   -30,   -30,   -30,   -30,   -30,   -30,   -30,   -30,
     -30,   -30,   -30,   -30,   -30,   -30,   -30,   -30,   -30,   -30,
     -30,   -30,   -30,   -30,   -30,   -30,   -30,   -30,   -30,   -30,
     -30,   -30,   -30
  };

  const signed char
  Parser::yydefact_[] =
  {
       2,     0,     1,     0,     0,     0,     0,     0,     0,     0,
       0,     4,     3,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    13,    15,     0,     0,     0,
       0,     0,     0,     0,    26,    28,     0,     0,    41,    43,
       0,     0,    46,    48,     0,     0,     0,    51,    53,     0,
       0,     0,    62,    64,    58,    60,     5,     6,     7,     8,
       9,    10,    11,    12,     0,     0,     0,     0,    14,     0,
       0,     0,     0,     0,     0,     0,    27,     0,     0,    42,
       0,     0,    47,     0,     0,     0,    52,     0,     0,     0,
      59,    16,    20,    21,    22,    17,    23,    24,    18,    19,
      25,    29,    33,    35,    36,    37,    30,    31,    38,    39,
      40,    32,    34,    44,    45,    50,    49,    54,    56,    55,
      57,    63,    61
  };

  const signed char
  Parser::yypgoto_[] =
  {
     -30,   -30,   -30,   -30,   -30,    44,   -13,   -30,   -30,   -30,
     -30,    62,   -30,   -30,   -30,   -30,    59,   -30,   -30,    15,
     -30,   -30,    60,   -30,   -30,   -30,     8
  };

  const signed char
  Parser::yydefgoto_[] =
  {
       0,     1,    12,    13,    25,    26,    95,    98,    14,    15,
      34,    35,   106,   111,    16,    38,    39,    17,    42,    43,
      18,    47,    48,    19,    20,    54,    55
  };

  const signed char
  Parser::yytable_[] =
  {
       2,    50,    28,    29,    44,    45,    27,    30,    49,    56,
      51,    31,    32,    21,    22,    57,    33,    46,    23,    52,
      53,    58,    24,    64,     3,     4,     5,     6,     7,     8,
       9,    10,    36,    59,    40,    96,    97,    37,    11,    41,
      92,    93,    94,   103,   104,   105,   108,   109,   110,    60,
      61,    62,    63,    65,    66,    67,    69,    82,   102,    70,
      71,    72,    90,    73,    74,    75,    77,    78,    80,    68,
      81,     0,     0,    83,    84,    85,    87,    88,    89,    91,
     112,    99,   100,   101,   114,   107,   113,   115,   116,   117,
     118,   122,   119,   120,   121,     0,    76,    79,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    86
  };

  const signed char
  Parser::yycheck_[] =
  {
       0,     3,     3,     4,    19,    20,    12,     8,    19,    38,
      12,    12,    13,     3,     4,    38,    17,    32,     8,    21,
      22,    38,    12,    37,    24,    25,    26,    27,    28,    29,
      30,    31,    12,    38,    13,    10,    11,    17,    38,    18,
       5,     6,     7,     9,    10,    11,    14,    15,    16,    38,
      38,    38,    38,    37,    37,    37,    37,    42,    71,    37,
      37,    37,    54,    37,    37,    37,    37,    37,    37,    25,
      37,    -1,    -1,    37,    37,    37,    37,    37,    37,    33,
      35,    34,    34,    33,    35,    34,    34,    23,    35,    35,
      35,    34,    36,    35,    33,    -1,    34,    38,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    47
  };

  const signed char
  Parser::yystos_[] =
  {
       0,    40,     0,    24,    25,    26,    27,    28,    29,    30,
      31,    38,    41,    42,    47,    48,    53,    56,    59,    62,
      63,     3,     4,     8,    12,    43,    44,    12,     3,     4,
       8,    12,    13,    17,    49,    50,    12,    17,    54,    55,
      13,    18,    57,    58,    19,    20,    32,    60,    61,    19,
       3,    12,    21,    22,    64,    65,    38,    38,    38,    38,
      38,    38,    38,    38,    37,    37,    37,    37,    44,    37,
      37,    37,    37,    37,    37,    37,    50,    37,    37,    55,
      37,    37,    58,    37,    37,    37,    61,    37,    37,    37,
      65,    33,     5,     6,     7,    45,    10,    11,    46,    34,
      34,    33,    45,     9,    10,    11,    51,    34,    14,    15,
      16,    52,    35,    34,    35,    23,    35,    35,    35,    36,
      35,    33,    34
  };

  const signed char
  Parser::yyr1_[] =
  {
       0,    39,    40,    40,    41,    41,    41,    41,    41,    41,
      41,    41,    41,    42,    43,    43,    44,    44,    44,    44,
      45,    45,    45,    46,    46,    47,    48,    49,    49,    50,
      50,    50,    50,    50,    50,    51,    51,    51,    52,    52,
      52,    53,    54,    54,    55,    55,    56,    57,    57,    58,
      58,    59,    60,    60,    61,    61,    61,    62,    63,    64,
      64,    65,    65,    65,    65
  };

  const signed char
  Parser::yyr2_[] =
  {
       0,     2,     0,     2,     1,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     1,     3,     3,     3,     3,
       1,     1,     1,     1,     1,     4,     2,     2,     1,     3,
       3,     3,     3,     3,     3,     1,     1,     1,     1,     1,
       1,     2,     2,     1,     3,     3,     2,     2,     1,     3,
       3,     2,     2,     1,     3,     3,     3,     4,     2,     2,
       1,     3,     1,     3,     1
  };


#if YYDEBUG
  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a YYNTOKENS, nonterminals.
  const char*
  const Parser::yytname_[] =
  {
  "\"end of file\"", "error", "\"invalid token\"", "SIZE", "FIT", "BF",
  "FF", "WF", "UNIT", "B", "K", "M", "PATH", "TYPE", "P", "E", "L", "NAME",
  "ID", "USER", "GROUP", "R", "COUNT", "FULL", "MKDISK", "RMDISK", "FDISK",
  "MOUNT", "MKFS", "MKUSR", "RMUSR", "MKFILE", "PASSWORD", "INTEGER",
  "PATH_VALUE", "ID_VALUE", "PASSWORD_VALUE", "EQUAL", "EOL", "$accept",
  "comandos", "comando", "mkdisk", "mkdisk_params", "mkdisk_p", "fit_v",
  "unit_v", "rmdisk", "fdisk", "fdisk_params", "fdisk_p", "unit_v2",
  "type_v", "mount", "mount_params", "mount_p", "mkfs", "mkfs_params",
  "mkfs_p", "mkusr", "mkusr_params", "mkusr_p", "rmusr", "mkfile",
  "mkfile_params", "mkfile_p", YY_NULLPTR
  };
#endif


#if YYDEBUG
  const short
  Parser::yyrline_[] =
  {
       0,   198,   198,   199,   202,   203,   204,   205,   206,   207,
     208,   209,   210,   215,   218,   219,   222,   223,   224,   225,
     228,   229,   230,   233,   234,   238,   241,   244,   245,   248,
     249,   250,   251,   252,   253,   256,   257,   258,   261,   262,
     263,   267,   270,   271,   274,   275,   279,   282,   283,   286,
     287,   291,   294,   295,   298,   299,   300,   304,   308,   311,
     312,   315,   316,   317,   318
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
      35,    36,    37,    38
    };
    // Last valid token kind.
    const int code_max = 293;

    if (t <= 0)
      return symbol_kind::S_YYEOF;
    else if (t <= code_max)
      return static_cast <symbol_kind_type> (translate_table[t]);
    else
      return symbol_kind::S_YYUNDEF;
  }

#line 12 "grammar.y"
} // calc
#line 1170 "Parser1.cpp"

#line 321 "grammar.y"

 
void calc::Parser::error(const std::string& msg) {
    std::cerr << "Error: " << msg << '\n';
}
