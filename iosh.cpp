#include <iostream>


using namespace std;
int main(){
   
    string input;
    do{
        cout<<"input$:";
        cin>>input;
        cout<<"input was: " <<input<<endl;

    } while (input.compare("exit")!=0);



return 0;
}

