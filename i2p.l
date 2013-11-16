%{
#include "global.h"
%}
 /* ** DEFINITIONS *** */
anyButQuotes [^"]
anyButMetaChars [^ #<>\n]
endline [\n]
delim [\t ]
whitespace {delim}+
alphanum [/_0-9a-zA-Z]
word {anyButMetaChars}*
quote ["]
string {quote}{anyButQuotes}*{quote}
metachar [#<>]
%%
{whitespace} {/* skip blanks and tabs */}
<<EOF>> {
    token t(ENDOFFILE, "endoffile");
    vTokens.push_back(t);
    return ENDOFFILE;
    }
{string} {
    char* n = yytext;
    n++;
    n[strlen(n)-1] = 0;
    token t(STRING, n);
    vTokens.push_back(t);
    return STRING;
    }
{word} {
    token t(WORD, yytext);
    vTokens.push_back(t);
    return WORD;
    }
{metachar} { 
    token t(METACHAR, yytext);
    vTokens.push_back(t);
    return METACHAR;
    }
{endline} {
    token t(ENDOFLINE, "\\n");
    vTokens.push_back(t);
    return ENDOFLINE;
    }
%%
 /********** USER CODE **********/
int yywrap(void) {return 1;}
