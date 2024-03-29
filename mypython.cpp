#include <iostream>
#include <string.h>
#include <vector>
#include <unordered_set>
using namespace std;
//token object and its attributes
struct token{
    string type;
    string value;
    int line;
    int index;
};

//function to see if string is a keyword, can add more keywords if needed
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
//function to see if string is comparison, can add more if needed
bool isComparison(const string& identifier) {
    // List of Python keywords
    static const string comparisons[] = {
        "==", "<", ">", "<=",">=","!="
    };

    // Search for the identifier in the keywords array
    for (const auto& comp : comparisons) {
        if (comp == identifier) {
            return true;
        }
    }
    return false;
}
//same thing for operators
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
//token goes through and tokenizes the input, if it doesnt know
//what type of token it is right away thats okay
//at the end of this function it loops through the new array
//of tokens and figures out what they are based on their position
//to known tokens like operators and assignments
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
            // If currentWord is not empty, push it to tokens
            if (!currentWord.empty()) {
                if (isKeyword(currentWord)) {
                    token newtoken = {"keyword", currentWord, line, index};
                    tokens.push_back(newtoken);
                } else if (isOperator(currentWord)) {
                    token newtoken = {"operator", currentWord, line, index};
                    tokens.push_back(newtoken);
                } else if(isComparison(currentWord)){
                    token newtoken = {"comparison", currentWord, line, index};
                    tokens.push_back(newtoken);
                } else if(currentWord=="#"){
                    break;
                } else if (currentWord == "=") {
                    token newtoken = {"assignment", currentWord, line, index};
                    tokens.push_back(newtoken);
                } else {
                    token newtoken = {"other", currentWord, line, index};
                    tokens.push_back(newtoken);
                }
                currentWord.clear(); // Clear currentWord after processing each token
            }
        }
    }
    // Process the last token if it's not empty
    if (!currentWord.empty()) {
        if (isKeyword(currentWord)) {
            token newtoken = {"keyword", currentWord, line, index};
            tokens.push_back(newtoken);
        } else if (isOperator(currentWord)) {
            token newtoken = {"operator", currentWord, line, index};
            tokens.push_back(newtoken);
        } else if (isComparison(currentWord)) {
            token newtoken = {"comparison", currentWord, line, index};
            tokens.push_back(newtoken);
        } else if (currentWord == "=") {
            token newtoken = {"assignment", currentWord, line, index};
            tokens.push_back(newtoken);
        } else {
            token newtoken = {"other", currentWord, line, index};
            tokens.push_back(newtoken);
        }
        for (int i = 0; i < tokens.size(); i++) {
        if (tokens[i].type == "other") {
            // Check if the next token is an operator
            if (i + 1 < tokens.size() && tokens[i + 1].type == "operator") {
                // Treat the current keyword as an operand
                tokens[i].type = "operand";
            }
        }
         if (tokens[i].type == "other") {
            // Check if the next token is an operator
            if (i + 1 < tokens.size() && tokens[i + 1].type == "assignment") {
                // Treat the current keyword as an operand
                tokens[i].type = "variable";
            }
        }
        if (tokens[i].type == "other") {
            // Check if the next token is an operator
            if (i + 1 < tokens.size() && tokens[i + 1].type == "comparison") {
                // Treat the current keyword as an operand
                tokens[i].type = "variable";
            }
        }
        if (tokens[i].type == "other") {
            // Check if the next token is an operator
            if (tokens[i - 1].type == "operator") {
                // Treat the current keyword as an operand
                tokens[i].type = "operand";
            }
        }
       
    }
    }
    return tokens;
}
//this goes through and verifies the expressions if theyre true or false
//based on the grammar you establish
bool parseTokens(const vector<token> tokens, int index = 0) {
    // Check if the index is within bounds
    if (index >= tokens.size()) {
        cout << "Error: Unexpected end of input" << endl;
        return false;
    }

    // Grammar for this: variable assignment (other||operand operator operand)
    if (tokens[index].type=="variable"){
        if(tokens[index+1].type!="assignment"){
            cout<<"missing assignment after variable declaration "<<tokens[index].value<<endl;
            return false;
        }else{
            return parseTokens(tokens,index+1);
        }
    }
    else if (tokens[index].type=="assignment"){
        if(tokens[index+1].type!="other"&&tokens[index+1].type!="operand"){
            cout<<"missing variable or operand after assignment "<<tokens[index].value<<endl;
            return false;
        }else{
            return parseTokens(tokens,index+1);
        }
    }
    else if (tokens[index].type=="operand"){
        if(index!=tokens.size()-1){
            return parseTokens(tokens,index+1);
        }else{
            return true;
        }
    }
    else if (tokens[index].type=="operator"){
        if(tokens[index+1].type!="operand"){
            cout<<"missing operand token after operator "<<tokens[index].value<<endl;
            return false;
        }else{
            return parseTokens(tokens,index+1);
        }
    }else{
        return true;
    }

    // Parsing succeeded
}


int main(){
    //testing different values
    string input = "resultSum = a + b + c * d";
    vector<token>tokens =lexer(input,1,0);
    for(int i=0;i<tokens.size();i++){
        cout<<tokens[i].type<<endl;
    }
    if(!parseTokens(tokens)){
        cout<<"this isnt valid";
    }else{
        cout<<"this is valid";
    }
    return 0;
}