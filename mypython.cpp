#include <iostream>
#include <string.h>
#include <vector>
#include <unordered_set>
#include <map>
#include <stack>
#include <sstream>
#include <ctype.h>
#include <string>
#include <fstream>
using namespace std;
//token object and its attributes
void debugCheckNull(void* ptr, const string& message) {
    if (ptr == nullptr) {
        cout << "Null pointer detected: " << message << endl;
        // You might want to handle this error condition appropriately
    }
}
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
    map<string, parser*> functions;
    vector<parse*> lines;
    vector<string> ins;
    bool error = false;
    bool skip = false;
    int nestLevel = -1;
    int result = -9999;
  
    void parser_construct(vector<token> tokens){
        if(tokens.empty()) {
            return;
        }
        parse_saveLines(tokens);
    }
    void parser_evaluate(){
        parseEvaluate();
        //print();
    }
    int funcResult(){
        parseEvaluate();
        //print();
        return result;
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
            if(tokens[index+1].type!="other"&&tokens[index+1].type!="operand"&&tokens[index+1].type!="function"){
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
            if(tokens[index+1].type!="operand"&&tokens[index+1].type!="function"){
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
        }else{
            debugCheckNull(nullptr, "parse_saveLines - invalidSyntax");
        }
    }

    void parseEvaluate(){
        for(int i=0; i<lines.size(); i++){
            parse *head = lines[i];

            debugCheckNull(head, "parseEvaluate - head");
            if(skip==true){
                parse *tmp = head;
                int tmp_i = 0;
                while(tmp->next!=nullptr && tmp_i < nestLevel){
                    tmp = tmp->next;
                    tmp_i++;
                }
                if(tmp_i == nestLevel && tmp->element.type == "indent"){
                    continue;
                }
                else if(tmp_i != nestLevel){
                    skip = false;
                }
                else if(tmp->element.value != "else"){
                    skip=false;
                }
            }      
            parse *tmp2 = head;
            int newlayer = 0;
            while(tmp2->element.type=="indent"){
                newlayer++;
                tmp2 = tmp2->next;
            }
            nestLevel = newlayer;
            while(head->element.type == "indent"){
                head = head->next;
            }
            if(head == nullptr) {
            // Skip if head is null
                continue;
            }
            

            if(head->element.type == "variable"){
                if(head->next!=nullptr){
                    parse *tmp = head;
                    parse *tmp2 = nullptr;
                    /*while(tmp!=nullptr){
                        //cout << tmp->element.value;
                        if(tmp->element.type == "function"){
                            //cout << "function evoked: " << tmp->element.value << endl;
                            parser *cu = functions[tmp->element.value];
                            while(tmp->element.value!="(")
                                tmp=tmp->next;
                            tmp=tmp->next;
                            for(int i=0; i<cu->ins.size(); i++){
                                string type = tmp->element.type;
                                if(type == "other" || type == "operand"){
                                    int inVar;
                                    if(type == "operand"){
                                        parse *tmp3 = tmp;
                                        parse *tmp4 = nullptr;
                                        while(tmp3->element.value!=")"){
                                            tmp4=tmp3;
                                            tmp3=tmp3->next;
                                        }
                                        tmp4->next = nullptr;
                                        
                                        inVar = computeAssignment(tmp);
                                        tmp4->next = tmp3;
                                        tmp = tmp4;
                                    }
                                    else if(isnumber(tmp->element.value)){
                                        inVar = stoi(tmp->element.value);
                                    }
                                    else{
                                        inVar = variables[tmp->element.value];
                                        //cout << "inVar is " << inVar << endl;
                                    }
                                    cu->variables[cu->ins[i]] = inVar;
                                }
                                tmp = tmp->next;
                            }
                            tmp = tmp->next;
                            int result = cu->funcResult();
                            int result = functionEvaluate(tmp);

                            token newtoken = {"operand", to_string(result), 1, 0};
                            parse *newIn = new parse(newtoken);
                            tmp2->next = newIn;
                            //cout << "tmp is " << tmp->element.value << endl;
                            newIn->next = tmp;
                            break;
                        }
                        tmp2 = tmp;
                        tmp = tmp->next;
                    }*/
                    //cout << endl;
                    if(head->next->next!=nullptr){
                        if(head->next->next->next!=nullptr){
                            //cout << "calulating for " << head->next->next->element.value << endl;
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
            }else if(head->element.type == "keyword"){
                if(head->element.value == "if"){
                    //cout << "IF triggered" << endl;
                    if(!comparison(head->next)){
                        //cout << "IF triggered" << endl;
                        skip = true;
                    }
                    else
                        nestLevel++;
                }else if (head->element.value == "else"){
                    if(skip){
                        skip = false;
                    }
                    else
                        skip = true;
                }else if (head->element.value == "def"){
                    parser *top = new parser;
                    parser *cu = top;
                    cu->functions=functions;
                    cu->variables=variables;
                    cu->ins=ins;
                    head = head->next;
                    functions[head->element.value] = top;
                    head = head->next->next;
                    while(head!=nullptr && head->element.value!=")"){
                        cu->ins.push_back(head->element.value);
                        head = head->next;
                    }
                    //cout << "suppose "<< cu->ins[0] << " " << cu->ins[1] << endl;
                    while(lines[i+1]!=nullptr && lines[i+1]->element.type == "indent"){
                        i++;
                        cu->lines.push_back(lines[i]->next);
                    }
                }

            }else if(head->element.type == "print"){
                parse *cu = head;
                while(cu != nullptr){
                    if(cu->element.type == "String")
                        cout << cu->element.value << " ";
                    else if (cu->element.type == "other" || cu->element.type == "variable"){
                        string tmp = cu->element.value;
                        auto declaredVal = variables.find(tmp);
                        if(declaredVal!=variables.end()){
                            cout << declaredVal->second;
                        }
                        else{
                            cout << "Semantic Error: Use of Undeclared Variable" << endl;
                            error = true;
                        }
                    }
                    cu = cu->next;
                }
                cout << endl;
            }else if(head->element.type == "return"){
                if(head->next->next!=nullptr)
                    result = computeAssignment(head->next);
                else if(isnumber(head->next->element.value))
                    result = stoi(head->next->element.value);
                else
                    result = variables[head->next->element.value];
            }
        }
    }

    int functionEvaluate(parse*& tmp){
        parser *cu = functions[tmp->element.value];
        while(tmp->element.value!="(")
            tmp=tmp->next;
        tmp=tmp->next;
        for(int i=0; i<cu->ins.size(); i++){
            string type = tmp->element.type;
            if(type == "other" || type == "operand"){
                int inVar;
                if(type == "operand"){
                    parse *tmp3 = tmp;
                    parse *tmp4 = nullptr;
                    while(tmp3->element.value!=")"){
                        tmp4=tmp3;
                        tmp3=tmp3->next;
                    }
                    tmp4->next = nullptr;
                    
                    inVar = computeAssignment(tmp);
                    tmp4->next = tmp3;
                    tmp = tmp4;
                }
                else if(isnumber(tmp->element.value)){
                    inVar = stoi(tmp->element.value);
                }
                else{
                    inVar = variables[tmp->element.value];
                    //cout << "inVar is " << inVar << endl;
                }
                cu->variables[cu->ins[i]] = inVar;
            }
            tmp = tmp->next;
        }
        //tmp = tmp->next;
        int newresult = cu->funcResult();
        return newresult;
    }

    int prec(string c) {
        if (c == "/" || c == "*")
            return 2;
        else if (c == "+" || c == "-")
            return 1;
        else
            return -1;
    }

    int computeAssignment(parse* head) {
        stack<string> stack1;
        string postfix;
        string token;

        parse* cu = head;
        parse* cu2 = nullptr;
        debugCheckNull(cu, "computeAssignment - cu");
        while (cu != nullptr) {
            // Simplified version: You need to implement operator precedence and associativity handling here
            // This is just a placeholder to illustrate the process
            if(cu->element.type == "function"){
                int result = functionEvaluate(cu);
                postfix += to_string(result) + " ";
            }
            else if (cu->element.type == "operand") {
                
                string tmp = cu->element.value;
                if(!isnumber(tmp)){
                    auto declaredVal = variables.find(tmp);
                    if(declaredVal!=variables.end()){
                        tmp = to_string(declaredVal->second);
                    }
                    else{
                        cout << "Semantic Error: Use of Undeclared Variable: " << tmp  << endl;
                        error = true;
                        return -9999;
                    }
                }

                postfix += tmp + " ";
            } else { // Operator
                
                string tmp = cu->element.value;
                while (!stack1.empty() && prec(tmp) <= prec(stack1.top())) {
                    postfix += stack1.top() + " ";
                    stack1.pop();
                }
                stack1.push(cu->element.value);
            }
            if(cu!=nullptr)
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

    bool comparison(parse *head){
        debugCheckNull(head, "comparison - head");
        string leftVal = head->element.value;
        int left;
        string logic = head->next->element.value;
        string rightVal = head->next->next->element.value;
        int right;
        if(isnumber(leftVal))
        {
            left = stoi(leftVal);
        }else{
            auto declaredVal = variables.find(leftVal);
            if(declaredVal!=variables.end()){
                left = declaredVal->second;
            }
            else{
                cout << "Semantic Error: Use of Undeclared Variable" << endl;
                error = true;
            }
        }
        if(isnumber(rightVal))
        {
            right = stoi(rightVal);
        }else{
            auto declaredVal = variables.find(rightVal);
            if(declaredVal!=variables.end()){
                right = declaredVal->second;
            }
            else{
                cout << "Semantic Error: Use of Undeclared Variable" << endl;
                error = true;
            }
        }
    
        if(logic == "==")
            return left == right;
        else if (logic == "<=")
            return left <= right;
        else if (logic == ">=")
            return left >= right;
        else if (logic == "<")
            return left < right;
        else if (logic == ">")
            return left > right;
        else if (logic == "!=")
            return left != right;
        else{
            error = true;
            cout << "Invalid syntax error" << endl;
            return false;
        }
        // Inside the comparison function


        
    }

    void print(){
        cout << "Input: " ;
        for(int i=0; i<lines.size(); i++){
            parse *cu = lines[i];
            while(cu!=nullptr){
                cout << cu->element.type << " " << cu->element.value << " : ";
                cu = cu->next;
            }
            cout << endl;
        }
        for (const auto& pair : variables) {
            cout << "Key: " << pair.first << ", Value: " << pair.second << endl;
        }
        for (const auto& pair : functions) {
            cout << "Functions: " << pair.first << endl;
        }
        
    }
};


//function to see if string is a keyword, can add more keywords if needed
bool isKeyword(const string& identifier) {
    // List of Python keywords
    static const string keywords[] = {
        "def", "else", "if"
    };

    // Search for the identifier in the keywords array
    for (const auto& keyword : keywords) {
        if (keyword == identifier) {
            return true;
        }
    }
    return false;
}

bool isPrint(const string& identifier) {
    // List of Python keywords
    static const string keywords[] = {
        "print"
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
vector<token> lexer(string input, int line, int index=0) {
    vector<char> characters;
    vector<token> tokens;
    for (int i = 0; i < input.length(); i++) {
        characters.push_back(input[i]);
    }
    for(int i = input.length()-1; i >= 0; i--){
        if(characters[i]==' ')
            characters.pop_back();
        else
            break;
    }
    string currentWord;
    int indentCheck = 0;

    while(characters.size() > 1 && characters[indentCheck]==' '){
        token newtoken = {"indent", " ", line, 0};
        tokens.push_back(newtoken);
        indentCheck+=4;
    }

    bool insideParenthesis = false;
    for (int i = 0; i < characters.size(); i++) {
        index = i;
        if (characters[i] != ' ') {
            if (characters[i] == '(' || characters[i] == ')'||characters[i]==',') {
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
                    } else if(isPrint(currentWord)){
                        token newtoken = {"print",currentWord,line,index};
                        tokens.push_back(newtoken);
                    } else if(currentWord=="#"){
                        currentWord.clear();
                        break;
                    } else if (currentWord == "=") {
                        token newtoken = {"assignment", currentWord, line, index};
                        tokens.push_back(newtoken);
                    }else if (currentWord.find('#') != string::npos) {
                        currentWord.clear();
                        break;
                    } else if (currentWord.find('+') != string::npos||currentWord.find('-') != string::npos||currentWord.find('*') != string::npos||currentWord.find('/') != string::npos) {
                    if(currentWord.find('+') != string::npos){
                    size_t pos = currentWord.find('+');
                    string variableName = currentWord.substr(0, pos);
                    string assignmentOperator = "+";
                    string value = currentWord.substr(pos + 1);
                    token variableToken = {"operand", variableName, line, index};
                    token assignmentToken = {"operator", assignmentOperator, line, index};
                    token valueToken = {"operand", value, line, index};
                    tokens.push_back(variableToken);
                    tokens.push_back(assignmentToken);
                    tokens.push_back(valueToken);
                    }else if(currentWord.find('-') != string::npos){
                    size_t pos = currentWord.find('-');
                    string variableName = currentWord.substr(0, pos);
                    string assignmentOperator = "-";
                    string value = currentWord.substr(pos + 1);
                    token variableToken = {"operand", variableName, line, index};
                    token assignmentToken = {"operator", assignmentOperator, line, index};
                    token valueToken = {"operand", value, line, index};
                    tokens.push_back(variableToken);
                    tokens.push_back(assignmentToken);
                    tokens.push_back(valueToken);
                    }else if(currentWord.find('*') != string::npos){
                    size_t pos = currentWord.find('*');
                    string variableName = currentWord.substr(0, pos);
                    string assignmentOperator = "*";
                    string value = currentWord.substr(pos + 1);
                    token variableToken = {"operand", variableName, line, index};
                    token assignmentToken = {"operator", assignmentOperator, line, index};
                    token valueToken = {"operand", value, line, index};
                    tokens.push_back(variableToken);
                    tokens.push_back(assignmentToken);
                    tokens.push_back(valueToken);
                    }else if(currentWord.find('/') != string::npos){
                    size_t pos = currentWord.find('/');
                    string variableName = currentWord.substr(0, pos);
                    string assignmentOperator = "/";
                    string value = currentWord.substr(pos + 1);
                    token variableToken = {"operand", variableName, line, index};
                    token assignmentToken = {"operator", assignmentOperator, line, index};
                    token valueToken = {"operand", value, line, index};
                    tokens.push_back(variableToken);
                    tokens.push_back(assignmentToken);
                    tokens.push_back(valueToken);    
                    }
                } else {
                        token newtoken = {"other", currentWord, line, index};
                        tokens.push_back(newtoken);
                    }
                    currentWord.clear(); // Clear currentWord after processing each token
                }
                // Tokenize parentheses
                string parenthesisToken(1, characters[i]);
                if (parenthesisToken=="("){
                    token newtoken = {"openParenthesis", parenthesisToken, line, index};
                    tokens.push_back(newtoken);
                }else if(parenthesisToken==")"){
                    token newtoken = {"closingParenthesis", parenthesisToken, line, index};
                    tokens.push_back(newtoken);
                }
            } else {
                if(characters[i]=='"'){
                    i++;
                    while(characters[i]!='"'){
                        currentWord+=characters[i];
                        i++;
                    }
                    token newtoken = {"String",currentWord,line,index};
                    tokens.push_back(newtoken);
                    currentWord.clear();
                    i++;
                }
                if(characters[i]==','||characters[i]==':'){
                    continue;
                }
                currentWord += characters[i];
            }
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
                } else if(isPrint(currentWord)){
                    token newtoken = {"print",currentWord,line,index};
                    tokens.push_back(newtoken);
                } else if(currentWord=="#"){
                    currentWord.clear();
                    break;
                } else if (currentWord == "=") {
                    token newtoken = {"assignment", currentWord, line, index};
                    tokens.push_back(newtoken);
                } else if (currentWord.find('=') != string::npos) {
                    size_t pos = currentWord.find('=');
                    string variableName = currentWord.substr(0, pos);
                    string assignmentOperator = "=";
                    string value = currentWord.substr(pos + 1);

                    token variableToken = {"variable", variableName, line, index};
                    token assignmentToken = {"assignment", assignmentOperator, line, index};
                    token valueToken = {"other", value, line, index};

                    tokens.push_back(variableToken);
                    tokens.push_back(assignmentToken);
                    tokens.push_back(valueToken);
                } else if (currentWord.find('+') != string::npos||currentWord.find('-') != string::npos||currentWord.find('*') != string::npos||currentWord.find('/') != string::npos) {
                    if(currentWord.find('+') != string::npos){
                    size_t pos = currentWord.find('+');
                    string variableName = currentWord.substr(0, pos);
                    string assignmentOperator = "+";
                    string value = currentWord.substr(pos + 1);
                    token variableToken = {"operand", variableName, line, index};
                    token assignmentToken = {"operator", assignmentOperator, line, index};
                    token valueToken = {"operand", value, line, index};
                    tokens.push_back(variableToken);
                    tokens.push_back(assignmentToken);
                    tokens.push_back(valueToken);
                    }else if(currentWord.find('-') != string::npos){
                    size_t pos = currentWord.find('-');
                    string variableName = currentWord.substr(0, pos);
                    string assignmentOperator = "-";
                    string value = currentWord.substr(pos + 1);
                    token variableToken = {"operand", variableName, line, index};
                    token assignmentToken = {"operator", assignmentOperator, line, index};
                    token valueToken = {"operand", value, line, index};
                    tokens.push_back(variableToken);
                    tokens.push_back(assignmentToken);
                    tokens.push_back(valueToken);
                    }else if(currentWord.find('*') != string::npos){
                    size_t pos = currentWord.find('*');
                    string variableName = currentWord.substr(0, pos);
                    string assignmentOperator = "*";
                    string value = currentWord.substr(pos + 1);
                    token variableToken = {"operand", variableName, line, index};
                    token assignmentToken = {"operator", assignmentOperator, line, index};
                    token valueToken = {"operand", value, line, index};
                    tokens.push_back(variableToken);
                    tokens.push_back(assignmentToken);
                    tokens.push_back(valueToken);
                    }else if(currentWord.find('/') != string::npos){
                    size_t pos = currentWord.find('/');
                    string variableName = currentWord.substr(0, pos);
                    string assignmentOperator = "/";
                    string value = currentWord.substr(pos + 1);
                    token variableToken = {"operand", variableName, line, index};
                    token assignmentToken = {"operator", assignmentOperator, line, index};
                    token valueToken = {"operand", value, line, index};
                    tokens.push_back(variableToken);
                    tokens.push_back(assignmentToken);
                    tokens.push_back(valueToken);    
                    }
                }else if ((currentWord.find('+') != string::npos||currentWord.find('-') != string::npos||currentWord.find('*') != string::npos||currentWord.find('/') != string::npos)&&currentWord.length()==2) {
                    if(currentWord.find('+') != string::npos){
                    size_t pos = currentWord.find('+');
                    string variableName = currentWord.substr(pos+1);
                    string assignmentOperator = "+";
                    token variableToken = {"operator", assignmentOperator, line, index};
                    token assignmentToken = {"operand", variableName, line, index};
                    tokens.push_back(variableToken);
                    tokens.push_back(assignmentToken);
                    }else if(currentWord.find('-') != string::npos){
                    size_t pos = currentWord.find('-');
                    string variableName = currentWord.substr(pos+1);
                    string assignmentOperator = "-";
                    token variableToken = {"operator", assignmentOperator, line, index};
                    token assignmentToken = {"operand", variableName, line, index};
                    tokens.push_back(variableToken);
                    tokens.push_back(assignmentToken);
                    }else if(currentWord.find('*') != string::npos){
                    size_t pos = currentWord.find('*');
                    string variableName = currentWord.substr(pos+1);
                    string assignmentOperator = "*";
                    token variableToken = {"operator", assignmentOperator, line, index};
                    token assignmentToken = {"operand", variableName, line, index};
                    tokens.push_back(variableToken);
                    tokens.push_back(assignmentToken);
                    }else if(currentWord.find('/') != string::npos){
                    size_t pos = currentWord.find('/');
                    string variableName = currentWord.substr(pos+1);
                    string assignmentOperator = "/";
                    token variableToken = {"operator", assignmentOperator, line, index};
                    token assignmentToken = {"operand", variableName, line, index};
                    tokens.push_back(variableToken);
                    tokens.push_back(assignmentToken); 
                    }  
                }else if (currentWord.find('#') != string::npos) {
                    currentWord.clear();
                    break;
                    
                }else {
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
        } else if (currentWord.find('=') != string::npos) {
            size_t pos = currentWord.find('=');
            string variableName = currentWord.substr(0, pos);
            string assignmentOperator = "=";
            string value = currentWord.substr(pos + 1);

            token variableToken = {"variable", variableName, line, index};
            token assignmentToken = {"assignment", assignmentOperator, line, index};
            token valueToken = {"other", value, line, index};

            tokens.push_back(variableToken);
            tokens.push_back(assignmentToken);
            tokens.push_back(valueToken);
        }else if (currentWord.find('+') != string::npos||currentWord.find('-') != string::npos||currentWord.find('*') != string::npos||currentWord.find('/') != string::npos) {
                    if(currentWord.find('+') != string::npos){
                    size_t pos = currentWord.find('+');
                    string variableName = currentWord.substr(0, pos);
                    string assignmentOperator = "+";
                    string value = currentWord.substr(pos + 1);
                    token variableToken = {"operand", variableName, line, index};
                    token assignmentToken = {"operator", assignmentOperator, line, index};
                    token valueToken = {"operand", value, line, index};
                    tokens.push_back(variableToken);
                    tokens.push_back(assignmentToken);
                    tokens.push_back(valueToken);
                    }else if(currentWord.find('-') != string::npos){
                    size_t pos = currentWord.find('-');
                    string variableName = currentWord.substr(0, pos);
                    string assignmentOperator = "-";
                    string value = currentWord.substr(pos + 1);
                    token variableToken = {"operand", variableName, line, index};
                    token assignmentToken = {"operator", assignmentOperator, line, index};
                    token valueToken = {"operand", value, line, index};
                    tokens.push_back(variableToken);
                    tokens.push_back(assignmentToken);
                    tokens.push_back(valueToken);
                    }else if(currentWord.find('*') != string::npos){
                    size_t pos = currentWord.find('*');
                    string variableName = currentWord.substr(0, pos);
                    string assignmentOperator = "*";
                    string value = currentWord.substr(pos + 1);
                    token variableToken = {"operand", variableName, line, index};
                    token assignmentToken = {"operator", assignmentOperator, line, index};
                    token valueToken = {"operand", value, line, index};
                    tokens.push_back(variableToken);
                    tokens.push_back(assignmentToken);
                    tokens.push_back(valueToken);
                    }else if(currentWord.find('/') != string::npos){
                    size_t pos = currentWord.find('/');
                    string variableName = currentWord.substr(0, pos);
                    string assignmentOperator = "/";
                    string value = currentWord.substr(pos + 1);
                    token variableToken = {"operand", variableName, line, index};
                    token assignmentToken = {"operator", assignmentOperator, line, index};
                    token valueToken = {"operand", value, line, index};
                    tokens.push_back(variableToken);
                    tokens.push_back(assignmentToken);
                    tokens.push_back(valueToken);    
                    }
                }else if ((currentWord.find('+') != string::npos||currentWord.find('-') != string::npos||currentWord.find('*') != string::npos||currentWord.find('/') != string::npos)&&currentWord.length()==2) {
                    if(currentWord.find('+') != string::npos){
                    size_t pos = currentWord.find('+');
                    string variableName = currentWord.substr(pos+1);
                    string assignmentOperator = "+";
                    token variableToken = {"operator", assignmentOperator, line, index};
                    token assignmentToken = {"operand", variableName, line, index};
                    tokens.push_back(variableToken);
                    tokens.push_back(assignmentToken);
                    }else if(currentWord.find('-') != string::npos){
                    size_t pos = currentWord.find('-');
                    string variableName = currentWord.substr(pos+1);
                    string assignmentOperator = "-";
                    token variableToken = {"operator", assignmentOperator, line, index};
                    token assignmentToken = {"operand", variableName, line, index};
                    tokens.push_back(variableToken);
                    tokens.push_back(assignmentToken);
                    }else if(currentWord.find('*') != string::npos){
                    size_t pos = currentWord.find('*');
                    string variableName = currentWord.substr(pos+1);
                    string assignmentOperator = "*";
                    token variableToken = {"operator", assignmentOperator, line, index};
                    token assignmentToken = {"operand", variableName, line, index};
                    tokens.push_back(variableToken);
                    tokens.push_back(assignmentToken);
                    }else if(currentWord.find('/') != string::npos){
                    size_t pos = currentWord.find('/');
                    string variableName = currentWord.substr(pos+1);
                    string assignmentOperator = "/";
                    token variableToken = {"operator", assignmentOperator, line, index};
                    token assignmentToken = {"operand", variableName, line, index};
                    tokens.push_back(variableToken);
                    tokens.push_back(assignmentToken); 
                    }  
                } else {
            token newtoken = {"other", currentWord, line, index};
            tokens.push_back(newtoken);
        }
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
                if(i>=1){
                if (tokens[i - 1].type == "operator") {
                    // Treat the current keyword as an operand
                    tokens[i].type = "operand";
                }
                }
            }
            if (tokens[i].type=="other"){
                if(tokens[i].value==")"){
                    tokens[i].type="closingParenthesis";
                }
            }
            if(tokens[i].type=="other"&&tokens[i].type!="print"){
                if(tokens[i+1].value=="("){
                    tokens[i].type="function";
                }
            }
            if(tokens[i].value=="return"){
                tokens[i].type="return";
            }
            if(tokens[i].type=="operand"){
                if(i + 1 < tokens.size()&&tokens[i+1].value=="("){
                    tokens[i].type="function";
                }
            }

        }
    return tokens;
}

int main(int argc, char* argv[]){
    string line;
	ifstream inFile;
	string filename = argv[1];
	inFile.open(filename);
    vector<token> tokens;
    int lineNum = 1;
    parser *newParse = new parser;
    
    while(getline(inFile,line)){
        vector<token> tokens = lexer(line,lineNum);
        newParse->parser_construct(tokens);
        int i=0;
        /*while(i<tokens.size()){
            cout<<tokens[i].value<<" "<<tokens[i].type<<endl;
            i++;
        }*/
        
        
    }
    newParse->parser_evaluate();

    return 0;
}