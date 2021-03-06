//============================================================================
//
//   SSSS    tt          lll  lll
//  SS  SS   tt           ll   ll
//  SS     tttttt  eeee   ll   ll   aaaa
//   SSSS    tt   ee  ee  ll   ll      aa
//      SS   tt   eeeeee  ll   ll   aaaaa  --  "An Atari 2600 VCS Emulator"
//  SS  SS   tt   ee      ll   ll  aa  aa
//   SSSS     ttt  eeeee llll llll  aaaaa
//
// Copyright (c) 1995-2017 by Bradford W. Mott, Stephen Anthony
// and the Stella Team
//
// See the file "License.txt" for information on usage and redistribution of
// this file, and for a DISCLAIMER OF ALL WARRANTIES.
//============================================================================

//#include "YaccParser.hxx"

//#ifdef __cplusplus
//extern "C" {
//#endif

#include "Base.hxx"
#include "Expression.hxx"
#include "CartDebug.hxx"
#include "CpuDebug.hxx"
#include "TIADebug.hxx"

#include "DebuggerExpressions.hxx"

namespace YaccParser {
#include <stdio.h>
#include <ctype.h>

#include "y.tab.h"
YYSTYPE result;
string errMsg;
#include "y.tab.c"

const string& errorMessage()
{
  return errMsg;
}

Expression* getResult()
{
  lastExp = 0;
  return result.exp;
}

const char *input, *c;

enum {
  ST_DEFAULT,
  ST_IDENTIFIER,
  ST_OPERATOR,
  ST_SPACE
};

int state = ST_DEFAULT;

//extern int yylval; // bison provides this

void setInput(const char *in)
{
  input = c = in;
  state = ST_DEFAULT;
}

int parse(const char *in)
{
  lastExp = 0;
  errMsg = "(no error)";
  setInput(in);
  return yyparse();
}

/* hand-rolled lexer. Hopefully faster than flex... */
inline bool is_base_prefix(char x)
{
  return ( (x=='\\' || x=='$' || x=='#') );
}

inline bool is_identifier(char x)
{
  return ( (x>='0' && x<='9') ||
           (x>='a' && x<='z') ||
           (x>='A' && x<='Z') ||
            x=='.' || x=='_'  );
}

inline bool is_operator(char x)
{
  return ( (x=='+' || x=='-' || x=='*' ||
            x=='/' || x=='<' || x=='>' ||
            x=='|' || x=='&' || x=='^' ||
            x=='!' || x=='~' || x=='(' ||
            x==')' || x=='=' || x=='%' ||
            x=='[' || x==']' ) );
}

// const_to_int converts a string into a number, in either the
// current base, or (if there's a base override) the selected base.
// Returns -1 on error, since negative numbers are the parser's
// responsibility, not the lexer's
int const_to_int(char* ch) {
  // what base is the input in?
  Common::Base::Format format = Common::Base::format();

  switch(*ch) {
    case '\\':
      format = Common::Base::F_2;
      ch++;
      break;

    case '#':
      format = Common::Base::F_10;
      ch++;
      break;

    case '$':
      format = Common::Base::F_16;
      ch++;
      break;

    default: // not a base_prefix, use default base
      break;
  }

  int ret = 0;
  switch(format) {
    case Common::Base::F_2:
      while(*ch) {
        if(*ch != '0' && *ch != '1')
          return -1;
        ret *= 2;
        ret += (*ch - '0');
        ch++;
      }
      return ret;

    case Common::Base::F_10:
      while(*ch) {
        if(!isdigit(*ch))
          return -1;
        ret *= 10;
        ret += (*ch - '0');
        ch++;
      }
      return ret;

    case Common::Base::F_16:
      while(*ch) { // FIXME: error check!
        if(!isxdigit(*ch))
          return -1;
        int dig = (*ch - '0');
        if(dig > 9) dig = tolower(*ch) - 'a' + 10;
        ret *= 16;
        ret += dig;
        ch++;
      }
      return ret;

    default:
      fprintf(stderr, "INVALID BASE in lexer!");
      return 0;
  }
}

// special methods that get e.g. CPU registers
CpuMethod getCpuSpecial(char* ch)
{
  if(BSPF::equalsIgnoreCase(ch, "a"))
    return &CpuDebug::a;
  else if(BSPF::equalsIgnoreCase(ch, "x"))
    return &CpuDebug::x;
  else if(BSPF::equalsIgnoreCase(ch, "y"))
    return &CpuDebug::y;
  else if(BSPF::equalsIgnoreCase(ch, "pc"))
    return &CpuDebug::pc;
  else if(BSPF::equalsIgnoreCase(ch, "sp"))
    return &CpuDebug::sp;
  else if(BSPF::equalsIgnoreCase(ch, "c"))
    return &CpuDebug::c;
  else if(BSPF::equalsIgnoreCase(ch, "z"))
    return &CpuDebug::z;
  else if(BSPF::equalsIgnoreCase(ch, "n"))
    return &CpuDebug::n;
  else if(BSPF::equalsIgnoreCase(ch, "v"))
    return &CpuDebug::v;
  else if(BSPF::equalsIgnoreCase(ch, "d"))
    return &CpuDebug::d;
  else if(BSPF::equalsIgnoreCase(ch, "i"))
    return &CpuDebug::i;
  else if(BSPF::equalsIgnoreCase(ch, "b"))
    return &CpuDebug::b;
  else
    return 0;
}

// special methods that get Cart RAM/ROM internal state
CartMethod getCartSpecial(char* ch)
{
  if(BSPF::equalsIgnoreCase(ch, "_bank"))
    return &CartDebug::getBank;
  else if(BSPF::equalsIgnoreCase(ch, "_rwport"))
    return &CartDebug::readFromWritePort;
  else
    return 0;
}

// special methods that get TIA internal state
TiaMethod getTiaSpecial(char* ch)
{
  if(BSPF::equalsIgnoreCase(ch, "_scan"))
    return &TIADebug::scanlines;
  else if(BSPF::equalsIgnoreCase(ch, "_fcount"))
    return &TIADebug::frameCount;
  else if(BSPF::equalsIgnoreCase(ch, "_fcycles"))
    return &TIADebug::frameCycles;
  else if(BSPF::equalsIgnoreCase(ch, "_cyclesLo"))
    return &TIADebug::cyclesLo;
  else if(BSPF::equalsIgnoreCase(ch, "_cyclesHi"))
    return &TIADebug::cyclesHi;
  else if(BSPF::equalsIgnoreCase(ch, "_cclocks"))
    return &TIADebug::clocksThisLine;
  else if(BSPF::equalsIgnoreCase(ch, "_vsync"))
    return &TIADebug::vsyncAsInt;
  else if(BSPF::equalsIgnoreCase(ch, "_vblank"))
    return &TIADebug::vblankAsInt;
  else
    return 0;
}

int yylex() {
  static char idbuf[255];
  char o, p;
  yylval.val = 0;
  while(*c != '\0') {
    //fprintf(stderr, "looking at %c, state %d\n", *c, state);
    switch(state) {
      case ST_SPACE:
        yylval.val = 0;
        if(isspace(*c)) {
          c++;
        } else if(is_identifier(*c) || is_base_prefix(*c)) {
          state = ST_IDENTIFIER;
        } else if(is_operator(*c)) {
          state = ST_OPERATOR;
        } else {
          state = ST_DEFAULT;
        }

        break;

      case ST_IDENTIFIER:
        {
          CartMethod cartMeth;
          CpuMethod  cpuMeth;
          TiaMethod  tiaMeth;

          char *bufp = idbuf;
          *bufp++ = *c++; // might be a base prefix
          while(is_identifier(*c)) { // may NOT be base prefixes
            *bufp++ = *c++;
            //fprintf(stderr, "yylval==%d, *c==%c\n", yylval, *c);
          }
          *bufp = '\0';
          state = ST_DEFAULT;

          // Note: specials (like "a" for accumulator) have priority over
          // numbers. So "a" always means accumulator, not hex 0xa. User
          // is welcome to use a base prefix ("$a"), or a capital "A",
          // to mean 0xa.

          // Also, labels have priority over specials, so Bad Things will
          // happen if the user defines a label that matches one of
          // the specials. Who would do that, though?

          if(Debugger::debugger().cartDebug().getAddress(idbuf) > -1) {
            yylval.Equate = idbuf;
            return EQUATE;
          } else if( (cpuMeth = getCpuSpecial(idbuf)) ) {
            yylval.cpuMethod = cpuMeth;
            return CPU_METHOD;
          } else if( (cartMeth = getCartSpecial(idbuf)) ) {
            yylval.cartMethod = cartMeth;
            return CART_METHOD;
          } else if( (tiaMeth = getTiaSpecial(idbuf)) ) {
            yylval.tiaMethod = tiaMeth;
            return TIA_METHOD;
          } else if( Debugger::debugger().getFunctionDef(idbuf) != EmptyString ) {
            yylval.DefinedFunction = idbuf;
            return FUNCTION;
          } else {
            yylval.val = const_to_int(idbuf);
            if(yylval.val >= 0)
              return NUMBER;
            else
              return ERR;
          }
        }

      case ST_OPERATOR:
        o = *c++;
        if(!*c) return o;
        if(isspace(*c)) {
          state = ST_SPACE;
          return o;
        } else if(is_identifier(*c) || is_base_prefix(*c)) {
          state = ST_IDENTIFIER;
          return o;
        } else {
          state = ST_DEFAULT;
          p = *c++;
          //fprintf(stderr, "o==%c, p==%c\n", o, p);
          if(o == '>' && p == '=')
            return GTE;
          else if(o == '<' && p == '=')
            return LTE;
          else if(o == '!' && p == '=')
            return NE;
          else if(o == '=' && p == '=')
            return EQ;
          else if(o == '|' && p == '|')
            return LOG_OR;
          else if(o == '&' && p == '&')
            return LOG_AND;
          else if(o == '<' && p == '<')
            return SHL;
          else if(o == '>' && p == '>')
            return SHR;
          else {
            c--;
            return o;
          }
        }
        // break;  Never executed

      case ST_DEFAULT:
      default:
        yylval.val = 0;
        if(isspace(*c)) {
          state = ST_SPACE;
        } else if(is_identifier(*c) || is_base_prefix(*c)) {
          state = ST_IDENTIFIER;
        } else if(is_operator(*c)) {
          state = ST_OPERATOR;
        } else {
          yylval.val = *c++;
          return yylval.val;
        }
        break;
    }
  }

  //fprintf(stderr, "end of input\n");
  return 0; // hit NUL, end of input.
}


#if 0
int main(int argc, char **argv) {
  int l;

  set_input(argv[1]);
  while( (l = yylex()) != 0 )
    printf("ret %d, %d\n", l, yylval);

  printf("%d\n", yylval);
}
#endif
}

//#ifdef __cplusplus
//}
//#endif
