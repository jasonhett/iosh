
#include <string>
#include <vector>

#define WORD 1
#define STRING 2 
#define ENDOFLINE 3
#define METACHAR 4

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

