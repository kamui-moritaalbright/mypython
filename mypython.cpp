#include <iostream>
#include <string.h>
#include <vector>
#include <unordered_set>
#include <map>
#include <stack>
#include <sstream>
#include <ctype.h>
#include <string>
using namespace std;
//token object and its attributes
struct token{
    string type;
    string value;
    int line;
    int index;
};

struct parse{
    token element;
    parse *next;

    parse(token ele) : element(ele), next(nullptr){}
};

class parser{
public:
    map<string, int> variables;
    vector<parse*> lines;
    bool error = 0;

    /*parser(vector<token> tokens){
        parse_saveLines(tokens);
    }*/
    void parser_construct(vector<token> tokens){
        parse_saveLines(tokens);
    }
    void parser_evaluate(){
        parseEvaluate();
        print();
    }

private:

    //this goes through and verifies the expressions if theyre true or false
//based on the grammar you establish
    bool validSyntax(const vector<token> tokens, int index = 0) {
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
                return validSyntax(tokens,index+1);
            }
        }
        else if (tokens[index].type=="assignment"){
            if(tokens[index+1].type!="other"&&tokens[index+1].type!="operand"){
                cout<<"missing variable or operand after assignment "<<tokens[index].value<<endl;
                return false;
            }else{
                return validSyntax(tokens,index+1);
            }
        }
        else if (tokens[index].type=="operand"){
            if(index!=tokens.size()-1){
                return validSyntax(tokens,index+1);
            }else{
                return true;
            }
        }
        else if (tokens[index].type=="operator"){
            if(tokens[index+1].type!="operand"){
                cout<<"missing operand token after operator "<<tokens[index].value<<endl;
                return false;
            }else{
                return validSyntax(tokens,index+1);
            }
        }
        else{
            return true;
        }

        // Parsing succeeded
    }

    void parse_saveLines(vector<token>tokens){
        if(validSyntax(tokens)){
            parse *top = new parse(tokens[0]);
            parse *cu = top;
            for(int i=1; i<tokens.size(); i++){
                parse *nextNode = new parse(tokens[i]);
                cu->next = nextNode;
                cu = nextNode; 
            }
            lines.push_back(top);
        }
    }

    void parseEvaluate(){
        for(int i=0; i<lines.size(); i++){
            parse *head = lines[i];
            if(head->element.type == "variable" && head){
                if(head->next!=nullptr){
                    if(head->next->next!=nullptr){
                        if(head->next->next->next!=nullptr){
                            variables[head->element.value] = computeAssignment(head->next->next);
                        }
                        else{
                            string tmp = head->next->next->element.value;
                            if(isnumber(tmp))
                                variables[head->element.value] = stoi(head->next->next->element.value);
                            else{
                                auto declaredVal = variables.find(tmp);
                                if(declaredVal!=variables.end()){
                                    variables[head->element.value] = declaredVal->second;
                                }
                                else{
                                    cout << "Semantic Error: Use of Undeclared Variable" << endl;
                                    error = true;
                                }
                                
                            }
                        }
                    }else{
                        cout << "Syntax Error: Missing variables after Assignment" << endl;
                    }
                }else{
                    cout << "Syntax Error: Invalid Declaration" << endl;
                }
                //int RH = computeAssignment()
                
            }
        }
    }

    int computeAssignment(parse* head) {
        stack<string> stack1;
        string postfix;
        string token;

        parse* cu = head;
        while (cu != nullptr) {
            // Simplified version: You need to implement operator precedence and associativity handling here
            // This is just a placeholder to illustrate the process
            if (cu->element.type == "operand") {
                string tmp = cu->element.value;
                if(!isnumber(cu->element.value)){
                    auto declaredVal = variables.find(tmp);
                    if(declaredVal!=variables.end()){
                        tmp = to_string(declaredVal->second);
                    }
                    else{
                        cout << "Semantic Error: Use of Undeclared Variable at line: " << cu->element.line  << endl;
                        error = true;
                        return -9999;
                    }
                }

                postfix += tmp + " ";
            } else { // Operator
                while (!stack1.empty()) {
                    postfix += stack1.top() + " ";
                    stack1.pop();
                }
                stack1.push(cu->element.value);
            }
            cu = cu->next;
        }

        while (!stack1.empty()) {
            postfix += stack1.top() + " ";
            stack1.pop();
        }

        stack<int> stack2;
        istringstream tokenStream(postfix);

        while (tokenStream >> token) {
            if (isnumber(token)) { // Operand
                stack2.push(stoi(token));
            } else { // Operator
                int val2 = stack2.top(); stack2.pop();
                int val1 = stack2.top(); stack2.pop();

                switch(token[0]) {
                    case '+': stack2.push(val1 + val2); break;
                    case '-': stack2.push(val1 - val2); break;
                    case '*': stack2.push(val1 * val2); break;
                    case '/': stack2.push(val1 / val2); break;
                }
            }
        }

        return stack2.top();
    }

    bool isnumber (string in){
        for(int i=0; i<in.size(); i++){
            if(!isdigit(in[i]))
                return false;
        }
        return true;
    }



    void print(){
        cout << "Input: " ;
        for(int i=0; i<lines.size(); i++){
            parse *cu = lines[i];
            while(cu!=nullptr){
                cout << cu->element.value;
                cu = cu->next;
            }
            cout << endl;
        }
        for (const auto& pair : variables) {
            cout << "Key: " << pair.first << ", Value: " << pair.second << endl;
        }
        
    }
};


//function to see if string is a keyword, can add more keywords if needed
bool isKeyword(const string& identifier) {
    // List of Python keywords
    static const string keywords[] = {
        "def", "else", "if", "print"
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

int main(){
    //testing different values
    string input1 = "a = 1";
    string input2 = "b = 2 + 3";
    string input3 = "result = a + b";
    vector<token>tokens1 =lexer(input1,1,0);
    vector<token>tokens2 =lexer(input2,1,0);
    vector<token>tokens3 =lexer(input3,1,0);
    /*for(int i=0;i<tokens.size();i++){
        cout<<tokens[i].type<<endl;
    }
    
    if(!parseTokens(tokens)){
        cout<<"this isnt valid";
    }else{
        cout<<"this is valid";
    }*/
    parser newin;
    newin.parser_construct(tokens1);
    newin.parser_construct(tokens2);
    newin.parser_construct(tokens3);
    newin.parser_evaluate();
    return 0;
}