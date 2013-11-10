%{
#include "global.h"
%}
 /* ** DEFINITIONS *** */
endline [\n]
delim [\t ]
whitespace {delim}+
alphanum [/_0-9a-zA-Z]
word {alphanum}*
quote ["]
string {quote}{alphanum}*{whitespace}*{quote}
metachar [#<>]
%%
{whitespace} {/* skip blanks and tabs */}
{string} {
    char* n = yytext;
    n++;
    n[strlen(n)-1]=0;
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
    token t(ENDOFLINE, yytext);
    vTokens.push_back(t);
    return ENDOFLINE;
    }
. {printf ("Unrecognizable token: [%s]\n", yytext);}
%%
 /********** USER CODE **********/
int yywrap(void) {return 1;}
