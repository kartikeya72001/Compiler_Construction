#include <iostream>
#include <cstring>
#include <unordered_set>
using namespace std;

char identifiers[] = "auto|double|int|struct|break|else|long|switch|case|enum|register|typedef|char|extern|return|union|continue|for|signed|void|do|if|static|while|default|goto|sizeof|volatile|const|float|short|unsigned";
unordered_set<string> id;

int main(int argc, char const *argv[]) {
    char *token = strtok(identifiers,"|");
    while(token!=NULL){
        id.insert(token);
        token = strtok(NULL,"|");
    }
    int t;
    cin>>t;
    while(t--){
        string input;
        cin>>input;
        id.find(input)!=id.end() ? cout<<"Identifier\n" : cout<<"Not an Identifier\n";
    }
    return 0;
}
