#include <iostream>
#include <cstring>
#include <vector>
using namespace std;

template<typename T>
class Node{
public:
    string name;
    string type;
    Node *next;
    T *address;
    int size;
    Node(T *variable, string name){
        this->name = name;
        address = variable;
        next = NULL;
        size = sizeof(variable);
        if(typeid(variable).name() == typeid(int*).name()){
            type = "int";
        }
        else if(typeid(variable).name() == typeid(string*).name()){
            type = "string";
        }
        else if(typeid(variable).name() == typeid(char*).name()){
            type = "char";
        }
        else if(typeid(variable).name() == typeid(bool*).name()){
            type = "bool";
        }
    }
};

template<typename T>
class List{
public:
    Node<T>* head;
    List(){
        head = NULL;
    }
    int addNode(T* variable, string name){
        if(head==NULL){
            head = new Node<T>(variable,name);
            return 0;
        }
        while(head->next){
            if(head->name == name){
                return -1;
            }
            head = head->next;
        }
        head->next = new Node<T>(variable,name);
        return 0;
    }

    Node<T>* searchNode(string name){
        if(!head){
            return NULL;
        }
        while(head){
            if(head->name == name)
                return head;
            head = head->next;
        }
        return NULL;
    }

    string getDataType(string name){
        if(!head)
            return "-1";
        while(head){
            if(head->name == name)
                return head->type;
        }
        return "-1";
    }

    void PrintList(){
        if(!head)
            return;
        while(head){
            cout<<"("<<head->name<<","<<head->type<<","<<head->address<<","<<head->size<<")"<<"\n";
            head = head->next;
        }
        return;
    }
};

class SymbolTable{
    public:
    List<int> l1;
    List<char> l2;
    List<string> l3;
    List<bool> l4;

    void addNode(int a,string name){
        l1.addNode(&a,name);
    }

    void addNode(char a,string name){
        l2.addNode(&a,name);
    }

    void addNode(string a,string name){
        l3.addNode(&a,name);
    }

    void addNode(bool a,string name){
        l4.addNode(&a,name);
    }

    void print(){
        l1.PrintList();
        l2.PrintList();
        l3.PrintList();
        l4.PrintList();
    }
    // Node<std::any>* Search(string a){
    //     Node<int>* n1 = l1.searchNode(a);
    //     if(n1)
    //         return n1;
    //     Node<char>* n2 = l2.searchNode(a);
    //     if(n2)
    //         return n2;
    //     Node<string>* n3 = l3.searchNode(a);
    //     if(n3)
    //         return n3;
    //     Node<bool>* n4 = l4.searchNode(a);
    //     if(n4)
    //         return n4;
    // }
};

int main(int argc, char const *argv[]) {
    SymbolTable s;
    int a = 5;
    char b = 'a';
    string str = "str";
    bool d = false;
    s.addNode(a,"a");
    s.addNode(b,"b");
    s.addNode(str,"str");
    s.addNode(d,"d");
    s.print();
    cout<<endl<<endl;
    return 0;
}
