/* C++ parsers require Bison 3 */
%require "3.0"
%language "C++"

/* Write-out tokens header file */
%defines

/* Use Bison's 'variant' to store values. 
 * This allows us to use non POD types (e.g.
 * with constructors/destrictors), which is
 * not possible with the default mode which
 * uses unions.
 */
%define api.value.type variant

/* 
 * Use the 'complete' symbol type (i.e. variant)
 * in the lexer
 */
%define api.token.constructor

/*
 * Add a prefix the make_* functions used to
 * create the symbols
 */
%define api.token.prefix {TOKEN_}

/* Wrap everything in our namespace */
%define api.namespace {vcdparse}

%define parser_class_name {Parser}

/* Extra checks for correct usage */
%define parse.assert

/* Enable debugging info */
%define parse.trace

/* Better error reporting */
%define parse.error verbose

/* 
 * Fixes inaccuracy in verbose error reporting.
 * May be slow for some grammars.
 */
/*%define parse.lac full*/

/* Track locations */
%locations

/* Generate a table of token names */
%token-table

/* The parser expects the lexer to be provided as a constructor argument */
%parse-param {vcdparse::Lexer& lexer}
%parse-param {vcdparse::Loader& driver}

/* Our yylex implementation expects the lexer to be passed as an argument */
%lex-param {vcdparse::Lexer& lexer}

%code requires {
    namespace vcdparse {
        class Lexer;
        class Loader;
    }

    #include "vcd_error.hpp"
    #include "vcd_data.hpp"

    //This is not defined by default for some reason...
    #define YY_NULLPTR nullptr
}

%code top {

    #include "vcd_lexer.hpp"
    #include "vcd_loader.hpp"

    //Bison calls yylex() to get the next token.
    //Since we have re-defined the equivalent function in the lexer
    //we need to tell Bison how to get the next token.
    static vcdparse::Parser::symbol_type yylex(vcdparse::Lexer& lexer) {
        return lexer.next_token();
    }

    #include <iostream> //For cout in error reporting
    using std::cout;
}

%token DATE "$date"
%token VERSION "$version"
%token COMMENT "$comment"
%token TIMESCALE "$timescale"
%token SCOPE "$scope"
%token MODULE "module"
%token VAR "$var"
%token WIRE "wire"
%token REG "reg"
%token PARAMETER "parameter"
%token UPSCOPE "$upscope"
%token ENDDEFINITIONS "$enddefinitions"
%token DUMPVARS "$dumpvars"
%token END "$end"
%token LOGIC_ONE "1"
%token LOGIC_ZERO "0"
%token LOGIC_UNKOWN "x"
%token LOGIC_HIGHZ "z"
%token <std::string> String "string"
%token <std::string> Multiline "multiline-string"
%token <std::string> VarId "var-id"
%token <std::string> BitString "bit-string"
%token <size_t> Time "time-value"
%token <size_t> Integer "integer-value"
%token <std::string> VarType "var-type"
%token EOF 0 "end-of-file"

%type <LogicValue> LogicValue

%start vcd_file

%%
vcd_file : vcd_header definitions dumpvars change_list { driver.vcd_data_ = VcdData(); }
         ;

vcd_header : date { }
           | vcd_header version { }
           | vcd_header timescale { }
           ;

date : DATE Multiline END { }
     ;

version : VERSION Multiline END { }
        ;

timescale : TIMESCALE Multiline END { }
          ;

scope : SCOPE MODULE String END { }
      ;

definitions : scope { }
     | definitions scope { }
     | definitions var { }
     | definitions upscope { }
     | definitions enddefinitions { }
     ;

var : VAR var_type String String String END { }
    | VAR var_type String String String String END { }
    ;

var_type : WIRE { }
         | REG { }
         | PARAMETER { }
         ;

upscope : UPSCOPE END { }
        ;

enddefinitions : ENDDEFINITIONS END { }

dumpvars : Time DUMPVARS var_values END { }




change_list : time_values { }
            | change_list time_values { }
            ;

time_values : Time var_values { }

var_values : LogicValue VarId { }
           | var_values LogicValue VarId { }
           ;

LogicValue : LOGIC_ONE    { $$ = vcdparse::LogicValue::ONE; }
           | LOGIC_ZERO   { $$ = vcdparse::LogicValue::ZERO; }
           | LOGIC_UNKOWN { $$ = vcdparse::LogicValue::UNKOWN; }
           | LOGIC_HIGHZ  { $$ = vcdparse::LogicValue::HIGHZ; }
           | BitString    { }
           ;

%%

//We need to provide an implementation for parser error handling
void vcdparse::Parser::error(const vcdparse::location& loc, const std::string& msg) {
    throw vcdparse::ParseError(msg, loc);
}
