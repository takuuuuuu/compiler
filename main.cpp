#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <stack>
#include <regex>
#include <fstream>

using namespace std;
enum class TokenType {
    UNKNOWN,
    MAIN_KEYWORD,
    COMMA,  // 添加逗号
    PRINTLN_INT,
    LEFT_PAREN,
    RIGHT_PAREN,
    LEFT_BRACE,
    RIGHT_BRACE,
    RETURN_KEYWORD,
    INT_KEYWORD,
    VOID_KEYWORD,  // 添加void关键字
    IDENTIFIER,
    ASSIGN_OP,
    INTEGER_LITERAL,
    PLUS_OP, MINUS_OP, MULTIPLY_OP, DIVIDE_OP, MODULO_OP,
    AND_OP, OR_OP, XOR_OP, LOGIC_OP,
    SEMICOLON, LESS_THAN, LESS_THAN_EQUAL,
    GREATER_THAN, GREATER_THAN_EQUAL, EQUAL, NOT_EQUAL,
    LOGICAL_AND_OP,  // 逻辑与
    LOGICAL_OR_OP,   // 逻辑或
    UNARY_MINUS,     // 添加一元减号
    LOGICAL_NOT,     // 添加逻辑非
    BITWISE_NOT,      // 添加按位非
	IF_KEYWORD,
	WHILE_KEYWORD,
	ELSE_KEYWORD,
	BREAK_KEYWORD,
	CONTINUE_KEYWORD
};

struct Token {
    TokenType type;
    string value;
};
// 判断是否为运算符
bool isOperator(TokenType type) {
             //type == TokenType::ARITH_OP || type == TokenType::LOGIC_OP ||
   return  type == TokenType::LESS_THAN || type == TokenType::LESS_THAN_EQUAL ||
           type == TokenType::GREATER_THAN || type == TokenType::GREATER_THAN_EQUAL ||
           type == TokenType::EQUAL || type == TokenType::NOT_EQUAL||
               //更新
           type == TokenType::PLUS_OP || type == TokenType::MINUS_OP ||
           type == TokenType::MULTIPLY_OP || type == TokenType::DIVIDE_OP ||
           type == TokenType::MODULO_OP || type == TokenType::AND_OP ||
           type == TokenType::OR_OP || type == TokenType::XOR_OP ||
           type == TokenType::LOGICAL_AND_OP || type == TokenType::LOGICAL_OR_OP||
           type == TokenType::UNARY_MINUS || type == TokenType::LOGICAL_NOT||
           type == TokenType::BITWISE_NOT;
}
vector<Token> tokenize(const string& src) {
    vector<Token> tokens;

    regex tokenPatterns(
        "\\bprintln_int\\b|\\bint\\b|\\bvoid\\b|\\breturn\\b|\\bmain\\b|&&|\\|\\||"
        "[A-Za-z_][0-9A-Za-z_]*|[0-9]+|(==)|(!=)|(<=)|(>=)|"
        "[=+\\-*/%&;<>|^~!]|,|[()]|[{}]"
    );

    auto words_begin = sregex_iterator(src.begin(), src.end(), tokenPatterns);
    auto words_end = sregex_iterator();

    for (sregex_iterator i = words_begin; i != words_end; ++i) {
        smatch match = *i;
        string matchStr = match.str();
        Token token;
        token.value = matchStr;

        if (matchStr == "println_int") token.type = TokenType::PRINTLN_INT;
        else if (matchStr == ",") token.type = TokenType::COMMA;
		else if (matchStr == "if") token.type = TokenType::IF_KEYWORD;
		else if (matchStr == "while") token.type = TokenType::WHILE_KEYWORD;
		else if (matchStr == "else") token.type = TokenType::ELSE_KEYWORD;
		else if (matchStr == "break") token.type = TokenType::BREAK_KEYWORD;
		else if (matchStr == "continue") token.type = TokenType::CONTINUE_KEYWORD;
        else if (matchStr == "void") token.type = TokenType::VOID_KEYWORD;
        else if (matchStr == "int") token.type = TokenType::INT_KEYWORD;
        else if (matchStr == "return") token.type = TokenType::RETURN_KEYWORD;
        else if (matchStr == "main") token.type = TokenType::MAIN_KEYWORD;
        else if (regex_match(matchStr, regex("[A-Za-z_][0-9A-Za-z_]*"))) token.type = TokenType::IDENTIFIER;//修改识别标识符的正则表达式
        else if (regex_match(matchStr, regex("[0-9]+"))) token.type = TokenType::INTEGER_LITERAL;
        else if (matchStr == "=") token.type = TokenType::ASSIGN_OP;
        else if (matchStr == ";") token.type = TokenType::SEMICOLON;
        else if (matchStr == "(") token.type = TokenType::LEFT_PAREN;
        else if (matchStr == ")") token.type = TokenType::RIGHT_PAREN;
        else if (matchStr == "{") token.type = TokenType::LEFT_BRACE;
        else if (matchStr == "}") token.type = TokenType::RIGHT_BRACE;
        else if (matchStr == "<") token.type = TokenType::LESS_THAN;
        else if (matchStr == "<=") token.type = TokenType::LESS_THAN_EQUAL;
        else if (matchStr == ">") token.type = TokenType::GREATER_THAN;
        else if (matchStr == ">=") token.type = TokenType::GREATER_THAN_EQUAL;
        else if (matchStr == "==") token.type = TokenType::EQUAL;
        else if (matchStr == "!=") token.type = TokenType::NOT_EQUAL;
        // 更新对应的条件分支，以识别新的运算符类型
        else if (matchStr == "+") token.type = TokenType::PLUS_OP;
        else if (matchStr == "-") token.type = TokenType::MINUS_OP;
        else if (matchStr == "*") token.type = TokenType::MULTIPLY_OP;
        else if (matchStr == "/") token.type = TokenType::DIVIDE_OP;
        else if (matchStr == "%") token.type = TokenType::MODULO_OP;
        else if (matchStr == "&") token.type = TokenType::AND_OP;
        else if (matchStr == "|") token.type = TokenType::OR_OP;
        else if (matchStr == "^") token.type = TokenType::XOR_OP;
        else if (matchStr == "&&") token.type = TokenType::LOGICAL_AND_OP;
        else if (matchStr == "||") token.type = TokenType::LOGICAL_OR_OP;
        else if (matchStr == "-") {
        if (tokens.back().type == TokenType::LEFT_PAREN ||
            tokens.back().type == TokenType::ASSIGN_OP ||
            tokens.back().type == TokenType::COMMA ||
            isOperator(tokens.back().type)) {
            token.type = TokenType::UNARY_MINUS;
        } else {token.type = TokenType::MINUS_OP;}
      } else if (matchStr == "!") {token.type = TokenType::LOGICAL_NOT;}
        else if (matchStr == "~") {token.type = TokenType::BITWISE_NOT;}
        else token.type = TokenType::UNKNOWN;

        tokens.push_back(token);
    }

    return tokens;
}


// 运算符优先级
int getPrecedence(TokenType type) {
    switch (type) {
        case TokenType::UNARY_MINUS:
        case TokenType::LOGICAL_NOT:
        case TokenType::BITWISE_NOT:
            return 8;
        case TokenType::MULTIPLY_OP:
        case TokenType::DIVIDE_OP:
        case TokenType::MODULO_OP:
            return 7;
        case TokenType::PLUS_OP:
        case TokenType::MINUS_OP:
            return 6;
        case TokenType::LESS_THAN:
        case TokenType::LESS_THAN_EQUAL:
        case TokenType::GREATER_THAN:
        case TokenType::GREATER_THAN_EQUAL:
            return 5;
        case TokenType::EQUAL:
        case TokenType::NOT_EQUAL:
            return 4;
        case TokenType::AND_OP:
            return 3;
        case TokenType::XOR_OP:
        case TokenType::LOGICAL_AND_OP:
            return 2;
        case TokenType::OR_OP:
        case TokenType::LOGICAL_OR_OP:
            return 1;
        default:
            return 0; // 对于未知类型或不适用的类型，返回最低优先级
    }
}



// 将中缀表达式转换为后缀表达式
vector<Token> infixToPostfix(const vector<Token>& infix) {
    stack<Token> opStack;
    vector<Token> postfix;
    for (const Token& token : infix) {
        if (token.type == TokenType::IDENTIFIER || token.type == TokenType::INTEGER_LITERAL) {
            postfix.push_back(token);
        } else if (token.type == TokenType::LEFT_PAREN) {
            opStack.push(token);
        } else if (token.type == TokenType::RIGHT_PAREN) {
            while (!opStack.empty() && opStack.top().type != TokenType::LEFT_PAREN) {
                postfix.push_back(opStack.top());
                opStack.pop();
            }
            if (!opStack.empty()) opStack.pop(); // 弹出左括号
        } else if (isOperator(token.type)) {
            while (!opStack.empty() && getPrecedence(opStack.top().type) >= getPrecedence(token.type)) {
                postfix.push_back(opStack.top());
                opStack.pop();
            }
            opStack.push(token);
        }
    }
    while (!opStack.empty()) {
        postfix.push_back(opStack.top());
        opStack.pop();
    }
        cout << "# Postfix Expression: ";
    for (const Token& token : postfix) {
        cout << token.value << " ";
    }
    cout << endl;
    return postfix;
}


// 根据后缀表达式生成MIPS代码
void generateMIPSFromPostfix(const vector<Token>& postfix, const map<string, int>& varMap, const string& resultVar) {
    // Unique label index for generating unique labels for branching
    int labelIndex = 0;
    auto generateLabel = [&labelIndex](const string& base) {
        return base + "_" + to_string(labelIndex++);
    };

    for (const Token& token : postfix) {
        if (token.type == TokenType::IDENTIFIER) {
            if (varMap.find(token.value) != varMap.end()) {
                int offset = varMap.at(token.value);
                cout << "    lw $t0, " << offset << "($fp)" << endl;
                cout << "    sw $t0, 0($sp)" << endl;
                cout << "    addiu $sp, $sp, -4" << endl;
            } else {
                cerr << "#Error: Variable '" << token.value << "' not found in the varMap." << endl;
            }
        } else if (token.type == TokenType::INTEGER_LITERAL) {
            cout << "    li $t0, " << token.value << endl;
            cout << "    sw $t0, 0($sp)" << endl;
            cout << "    addiu $sp, $sp, -4" << endl;
        }
        else if(token.type==TokenType::UNARY_MINUS||token.type==TokenType::LOGICAL_NOT||token.type==TokenType::BITWISE_NOT)
        {
        
        switch (token.type) {
         case TokenType::UNARY_MINUS: {
                        cout << "    lw $t1, 4($sp)" << endl;
                        
                        cout << "    neg $t0, $t1" << endl;
                        cout << "    sw $t0, 4($sp)" << endl;
                    }
                    break;
                case TokenType::LOGICAL_NOT:{
                        cout << "    lw $t1, 4($sp)" << endl;
                        
                        cout << "    seq $t0, $t1, $zero" << endl;
                        cout << "    sw $t0, 4($sp)" << endl;
                    }
                    break;
                case TokenType::BITWISE_NOT:{
                        cout << "    lw $t1, 4($sp)" << endl;
              
                        cout << "    not $t0, $t1" << endl;
                        cout << "    sw $t0, 4($sp)" << endl;
                    }
                    break;
        }
           
         }
        else if (isOperator(token.type)) {
            cout << "    lw $t1, 8($sp)" << endl; // Pop first operand
            cout << "    lw $t2, 4($sp)" << endl; // Pop second operand
            cout << "    addiu $sp, $sp, 8" << endl; // Free space on the stack

            switch (token.type) {
                case TokenType::MULTIPLY_OP:
                    cout << "    mul $t0, $t1, $t2" << endl;
                    break;
                case TokenType::DIVIDE_OP:
                    cout << "    div $t1, $t2" << endl;
                    cout << "    mflo $t0" << endl;
                    break;
                case TokenType::MODULO_OP:
                    cout << "    div $t1, $t2" << endl;
                    cout << "    mfhi $t0" << endl;
                    break;
                case TokenType::PLUS_OP:
                    cout << "    add $t0, $t1, $t2" << endl;
                    break;
                case TokenType::MINUS_OP:
                    cout << "    sub $t0, $t1, $t2" << endl;
                    break;
                case TokenType::LESS_THAN:
                    cout << "    slt $t0, $t1, $t2" << endl;
                    break;
                case TokenType::LESS_THAN_EQUAL:
                    cout << "    slt $t0, $t2, $t1" << endl;
                    cout << "    xori $t0, $t0, 1" << endl;
                    break;
                case TokenType::GREATER_THAN:
                    cout << "    slt $t0, $t2, $t1" << endl;
                    break;
                case TokenType::GREATER_THAN_EQUAL:
                    cout << "    slt $t0, $t1, $t2" << endl;
                    cout << "    xori $t0, $t0, 1" << endl;
                    break;
                case TokenType::EQUAL:
                    cout << "    seq $t0, $t1, $t2" << endl;
                    break;
                case TokenType::NOT_EQUAL:
                    cout << "    sne $t0, $t1, $t2" << endl;
                    break;
                case TokenType::AND_OP:
                    cout << "    and $t0, $t1, $t2" << endl;
                    break;
                case TokenType::OR_OP:
                    cout << "    or $t0, $t1, $t2" << endl;
                    break;
                case TokenType::XOR_OP:
                    cout << "    xor $t0, $t1, $t2" << endl;
                    break;
                case TokenType::LOGICAL_AND_OP:
                    // Implementing short-circuit logic for logical AND
                    {
                        cout << "Processing token: " << token.value << " Type: " << static_cast<int>(token.type) << endl;
                        string labelFalse = generateLabel("and_false");
                        string labelEnd = generateLabel("and_end");
                        cout << "    beq $t1, $zero, " << labelFalse << endl;
                        cout << "    beq $t2, $zero, " << labelFalse << endl;
                        cout << "    li $t0, 1" << endl;
                        cout << "    j " << labelEnd << endl;
                        cout << labelFalse << ":" << endl;
                        cout << "    li $t0, 0" << endl;
                        cout << labelEnd << ":" << endl;
                    }
                    break;
                case TokenType::LOGICAL_OR_OP:
                    // Implementing short-circuit logic for logical OR
                    {
                        cout << "Processing token: " << token.value << " Type: " << static_cast<int>(token.type) << endl;
                        string labelTrue = generateLabel("or_true");
                        string labelEnd = generateLabel("or_end");
                        cout << "    bne $t1, $zero, " << labelTrue << endl;
                        cout << "    bne $t2, $zero, " << labelTrue << endl;
                        cout << "    li $t0, 0" << endl;
                        cout << "    j " << labelEnd << endl;
                        cout << labelTrue << ":" << endl;
                        cout << "    li $t0, 1" << endl;
                        cout << labelEnd << ":" << endl;
                    }
                    break;
               
                default:
                    break;
            }
            cout << "    sw $t0, 0($sp)" << endl;
            cout << "    addiu $sp, $sp, -4" << endl;
        }
    }

    // Pop the result back into the designated variable
    cout << "    lw $t0, 4($sp)" << endl;
    cout << "    sw $t0, " << varMap.at(resultVar) << "($fp)" << endl;
    cout << "    addiu $sp, $sp, 4" << endl;
}


void generateMIPSFromPostfix1(const vector<Token>& postfix, const map<string, int>& varMap) {
    // Unique label index for generating unique labels for branching
    int labelIndex = 0;
    auto generateLabel = [&labelIndex](const string& base) {
        return base + "_" + to_string(labelIndex++);
    };

    for (const Token& token : postfix) {
        if (token.type == TokenType::IDENTIFIER) {
            if (varMap.find(token.value) != varMap.end()) {
                int offset = varMap.at(token.value);
                cout << "    lw $t0, " << offset << "($fp)" << endl;
                cout << "    sw $t0, 0($sp)" << endl;
                cout << "    addiu $sp, $sp, -4" << endl;
            } else {
                cerr << "Error: Variable '" << token.value << "' not found in the varMap." << endl;
            }
        } else if (token.type == TokenType::INTEGER_LITERAL) {
            cout << "    li $t0, " << token.value << endl;
            cout << "    sw $t0, 0($sp)" << endl;
            cout << "    addiu $sp, $sp, -4" << endl;
        } else if (isOperator(token.type)) {
            cout << "    lw $t1, 8($sp)" << endl; // Pop first operand
            cout << "    lw $t2, 4($sp)" << endl; // Pop second operand
            cout << "    addiu $sp, $sp, 8" << endl; // Free space on the stack

            switch (token.type) {
                case TokenType::MULTIPLY_OP:
                    cout << "    mul $t0, $t1, $t2" << endl;
                    break;
                case TokenType::DIVIDE_OP:
                    cout << "    div $t1, $t2" << endl;
                    cout << "    mflo $t0" << endl;
                    break;
                case TokenType::MODULO_OP:
                    cout << "    div $t1, $t2" << endl;
                    cout << "    mfhi $t0" << endl;
                    break;
                case TokenType::PLUS_OP:
                    cout << "    add $t0, $t1, $t2" << endl;
                    break;
                case TokenType::MINUS_OP:
                    cout << "    sub $t0, $t1, $t2" << endl;
                    break;
                case TokenType::LESS_THAN:
                    cout << "    slt $t0, $t1, $t2" << endl;
                    break;
                case TokenType::LESS_THAN_EQUAL:
                    cout << "    slt $t0, $t2, $t1" << endl;
                    cout << "    xori $t0, $t0, 1" << endl;
                    break;
                case TokenType::GREATER_THAN:
                    cout << "    slt $t0, $t2, $t1" << endl;
                    break;
                case TokenType::GREATER_THAN_EQUAL:
                    cout << "    slt $t0, $t1, $t2" << endl;
                    cout << "    xori $t0, $t0, 1" << endl;
                    break;
                case TokenType::EQUAL:
                    cout << "    seq $t0, $t1, $t2" << endl;
                    break;
                case TokenType::NOT_EQUAL:
                    cout << "    sne $t0, $t1, $t2" << endl;
                    break;
                case TokenType::AND_OP:
                    cout << "    and $t0, $t1, $t2" << endl;
                    break;
                case TokenType::OR_OP:
                    cout << "    or $t0, $t1, $t2" << endl;
                    break;
                case TokenType::XOR_OP:
                    cout << "    xor $t0, $t1, $t2" << endl;
                    break;
                case TokenType::LOGICAL_AND_OP:
                    // Implementing short-circuit logic for logical AND
                    {
                        cout << "Processing token: " << token.value << " Type: " << static_cast<int>(token.type) << endl;
                        string labelFalse = generateLabel("and_false");
                        string labelEnd = generateLabel("and_end");
                        cout << "    beq $t1, $zero, " << labelFalse << endl;
                        cout << "    beq $t2, $zero, " << labelFalse << endl;
                        cout << "    li $t0, 1" << endl;
                        cout << "    j " << labelEnd << endl;
                        cout << labelFalse << ":" << endl;
                        cout << "    li $t0, 0" << endl;
                        cout << labelEnd << ":" << endl;
                    }
                    break;
                case TokenType::LOGICAL_OR_OP:
                    // Implementing short-circuit logic for logical OR
                    {
                        cout << "Processing token: " << token.value << " Type: " << static_cast<int>(token.type) << endl;
                        string labelTrue = generateLabel("or_true");
                        string labelEnd = generateLabel("or_end");
                        cout << "    bne $t1, $zero, " << labelTrue << endl;
                        cout << "    bne $t2, $zero, " << labelTrue << endl;
                        cout << "    li $t0, 0" << endl;
                        cout << "    j " << labelEnd << endl;
                        cout << labelTrue << ":" << endl;
                        cout << "    li $t0, 1" << endl;
                        cout << labelEnd << ":" << endl;
                    }
                    break;
                case TokenType::UNARY_MINUS: {
                        cout << "    lw $t1, 4($sp)" << endl;
                        cout << "    neg $t0, $t1" << endl;
                        cout << "    sw $t0, 4($sp)" << endl;
                    }
                    break;
                case TokenType::LOGICAL_NOT:{
                        cout << "    lw $t1, 4($sp)" << endl;
                        cout << "    seq $t0, $t1, $zero" << endl;
                        cout << "    sw $t0, 4($sp)" << endl;
                    }
                    break;
                case TokenType::BITWISE_NOT:{
                        cout << "    lw $t1, 4($sp)" << endl;
                        cout << "    not $t0, $t1" << endl;
                        cout << "    sw $t0, 4($sp)" << endl;
                    }
                    break;
                default:
                    break;
            }
            cout << "    sw $t0, 0($sp)" << endl;
            cout << "    addiu $sp, $sp, -4" << endl;
        }
    }

}

// 完整的generateMIPS函数（包含中缀到后缀的转换和后缀表达式求值）
void generateMIPS(const vector<Token>& tokens) {
    map<string, int> varMap;
    map<string, int> funcvarMap;
    int variableOffset_canshu = 8;
    int canshucount = -1; //canshugeshu
    int variableOffset = -4; // 假设栈帧从$fp向下增长
    vector<Token> expression; // 用于收集变量赋值的表达式
    int flag=1;
    int argv=0;
    bool infunc=false;
    bool invoid=false;
	int ifcount=1;
	bool inif=false;
	bool inelse=false;
	int whilecount=1;
	bool inwhile=false;
	int countwhile=1;
    
    //框架
    cout << ".data" << endl;
    cout << "newline: .asciiz \"\\n\"" << endl;
    cout << ".text" << endl;
    cout << ".globl main" << endl;
    
    for (size_t i = 0; i < tokens.size(); ++i) {
        
        //处理main函数
        //varMap.clear();
        if(tokens[i].type == TokenType::INT_KEYWORD && tokens[i+1].type == TokenType::MAIN_KEYWORD && tokens[i+2].type == TokenType::LEFT_PAREN && tokens[i+3].type == TokenType::RIGHT_PAREN){
                    cout << "main:" << endl;
                    cout << "move $fp, $sp # 设置帧指针" << endl;
                    cout << "addiu $sp, $sp, -0x100 # 为局部变量分配栈空间" << endl;
                    i += 4;
                    continue;
                } else if(tokens[i].type == TokenType::INT_KEYWORD && tokens[i+1].type == TokenType::MAIN_KEYWORD && tokens[i+2].type == TokenType::LEFT_PAREN && tokens[i+3].type == TokenType::INT_KEYWORD){
                    cout << "main:" << endl;
                    cout << "move $fp, $sp # 设置帧指针" << endl;
                    cout << "addiu $sp, $sp, -0x100 # 为局部变量分配栈空间" << endl;
                    i += 8;
                    argv = 1;
                    continue;
                }
        
        else if ((tokens[i].type == TokenType::INT_KEYWORD || tokens[i].type == TokenType::VOID_KEYWORD) && tokens[i+1].type == TokenType::IDENTIFIER && tokens[i+2].type == TokenType::LEFT_PAREN) {
            // 处理函数声明
            infunc=true;
            if(tokens[i].type == TokenType::VOID_KEYWORD)
            {
            infunc = false;
            invoid = true;
            }
            string functionName = tokens[i+1].value;
            cout << functionName << ":" << endl; // 添加函数标签
            cout << "    addiu $sp, $sp, -4 # 为保存寄存器分配空间" << endl;
            cout << "    sw $ra, 0($sp) # 保存 ra" << endl;
            cout << "    sw $fp, 4($sp) # 保存 fp" << endl;
            cout << "    move $fp, $sp # 设置 fp, 便于访问局部变量" << endl;
            cout << "     addiu $sp, $sp, -8 # 为局部变量分配栈空间" << endl;
            i+=3;
            //int func(){
            if(tokens[i].type==TokenType::RIGHT_PAREN) {
                i+=1;
                continue;}
            // int func(int a) {}
        int paramOffset = 8;
        int count = 0;
        while(tokens[i].type != TokenType::RIGHT_PAREN) {
            if(tokens[i].type == TokenType::INT_KEYWORD) {
                canshucount++;
            }
            i++;
            count++;
        }
        i = i - count;
        while(tokens[i].type != TokenType::RIGHT_PAREN) {
            if(tokens[i].type == TokenType::INT_KEYWORD) {
                string funcvarName = tokens[i+1].value;
                cout << "    lw $v0, " << paramOffset + canshucount*4 << "($fp) # int " << funcvarName << endl;
                varMap[funcvarName] = paramOffset+canshucount*4;
                canshucount--;
            }
            i++;
        }
            i++;
            
        continue;
        }
    
    else if(tokens[i].type == TokenType::RIGHT_BRACE && invoid )
    {
    invoid = false;
    cout<<" move $sp, $fp # 清理局部变量"<<endl;
            
            cout<<" lw $fp, 4($sp) # 恢复 fp"<<endl;

            cout<<" lw $ra, 0($sp) # 恢复 ra"<<endl;

     cout<<" addiu $sp, $sp, 4 # 清理为保存寄存器分配的空间，恢复栈指针 "<<endl;

            cout<<"     jr $ra # 返回"<<endl;
    }
//a = F(a+b,c)
else if (tokens[i].type == TokenType::IDENTIFIER && tokens[i+1].type == TokenType::ASSIGN_OP && tokens[i+2].type == TokenType::IDENTIFIER &&
                 tokens[i+3].type == TokenType::LEFT_PAREN) {
    string varName1 = tokens[i].value; // 获取赋值的变量名
    
    string funcName = tokens[i+2].value; // 获取函数名
    i += 4; // 跳过赋值符号和函数名

    int argCount = 0;
    vector<Token> expression; // 用于存储每个参数的表达式

    while (tokens[i].type != TokenType::RIGHT_PAREN) {
        while (tokens[i].type != TokenType::COMMA && tokens[i].type != TokenType::RIGHT_PAREN) {
            expression.push_back(tokens[i]);
            ++i;
        }

        // 将参数表达式转换为后缀表达式
        vector<Token> postfix = infixToPostfix(expression);

        // 生成MIPS代码
        generateMIPSFromPostfix1(postfix, varMap);

        expression.clear(); // 清空表达式以便下次使用
        ++argCount;

        if (tokens[i].type == TokenType::COMMA) {
            ++i; // 跳过逗号
        }
    }

    // 调用函数
    cout << "    jal " << funcName << endl;

    // 清理栈
    cout << "    addiu $sp, $sp, " << -4 * argCount << " # 清理栈" << endl;

    // 将返回值存入局部变量
    cout << "    sw $v0, "<<varMap[varName1]<<"($fp) # 将返回值存入局部变量" << endl;

    continue; // 跳过整个函数调用语句
}

//func(a+b,c)
    else if (tokens[i].type == TokenType::IDENTIFIER &&tokens[i+1].type == TokenType::LEFT_PAREN) {
    
    //string varName1 = tokens[i].value; // 获取赋值的变量名
    string funcName = tokens[i].value; // 获取函数名
    i += 2; // 跳过赋值符号和函数名
    int argCount = 0;
    vector<Token> expression; // 用于存储每个参数的表达式

    while (tokens[i].type != TokenType::RIGHT_PAREN) {
        while (tokens[i].type != TokenType::COMMA && tokens[i].type != TokenType::RIGHT_PAREN) {
            expression.push_back(tokens[i]);
            ++i;
        }
        // 将参数表达式转换为后缀表达式
        vector<Token> postfix = infixToPostfix(expression);
        // 生成MIPS代码
        generateMIPSFromPostfix1(postfix, varMap);
        expression.clear(); // 清空表达式以便下次使用
        ++argCount;
        if (tokens[i].type == TokenType::COMMA) {
            ++i; // 跳过逗号
        }
    }
    // 调用函数
    cout << "    jal " << funcName << endl;
    // 清理栈
    cout << "    addiu $sp, $sp, " << -4 * argCount << " # 清理栈" << endl;
    // 将返回值存入局部变量
    //cout << "    sw $v0, "<<varMap[varName1]<<"($fp) # 将返回值存入局部变量" << endl;
    continue; // 跳过整个函数调用语句
}
    

        //int
        else if (tokens[i].type == TokenType::INT_KEYWORD) {

            i++; // 跳过 'int'

            while (tokens[i].type != TokenType::SEMICOLON) {

                string varName;

                if (tokens[i].type == TokenType::IDENTIFIER) {

                    varName = tokens[i].value;

                    if (varMap.find(varName) == varMap.end()) {

                        varMap[varName] = variableOffset;

                    } else {

                        cerr << "#Variable '" << varName << "' redeclared." << endl;

                    }

                    i++; // 跳过变量名



                    // 检查是否有初始化表达式

                    if (tokens[i].type == TokenType::ASSIGN_OP) {

                        i++; // 跳过 '='

                        // 收集初始化表达式直到遇到逗号或分号

                        while (tokens[i].type != TokenType::COMMA && tokens[i].type != TokenType::SEMICOLON) {

                            expression.push_back(tokens[i]);

                            i++;

                        }

                        // 计算并赋值

                        vector<Token> postfix = infixToPostfix(expression);

                        generateMIPSFromPostfix(postfix, varMap, varName);

                        expression.clear();

                    }



                    variableOffset -= 4; // 为下一个变量预留空间

                }

                if (tokens[i].type == TokenType::COMMA) i++; // 跳过逗号，继续下一个变量

            }

        }
        


        //a =
        else if (tokens[i].type == TokenType::IDENTIFIER && tokens[i+1].type == TokenType::ASSIGN_OP) {
        
        
        if(tokens[i+2].type == TokenType::INTEGER_LITERAL && tokens[i+3].type == TokenType::SEMICOLON)
        {
            string varName = tokens[i].value;
                i += 2; // 跳过标识符和赋值操作符
                cout<<"li $t0, "<<tokens[i].value<<"# 局部变量"<<endl;
                cout<<"sw $t0, "<<varMap[tokens[i-2].value]<<"($fp)"<<"# 局部变量"<<endl;
                
        }
        else{
    // 开始收集赋值表达式
    cout << "    # 开始处理表达式 " << tokens[i].value << endl;
    string varName = tokens[i].value;
    i += 2; // 跳过标识符和赋值操作符

    while (i < tokens.size() && tokens[i].type != TokenType::SEMICOLON) {
        expression.push_back(tokens[i]);
        ++i;
    }
    // 将中缀表达式转换为后缀表达式
    vector<Token> postfix = infixToPostfix(expression);
    // 根据后缀表达式生成MIPS代码
    generateMIPSFromPostfix(postfix, varMap, varName);
    expression.clear(); // 清空表达式以便下次使用
}
}

        //处理printf函数和return语句
       
           
        else if (tokens[i].type == TokenType::PRINTLN_INT){
        
        if (tokens[i+2].type == TokenType::IDENTIFIER && tokens[i+3].value == ")") {
            cout << "    lw $a0, " << varMap[tokens[i+2].value] << "($fp)" << endl;
        }
        else{
        
         //if:
         i += 2; // 跳过println_int和左括号
            while (tokens[i].type != TokenType::RIGHT_PAREN) {
                expression.push_back(tokens[i]);
                i++;
            }
            vector<Token> postfix = infixToPostfix(expression);
            generateMIPSFromPostfix1(postfix, varMap);
			expression.clear();
            cout << "    move $a0, $t0" << endl;
         i = i - 4;
        }
        
        cout << "    li $v0, 1" << endl;
        cout << "    syscall" << endl;
        if (flag != 0) {
            cout << "    li $v0, 4" << endl;
            cout << "    la $a0, newline" << endl;
            cout << "    syscall" << endl;
        }
        flag++;
        i += 4;
        continue;
        }
		
		//if语句
		else if(tokens[i].type == TokenType::IF_KEYWORD&&tokens[i+1].type == TokenType::LEFT_PAREN){
			i += 2; // 跳过if和左括号
			while (tokens[i].type != TokenType::RIGHT_PAREN) {
				expression.push_back(tokens[i]);
				i++;
			}
			vector<Token> postfix = infixToPostfix(expression);
			generateMIPSFromPostfix1(postfix, varMap);
			expression.clear();


			cout<<"beq $t0,$zero,$if_else_"<<ifcount<<endl;
			ifcount++;
			i++;//right paren
			inif=true;
			continue;
		}
		//处理“}”
		else if(tokens[i].type == TokenType::RIGHT_BRACE){
			if(inif&&tokens[i+1].type!=TokenType::ELSE_KEYWORD&&(!inelse)){
			cout<<"j $if_end_"<<ifcount-1<<endl;
			cout<<"$if_else_"<<ifcount-1<<": "<<endl;
			cout<<"$if_end_"<<ifcount-1<<": "<<endl;
			inif=false;
			continue;}
			else if(inif&&tokens[i+1].type==TokenType::ELSE_KEYWORD){
				cout<<"j $if_end_"<<ifcount-1<<endl;
				inif=false;
				continue;
			}

			else if(inelse){
				cout<<"$if_end_"<<ifcount-1<<": "<<endl;
				inelse=false;
				continue;
			}
			else if(inwhile&&(!inif)&&(!inelse)&&(countwhile+1<=whilecount)){
				if(whilecount==2){
				cout<<"j $while_con_"<<whilecount-1<<endl;
				cout<<"$while_end_"<<whilecount-1<<": "<<endl;
				inwhile=false;
				continue;}
				else{
					cout<<"j $while_con_"<<whilecount-countwhile<<endl;
					cout<<"$while_end_"<<whilecount-countwhile<<": "<<endl;
					//inwhile=false;
					countwhile++;
					continue;
				}
			}

		}

		else if(tokens[i].type == TokenType::ELSE_KEYWORD){
			cout<<"$if_else_"<<ifcount-1<<": "<<endl;
			i+=1;
			inelse=true;
			continue;
		}
        //break
		else if(tokens[i].type == TokenType::BREAK_KEYWORD){
			cout<<"j $while_end_"<<whilecount-countwhile<<endl;
		}
		//continue
		else if(tokens[i].type == TokenType::CONTINUE_KEYWORD){
			cout<<"j $while_con_"<<whilecount-countwhile<<endl;
		}


		//while语句
		else if(tokens[i].type==TokenType::WHILE_KEYWORD&&tokens[i+1].type==TokenType::LEFT_PAREN){
			cout<<"$while_con_"<<whilecount<<": "<<endl;
			i+=2;
			while (tokens[i].type != TokenType::RIGHT_PAREN) {
				expression.push_back(tokens[i]);
				i++;
			}
			vector<Token> postfix = infixToPostfix(expression);
			generateMIPSFromPostfix1(postfix, varMap);
			expression.clear();
			//不满足就跳到while结束的位置
			cout<<"beq $t0,$zero,$while_end_"<<whilecount<<endl;
			whilecount++;
			inwhile=true;
			i++;
			continue;

		}
		
        else if (tokens[i].type == TokenType::RETURN_KEYWORD) {
            //函数中的return
            
            if(infunc){

      

            infunc=false;

            

            if(tokens[i+2].type != TokenType::SEMICOLON ){

                

      i += 1; // 跳过return

            

          while (i < tokens.size() && tokens[i].type != TokenType::SEMICOLON) {

           expression.push_back(tokens[i]);

           ++i;

       }

       // 将中缀表达式转换为后缀表达式

       vector<Token> postfix = infixToPostfix(expression);

       // 根据后缀表达式生成MIPS代码

       generateMIPSFromPostfix1(postfix, varMap);

       expression.clear(); // 清空表达式以便下次使用

            cout << "   move $v0, $t0" << endl;
            
            cout<<" move $sp, $fp # 清理局部变量"<<endl;
            
            cout<<" lw $fp, 4($sp) # 恢复 fp"<<endl;

            cout<<" lw $ra, 0($sp) # 恢复 ra"<<endl;

     cout<<" addiu $sp, $sp, 4 # 清理为保存寄存器分配的空间，恢复栈指针 "<<endl;

            cout<<"     jr $ra # 返回"<<endl;
            }

            else{

            cout << "   lw $v0, " << varMap[tokens[i+1].value] << "($fp)" << endl;

            cout<<" move $sp, $fp # 清理局部变量"<<endl;

            cout<<" lw $fp, 4($sp) # 恢复 fp"<<endl;

            cout<<" lw $ra, 0($sp) # 恢复 ra"<<endl;

     cout<<" addiu $sp, $sp, 4 # 清理为保存寄存器分配的空间，恢复栈指针 "<<endl;

            cout<<"     jr $ra # 返回"<<endl;

            }

            continue;

            }
            
            
            
            //main函数中的return
            if(tokens[i+1].type == TokenType::IDENTIFIER && (!infunc)){
            cout << "    lw $v0, " << varMap[tokens[i+1].value] << "($fp)" << endl;
            break;}
            
            else if(tokens[i+1].type == TokenType::INTEGER_LITERAL && (!infunc)){
                cout<<"    li $v0, 0"<<"\n";
                cout<<"    li $v0, 10"<<"\n";
                cout<<"    syscall"<<"\n";
                break;
            }
        }
        
        
    }
}


int main(int argc, char* argv[]) {
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <source_file>" << endl;
        return 1;
    }

    ifstream file(argv[1]);
    if (!file.is_open()) {
        cerr << "Error opening file: " << argv[1] << endl;
        return 1;
    }

    stringstream buffer;
    buffer << file.rdbuf();
    string src = buffer.str();
    file.close();

    vector<Token> tokens = tokenize(src);
    
    generateMIPS(tokens);

    return 0;
}
