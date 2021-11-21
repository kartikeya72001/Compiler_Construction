#include <iostream>
#include<string>
#include<vector>
using namespace std;
class Node{
public:
    string name;
    string scope;
    string type;
    string value;
    Node* next;
    Node(string name, string scope, string type, string value){
        this->name = name;
        this->scope = scope;
        this->type = type;
        this->value = value;
        next = NULL;
    }
    ~Node(){
        if(next)
            delete next;
    }
};

class SymbolTable{
private:
    vector<Node*> table;
    int cs;
    int ts;

    int hashFn(string key){
        int idx = 0;
        int p = 1;
        for(int j=0;j<key.length();j++){
            idx = idx+(key[j]*p)%ts;
            idx = idx%ts;
            p = (p*27)%ts;
        }
        return idx;
    }
    void rehash(){
        vector<Node*> old = table;
        ts = 2*ts;
        vector<Node*> arr(ts,NULL);
        table = arr;
        cs = 0;

        for(int i=0;i<(ts/2);i++){
            Node*temp = old[i];
            while(temp!=NULL){
                insert(temp->name,temp->scope,temp->type,temp->value);
                temp=temp->next;
            }
            if(old[i]!=NULL){
                delete old[i];
            }
        }
    }
public:
    SymbolTable(int ts = 7){
        this->ts = ts;
        vector<Node*> arr(ts,NULL);
        table = arr;
        cs = 0;
    }
    void insert(string name, string scope, string type, string value){
        Node* obj = Search(name,false,false);
        if(obj && obj->scope == scope){
            cout<<"Record Already Present\n\n"<<endl;
            return;
        }
        int idx = hashFn(name);
        Node *n = new Node(name, scope, type, value);
        n->next = table[idx];
        table[idx] = n;
        cs++;
        float lf = cs/(1.0*ts);
        if(lf>0.8){
            cout<<"***Auto Rehashing***\n\n";
            rehash();
        }
    }
    void printTable(){
        for(int i=0;i<ts;i++){
            cout<<"Table "<<i<<" --> ";
            Node *temp = table[i];
            while(temp!=NULL){
                cout<<temp->name<<" --> ";
                temp = temp->next;
            }
            cout<<endl;
        }
        cout<<endl;
    }
    Node* Search(string name, bool print = true, bool recrd = true){
        int idx = hashFn(name);
        Node *temp = table[idx];
        while(temp!=NULL){
            if(temp->name==name){
                if(print){
                    cout<<"Variable: "<<temp->name<<endl;
                    cout<<"Value: "<<temp->value<<endl;
                    cout<<"Type: "<<temp->type<<endl;
                    cout<<"Scope: "<<temp->scope<<endl;
                }
                return temp;
            }
            temp=temp->next;
        }
        if(recrd)
            cout<<"Record not Found, Cannot be deleted\n\n"<<endl;
        return NULL;
    }
    void erase(string key){
        if(Search(key, false, true)!=NULL){
            int idx = hashFn(key);
            Node* temp = table[idx];
            if(temp->name == key){
                table[idx] = temp->next;
                delete temp;
                cs--;
                return;
            }
            while(temp->next!=NULL){
                if(temp->next->name==key)
                    break;
                temp=temp->next;
            }
            if(temp->next == NULL){
                Node* prev = temp;
                prev->next = NULL;
                temp = temp->next;
                delete temp;
                cs--;
                return;
            }
            Node* prev = temp;
            temp=temp->next;
            prev->next = temp->next;
            delete temp;
            cs--;
            return;
        }
        return;
    }
    void display() {
        cout << "\nIdentifier\t" << "Scope\t\t" << "Type\t\t" << "Value\t\t\n";
        for (int i = 0; i < ts; i++) {
            Node *temp = table[i];
            while (temp != NULL) {
                cout << temp->name << "\t\t" << temp->scope << "\t\t" << temp->type << "\t\t" << temp->value<<endl;
                temp = temp->next;
            }
        }
        cout << "\n";
    }
};

int main(int argc, char const *argv[]) {
    SymbolTable symb;

    symb.insert("a","local","string","Hello World");
    symb.insert("i","global","int","10");
    symb.insert("head","local","Node*","NULL");
    symb.insert("count","global","int","0");
    symb.insert("flag","local","bool","false");
    symb.printTable();
    Node* symbObj = symb.Search("a",true,false);
    if(symbObj==NULL){
        cout<<"Not Found";
    }
    cout<<"\n\n";
    symb.insert("flag","global","bool","false");
    cout<<"Erasing \"a\" from the symbol table\n\n";
    symb.erase("a");
    symb.insert("root","local","Node*","0x7ffe67fcdb24");
    symb.insert("counter","local","int","-1");
    symb.insert("check","local","bool","true");
    symb.insert("ch","global","char","g");
    symb.printTable();
    symb.display();
    return 0;
}
