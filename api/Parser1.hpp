// A Bison parser, made by GNU Bison 3.8.2.

// Skeleton interface for Bison LALR(1) parsers in C++

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


/**
 ** \file Parser1.hpp
 ** Define the calc::parser class.
 */

// C++ LALR(1) parser skeleton written by Akim Demaille.

// DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
// especially those whose name start with YY_ or yy_.  They are
// private implementation details that can be changed or removed.

#ifndef YY_YY_PARSER1_HPP_INCLUDED
# define YY_YY_PARSER1_HPP_INCLUDED
// "%code requires" blocks.
#line 17 "grammar.y"

  typedef void* yyscan_t;

#line 53 "Parser1.hpp"


# include <cstdlib> // std::abort
# include <iostream>
# include <stdexcept>
# include <string>
# include <vector>

#if defined __cplusplus
# define YY_CPLUSPLUS __cplusplus
#else
# define YY_CPLUSPLUS 199711L
#endif

// Support move semantics when possible.
#if 201103L <= YY_CPLUSPLUS
# define YY_MOVE           std::move
# define YY_MOVE_OR_COPY   move
# define YY_MOVE_REF(Type) Type&&
# define YY_RVREF(Type)    Type&&
# define YY_COPY(Type)     Type
#else
# define YY_MOVE
# define YY_MOVE_OR_COPY   copy
# define YY_MOVE_REF(Type) Type&
# define YY_RVREF(Type)    const Type&
# define YY_COPY(Type)     const Type&
#endif

// Support noexcept when possible.
#if 201103L <= YY_CPLUSPLUS
# define YY_NOEXCEPT noexcept
# define YY_NOTHROW
#else
# define YY_NOEXCEPT
# define YY_NOTHROW throw ()
#endif

// Support constexpr when possible.
#if 201703 <= YY_CPLUSPLUS
# define YY_CONSTEXPR constexpr
#else
# define YY_CONSTEXPR
#endif



#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

#line 12 "grammar.y"
namespace calc {
#line 189 "Parser1.hpp"




  /// A Bison parser.
  class Parser
  {
  public:
#ifdef YYSTYPE
# ifdef __GNUC__
#  pragma GCC message "bison: do not #define YYSTYPE in C++, use %define api.value.type"
# endif
    typedef YYSTYPE value_type;
#else
  /// A buffer to store and retrieve objects.
  ///
  /// Sort of a variant, but does not keep track of the nature
  /// of the stored data, since that knowledge is available
  /// via the current parser state.
  class value_type
  {
  public:
    /// Type of *this.
    typedef value_type self_type;

    /// Empty construction.
    value_type () YY_NOEXCEPT
      : yyraw_ ()
    {}

    /// Construct and fill.
    template <typename T>
    value_type (YY_RVREF (T) t)
    {
      new (yyas_<T> ()) T (YY_MOVE (t));
    }

#if 201103L <= YY_CPLUSPLUS
    /// Non copyable.
    value_type (const self_type&) = delete;
    /// Non copyable.
    self_type& operator= (const self_type&) = delete;
#endif

    /// Destruction, allowed only if empty.
    ~value_type () YY_NOEXCEPT
    {}

# if 201103L <= YY_CPLUSPLUS
    /// Instantiate a \a T in here from \a t.
    template <typename T, typename... U>
    T&
    emplace (U&&... u)
    {
      return *new (yyas_<T> ()) T (std::forward <U>(u)...);
    }
# else
    /// Instantiate an empty \a T in here.
    template <typename T>
    T&
    emplace ()
    {
      return *new (yyas_<T> ()) T ();
    }

    /// Instantiate a \a T in here from \a t.
    template <typename T>
    T&
    emplace (const T& t)
    {
      return *new (yyas_<T> ()) T (t);
    }
# endif

    /// Instantiate an empty \a T in here.
    /// Obsolete, use emplace.
    template <typename T>
    T&
    build ()
    {
      return emplace<T> ();
    }

    /// Instantiate a \a T in here from \a t.
    /// Obsolete, use emplace.
    template <typename T>
    T&
    build (const T& t)
    {
      return emplace<T> (t);
    }

    /// Accessor to a built \a T.
    template <typename T>
    T&
    as () YY_NOEXCEPT
    {
      return *yyas_<T> ();
    }

    /// Const accessor to a built \a T (for %printer).
    template <typename T>
    const T&
    as () const YY_NOEXCEPT
    {
      return *yyas_<T> ();
    }

    /// Swap the content with \a that, of same type.
    ///
    /// Both variants must be built beforehand, because swapping the actual
    /// data requires reading it (with as()), and this is not possible on
    /// unconstructed variants: it would require some dynamic testing, which
    /// should not be the variant's responsibility.
    /// Swapping between built and (possibly) non-built is done with
    /// self_type::move ().
    template <typename T>
    void
    swap (self_type& that) YY_NOEXCEPT
    {
      std::swap (as<T> (), that.as<T> ());
    }

    /// Move the content of \a that to this.
    ///
    /// Destroys \a that.
    template <typename T>
    void
    move (self_type& that)
    {
# if 201103L <= YY_CPLUSPLUS
      emplace<T> (std::move (that.as<T> ()));
# else
      emplace<T> ();
      swap<T> (that);
# endif
      that.destroy<T> ();
    }

# if 201103L <= YY_CPLUSPLUS
    /// Move the content of \a that to this.
    template <typename T>
    void
    move (self_type&& that)
    {
      emplace<T> (std::move (that.as<T> ()));
      that.destroy<T> ();
    }
#endif

    /// Copy the content of \a that to this.
    template <typename T>
    void
    copy (const self_type& that)
    {
      emplace<T> (that.as<T> ());
    }

    /// Destroy the stored \a T.
    template <typename T>
    void
    destroy ()
    {
      as<T> ().~T ();
    }

  private:
#if YY_CPLUSPLUS < 201103L
    /// Non copyable.
    value_type (const self_type&);
    /// Non copyable.
    self_type& operator= (const self_type&);
#endif

    /// Accessor to raw memory as \a T.
    template <typename T>
    T*
    yyas_ () YY_NOEXCEPT
    {
      void *yyp = yyraw_;
      return static_cast<T*> (yyp);
     }

    /// Const accessor to raw memory as \a T.
    template <typename T>
    const T*
    yyas_ () const YY_NOEXCEPT
    {
      const void *yyp = yyraw_;
      return static_cast<const T*> (yyp);
     }

    /// An auxiliary type to compute the largest semantic type.
    union union_type
    {
      // INTEGER
      char dummy1[sizeof (long long)];

      // PATH_VALUE
      // ID_VALUE
      // PASSWORD_VALUE
      char dummy2[sizeof (std::string)];
    };

    /// The size of the largest semantic type.
    enum { size = sizeof (union_type) };

    /// A buffer to store semantic values.
    union
    {
      /// Strongest alignment constraints.
      long double yyalign_me_;
      /// A buffer large enough to store any of the semantic values.
      char yyraw_[size];
    };
  };

#endif
    /// Backward compatibility (Bison 3.8).
    typedef value_type semantic_type;


    /// Syntax errors thrown from user actions.
    struct syntax_error : std::runtime_error
    {
      syntax_error (const std::string& m)
        : std::runtime_error (m)
      {}

      syntax_error (const syntax_error& s)
        : std::runtime_error (s.what ())
      {}

      ~syntax_error () YY_NOEXCEPT YY_NOTHROW;
    };

    /// Token kinds.
    struct token
    {
      enum token_kind_type
      {
        YYEMPTY = -2,
    YYEOF = 0,                     // "end of file"
    YYerror = 256,                 // error
    YYUNDEF = 257,                 // "invalid token"
    SIZE = 258,                    // SIZE
    FIT = 259,                     // FIT
    BF = 260,                      // BF
    FF = 261,                      // FF
    WF = 262,                      // WF
    UNIT = 263,                    // UNIT
    B = 264,                       // B
    K = 265,                       // K
    M = 266,                       // M
    PATH = 267,                    // PATH
    TYPE = 268,                    // TYPE
    P = 269,                       // P
    E = 270,                       // E
    L = 271,                       // L
    NAME = 272,                    // NAME
    ID = 273,                      // ID
    USER = 274,                    // USER
    GROUP = 275,                   // GROUP
    R = 276,                       // R
    COUNT = 277,                   // COUNT
    FULL = 278,                    // FULL
    MKDISK = 279,                  // MKDISK
    RMDISK = 280,                  // RMDISK
    FDISK = 281,                   // FDISK
    MOUNT = 282,                   // MOUNT
    MKFS = 283,                    // MKFS
    MKUSR = 284,                   // MKUSR
    RMUSR = 285,                   // RMUSR
    MKFILE = 286,                  // MKFILE
    PASSWORD = 287,                // PASSWORD
    INTEGER = 288,                 // INTEGER
    PATH_VALUE = 289,              // PATH_VALUE
    ID_VALUE = 290,                // ID_VALUE
    PASSWORD_VALUE = 291,          // PASSWORD_VALUE
    EQUAL = 292,                   // EQUAL
    EOL = 293                      // EOL
      };
      /// Backward compatibility alias (Bison 3.6).
      typedef token_kind_type yytokentype;
    };

    /// Token kind, as returned by yylex.
    typedef token::token_kind_type token_kind_type;

    /// Backward compatibility alias (Bison 3.6).
    typedef token_kind_type token_type;

    /// Symbol kinds.
    struct symbol_kind
    {
      enum symbol_kind_type
      {
        YYNTOKENS = 39, ///< Number of tokens.
        S_YYEMPTY = -2,
        S_YYEOF = 0,                             // "end of file"
        S_YYerror = 1,                           // error
        S_YYUNDEF = 2,                           // "invalid token"
        S_SIZE = 3,                              // SIZE
        S_FIT = 4,                               // FIT
        S_BF = 5,                                // BF
        S_FF = 6,                                // FF
        S_WF = 7,                                // WF
        S_UNIT = 8,                              // UNIT
        S_B = 9,                                 // B
        S_K = 10,                                // K
        S_M = 11,                                // M
        S_PATH = 12,                             // PATH
        S_TYPE = 13,                             // TYPE
        S_P = 14,                                // P
        S_E = 15,                                // E
        S_L = 16,                                // L
        S_NAME = 17,                             // NAME
        S_ID = 18,                               // ID
        S_USER = 19,                             // USER
        S_GROUP = 20,                            // GROUP
        S_R = 21,                                // R
        S_COUNT = 22,                            // COUNT
        S_FULL = 23,                             // FULL
        S_MKDISK = 24,                           // MKDISK
        S_RMDISK = 25,                           // RMDISK
        S_FDISK = 26,                            // FDISK
        S_MOUNT = 27,                            // MOUNT
        S_MKFS = 28,                             // MKFS
        S_MKUSR = 29,                            // MKUSR
        S_RMUSR = 30,                            // RMUSR
        S_MKFILE = 31,                           // MKFILE
        S_PASSWORD = 32,                         // PASSWORD
        S_INTEGER = 33,                          // INTEGER
        S_PATH_VALUE = 34,                       // PATH_VALUE
        S_ID_VALUE = 35,                         // ID_VALUE
        S_PASSWORD_VALUE = 36,                   // PASSWORD_VALUE
        S_EQUAL = 37,                            // EQUAL
        S_EOL = 38,                              // EOL
        S_YYACCEPT = 39,                         // $accept
        S_comandos = 40,                         // comandos
        S_comando = 41,                          // comando
        S_mkdisk = 42,                           // mkdisk
        S_mkdisk_params = 43,                    // mkdisk_params
        S_mkdisk_p = 44,                         // mkdisk_p
        S_fit_v = 45,                            // fit_v
        S_unit_v = 46,                           // unit_v
        S_rmdisk = 47,                           // rmdisk
        S_fdisk = 48,                            // fdisk
        S_fdisk_params = 49,                     // fdisk_params
        S_fdisk_p = 50,                          // fdisk_p
        S_unit_v2 = 51,                          // unit_v2
        S_type_v = 52,                           // type_v
        S_mount = 53,                            // mount
        S_mount_params = 54,                     // mount_params
        S_mount_p = 55,                          // mount_p
        S_mkfs = 56,                             // mkfs
        S_mkfs_params = 57,                      // mkfs_params
        S_mkfs_p = 58,                           // mkfs_p
        S_mkusr = 59,                            // mkusr
        S_mkusr_params = 60,                     // mkusr_params
        S_mkusr_p = 61,                          // mkusr_p
        S_rmusr = 62,                            // rmusr
        S_mkfile = 63,                           // mkfile
        S_mkfile_params = 64,                    // mkfile_params
        S_mkfile_p = 65                          // mkfile_p
      };
    };

    /// (Internal) symbol kind.
    typedef symbol_kind::symbol_kind_type symbol_kind_type;

    /// The number of tokens.
    static const symbol_kind_type YYNTOKENS = symbol_kind::YYNTOKENS;

    /// A complete symbol.
    ///
    /// Expects its Base type to provide access to the symbol kind
    /// via kind ().
    ///
    /// Provide access to semantic value.
    template <typename Base>
    struct basic_symbol : Base
    {
      /// Alias to Base.
      typedef Base super_type;

      /// Default constructor.
      basic_symbol () YY_NOEXCEPT
        : value ()
      {}

#if 201103L <= YY_CPLUSPLUS
      /// Move constructor.
      basic_symbol (basic_symbol&& that)
        : Base (std::move (that))
        , value ()
      {
        switch (this->kind ())
    {
      case symbol_kind::S_INTEGER: // INTEGER
        value.move< long long > (std::move (that.value));
        break;

      case symbol_kind::S_PATH_VALUE: // PATH_VALUE
      case symbol_kind::S_ID_VALUE: // ID_VALUE
      case symbol_kind::S_PASSWORD_VALUE: // PASSWORD_VALUE
        value.move< std::string > (std::move (that.value));
        break;

      default:
        break;
    }

      }
#endif

      /// Copy constructor.
      basic_symbol (const basic_symbol& that);

      /// Constructors for typed symbols.
#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t)
        : Base (t)
      {}
#else
      basic_symbol (typename Base::kind_type t)
        : Base (t)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, long long&& v)
        : Base (t)
        , value (std::move (v))
      {}
#else
      basic_symbol (typename Base::kind_type t, const long long& v)
        : Base (t)
        , value (v)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, std::string&& v)
        : Base (t)
        , value (std::move (v))
      {}
#else
      basic_symbol (typename Base::kind_type t, const std::string& v)
        : Base (t)
        , value (v)
      {}
#endif

      /// Destroy the symbol.
      ~basic_symbol ()
      {
        clear ();
      }



      /// Destroy contents, and record that is empty.
      void clear () YY_NOEXCEPT
      {
        // User destructor.
        symbol_kind_type yykind = this->kind ();
        basic_symbol<Base>& yysym = *this;
        (void) yysym;
        switch (yykind)
        {
       default:
          break;
        }

        // Value type destructor.
switch (yykind)
    {
      case symbol_kind::S_INTEGER: // INTEGER
        value.template destroy< long long > ();
        break;

      case symbol_kind::S_PATH_VALUE: // PATH_VALUE
      case symbol_kind::S_ID_VALUE: // ID_VALUE
      case symbol_kind::S_PASSWORD_VALUE: // PASSWORD_VALUE
        value.template destroy< std::string > ();
        break;

      default:
        break;
    }

        Base::clear ();
      }

#if YYDEBUG || 0
      /// The user-facing name of this symbol.
      const char *name () const YY_NOEXCEPT
      {
        return Parser::symbol_name (this->kind ());
      }
#endif // #if YYDEBUG || 0


      /// Backward compatibility (Bison 3.6).
      symbol_kind_type type_get () const YY_NOEXCEPT;

      /// Whether empty.
      bool empty () const YY_NOEXCEPT;

      /// Destructive move, \a s is emptied into this.
      void move (basic_symbol& s);

      /// The semantic value.
      value_type value;

    private:
#if YY_CPLUSPLUS < 201103L
      /// Assignment operator.
      basic_symbol& operator= (const basic_symbol& that);
#endif
    };

    /// Type access provider for token (enum) based symbols.
    struct by_kind
    {
      /// The symbol kind as needed by the constructor.
      typedef token_kind_type kind_type;

      /// Default constructor.
      by_kind () YY_NOEXCEPT;

#if 201103L <= YY_CPLUSPLUS
      /// Move constructor.
      by_kind (by_kind&& that) YY_NOEXCEPT;
#endif

      /// Copy constructor.
      by_kind (const by_kind& that) YY_NOEXCEPT;

      /// Constructor from (external) token numbers.
      by_kind (kind_type t) YY_NOEXCEPT;



      /// Record that this symbol is empty.
      void clear () YY_NOEXCEPT;

      /// Steal the symbol kind from \a that.
      void move (by_kind& that);

      /// The (internal) type number (corresponding to \a type).
      /// \a empty when empty.
      symbol_kind_type kind () const YY_NOEXCEPT;

      /// Backward compatibility (Bison 3.6).
      symbol_kind_type type_get () const YY_NOEXCEPT;

      /// The symbol kind.
      /// \a S_YYEMPTY when empty.
      symbol_kind_type kind_;
    };

    /// Backward compatibility for a private implementation detail (Bison 3.6).
    typedef by_kind by_type;

    /// "External" symbols: returned by the scanner.
    struct symbol_type : basic_symbol<by_kind>
    {
      /// Superclass.
      typedef basic_symbol<by_kind> super_type;

      /// Empty symbol.
      symbol_type () YY_NOEXCEPT {}

      /// Constructor for valueless symbols, and symbols from each type.
#if 201103L <= YY_CPLUSPLUS
      symbol_type (int tok)
        : super_type (token_kind_type (tok))
#else
      symbol_type (int tok)
        : super_type (token_kind_type (tok))
#endif
      {}
#if 201103L <= YY_CPLUSPLUS
      symbol_type (int tok, long long v)
        : super_type (token_kind_type (tok), std::move (v))
#else
      symbol_type (int tok, const long long& v)
        : super_type (token_kind_type (tok), v)
#endif
      {}
#if 201103L <= YY_CPLUSPLUS
      symbol_type (int tok, std::string v)
        : super_type (token_kind_type (tok), std::move (v))
#else
      symbol_type (int tok, const std::string& v)
        : super_type (token_kind_type (tok), v)
#endif
      {}
    };

    /// Build a parser object.
    Parser (yyscan_t scanner_yyarg);
    virtual ~Parser ();

#if 201103L <= YY_CPLUSPLUS
    /// Non copyable.
    Parser (const Parser&) = delete;
    /// Non copyable.
    Parser& operator= (const Parser&) = delete;
#endif

    /// Parse.  An alias for parse ().
    /// \returns  0 iff parsing succeeded.
    int operator() ();

    /// Parse.
    /// \returns  0 iff parsing succeeded.
    virtual int parse ();

#if YYDEBUG
    /// The current debugging stream.
    std::ostream& debug_stream () const YY_ATTRIBUTE_PURE;
    /// Set the current debugging stream.
    void set_debug_stream (std::ostream &);

    /// Type for debugging levels.
    typedef int debug_level_type;
    /// The current debugging level.
    debug_level_type debug_level () const YY_ATTRIBUTE_PURE;
    /// Set the current debugging level.
    void set_debug_level (debug_level_type l);
#endif

    /// Report a syntax error.
    /// \param msg    a description of the syntax error.
    virtual void error (const std::string& msg);

    /// Report a syntax error.
    void error (const syntax_error& err);

#if YYDEBUG || 0
    /// The user-facing name of the symbol whose (internal) number is
    /// YYSYMBOL.  No bounds checking.
    static const char *symbol_name (symbol_kind_type yysymbol);
#endif // #if YYDEBUG || 0


    // Implementation of make_symbol for each token kind.
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_YYEOF ()
      {
        return symbol_type (token::YYEOF);
      }
#else
      static
      symbol_type
      make_YYEOF ()
      {
        return symbol_type (token::YYEOF);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_YYerror ()
      {
        return symbol_type (token::YYerror);
      }
#else
      static
      symbol_type
      make_YYerror ()
      {
        return symbol_type (token::YYerror);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_YYUNDEF ()
      {
        return symbol_type (token::YYUNDEF);
      }
#else
      static
      symbol_type
      make_YYUNDEF ()
      {
        return symbol_type (token::YYUNDEF);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_SIZE ()
      {
        return symbol_type (token::SIZE);
      }
#else
      static
      symbol_type
      make_SIZE ()
      {
        return symbol_type (token::SIZE);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FIT ()
      {
        return symbol_type (token::FIT);
      }
#else
      static
      symbol_type
      make_FIT ()
      {
        return symbol_type (token::FIT);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_BF ()
      {
        return symbol_type (token::BF);
      }
#else
      static
      symbol_type
      make_BF ()
      {
        return symbol_type (token::BF);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FF ()
      {
        return symbol_type (token::FF);
      }
#else
      static
      symbol_type
      make_FF ()
      {
        return symbol_type (token::FF);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_WF ()
      {
        return symbol_type (token::WF);
      }
#else
      static
      symbol_type
      make_WF ()
      {
        return symbol_type (token::WF);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_UNIT ()
      {
        return symbol_type (token::UNIT);
      }
#else
      static
      symbol_type
      make_UNIT ()
      {
        return symbol_type (token::UNIT);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_B ()
      {
        return symbol_type (token::B);
      }
#else
      static
      symbol_type
      make_B ()
      {
        return symbol_type (token::B);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_K ()
      {
        return symbol_type (token::K);
      }
#else
      static
      symbol_type
      make_K ()
      {
        return symbol_type (token::K);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_M ()
      {
        return symbol_type (token::M);
      }
#else
      static
      symbol_type
      make_M ()
      {
        return symbol_type (token::M);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_PATH ()
      {
        return symbol_type (token::PATH);
      }
#else
      static
      symbol_type
      make_PATH ()
      {
        return symbol_type (token::PATH);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_TYPE ()
      {
        return symbol_type (token::TYPE);
      }
#else
      static
      symbol_type
      make_TYPE ()
      {
        return symbol_type (token::TYPE);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_P ()
      {
        return symbol_type (token::P);
      }
#else
      static
      symbol_type
      make_P ()
      {
        return symbol_type (token::P);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_E ()
      {
        return symbol_type (token::E);
      }
#else
      static
      symbol_type
      make_E ()
      {
        return symbol_type (token::E);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_L ()
      {
        return symbol_type (token::L);
      }
#else
      static
      symbol_type
      make_L ()
      {
        return symbol_type (token::L);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_NAME ()
      {
        return symbol_type (token::NAME);
      }
#else
      static
      symbol_type
      make_NAME ()
      {
        return symbol_type (token::NAME);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_ID ()
      {
        return symbol_type (token::ID);
      }
#else
      static
      symbol_type
      make_ID ()
      {
        return symbol_type (token::ID);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_USER ()
      {
        return symbol_type (token::USER);
      }
#else
      static
      symbol_type
      make_USER ()
      {
        return symbol_type (token::USER);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_GROUP ()
      {
        return symbol_type (token::GROUP);
      }
#else
      static
      symbol_type
      make_GROUP ()
      {
        return symbol_type (token::GROUP);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_R ()
      {
        return symbol_type (token::R);
      }
#else
      static
      symbol_type
      make_R ()
      {
        return symbol_type (token::R);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_COUNT ()
      {
        return symbol_type (token::COUNT);
      }
#else
      static
      symbol_type
      make_COUNT ()
      {
        return symbol_type (token::COUNT);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FULL ()
      {
        return symbol_type (token::FULL);
      }
#else
      static
      symbol_type
      make_FULL ()
      {
        return symbol_type (token::FULL);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_MKDISK ()
      {
        return symbol_type (token::MKDISK);
      }
#else
      static
      symbol_type
      make_MKDISK ()
      {
        return symbol_type (token::MKDISK);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_RMDISK ()
      {
        return symbol_type (token::RMDISK);
      }
#else
      static
      symbol_type
      make_RMDISK ()
      {
        return symbol_type (token::RMDISK);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FDISK ()
      {
        return symbol_type (token::FDISK);
      }
#else
      static
      symbol_type
      make_FDISK ()
      {
        return symbol_type (token::FDISK);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_MOUNT ()
      {
        return symbol_type (token::MOUNT);
      }
#else
      static
      symbol_type
      make_MOUNT ()
      {
        return symbol_type (token::MOUNT);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_MKFS ()
      {
        return symbol_type (token::MKFS);
      }
#else
      static
      symbol_type
      make_MKFS ()
      {
        return symbol_type (token::MKFS);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_MKUSR ()
      {
        return symbol_type (token::MKUSR);
      }
#else
      static
      symbol_type
      make_MKUSR ()
      {
        return symbol_type (token::MKUSR);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_RMUSR ()
      {
        return symbol_type (token::RMUSR);
      }
#else
      static
      symbol_type
      make_RMUSR ()
      {
        return symbol_type (token::RMUSR);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_MKFILE ()
      {
        return symbol_type (token::MKFILE);
      }
#else
      static
      symbol_type
      make_MKFILE ()
      {
        return symbol_type (token::MKFILE);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_PASSWORD ()
      {
        return symbol_type (token::PASSWORD);
      }
#else
      static
      symbol_type
      make_PASSWORD ()
      {
        return symbol_type (token::PASSWORD);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_INTEGER (long long v)
      {
        return symbol_type (token::INTEGER, std::move (v));
      }
#else
      static
      symbol_type
      make_INTEGER (const long long& v)
      {
        return symbol_type (token::INTEGER, v);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_PATH_VALUE (std::string v)
      {
        return symbol_type (token::PATH_VALUE, std::move (v));
      }
#else
      static
      symbol_type
      make_PATH_VALUE (const std::string& v)
      {
        return symbol_type (token::PATH_VALUE, v);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_ID_VALUE (std::string v)
      {
        return symbol_type (token::ID_VALUE, std::move (v));
      }
#else
      static
      symbol_type
      make_ID_VALUE (const std::string& v)
      {
        return symbol_type (token::ID_VALUE, v);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_PASSWORD_VALUE (std::string v)
      {
        return symbol_type (token::PASSWORD_VALUE, std::move (v));
      }
#else
      static
      symbol_type
      make_PASSWORD_VALUE (const std::string& v)
      {
        return symbol_type (token::PASSWORD_VALUE, v);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_EQUAL ()
      {
        return symbol_type (token::EQUAL);
      }
#else
      static
      symbol_type
      make_EQUAL ()
      {
        return symbol_type (token::EQUAL);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_EOL ()
      {
        return symbol_type (token::EOL);
      }
#else
      static
      symbol_type
      make_EOL ()
      {
        return symbol_type (token::EOL);
      }
#endif


  private:
#if YY_CPLUSPLUS < 201103L
    /// Non copyable.
    Parser (const Parser&);
    /// Non copyable.
    Parser& operator= (const Parser&);
#endif


    /// Stored state numbers (used for stacks).
    typedef signed char state_type;

    /// Compute post-reduction state.
    /// \param yystate   the current state
    /// \param yysym     the nonterminal to push on the stack
    static state_type yy_lr_goto_state_ (state_type yystate, int yysym);

    /// Whether the given \c yypact_ value indicates a defaulted state.
    /// \param yyvalue   the value to check
    static bool yy_pact_value_is_default_ (int yyvalue) YY_NOEXCEPT;

    /// Whether the given \c yytable_ value indicates a syntax error.
    /// \param yyvalue   the value to check
    static bool yy_table_value_is_error_ (int yyvalue) YY_NOEXCEPT;

    static const signed char yypact_ninf_;
    static const signed char yytable_ninf_;

    /// Convert a scanner token kind \a t to a symbol kind.
    /// In theory \a t should be a token_kind_type, but character literals
    /// are valid, yet not members of the token_kind_type enum.
    static symbol_kind_type yytranslate_ (int t) YY_NOEXCEPT;

#if YYDEBUG || 0
    /// For a symbol, its name in clear.
    static const char* const yytname_[];
#endif // #if YYDEBUG || 0


    // Tables.
    // YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
    // STATE-NUM.
    static const signed char yypact_[];

    // YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
    // Performed when YYTABLE does not specify something else to do.  Zero
    // means the default is an error.
    static const signed char yydefact_[];

    // YYPGOTO[NTERM-NUM].
    static const signed char yypgoto_[];

    // YYDEFGOTO[NTERM-NUM].
    static const signed char yydefgoto_[];

    // YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
    // positive, shift that token.  If negative, reduce the rule whose
    // number is the opposite.  If YYTABLE_NINF, syntax error.
    static const signed char yytable_[];

    static const signed char yycheck_[];

    // YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
    // state STATE-NUM.
    static const signed char yystos_[];

    // YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.
    static const signed char yyr1_[];

    // YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.
    static const signed char yyr2_[];


#if YYDEBUG
    // YYRLINE[YYN] -- Source line where rule number YYN was defined.
    static const short yyrline_[];
    /// Report on the debug stream that the rule \a r is going to be reduced.
    virtual void yy_reduce_print_ (int r) const;
    /// Print the state stack on the debug stream.
    virtual void yy_stack_print_ () const;

    /// Debugging level.
    int yydebug_;
    /// Debug stream.
    std::ostream* yycdebug_;

    /// \brief Display a symbol kind, value and location.
    /// \param yyo    The output stream.
    /// \param yysym  The symbol.
    template <typename Base>
    void yy_print_ (std::ostream& yyo, const basic_symbol<Base>& yysym) const;
#endif

    /// \brief Reclaim the memory associated to a symbol.
    /// \param yymsg     Why this token is reclaimed.
    ///                  If null, print nothing.
    /// \param yysym     The symbol.
    template <typename Base>
    void yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const;

  private:
    /// Type access provider for state based symbols.
    struct by_state
    {
      /// Default constructor.
      by_state () YY_NOEXCEPT;

      /// The symbol kind as needed by the constructor.
      typedef state_type kind_type;

      /// Constructor.
      by_state (kind_type s) YY_NOEXCEPT;

      /// Copy constructor.
      by_state (const by_state& that) YY_NOEXCEPT;

      /// Record that this symbol is empty.
      void clear () YY_NOEXCEPT;

      /// Steal the symbol kind from \a that.
      void move (by_state& that);

      /// The symbol kind (corresponding to \a state).
      /// \a symbol_kind::S_YYEMPTY when empty.
      symbol_kind_type kind () const YY_NOEXCEPT;

      /// The state number used to denote an empty symbol.
      /// We use the initial state, as it does not have a value.
      enum { empty_state = 0 };

      /// The state.
      /// \a empty when empty.
      state_type state;
    };

    /// "Internal" symbol: element of the stack.
    struct stack_symbol_type : basic_symbol<by_state>
    {
      /// Superclass.
      typedef basic_symbol<by_state> super_type;
      /// Construct an empty symbol.
      stack_symbol_type ();
      /// Move or copy construction.
      stack_symbol_type (YY_RVREF (stack_symbol_type) that);
      /// Steal the contents from \a sym to build this.
      stack_symbol_type (state_type s, YY_MOVE_REF (symbol_type) sym);
#if YY_CPLUSPLUS < 201103L
      /// Assignment, needed by push_back by some old implementations.
      /// Moves the contents of that.
      stack_symbol_type& operator= (stack_symbol_type& that);

      /// Assignment, needed by push_back by other implementations.
      /// Needed by some other old implementations.
      stack_symbol_type& operator= (const stack_symbol_type& that);
#endif
    };

    /// A stack with random access from its top.
    template <typename T, typename S = std::vector<T> >
    class stack
    {
    public:
      // Hide our reversed order.
      typedef typename S::iterator iterator;
      typedef typename S::const_iterator const_iterator;
      typedef typename S::size_type size_type;
      typedef typename std::ptrdiff_t index_type;

      stack (size_type n = 200) YY_NOEXCEPT
        : seq_ (n)
      {}

#if 201103L <= YY_CPLUSPLUS
      /// Non copyable.
      stack (const stack&) = delete;
      /// Non copyable.
      stack& operator= (const stack&) = delete;
#endif

      /// Random access.
      ///
      /// Index 0 returns the topmost element.
      const T&
      operator[] (index_type i) const
      {
        return seq_[size_type (size () - 1 - i)];
      }

      /// Random access.
      ///
      /// Index 0 returns the topmost element.
      T&
      operator[] (index_type i)
      {
        return seq_[size_type (size () - 1 - i)];
      }

      /// Steal the contents of \a t.
      ///
      /// Close to move-semantics.
      void
      push (YY_MOVE_REF (T) t)
      {
        seq_.push_back (T ());
        operator[] (0).move (t);
      }

      /// Pop elements from the stack.
      void
      pop (std::ptrdiff_t n = 1) YY_NOEXCEPT
      {
        for (; 0 < n; --n)
          seq_.pop_back ();
      }

      /// Pop all elements from the stack.
      void
      clear () YY_NOEXCEPT
      {
        seq_.clear ();
      }

      /// Number of elements on the stack.
      index_type
      size () const YY_NOEXCEPT
      {
        return index_type (seq_.size ());
      }

      /// Iterator on top of the stack (going downwards).
      const_iterator
      begin () const YY_NOEXCEPT
      {
        return seq_.begin ();
      }

      /// Bottom of the stack.
      const_iterator
      end () const YY_NOEXCEPT
      {
        return seq_.end ();
      }

      /// Present a slice of the top of a stack.
      class slice
      {
      public:
        slice (const stack& stack, index_type range) YY_NOEXCEPT
          : stack_ (stack)
          , range_ (range)
        {}

        const T&
        operator[] (index_type i) const
        {
          return stack_[range_ - i];
        }

      private:
        const stack& stack_;
        index_type range_;
      };

    private:
#if YY_CPLUSPLUS < 201103L
      /// Non copyable.
      stack (const stack&);
      /// Non copyable.
      stack& operator= (const stack&);
#endif
      /// The wrapped container.
      S seq_;
    };


    /// Stack type.
    typedef stack<stack_symbol_type> stack_type;

    /// The stack.
    stack_type yystack_;

    /// Push a new state on the stack.
    /// \param m    a debug message to display
    ///             if null, no trace is output.
    /// \param sym  the symbol
    /// \warning the contents of \a s.value is stolen.
    void yypush_ (const char* m, YY_MOVE_REF (stack_symbol_type) sym);

    /// Push a new look ahead token on the state on the stack.
    /// \param m    a debug message to display
    ///             if null, no trace is output.
    /// \param s    the state
    /// \param sym  the symbol (for its value and location).
    /// \warning the contents of \a sym.value is stolen.
    void yypush_ (const char* m, state_type s, YY_MOVE_REF (symbol_type) sym);

    /// Pop \a n symbols from the stack.
    void yypop_ (int n = 1) YY_NOEXCEPT;

    /// Constants.
    enum
    {
      yylast_ = 107,     ///< Last index in yytable_.
      yynnts_ = 27,  ///< Number of nonterminal symbols.
      yyfinal_ = 2 ///< Termination state number.
    };


    // User arguments.
    yyscan_t scanner;

  };


#line 12 "grammar.y"
} // calc
#line 1743 "Parser1.hpp"


// "%code provides" blocks.
#line 22 "grammar.y"

        #define YY_DECL \
        int yylex(calc::Parser::semantic_type *yylval_param, yyscan_t yyscanner)

    YY_DECL;

#line 1754 "Parser1.hpp"


#endif // !YY_YY_PARSER1_HPP_INCLUDED
