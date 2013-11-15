#include <cstring>
#include <sys/wait.h>
#include <string>
#include <vector>
#include <stdlib.h>
#include <unistd.h>

#define WORD 258
#define STRING 259 
#define ENDOFLINE 260
#define METACHAR 261

int yylex(void);

class token{
    public:
    token(int t, std::string s){
        type = t;
        value = s;
    }
    int getType(){
        return type;
    }
    std::string getValue(){
        return value;
    }
    void setType(int t){
        type = t;
    }
    void setValue(std::string s){
        value = s;
    }
    private: 
    int type;
    std::string value;
};

extern std::vector<token> vTokens;

