/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "parser/SysY_parser.y"

#include <fstream>
#include "SysY_tree.h"
#include "type.h"
Program ast_root;

void yyerror(char *s, ...);
int yylex();
int error_num = 0;
extern int line_number;
extern std::ofstream fout;
extern IdTable id_table;

#line 85 "SysY_parser.tab.c"

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

#include "SysY_parser.tab.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_STR_CONST = 3,                  /* STR_CONST  */
  YYSYMBOL_IDENT = 4,                      /* IDENT  */
  YYSYMBOL_FLOAT_CONST = 5,                /* FLOAT_CONST  */
  YYSYMBOL_INT_CONST = 6,                  /* INT_CONST  */
  YYSYMBOL_LEQ = 7,                        /* LEQ  */
  YYSYMBOL_GEQ = 8,                        /* GEQ  */
  YYSYMBOL_EQ = 9,                         /* EQ  */
  YYSYMBOL_NE = 10,                        /* NE  */
  YYSYMBOL_AND = 11,                       /* AND  */
  YYSYMBOL_OR = 12,                        /* OR  */
  YYSYMBOL_CONST = 13,                     /* CONST  */
  YYSYMBOL_IF = 14,                        /* IF  */
  YYSYMBOL_ELSE = 15,                      /* ELSE  */
  YYSYMBOL_WHILE = 16,                     /* WHILE  */
  YYSYMBOL_NONE_TYPE = 17,                 /* NONE_TYPE  */
  YYSYMBOL_INT = 18,                       /* INT  */
  YYSYMBOL_FLOAT = 19,                     /* FLOAT  */
  YYSYMBOL_FOR = 20,                       /* FOR  */
  YYSYMBOL_RETURN = 21,                    /* RETURN  */
  YYSYMBOL_BREAK = 22,                     /* BREAK  */
  YYSYMBOL_CONTINUE = 23,                  /* CONTINUE  */
  YYSYMBOL_ERROR = 24,                     /* ERROR  */
  YYSYMBOL_TODO = 25,                      /* TODO  */
  YYSYMBOL_THEN = 26,                      /* THEN  */
  YYSYMBOL_27_ = 27,                       /* ';'  */
  YYSYMBOL_28_ = 28,                       /* ','  */
  YYSYMBOL_29_ = 29,                       /* '('  */
  YYSYMBOL_30_ = 30,                       /* ')'  */
  YYSYMBOL_31_ = 31,                       /* '['  */
  YYSYMBOL_32_ = 32,                       /* ']'  */
  YYSYMBOL_33_ = 33,                       /* '='  */
  YYSYMBOL_34_ = 34,                       /* '{'  */
  YYSYMBOL_35_ = 35,                       /* '}'  */
  YYSYMBOL_36_ = 36,                       /* '+'  */
  YYSYMBOL_37_ = 37,                       /* '-'  */
  YYSYMBOL_38_ = 38,                       /* '!'  */
  YYSYMBOL_39_ = 39,                       /* '*'  */
  YYSYMBOL_40_ = 40,                       /* '/'  */
  YYSYMBOL_41_ = 41,                       /* '%'  */
  YYSYMBOL_42_ = 42,                       /* '<'  */
  YYSYMBOL_43_ = 43,                       /* '>'  */
  YYSYMBOL_YYACCEPT = 44,                  /* $accept  */
  YYSYMBOL_Program = 45,                   /* Program  */
  YYSYMBOL_Comp_list = 46,                 /* Comp_list  */
  YYSYMBOL_CompUnit = 47,                  /* CompUnit  */
  YYSYMBOL_Decl = 48,                      /* Decl  */
  YYSYMBOL_VarDecl = 49,                   /* VarDecl  */
  YYSYMBOL_ConstDecl = 50,                 /* ConstDecl  */
  YYSYMBOL_VarDef_list = 51,               /* VarDef_list  */
  YYSYMBOL_ConstDef_list = 52,             /* ConstDef_list  */
  YYSYMBOL_FuncDef = 53,                   /* FuncDef  */
  YYSYMBOL_Array_list = 54,                /* Array_list  */
  YYSYMBOL_Constarray_list = 55,           /* Constarray_list  */
  YYSYMBOL_VarDef = 56,                    /* VarDef  */
  YYSYMBOL_ConstDef = 57,                  /* ConstDef  */
  YYSYMBOL_ConstInitVal = 58,              /* ConstInitVal  */
  YYSYMBOL_VarInitVal = 59,                /* VarInitVal  */
  YYSYMBOL_ConstInitVal_list = 60,         /* ConstInitVal_list  */
  YYSYMBOL_VarInitVal_list = 61,           /* VarInitVal_list  */
  YYSYMBOL_FuncFParams = 62,               /* FuncFParams  */
  YYSYMBOL_FuncFParam = 63,                /* FuncFParam  */
  YYSYMBOL_Block = 64,                     /* Block  */
  YYSYMBOL_BlockItem_list = 65,            /* BlockItem_list  */
  YYSYMBOL_BlockItem = 66,                 /* BlockItem  */
  YYSYMBOL_Stmt = 67,                      /* Stmt  */
  YYSYMBOL_Exp = 68,                       /* Exp  */
  YYSYMBOL_Cond = 69,                      /* Cond  */
  YYSYMBOL_Lval = 70,                      /* Lval  */
  YYSYMBOL_PrimaryExp = 71,                /* PrimaryExp  */
  YYSYMBOL_IntConst = 72,                  /* IntConst  */
  YYSYMBOL_FloatConst = 73,                /* FloatConst  */
  YYSYMBOL_UnaryExp = 74,                  /* UnaryExp  */
  YYSYMBOL_FuncRParams = 75,               /* FuncRParams  */
  YYSYMBOL_Exp_list = 76,                  /* Exp_list  */
  YYSYMBOL_MulExp = 77,                    /* MulExp  */
  YYSYMBOL_AddExp = 78,                    /* AddExp  */
  YYSYMBOL_RelExp = 79,                    /* RelExp  */
  YYSYMBOL_EqExp = 80,                     /* EqExp  */
  YYSYMBOL_LAndExp = 81,                   /* LAndExp  */
  YYSYMBOL_LOrExp = 82,                    /* LOrExp  */
  YYSYMBOL_ConstExp = 83                   /* ConstExp  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_uint8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
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


#define YY_ASSERT(E) ((void) (0 && (E)))

#if !defined yyoverflow

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL \
             && defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
  YYLTYPE yyls_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE) \
             + YYSIZEOF (YYLTYPE)) \
      + 2 * YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  20
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   282

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  44
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  40
/* YYNRULES -- Number of rules.  */
#define YYNRULES  106
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  202

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   281


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    38,     2,     2,     2,    41,     2,     2,
      29,    30,    39,    36,    28,    37,     2,    40,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    27,
      42,    33,    43,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    31,     2,    32,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    34,     2,    35,     2,     2,     2,     2,
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
      25,    26
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,    68,    68,    76,    81,    88,    92,    99,   103,   110,
     115,   124,   129,   139,   143,   152,   156,   164,   169,   175,
     180,   185,   190,   200,   205,   212,   217,   225,   227,   230,
     235,   245,   250,   259,   264,   269,   278,   283,   288,   297,
     301,   310,   314,   322,   326,   333,   338,   342,   348,   354,
     359,   368,   372,   380,   384,   393,   397,   405,   409,   413,
     417,   421,   425,   429,   433,   437,   441,   445,   478,   482,
     487,   491,   499,   503,   507,   511,   519,   526,   533,   534,
     538,   562,   566,   570,   578,   587,   591,   601,   605,   609,
     613,   621,   625,   629,   637,   641,   645,   649,   653,   661,
     665,   669,   677,   681,   689,   693,   702
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "STR_CONST", "IDENT",
  "FLOAT_CONST", "INT_CONST", "LEQ", "GEQ", "EQ", "NE", "AND", "OR",
  "CONST", "IF", "ELSE", "WHILE", "NONE_TYPE", "INT", "FLOAT", "FOR",
  "RETURN", "BREAK", "CONTINUE", "ERROR", "TODO", "THEN", "';'", "','",
  "'('", "')'", "'['", "']'", "'='", "'{'", "'}'", "'+'", "'-'", "'!'",
  "'*'", "'/'", "'%'", "'<'", "'>'", "$accept", "Program", "Comp_list",
  "CompUnit", "Decl", "VarDecl", "ConstDecl", "VarDef_list",
  "ConstDef_list", "FuncDef", "Array_list", "Constarray_list", "VarDef",
  "ConstDef", "ConstInitVal", "VarInitVal", "ConstInitVal_list",
  "VarInitVal_list", "FuncFParams", "FuncFParam", "Block",
  "BlockItem_list", "BlockItem", "Stmt", "Exp", "Cond", "Lval",
  "PrimaryExp", "IntConst", "FloatConst", "UnaryExp", "FuncRParams",
  "Exp_list", "MulExp", "AddExp", "RelExp", "EqExp", "LAndExp", "LOrExp",
  "ConstExp", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-164)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      87,    65,    18,    41,    48,    90,    87,  -164,  -164,  -164,
    -164,  -164,    54,    54,    63,    38,    91,  -164,   126,   105,
    -164,  -164,    11,   120,  -164,   122,     0,     2,   200,   196,
      17,  -164,   103,    10,  -164,   200,   210,    35,  -164,    54,
    -164,   107,   111,    94,     3,  -164,    94,    46,    58,  -164,
    -164,   200,   200,   200,   200,   109,  -164,  -164,  -164,  -164,
    -164,    62,   131,   174,  -164,  -164,   200,   196,    92,  -164,
      94,   110,  -164,   124,   184,  -164,  -164,   200,   210,  -164,
     127,   130,    59,  -164,   155,    94,  -164,    94,   222,   132,
     146,  -164,  -164,  -164,  -164,   200,   200,   200,   200,   200,
    -164,  -164,   -11,   149,  -164,  -164,    94,  -164,  -164,  -164,
       8,   159,  -164,   175,   185,   154,   158,   103,   103,   236,
     112,   204,  -164,  -164,  -164,  -164,   108,  -164,  -164,   208,
     212,  -164,  -164,  -164,  -164,  -164,   213,   227,  -164,  -164,
    -164,  -164,    62,    62,   196,  -164,  -164,  -164,   210,  -164,
    -164,   225,   225,   200,   200,  -164,   230,  -164,  -164,  -164,
    -164,  -164,   200,  -164,   200,  -164,  -164,   132,   132,   231,
     131,    19,   214,   251,   252,   237,  -164,   239,  -164,   148,
     200,   200,   200,   200,   200,   200,   200,   200,   148,  -164,
     253,   131,   131,   131,   131,    19,    19,   214,   251,  -164,
     148,  -164
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,     0,     0,     0,     0,     0,     2,     3,     5,     7,
       8,     6,     0,     0,     0,    28,     0,    13,    28,     0,
       1,     4,     0,     0,    15,     0,     0,     0,     0,     0,
      29,     9,     0,     0,    10,     0,     0,     0,    11,     0,
      12,     0,     0,     0,     0,    43,     0,     0,    70,    77,
      76,     0,     0,     0,     0,     0,    73,    78,    74,    75,
      87,    91,    68,     0,    27,    36,     0,     0,    28,    14,
       0,     0,   106,     0,     0,    31,    33,     0,     0,    16,
      45,    46,     0,    22,     0,     0,    18,     0,     0,    71,
       0,    81,    82,    83,    23,     0,     0,     0,     0,     0,
      37,    41,     0,     0,    30,    21,     0,    25,    34,    39,
       0,     0,    32,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    59,    52,    55,    60,     0,    53,    56,     0,
      73,    44,    20,    17,    80,    85,     0,    84,    72,    88,
      89,    90,    92,    93,     0,    38,    24,    19,     0,    35,
      26,    49,    50,     0,     0,    67,     0,    64,    65,    51,
      54,    58,     0,    79,     0,    42,    40,    47,    48,     0,
      94,    99,   102,   104,    69,     0,    66,     0,    86,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    57,
      61,    97,    98,    95,    96,   100,   101,   103,   105,    63,
       0,    62
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -164,  -164,  -164,   263,   -67,  -164,  -164,    -1,   257,  -164,
     -42,  -164,   243,   232,   -69,   -59,  -164,  -164,    14,   192,
     -36,  -164,   152,  -163,   -28,   123,   -80,  -164,  -164,  -164,
     -40,  -164,  -164,   151,    12,    69,    93,    95,  -164,   -24
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_uint8 yydefgoto[] =
{
       0,     5,     6,     7,     8,     9,    10,    16,    23,    11,
      30,    37,    17,    24,    75,    64,   110,   102,    44,    45,
     125,   126,   127,   128,    72,   169,    56,    57,    58,    59,
      60,   136,   137,    61,    62,   171,   172,   173,   174,    76
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint8 yytable[] =
{
      55,    65,   130,    19,   101,   109,    89,    83,   104,   112,
      86,    73,    91,    92,    93,   124,   190,   144,    41,    42,
      41,    42,    14,    90,   145,   199,   180,   181,    41,    42,
      43,    84,    46,    85,   105,    65,   148,   201,   103,    65,
      70,    47,    35,   149,    36,    15,   130,    71,    66,   132,
      67,   133,    18,   111,   129,   139,   140,   141,    22,   124,
     135,   182,   183,    48,    49,    50,    77,    27,    78,    28,
     147,    29,     1,   115,    84,   116,    87,   117,   118,   166,
     119,   120,   121,    12,    13,   165,   122,    88,    51,    28,
      20,   156,    26,    82,   123,    52,    53,    54,   129,   130,
       1,    95,    96,    97,     2,     3,     4,    68,   130,   167,
     168,    80,    48,    49,    50,    81,    65,    19,    31,    32,
     130,     1,   115,    28,   116,    29,   117,   118,    82,   119,
     120,   121,    34,    32,   177,   122,   178,    51,    84,   157,
     106,    94,    82,   159,    52,    53,    54,    38,    39,    40,
      39,   129,    48,    49,    50,    33,   107,    28,   113,    29,
     129,   114,   115,    66,   116,   170,   170,    98,    99,   119,
     120,   121,   129,    41,    42,   122,   138,    51,    48,    49,
      50,   146,    82,   153,    52,    53,    54,   154,    48,    49,
      50,   150,   191,   192,   193,   194,   170,   170,   170,   170,
      48,    49,    50,    51,    48,    49,    50,   151,    63,   100,
      52,    53,    54,    51,    48,    49,    50,   152,    74,   108,
      52,    53,    54,   184,   185,    51,    48,    49,    50,    51,
      63,   158,    52,    53,    54,   161,    52,    53,    54,    51,
      48,    49,    50,   163,    74,   162,    52,    53,    54,   142,
     143,    51,   134,   195,   196,   164,    28,   176,    52,    53,
      54,   179,   186,   155,   187,    51,   189,   188,   200,    21,
      25,    79,    52,    53,    54,    69,   131,   175,   160,   197,
       0,     0,   198
};

static const yytype_int16 yycheck[] =
{
      28,    29,    82,     4,    63,    74,    48,    43,    67,    78,
      46,    35,    52,    53,    54,    82,   179,    28,    18,    19,
      18,    19,     4,    51,    35,   188,     7,     8,    18,    19,
      30,    28,    30,    30,    70,    63,    28,   200,    66,    67,
      30,    27,    31,    35,    33,     4,   126,    33,    31,    85,
      33,    87,     4,    77,    82,    95,    96,    97,     4,   126,
      88,    42,    43,     4,     5,     6,    31,    29,    33,    31,
     106,    33,    13,    14,    28,    16,    30,    18,    19,   148,
      21,    22,    23,    18,    19,   144,    27,    29,    29,    31,
       0,   119,    29,    34,    35,    36,    37,    38,   126,   179,
      13,    39,    40,    41,    17,    18,    19,     4,   188,   151,
     152,     4,     4,     5,     6,     4,   144,   118,    27,    28,
     200,    13,    14,    31,    16,    33,    18,    19,    34,    21,
      22,    23,    27,    28,   162,    27,   164,    29,    28,    27,
      30,    32,    34,    35,    36,    37,    38,    27,    28,    27,
      28,   179,     4,     5,     6,    29,    32,    31,    31,    33,
     188,    31,    14,    31,    16,   153,   154,    36,    37,    21,
      22,    23,   200,    18,    19,    27,    30,    29,     4,     5,
       6,    32,    34,    29,    36,    37,    38,    29,     4,     5,
       6,    32,   180,   181,   182,   183,   184,   185,   186,   187,
       4,     5,     6,    29,     4,     5,     6,    32,    34,    35,
      36,    37,    38,    29,     4,     5,     6,    32,    34,    35,
      36,    37,    38,     9,    10,    29,     4,     5,     6,    29,
      34,    27,    36,    37,    38,    27,    36,    37,    38,    29,
       4,     5,     6,    30,    34,    33,    36,    37,    38,    98,
      99,    29,    30,   184,   185,    28,    31,    27,    36,    37,
      38,    30,    11,    27,    12,    29,    27,    30,    15,     6,
      13,    39,    36,    37,    38,    32,    84,   154,   126,   186,
      -1,    -1,   187
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    13,    17,    18,    19,    45,    46,    47,    48,    49,
      50,    53,    18,    19,     4,     4,    51,    56,     4,    51,
       0,    47,     4,    52,    57,    52,    29,    29,    31,    33,
      54,    27,    28,    29,    27,    31,    33,    55,    27,    28,
      27,    18,    19,    30,    62,    63,    30,    62,     4,     5,
       6,    29,    36,    37,    38,    68,    70,    71,    72,    73,
      74,    77,    78,    34,    59,    68,    31,    33,     4,    56,
      30,    62,    68,    83,    34,    58,    83,    31,    33,    57,
       4,     4,    34,    64,    28,    30,    64,    30,    29,    54,
      68,    74,    74,    74,    32,    39,    40,    41,    36,    37,
      35,    59,    61,    68,    59,    64,    30,    32,    35,    58,
      60,    83,    58,    31,    31,    14,    16,    18,    19,    21,
      22,    23,    27,    35,    48,    64,    65,    66,    67,    68,
      70,    63,    64,    64,    30,    68,    75,    76,    30,    74,
      74,    74,    77,    77,    28,    35,    32,    64,    28,    35,
      32,    32,    32,    29,    29,    27,    68,    27,    27,    35,
      66,    27,    33,    30,    28,    59,    58,    54,    54,    69,
      78,    79,    80,    81,    82,    69,    27,    68,    68,    30,
       7,     8,    42,    43,     9,    10,    11,    12,    30,    27,
      67,    78,    78,    78,    78,    79,    79,    80,    81,    67,
      15,    67
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    44,    45,    46,    46,    47,    47,    48,    48,    49,
      49,    50,    50,    51,    51,    52,    52,    53,    53,    53,
      53,    53,    53,    54,    54,    55,    55,    56,    56,    56,
      56,    57,    57,    58,    58,    58,    59,    59,    59,    60,
      60,    61,    61,    62,    62,    63,    63,    63,    63,    63,
      63,    64,    64,    65,    65,    66,    66,    67,    67,    67,
      67,    67,    67,    67,    67,    67,    67,    67,    68,    69,
      70,    70,    71,    71,    71,    71,    72,    73,    74,    74,
      74,    74,    74,    74,    75,    76,    76,    77,    77,    77,
      77,    78,    78,    78,    79,    79,    79,    79,    79,    80,
      80,    80,    81,    81,    82,    82,    83
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     1,     2,     1,     1,     1,     1,     3,
       3,     4,     4,     1,     3,     1,     3,     6,     5,     6,
       6,     5,     5,     3,     4,     3,     4,     3,     1,     2,
       4,     3,     4,     1,     2,     3,     1,     2,     3,     1,
       3,     1,     3,     1,     3,     2,     2,     5,     5,     4,
       4,     3,     2,     1,     2,     1,     1,     4,     2,     1,
       1,     5,     7,     5,     2,     2,     3,     2,     1,     1,
       1,     2,     3,     1,     1,     1,     1,     1,     1,     4,
       3,     2,     2,     2,     1,     1,     3,     1,     3,     3,
       3,     1,     3,     3,     1,     3,     3,     3,     3,     1,
       3,     3,     1,     3,     1,     3,     1
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF

/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)                                \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;        \
          (Current).first_column = YYRHSLOC (Rhs, 1).first_column;      \
          (Current).last_line    = YYRHSLOC (Rhs, N).last_line;         \
          (Current).last_column  = YYRHSLOC (Rhs, N).last_column;       \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).first_line   = (Current).last_line   =              \
            YYRHSLOC (Rhs, 0).last_line;                                \
          (Current).first_column = (Current).last_column =              \
            YYRHSLOC (Rhs, 0).last_column;                              \
        }                                                               \
    while (0)
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K])


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)


/* YYLOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

# ifndef YYLOCATION_PRINT

#  if defined YY_LOCATION_PRINT

   /* Temporary convenience wrapper in case some people defined the
      undocumented and private YY_LOCATION_PRINT macros.  */
#   define YYLOCATION_PRINT(File, Loc)  YY_LOCATION_PRINT(File, *(Loc))

#  elif defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL

/* Print *YYLOCP on YYO.  Private, do not rely on its existence. */

YY_ATTRIBUTE_UNUSED
static int
yy_location_print_ (FILE *yyo, YYLTYPE const * const yylocp)
{
  int res = 0;
  int end_col = 0 != yylocp->last_column ? yylocp->last_column - 1 : 0;
  if (0 <= yylocp->first_line)
    {
      res += YYFPRINTF (yyo, "%d", yylocp->first_line);
      if (0 <= yylocp->first_column)
        res += YYFPRINTF (yyo, ".%d", yylocp->first_column);
    }
  if (0 <= yylocp->last_line)
    {
      if (yylocp->first_line < yylocp->last_line)
        {
          res += YYFPRINTF (yyo, "-%d", yylocp->last_line);
          if (0 <= end_col)
            res += YYFPRINTF (yyo, ".%d", end_col);
        }
      else if (0 <= end_col && yylocp->first_column < end_col)
        res += YYFPRINTF (yyo, "-%d", end_col);
    }
  return res;
}

#   define YYLOCATION_PRINT  yy_location_print_

    /* Temporary convenience wrapper in case some people defined the
       undocumented and private YY_LOCATION_PRINT macros.  */
#   define YY_LOCATION_PRINT(File, Loc)  YYLOCATION_PRINT(File, &(Loc))

#  else

#   define YYLOCATION_PRINT(File, Loc) ((void) 0)
    /* Temporary convenience wrapper in case some people defined the
       undocumented and private YY_LOCATION_PRINT macros.  */
#   define YY_LOCATION_PRINT  YYLOCATION_PRINT

#  endif
# endif /* !defined YYLOCATION_PRINT */


# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value, Location); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  YY_USE (yylocationp);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  YYLOCATION_PRINT (yyo, yylocationp);
  YYFPRINTF (yyo, ": ");
  yy_symbol_value_print (yyo, yykind, yyvaluep, yylocationp);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp, YYLTYPE *yylsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)],
                       &(yylsp[(yyi + 1) - (yynrhs)]));
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, yylsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep, YYLTYPE *yylocationp)
{
  YY_USE (yyvaluep);
  YY_USE (yylocationp);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Location data for the lookahead symbol.  */
YYLTYPE yylloc
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
  = { 1, 1, 1, 1 }
# endif
;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

    /* The location stack: array, bottom, top.  */
    YYLTYPE yylsa[YYINITDEPTH];
    YYLTYPE *yyls = yylsa;
    YYLTYPE *yylsp = yyls;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
  YYLTYPE yyloc;

  /* The locations where the error started and ended.  */
  YYLTYPE yyerror_range[3];



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N), yylsp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  yylsp[0] = yylloc;
  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;
        YYLTYPE *yyls1 = yyls;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yyls1, yysize * YYSIZEOF (*yylsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
        yyls = yyls1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
        YYSTACK_RELOCATE (yyls_alloc, yyls);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
      yylsp = yyls + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      yyerror_range[1] = yylloc;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END
  *++yylsp = yylloc;

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];

  /* Default location. */
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
  yyerror_range[1] = yyloc;
  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* Program: Comp_list  */
#line 69 "parser/SysY_parser.y"
{
    (yyloc) = (yylsp[0]);
    ast_root = new __Program((yyvsp[0].comps));
    ast_root->SetLineNumber(line_number);
}
#line 1428 "SysY_parser.tab.c"
    break;

  case 3: /* Comp_list: CompUnit  */
#line 77 "parser/SysY_parser.y"
{
    (yyval.comps) = new std::vector<CompUnit>;
    ((yyval.comps))->push_back((yyvsp[0].comp_unit));
}
#line 1437 "SysY_parser.tab.c"
    break;

  case 4: /* Comp_list: Comp_list CompUnit  */
#line 82 "parser/SysY_parser.y"
{
    ((yyvsp[-1].comps))->push_back((yyvsp[0].comp_unit));
    (yyval.comps) = (yyvsp[-1].comps);
}
#line 1446 "SysY_parser.tab.c"
    break;

  case 5: /* CompUnit: Decl  */
#line 88 "parser/SysY_parser.y"
     {
    (yyval.comp_unit) = new CompUnit_Decl((yyvsp[0].decl)); 
    (yyval.comp_unit)->SetLineNumber(line_number);
}
#line 1455 "SysY_parser.tab.c"
    break;

  case 6: /* CompUnit: FuncDef  */
#line 92 "parser/SysY_parser.y"
        {
    (yyval.comp_unit) = new CompUnit_FuncDef((yyvsp[0].func_def)); 
    (yyval.comp_unit)->SetLineNumber(line_number);
}
#line 1464 "SysY_parser.tab.c"
    break;

  case 7: /* Decl: VarDecl  */
#line 99 "parser/SysY_parser.y"
        {
    (yyval.decl) = (yyvsp[0].decl); 
    (yyval.decl)->SetLineNumber(line_number);
}
#line 1473 "SysY_parser.tab.c"
    break;

  case 8: /* Decl: ConstDecl  */
#line 103 "parser/SysY_parser.y"
          {
    (yyval.decl) = (yyvsp[0].decl); 
    (yyval.decl)->SetLineNumber(line_number);
}
#line 1482 "SysY_parser.tab.c"
    break;

  case 9: /* VarDecl: INT VarDef_list ';'  */
#line 110 "parser/SysY_parser.y"
                    {
    (yyval.decl) = new VarDecl(Type::INT,(yyvsp[-1].defs)); 
    (yyval.decl)->SetLineNumber(line_number);
}
#line 1491 "SysY_parser.tab.c"
    break;

  case 10: /* VarDecl: FLOAT VarDef_list ';'  */
#line 115 "parser/SysY_parser.y"
                      {
	(yyval.decl)=new VarDecl(Type::FLOAT,(yyvsp[-1].defs));
	(yyval.decl)->SetLineNumber(line_number);
}
#line 1500 "SysY_parser.tab.c"
    break;

  case 11: /* ConstDecl: CONST INT ConstDef_list ';'  */
#line 124 "parser/SysY_parser.y"
                            {
    (yyval.decl)=new ConstDecl(Type::INT,(yyvsp[-1].defs)); 
    (yyval.decl)->SetLineNumber(line_number);
}
#line 1509 "SysY_parser.tab.c"
    break;

  case 12: /* ConstDecl: CONST FLOAT ConstDef_list ';'  */
#line 129 "parser/SysY_parser.y"
                              {
    (yyval.decl)=new ConstDecl(Type::FLOAT,(yyvsp[-1].defs)); 
    (yyval.decl)->SetLineNumber(line_number);
}
#line 1518 "SysY_parser.tab.c"
    break;

  case 13: /* VarDef_list: VarDef  */
#line 139 "parser/SysY_parser.y"
       {
	(yyval.defs)=new std::vector<Def>{(yyvsp[0].def)}; 
    //$$->SetLineNumber(line_number);
}
#line 1527 "SysY_parser.tab.c"
    break;

  case 14: /* VarDef_list: VarDef_list ',' VarDef  */
#line 143 "parser/SysY_parser.y"
                       {
	(yyval.defs)=(yyvsp[-2].defs);
	(yyval.defs)->push_back((yyvsp[0].def));    
    //$$->SetLineNumber(line_number);
}
#line 1537 "SysY_parser.tab.c"
    break;

  case 15: /* ConstDef_list: ConstDef  */
#line 152 "parser/SysY_parser.y"
         {
	(yyval.defs)=new std::vector<Def>{(yyvsp[0].def)};
	//$$->SetLineNumber(line_number);
}
#line 1546 "SysY_parser.tab.c"
    break;

  case 16: /* ConstDef_list: ConstDef_list ',' ConstDef  */
#line 156 "parser/SysY_parser.y"
                           {
	(yyval.defs)=(yyvsp[-2].defs);
	(yyval.defs)->push_back((yyvsp[0].def)); 
    //$$->SetLineNumber(line_number);
}
#line 1556 "SysY_parser.tab.c"
    break;

  case 17: /* FuncDef: INT IDENT '(' FuncFParams ')' Block  */
#line 165 "parser/SysY_parser.y"
{
    (yyval.func_def) = new __FuncDef(Type::INT,(yyvsp[-4].symbol_token),(yyvsp[-2].formals),(yyvsp[0].block));
    (yyval.func_def)->SetLineNumber(line_number);
}
#line 1565 "SysY_parser.tab.c"
    break;

  case 18: /* FuncDef: INT IDENT '(' ')' Block  */
#line 170 "parser/SysY_parser.y"
{
    (yyval.func_def) = new __FuncDef(Type::INT,(yyvsp[-3].symbol_token),new std::vector<FuncFParam>(),(yyvsp[0].block)); 
    (yyval.func_def)->SetLineNumber(line_number);
}
#line 1574 "SysY_parser.tab.c"
    break;

  case 19: /* FuncDef: FLOAT IDENT '(' FuncFParams ')' Block  */
#line 176 "parser/SysY_parser.y"
{
	(yyval.func_def)=new __FuncDef(Type::FLOAT,(yyvsp[-4].symbol_token),(yyvsp[-2].formals),(yyvsp[0].block));
    (yyval.func_def)->SetLineNumber(line_number);
}
#line 1583 "SysY_parser.tab.c"
    break;

  case 20: /* FuncDef: NONE_TYPE IDENT '(' FuncFParams ')' Block  */
#line 181 "parser/SysY_parser.y"
{
	(yyval.func_def)=new __FuncDef(Type::VOID,(yyvsp[-4].symbol_token),(yyvsp[-2].formals),(yyvsp[0].block));
    (yyval.func_def)->SetLineNumber(line_number);
}
#line 1592 "SysY_parser.tab.c"
    break;

  case 21: /* FuncDef: FLOAT IDENT '(' ')' Block  */
#line 186 "parser/SysY_parser.y"
{
    (yyval.func_def) = new __FuncDef(Type::FLOAT,(yyvsp[-3].symbol_token),new std::vector<FuncFParam>(),(yyvsp[0].block)); 
    (yyval.func_def)->SetLineNumber(line_number);
}
#line 1601 "SysY_parser.tab.c"
    break;

  case 22: /* FuncDef: NONE_TYPE IDENT '(' ')' Block  */
#line 191 "parser/SysY_parser.y"
{
    (yyval.func_def) = new __FuncDef(Type::VOID,(yyvsp[-3].symbol_token),new std::vector<FuncFParam>(),(yyvsp[0].block)); 
    (yyval.func_def)->SetLineNumber(line_number);
}
#line 1610 "SysY_parser.tab.c"
    break;

  case 23: /* Array_list: '[' Exp ']'  */
#line 201 "parser/SysY_parser.y"
{
	(yyval.expressions)=new std::vector<Expression> {(yyvsp[-1].expression)};
	//$$->SetLineNumber(line_number);
}
#line 1619 "SysY_parser.tab.c"
    break;

  case 24: /* Array_list: Array_list '[' Exp ']'  */
#line 206 "parser/SysY_parser.y"
{
	(yyval.expressions)=(yyvsp[-3].expressions);
	(yyval.expressions)->push_back((yyvsp[-1].expression));
	//$$->SetLineNumber(line_number);
}
#line 1629 "SysY_parser.tab.c"
    break;

  case 25: /* Constarray_list: '[' ConstExp ']'  */
#line 213 "parser/SysY_parser.y"
{
	(yyval.expressions)=new std::vector<Expression> {(yyvsp[-1].expression)};
	//$$->SetLineNumber(line_number);
}
#line 1638 "SysY_parser.tab.c"
    break;

  case 26: /* Constarray_list: Constarray_list '[' ConstExp ']'  */
#line 218 "parser/SysY_parser.y"
{
	(yyval.expressions)=(yyvsp[-3].expressions);
	(yyval.expressions)->push_back((yyvsp[-1].expression));
	//$$->SetLineNumber(line_number);
}
#line 1648 "SysY_parser.tab.c"
    break;

  case 27: /* VarDef: IDENT '=' VarInitVal  */
#line 226 "parser/SysY_parser.y"
{(yyval.def) = new VarDef((yyvsp[-2].symbol_token),nullptr,(yyvsp[0].initval)); (yyval.def)->SetLineNumber(line_number);}
#line 1654 "SysY_parser.tab.c"
    break;

  case 28: /* VarDef: IDENT  */
#line 228 "parser/SysY_parser.y"
{(yyval.def) = new VarDef_no_init((yyvsp[0].symbol_token),nullptr); (yyval.def)->SetLineNumber(line_number);}
#line 1660 "SysY_parser.tab.c"
    break;

  case 29: /* VarDef: IDENT Array_list  */
#line 231 "parser/SysY_parser.y"
{
	(yyval.def)=new VarDef_no_init((yyvsp[-1].symbol_token),(yyvsp[0].expressions));
	(yyval.def)->SetLineNumber(line_number);
}
#line 1669 "SysY_parser.tab.c"
    break;

  case 30: /* VarDef: IDENT Array_list '=' VarInitVal  */
#line 236 "parser/SysY_parser.y"
{
	(yyval.def)=new VarDef((yyvsp[-3].symbol_token),(yyvsp[-2].expressions),(yyvsp[0].initval));
	(yyval.def)->SetLineNumber(line_number);
}
#line 1678 "SysY_parser.tab.c"
    break;

  case 31: /* ConstDef: IDENT '=' ConstInitVal  */
#line 246 "parser/SysY_parser.y"
{
	(yyval.def) = new ConstDef((yyvsp[-2].symbol_token),nullptr,(yyvsp[0].initval));
	(yyval.def)->SetLineNumber(line_number);
}
#line 1687 "SysY_parser.tab.c"
    break;

  case 32: /* ConstDef: IDENT Constarray_list '=' ConstInitVal  */
#line 251 "parser/SysY_parser.y"
{
	(yyval.def) = new ConstDef((yyvsp[-3].symbol_token),(yyvsp[-2].expressions),(yyvsp[0].initval));
	(yyval.def)->SetLineNumber(line_number);
}
#line 1696 "SysY_parser.tab.c"
    break;

  case 33: /* ConstInitVal: ConstExp  */
#line 260 "parser/SysY_parser.y"
{
	(yyval.initval)=new ConstInitVal_exp((yyvsp[0].expression));
	(yyval.initval)->SetLineNumber(line_number);
}
#line 1705 "SysY_parser.tab.c"
    break;

  case 34: /* ConstInitVal: '{' '}'  */
#line 265 "parser/SysY_parser.y"
{
	(yyval.initval)=new ConstInitVal(new std::vector<InitVal>());
	(yyval.initval)->SetLineNumber(line_number);
}
#line 1714 "SysY_parser.tab.c"
    break;

  case 35: /* ConstInitVal: '{' ConstInitVal_list '}'  */
#line 270 "parser/SysY_parser.y"
{
	(yyval.initval)=new ConstInitVal((yyvsp[-1].initvals));
	(yyval.initval)->SetLineNumber(line_number);
}
#line 1723 "SysY_parser.tab.c"
    break;

  case 36: /* VarInitVal: Exp  */
#line 279 "parser/SysY_parser.y"
{
	(yyval.initval)=new VarInitVal_exp((yyvsp[0].expression));
	(yyval.initval)->SetLineNumber(line_number);
}
#line 1732 "SysY_parser.tab.c"
    break;

  case 37: /* VarInitVal: '{' '}'  */
#line 284 "parser/SysY_parser.y"
{
	(yyval.initval)=new VarInitVal(new std::vector<InitVal>());
	(yyval.initval)->SetLineNumber(line_number);
}
#line 1741 "SysY_parser.tab.c"
    break;

  case 38: /* VarInitVal: '{' VarInitVal_list '}'  */
#line 289 "parser/SysY_parser.y"
{
	(yyval.initval)=new VarInitVal((yyvsp[-1].initvals));
	(yyval.initval)->SetLineNumber(line_number);
}
#line 1750 "SysY_parser.tab.c"
    break;

  case 39: /* ConstInitVal_list: ConstInitVal  */
#line 297 "parser/SysY_parser.y"
             {
	(yyval.initvals)=new std::vector<InitVal> {(yyvsp[0].initval)};
	//$$->SetLineNumber(line_number);
}
#line 1759 "SysY_parser.tab.c"
    break;

  case 40: /* ConstInitVal_list: ConstInitVal_list ',' ConstInitVal  */
#line 301 "parser/SysY_parser.y"
                                   {
	(yyval.initvals)=(yyvsp[-2].initvals);
	(yyval.initvals)->push_back((yyvsp[0].initval));
	//$$->SetLineNumber(line_number);
}
#line 1769 "SysY_parser.tab.c"
    break;

  case 41: /* VarInitVal_list: VarInitVal  */
#line 310 "parser/SysY_parser.y"
           {
	(yyval.initvals)=new std::vector<InitVal> {(yyvsp[0].initval)};
	//$$->SetLineNumber(line_number);
}
#line 1778 "SysY_parser.tab.c"
    break;

  case 42: /* VarInitVal_list: VarInitVal_list ',' VarInitVal  */
#line 314 "parser/SysY_parser.y"
                               {
	(yyval.initvals)=(yyvsp[-2].initvals);
	(yyval.initvals)->push_back((yyvsp[0].initval));
	//$$->SetLineNumber(line_number);
}
#line 1788 "SysY_parser.tab.c"
    break;

  case 43: /* FuncFParams: FuncFParam  */
#line 322 "parser/SysY_parser.y"
           {
    (yyval.formals) = new std::vector<FuncFParam>;
    ((yyval.formals))->push_back((yyvsp[0].formal));
}
#line 1797 "SysY_parser.tab.c"
    break;

  case 44: /* FuncFParams: FuncFParams ',' FuncFParam  */
#line 326 "parser/SysY_parser.y"
                           {
    ((yyvsp[-2].formals))->push_back((yyvsp[0].formal));
    (yyval.formals) = (yyvsp[-2].formals);
}
#line 1806 "SysY_parser.tab.c"
    break;

  case 45: /* FuncFParam: INT IDENT  */
#line 333 "parser/SysY_parser.y"
          {
    (yyval.formal) = new __FuncFParam(Type::INT,(yyvsp[0].symbol_token),nullptr);
    (yyval.formal)->SetLineNumber(line_number);
}
#line 1815 "SysY_parser.tab.c"
    break;

  case 46: /* FuncFParam: FLOAT IDENT  */
#line 338 "parser/SysY_parser.y"
            {
	(yyval.formal) = new __FuncFParam(Type::FLOAT,(yyvsp[0].symbol_token),nullptr);
    (yyval.formal)->SetLineNumber(line_number);
}
#line 1824 "SysY_parser.tab.c"
    break;

  case 47: /* FuncFParam: INT IDENT '[' ']' Array_list  */
#line 342 "parser/SysY_parser.y"
                             {
	std::vector<Expression>* temp = new std::vector<Expression>{nullptr};
	temp->insert(temp->end(),(yyvsp[0].expressions)->begin(),(yyvsp[0].expressions)->end());
	(yyval.formal) = new __FuncFParam(Type::INT,(yyvsp[-3].symbol_token),temp);
    (yyval.formal)->SetLineNumber(line_number);
}
#line 1835 "SysY_parser.tab.c"
    break;

  case 48: /* FuncFParam: FLOAT IDENT '[' ']' Array_list  */
#line 348 "parser/SysY_parser.y"
                               {
	std::vector<Expression>* temp = new std::vector<Expression>{nullptr};
	temp->insert(temp->end(),(yyvsp[0].expressions)->begin(),(yyvsp[0].expressions)->end());
	(yyval.formal) = new __FuncFParam(Type::FLOAT,(yyvsp[-3].symbol_token),temp);
    (yyval.formal)->SetLineNumber(line_number);
}
#line 1846 "SysY_parser.tab.c"
    break;

  case 49: /* FuncFParam: INT IDENT '[' ']'  */
#line 354 "parser/SysY_parser.y"
                  {
	std::vector<Expression>* temp = new std::vector<Expression>{nullptr};
    (yyval.formal) = new __FuncFParam(Type::INT,(yyvsp[-2].symbol_token),temp);
    (yyval.formal)->SetLineNumber(line_number);
}
#line 1856 "SysY_parser.tab.c"
    break;

  case 50: /* FuncFParam: FLOAT IDENT '[' ']'  */
#line 359 "parser/SysY_parser.y"
                    {
	std::vector<Expression>* temp = new std::vector<Expression>{nullptr};
    (yyval.formal) = new __FuncFParam(Type::FLOAT,(yyvsp[-2].symbol_token),temp);
    (yyval.formal)->SetLineNumber(line_number);
}
#line 1866 "SysY_parser.tab.c"
    break;

  case 51: /* Block: '{' BlockItem_list '}'  */
#line 368 "parser/SysY_parser.y"
                       {
    (yyval.block) = new __Block((yyvsp[-1].block_items));
    (yyval.block)->SetLineNumber(line_number);
}
#line 1875 "SysY_parser.tab.c"
    break;

  case 52: /* Block: '{' '}'  */
#line 372 "parser/SysY_parser.y"
        {
    (yyval.block) = new __Block(new std::vector<BlockItem>);
    (yyval.block)->SetLineNumber(line_number);
}
#line 1884 "SysY_parser.tab.c"
    break;

  case 53: /* BlockItem_list: BlockItem  */
#line 380 "parser/SysY_parser.y"
          {
	(yyval.block_items)=new std::vector<BlockItem>{(yyvsp[0].block_item)};
	//$$->SetLineNumber(line_number);
}
#line 1893 "SysY_parser.tab.c"
    break;

  case 54: /* BlockItem_list: BlockItem_list BlockItem  */
#line 384 "parser/SysY_parser.y"
                         {
	(yyval.block_items)=(yyvsp[-1].block_items);
	(yyval.block_items)->push_back((yyvsp[0].block_item));
	//$$->SetLineNumber(line_number);
}
#line 1903 "SysY_parser.tab.c"
    break;

  case 55: /* BlockItem: Decl  */
#line 393 "parser/SysY_parser.y"
     {
	(yyval.block_item) = new BlockItem_Decl((yyvsp[0].decl));
    (yyval.block_item)->SetLineNumber(line_number);
}
#line 1912 "SysY_parser.tab.c"
    break;

  case 56: /* BlockItem: Stmt  */
#line 397 "parser/SysY_parser.y"
     {
	(yyval.block_item) = new BlockItem_Stmt((yyvsp[0].stmt));
    (yyval.block_item)->SetLineNumber(line_number);
}
#line 1921 "SysY_parser.tab.c"
    break;

  case 57: /* Stmt: Lval '=' Exp ';'  */
#line 405 "parser/SysY_parser.y"
                 {
    (yyval.stmt) = new assign_stmt((yyvsp[-3].expression),(yyvsp[-1].expression));
    (yyval.stmt)->SetLineNumber(line_number);
}
#line 1930 "SysY_parser.tab.c"
    break;

  case 58: /* Stmt: Exp ';'  */
#line 409 "parser/SysY_parser.y"
         {
    (yyval.stmt) = new expr_stmt((yyvsp[-1].expression));
    (yyval.stmt)->SetLineNumber(line_number);
}
#line 1939 "SysY_parser.tab.c"
    break;

  case 59: /* Stmt: ';'  */
#line 413 "parser/SysY_parser.y"
     {
    (yyval.stmt) = new null_stmt();
    (yyval.stmt)->SetLineNumber(line_number);
}
#line 1948 "SysY_parser.tab.c"
    break;

  case 60: /* Stmt: Block  */
#line 417 "parser/SysY_parser.y"
       {
    (yyval.stmt) = new block_stmt((yyvsp[0].block));
    (yyval.stmt)->SetLineNumber(line_number);
}
#line 1957 "SysY_parser.tab.c"
    break;

  case 61: /* Stmt: IF '(' Cond ')' Stmt  */
#line 421 "parser/SysY_parser.y"
                                 {
    (yyval.stmt) = new if_stmt((yyvsp[-2].expression), (yyvsp[0].stmt));
    (yyval.stmt)->SetLineNumber(line_number);
}
#line 1966 "SysY_parser.tab.c"
    break;

  case 62: /* Stmt: IF '(' Cond ')' Stmt ELSE Stmt  */
#line 425 "parser/SysY_parser.y"
                                {
    (yyval.stmt) = new ifelse_stmt((yyvsp[-4].expression), (yyvsp[-2].stmt), (yyvsp[0].stmt));
    (yyval.stmt)->SetLineNumber(line_number);
}
#line 1975 "SysY_parser.tab.c"
    break;

  case 63: /* Stmt: WHILE '(' Cond ')' Stmt  */
#line 429 "parser/SysY_parser.y"
                         {
    (yyval.stmt) = new while_stmt((yyvsp[-2].expression), (yyvsp[0].stmt));
    (yyval.stmt)->SetLineNumber(line_number);
}
#line 1984 "SysY_parser.tab.c"
    break;

  case 64: /* Stmt: BREAK ';'  */
#line 433 "parser/SysY_parser.y"
           {
    (yyval.stmt) = new break_stmt();
    (yyval.stmt)->SetLineNumber(line_number);
}
#line 1993 "SysY_parser.tab.c"
    break;

  case 65: /* Stmt: CONTINUE ';'  */
#line 437 "parser/SysY_parser.y"
              {
    (yyval.stmt) = new continue_stmt();
    (yyval.stmt)->SetLineNumber(line_number);
}
#line 2002 "SysY_parser.tab.c"
    break;

  case 66: /* Stmt: RETURN Exp ';'  */
#line 441 "parser/SysY_parser.y"
                {
    (yyval.stmt) = new return_stmt((yyvsp[-1].expression));
    (yyval.stmt)->SetLineNumber(line_number);
}
#line 2011 "SysY_parser.tab.c"
    break;

  case 67: /* Stmt: RETURN ';'  */
#line 445 "parser/SysY_parser.y"
            {
    (yyval.stmt) = new return_stmt_void();
    (yyval.stmt)->SetLineNumber(line_number);
}
#line 2020 "SysY_parser.tab.c"
    break;

  case 68: /* Exp: AddExp  */
#line 478 "parser/SysY_parser.y"
       {(yyval.expression) = (yyvsp[0].expression); (yyval.expression)->SetLineNumber(line_number);}
#line 2026 "SysY_parser.tab.c"
    break;

  case 69: /* Cond: LOrExp  */
#line 482 "parser/SysY_parser.y"
       {(yyval.expression) = (yyvsp[0].expression); (yyval.expression)->SetLineNumber(line_number);}
#line 2032 "SysY_parser.tab.c"
    break;

  case 70: /* Lval: IDENT  */
#line 487 "parser/SysY_parser.y"
       {
    (yyval.expression) = new Lval((yyvsp[0].symbol_token), nullptr);
    (yyval.expression)->SetLineNumber(line_number);
}
#line 2041 "SysY_parser.tab.c"
    break;

  case 71: /* Lval: IDENT Array_list  */
#line 491 "parser/SysY_parser.y"
                  {
    (yyval.expression) = new Lval((yyvsp[-1].symbol_token), (yyvsp[0].expressions));
    (yyval.expression)->SetLineNumber(line_number);
}
#line 2050 "SysY_parser.tab.c"
    break;

  case 72: /* PrimaryExp: '(' Exp ')'  */
#line 499 "parser/SysY_parser.y"
              {
    (yyval.expression) = (yyvsp[-1].expression);
    (yyval.expression)->SetLineNumber(line_number);
}
#line 2059 "SysY_parser.tab.c"
    break;

  case 73: /* PrimaryExp: Lval  */
#line 503 "parser/SysY_parser.y"
       {
    (yyval.expression)=(yyvsp[0].expression);
    (yyval.expression)->SetLineNumber(line_number);
}
#line 2068 "SysY_parser.tab.c"
    break;

  case 74: /* PrimaryExp: IntConst  */
#line 507 "parser/SysY_parser.y"
           {
    (yyval.expression)=(yyvsp[0].expression);
    (yyval.expression)->SetLineNumber(line_number);
}
#line 2077 "SysY_parser.tab.c"
    break;

  case 75: /* PrimaryExp: FloatConst  */
#line 511 "parser/SysY_parser.y"
             {
    (yyval.expression)=(yyvsp[0].expression);
    (yyval.expression)->SetLineNumber(line_number);
}
#line 2086 "SysY_parser.tab.c"
    break;

  case 76: /* IntConst: INT_CONST  */
#line 519 "parser/SysY_parser.y"
          {
    (yyval.expression) = new IntConst((yyvsp[0].int_token));
    (yyval.expression)->SetLineNumber(line_number);
}
#line 2095 "SysY_parser.tab.c"
    break;

  case 77: /* FloatConst: FLOAT_CONST  */
#line 526 "parser/SysY_parser.y"
            {
    (yyval.expression) = new FloatConst((yyvsp[0].float_token));
    (yyval.expression)->SetLineNumber(line_number);
}
#line 2104 "SysY_parser.tab.c"
    break;

  case 78: /* UnaryExp: PrimaryExp  */
#line 533 "parser/SysY_parser.y"
           {(yyval.expression) = (yyvsp[0].expression);}
#line 2110 "SysY_parser.tab.c"
    break;

  case 79: /* UnaryExp: IDENT '(' FuncRParams ')'  */
#line 534 "parser/SysY_parser.y"
                          {
    (yyval.expression) = new Func_call((yyvsp[-3].symbol_token),(yyvsp[-1].expression));
    (yyval.expression)->SetLineNumber(line_number);
}
#line 2119 "SysY_parser.tab.c"
    break;

  case 80: /* UnaryExp: IDENT '(' ')'  */
#line 538 "parser/SysY_parser.y"
              {
    // 在sylib.h这个文件中,starttime()是一个宏定义
    // #define starttime() _sysy_starttime(__LINE__)
    // 我们在语法分析中将其替换为_sysy_starttime(line_number)
    // stoptime同理
    if((yyvsp[-2].symbol_token)->get_string() == "starttime"){
        auto params = new std::vector<Expression>;
        params->push_back(new IntConst(line_number));
        Expression temp = new FuncRParams(params);
        (yyval.expression) = new Func_call(id_table.add_id("_sysy_starttime"),temp);
        (yyval.expression)->SetLineNumber(line_number);
    }
    else if((yyvsp[-2].symbol_token)->get_string() == "stoptime"){
        auto params = new std::vector<Expression>;
        params->push_back(new IntConst(line_number));
        Expression temp = new FuncRParams(params);
        (yyval.expression) = new Func_call(id_table.add_id("_sysy_stoptime"),temp);
        (yyval.expression)->SetLineNumber(line_number);
    }
    else{
        (yyval.expression) = new Func_call((yyvsp[-2].symbol_token),nullptr);
        (yyval.expression)->SetLineNumber(line_number);
    }
}
#line 2148 "SysY_parser.tab.c"
    break;

  case 81: /* UnaryExp: '+' UnaryExp  */
#line 562 "parser/SysY_parser.y"
             {
    (yyval.expression) = new UnaryExp_plus((yyvsp[0].expression));
    (yyval.expression)->SetLineNumber(line_number);
}
#line 2157 "SysY_parser.tab.c"
    break;

  case 82: /* UnaryExp: '-' UnaryExp  */
#line 566 "parser/SysY_parser.y"
             {
    (yyval.expression) = new UnaryExp_neg((yyvsp[0].expression));
    (yyval.expression)->SetLineNumber(line_number);
}
#line 2166 "SysY_parser.tab.c"
    break;

  case 83: /* UnaryExp: '!' UnaryExp  */
#line 570 "parser/SysY_parser.y"
             {
    (yyval.expression) = new UnaryExp_not((yyvsp[0].expression));
    (yyval.expression)->SetLineNumber(line_number);
}
#line 2175 "SysY_parser.tab.c"
    break;

  case 84: /* FuncRParams: Exp_list  */
#line 578 "parser/SysY_parser.y"
           {
    (yyval.expression) = new FuncRParams((yyvsp[0].expressions));
    (yyval.expression)->SetLineNumber(line_number);
}
#line 2184 "SysY_parser.tab.c"
    break;

  case 85: /* Exp_list: Exp  */
#line 587 "parser/SysY_parser.y"
      {
    (yyval.expressions) = new std::vector<Expression>{(yyvsp[0].expression)};
    //$$->SetLineNumber(line_number);
}
#line 2193 "SysY_parser.tab.c"
    break;

  case 86: /* Exp_list: Exp_list ',' Exp  */
#line 591 "parser/SysY_parser.y"
                   {
    (yyval.expressions) = (yyvsp[-2].expressions);
    (yyval.expressions)->push_back((yyvsp[0].expression));
    //$$->SetLineNumber(line_number);
}
#line 2203 "SysY_parser.tab.c"
    break;

  case 87: /* MulExp: UnaryExp  */
#line 601 "parser/SysY_parser.y"
           {
    (yyval.expression) = (yyvsp[0].expression);
    (yyval.expression)->SetLineNumber(line_number);
}
#line 2212 "SysY_parser.tab.c"
    break;

  case 88: /* MulExp: MulExp '*' UnaryExp  */
#line 605 "parser/SysY_parser.y"
                      {
    (yyval.expression) = new MulExp_mul((yyvsp[-2].expression), (yyvsp[0].expression));
    (yyval.expression)->SetLineNumber(line_number);
}
#line 2221 "SysY_parser.tab.c"
    break;

  case 89: /* MulExp: MulExp '/' UnaryExp  */
#line 609 "parser/SysY_parser.y"
                      {
    (yyval.expression) = new MulExp_div((yyvsp[-2].expression), (yyvsp[0].expression));
    (yyval.expression)->SetLineNumber(line_number);
}
#line 2230 "SysY_parser.tab.c"
    break;

  case 90: /* MulExp: MulExp '%' UnaryExp  */
#line 613 "parser/SysY_parser.y"
                      {
    (yyval.expression) = new MulExp_mod((yyvsp[-2].expression), (yyvsp[0].expression));
    (yyval.expression)->SetLineNumber(line_number);
}
#line 2239 "SysY_parser.tab.c"
    break;

  case 91: /* AddExp: MulExp  */
#line 621 "parser/SysY_parser.y"
       {
    (yyval.expression) = (yyvsp[0].expression);
    (yyval.expression)->SetLineNumber(line_number);
}
#line 2248 "SysY_parser.tab.c"
    break;

  case 92: /* AddExp: AddExp '+' MulExp  */
#line 625 "parser/SysY_parser.y"
                  {
    (yyval.expression) = new AddExp_plus((yyvsp[-2].expression),(yyvsp[0].expression)); 
    (yyval.expression)->SetLineNumber(line_number);
}
#line 2257 "SysY_parser.tab.c"
    break;

  case 93: /* AddExp: AddExp '-' MulExp  */
#line 629 "parser/SysY_parser.y"
                  {
    (yyval.expression) = new AddExp_sub((yyvsp[-2].expression),(yyvsp[0].expression)); 
    (yyval.expression)->SetLineNumber(line_number);
}
#line 2266 "SysY_parser.tab.c"
    break;

  case 94: /* RelExp: AddExp  */
#line 637 "parser/SysY_parser.y"
         {
    (yyval.expression) = (yyvsp[0].expression);
    (yyval.expression)->SetLineNumber(line_number);
}
#line 2275 "SysY_parser.tab.c"
    break;

  case 95: /* RelExp: RelExp '<' AddExp  */
#line 641 "parser/SysY_parser.y"
                    {
    (yyval.expression) = new RelExp_lt((yyvsp[-2].expression), (yyvsp[0].expression));
    (yyval.expression)->SetLineNumber(line_number);
}
#line 2284 "SysY_parser.tab.c"
    break;

  case 96: /* RelExp: RelExp '>' AddExp  */
#line 645 "parser/SysY_parser.y"
                    {
    (yyval.expression) = new RelExp_gt((yyvsp[-2].expression), (yyvsp[0].expression));
    (yyval.expression)->SetLineNumber(line_number);
}
#line 2293 "SysY_parser.tab.c"
    break;

  case 97: /* RelExp: RelExp LEQ AddExp  */
#line 649 "parser/SysY_parser.y"
                    {
    (yyval.expression) = new RelExp_leq((yyvsp[-2].expression), (yyvsp[0].expression));
    (yyval.expression)->SetLineNumber(line_number);
}
#line 2302 "SysY_parser.tab.c"
    break;

  case 98: /* RelExp: RelExp GEQ AddExp  */
#line 653 "parser/SysY_parser.y"
                    {
    (yyval.expression) = new RelExp_geq((yyvsp[-2].expression), (yyvsp[0].expression));
    (yyval.expression)->SetLineNumber(line_number);
}
#line 2311 "SysY_parser.tab.c"
    break;

  case 99: /* EqExp: RelExp  */
#line 661 "parser/SysY_parser.y"
         {
    (yyval.expression) = (yyvsp[0].expression);
    (yyval.expression)->SetLineNumber(line_number);
}
#line 2320 "SysY_parser.tab.c"
    break;

  case 100: /* EqExp: EqExp EQ RelExp  */
#line 665 "parser/SysY_parser.y"
                  {
    (yyval.expression) = new EqExp_eq((yyvsp[-2].expression), (yyvsp[0].expression));
    (yyval.expression)->SetLineNumber(line_number);
}
#line 2329 "SysY_parser.tab.c"
    break;

  case 101: /* EqExp: EqExp NE RelExp  */
#line 669 "parser/SysY_parser.y"
                  {
    (yyval.expression) = new EqExp_neq((yyvsp[-2].expression), (yyvsp[0].expression));
    (yyval.expression)->SetLineNumber(line_number);
}
#line 2338 "SysY_parser.tab.c"
    break;

  case 102: /* LAndExp: EqExp  */
#line 677 "parser/SysY_parser.y"
        {
    (yyval.expression) = (yyvsp[0].expression);
    (yyval.expression)->SetLineNumber(line_number);
}
#line 2347 "SysY_parser.tab.c"
    break;

  case 103: /* LAndExp: LAndExp AND EqExp  */
#line 681 "parser/SysY_parser.y"
                    {
    (yyval.expression) = new LAndExp_and((yyvsp[-2].expression), (yyvsp[0].expression));
    (yyval.expression)->SetLineNumber(line_number);
}
#line 2356 "SysY_parser.tab.c"
    break;

  case 104: /* LOrExp: LAndExp  */
#line 689 "parser/SysY_parser.y"
          {
    (yyval.expression) = (yyvsp[0].expression);
    (yyval.expression)->SetLineNumber(line_number);
}
#line 2365 "SysY_parser.tab.c"
    break;

  case 105: /* LOrExp: LOrExp OR LAndExp  */
#line 693 "parser/SysY_parser.y"
                    {
    (yyval.expression) = new LOrExp_or((yyvsp[-2].expression), (yyvsp[0].expression));
    (yyval.expression)->SetLineNumber(line_number);
}
#line 2374 "SysY_parser.tab.c"
    break;

  case 106: /* ConstExp: Exp  */
#line 702 "parser/SysY_parser.y"
    {
	(yyval.expression)=(yyvsp[0].expression);
	(yyval.expression)->SetLineNumber(line_number);
}
#line 2383 "SysY_parser.tab.c"
    break;


#line 2387 "SysY_parser.tab.c"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;
  *++yylsp = yyloc;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (YY_("syntax error"));
    }

  yyerror_range[1] = yylloc;
  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval, &yylloc);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;

      yyerror_range[1] = *yylsp;
      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp, yylsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  yyerror_range[2] = yylloc;
  ++yylsp;
  YYLLOC_DEFAULT (*yylsp, yyerror_range, 2);

  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval, &yylloc);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp, yylsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 712 "parser/SysY_parser.y"
 

void yyerror(char* s, ...)
{
    ++error_num;
    fout<<"parser error in line "<<line_number<<"\n";
}
