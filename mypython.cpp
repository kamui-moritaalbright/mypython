#include <iostream>
#include <string.h>
#include <vector>
#include <unordered_set>
using namespace std;
struct token{
    string type;
    string value;
    int line;
    int index;
};

bool isKeyword(const string& identifier) {
    // List of Python keywords
    static const string keywords[] = {
        "def", "else", "for", "if"
    };

    // Search for the identifier in the keywords array
    for (const auto& keyword : keywords) {
        if (keyword == identifier) {
            return true;
        }
    }
    return false;
}

// Function to check if a string is a Python operator
bool isOperator(const string& identifier) {
    // List of Python operators
    static const string operators[] = {
        "+", "-", "*", "/"
    };

    // Search for the identifier in the operators array
    for (const auto& op : operators) {
        if (op == identifier) {
            return true;
        }
    }
    return false;
}

vector<token> lexer(string input, int line, int index) {
    vector<char> characters;
    vector<token> tokens;
    for (int i = 0; i < input.length(); i++) {
        characters.push_back(input[i]);
    }
    string currentWord;
    for (int i = 0; i < characters.size(); i++) {
        index = i;
        if (characters[i] != ' ') {
            currentWord += characters[i];
        } else {
            if (isKeyword(currentWord)) {
                token newtoken = {"keyword", currentWord, line, index};
                tokens.push_back(newtoken);
            }
            if (isOperator(currentWord)) {
                token newtoken = {"operator", currentWord, line, index};
                tokens.push_back(newtoken);
            }
            currentWord.clear(); // Clear currentWord after processing each token
        }
    }
    // Process the last token if it's not empty
    if (!currentWord.empty()) {
        if (isKeyword(currentWord)) {
            token newtoken = {"keyword", currentWord, line, index};
            tokens.push_back(newtoken);
        }
        if (isOperator(currentWord)) {
            token newtoken = {"operator", currentWord, line, index};
            tokens.push_back(newtoken);
        }
    }
    return tokens;
}



int main(){
    string input = "def + if";
    vector<token>tokens =lexer(input,1,0);
    for(int i=0;i<tokens.size();i++){
        cout<<tokens[i].type<<endl;
    }

    return 0;
}