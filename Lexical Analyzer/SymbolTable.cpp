#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <iomanip>
using namespace std;

class Node {
public:
    string name;
    string scope;
    string type;
    string value;
    string address;
    Node *next;

    Node(string name, string scope, string type, string value, string address) {
        this->name = name;
        this->scope = scope;
        this->type = type;
        this->value = value;
        this->address = address;
        next = nullptr;
    }
};

class SymTab {
public:
    vector<Node *> buckets;
    int currentSize;
    int totalSize;

    SymTab(int totalSize) {
        this->totalSize = totalSize;
        vector<Node *> b(totalSize,nullptr);
        buckets = b;
        currentSize = 0;
    }

    int hashCodeGen(string k) {
        long long int h = 5381;
        for (int i = 0; i < k.length(); i++) {
            h = ((h << 5) + h) + k[i];
        }

        return h % totalSize;
    }

    void tableRehash() {
        vector<Node *> prevTab = buckets;
        totalSize *= 2;
        vector<Node *> arr(totalSize, nullptr);
        buckets = arr;

        currentSize = 0;

        for (int i = 0; i < totalSize/2; i++) {
            Node *temp = prevTab[i];

            while (temp != nullptr) {
                insert(temp->name, temp->scope, temp->type, temp->value, temp->address);
                temp = temp->next;
            }

            if (prevTab[i] != nullptr)
                delete prevTab[i];
        }
    }

    void insert(string name, string scope, string type, string value, string address) {
        int index = hashCodeGen(name);
        Node *val = new Node(name, scope, type, value, address);
        val->next = buckets[index];
        buckets[index] = val;
        currentSize++;

        double rehashFactor = currentSize / (1.0 * totalSize);
        if (rehashFactor > 0.9)
            tableRehash();
    }

    Node *findVal(string key) {
        int index = hashCodeGen(key);
        Node *beginPtr = buckets[index];

        while (beginPtr != nullptr) {
            if (beginPtr->name == key)
                return beginPtr;
            beginPtr = beginPtr->next;
        }

        return nullptr;
    }

    void find(string name) {
        Node* rval = findVal(name);
        cout << "Identifier: " << rval->name << "\n";
        cout << "Scope: " << rval->scope << "\n";
        cout << "Type: " << rval->type << "\n";
        cout << "Value: " << rval->value << "\n";
        cout << "Address: 0x" << std::hex << stoi(rval->address) << "\n\n";
    }

    void deleteVal(string key) {
        if (findVal(key) == nullptr) {
            cout << "Value not found\n\n";
            return;
        }

        int index = hashCodeGen(key);
        Node *temp = buckets[index];

        if (temp->name == key) {
            buckets[index] = temp->next;
            delete temp;
            currentSize--;
            return;
        }

        while (temp->next != nullptr) {
            if (temp->next->name == key)
                break;
            temp = temp->next;
        }

        if (temp->next == nullptr) {
            Node *last = temp;
            last->next = nullptr;
            temp = temp->next;
            currentSize--;
            return;
        }

        Node *last = temp;
        temp = temp->next;
        last->next = temp->next;
        currentSize--;
        return;
    }

    void display() {
        cout << "\nIdentifier\t" << "Scope\t\t" << "Type\t\t" << "Value\t\t" << "Address\t\n";
        for (int i = 0; i < totalSize; i++) {
            Node *temp = buckets[i];
            while (temp != NULL) {
                cout << temp->name << "\t\t" << temp->scope << "\t\t" << temp->type << "\t\t" << temp->value << "\t\t0x" << std::hex << stoi(temp->address) << "\t\n";
                temp = temp->next;
            }
        }
        cout << "\n";
    }
};

void extractData(vector<vector<string>> &line, SymTab &mySymbolTable) {
    for(int i = 0; i < line.size(); i++) {
        string lastKeyword = "";
        for(int j = 0; j < line[i].size(); j++) {
            if(line[i][j].substr(0,7) == "Keyword") {
                lastKeyword = line[i][j].substr(10);
                int addr = j + (i+1)*100000;
                mySymbolTable.insert(lastKeyword, "local", "keyword", "-", to_string(addr));
            }

            else if(line[i][j].substr(0,10) == "Identifier") {
                string key = line[i][j].substr(13);
                string val = "";
                if(mySymbolTable.findVal(key) == nullptr) {
                    int addr = j + (i+1)*100000;
                    for(int k = j; k < line[i].size(); k++) {
                        if(line[i][k].substr(0,8) == "Constant") {
                            val = line[i][k].substr(11);
                            break;
                        }
                    }
                    mySymbolTable.insert(key, "local", lastKeyword, val, to_string(addr));
                }
            }

            else if(line[i][j].substr(0,8) == "Function") {
                int addr = j + (i+1)*100000;
                string word = line[i][j].substr(10);
                word = word.substr(0,word.length() - 1);
                mySymbolTable.insert(word, "global", "function", "-",to_string(addr));
            }
        }
    }
}

int main(int argc, char **argv) {
    SymTab mySymbolTable(2);
    // mySymbolTable.insert("check", "local", "bool","true", "100010");
    // mySymbolTable.insert("myStr", "local", "string", "Welcome", "201101");
    // mySymbolTable.insert("count", "global", "int", "10", "410010");
    // mySymbolTable.display();
    // mySymbolTable.deleteVal("while");
    // mySymbolTable.find("count");
    // mySymbolTable.insert("while","local","keyword", "-", "500500");
    // mySymbolTable.display();
    // mySymbolTable.deleteVal("while");
    // mySymbolTable.display();
    
    fstream file;
    string filename;
    filename = "output.txt";
    file.open(filename.c_str());
    string str;
    vector<string> lines;
    while (getline(file, str))  
        lines.push_back(str);
    
    vector<vector<string>> line;
    vector<string> temp;
    for(int i = 0; i < lines.size(); i++) {
        if(lines[i] == "") {
            line.push_back(temp);
            vector<string> arr;
            temp = arr;
        }

        temp.push_back(lines[i]);
    }
    line.push_back(temp);
    extractData(line, mySymbolTable);
    mySymbolTable.display();
}