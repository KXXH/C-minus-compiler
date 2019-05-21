#include"Syntax_Analyzer.h"
#include"util.h"
#include<iostream>
using namespace std;
TreeNode* Syntax_Analyzer::getSyntaxTree(){
    program();
    return root;
}

TockenRecord Syntax_Analyzer::getCurrentToken(){
    if(buffer.empty()){
        return currentToken;
    }
    else{
        return buffer.front();
    }
}

void Syntax_Analyzer::getNextToken(){
    TockenRecord token = lexiacl_analyzer.get_next_tocken();
    currentToken=token;
}

bool Syntax_Analyzer::isMulop(){
    if(is_match("*")||is_match("/")){
        return true;
    }
    else{
        return false;
    }
}

bool Syntax_Analyzer::isAddop(){
    if(is_match("+")||is_match("-")){
        return true;
    }
    else{
        return false;
    }
}

bool Syntax_Analyzer::isRelop(){
    if(is_match("==")||is_match("<=")||is_match("<")||is_match(">")||is_match(">=")||is_match("!=")){
        return true;
    }
    else{
        return false;
    }
}

//智能创建孩子节点，以当前节点为父节点，创建孩子节点
void Syntax_Analyzer::createChildNode(sa::Syntax_type type){
    if(currentNode->leftChild==NULL){
        currentNode->leftChild=new TreeNode;
        currentNode->leftChild->parent=currentNode;
        currentNode=currentNode->leftChild;
        currentNode->type=type;
    }
    else if(currentNode->leftChild->rightSibling==NULL){
        currentNode->leftChild->rightSibling=new TreeNode;
        currentNode->leftChild->rightSibling->parent=currentNode;
        currentNode=currentNode->leftChild->rightSibling;
        currentNode->type=type;
    }
    else{
        TreeNode* p=currentNode->leftChild->rightSibling;
        while(p->rightSibling!=NULL){
            p=p->rightSibling;
        }
        p->rightSibling=new TreeNode;
        p->rightSibling->parent=currentNode;
        currentNode=p->rightSibling;
        currentNode->type=type;
    }
}

void Syntax_Analyzer::createSiblingNode(sa::Syntax_type type){
    currentNode->rightSibling=new TreeNode;
    currentNode->rightSibling->parent=currentNode->parent;
    currentNode=currentNode->rightSibling;
    currentNode->type=type;
}

void Syntax_Analyzer::returnToParentNode(){
    currentNode=currentNode->parent;
}

bool Syntax_Analyzer::isType(){
    return (is_match("int")||is_match("char")||is_match("float")||is_match("string")||is_match("void"));
}

bool Syntax_Analyzer::isStatement(){
    if(getCurrentToken().tockenType==la::ID){
        return true;
    }
    else if(getCurrentToken().tockenType==la::OPERATOR&&getCurrentToken().stringVal=="("){
        return true;
    }
    else if(getCurrentToken().tockenType==la::CONST){
        return true;
    }
    else if(getCurrentToken().tockenType==la::RESERVED){
        if(getCurrentToken().stringVal=="while"||getCurrentToken().stringVal=="for"||getCurrentToken().stringVal=="if"||getCurrentToken().stringVal=="return")
            return true;
    }
    else if(getCurrentToken().tockenType==la::SEPARATE&&getCurrentToken().stringVal=="{"){
        return true;
    }
    else{
        return false;
    }
}

void Syntax_Analyzer::error(){
    cout<<endl<<"error!"<<endl;
    cout<<"current token is "<<currentToken.stringVal<<endl;
    cout<<"current node is "<<currentNode->type<<endl;
    errorFlag=true;
    getNextToken();
}



void Syntax_Analyzer::program(){
    createChildNode(sa::PROGRAM);
    //createChildNode(DECLARATION_LIST);
    declaration_list();
    returnToParentNode();
}

void Syntax_Analyzer::declaration_list(){
    createChildNode(sa::DECLARATION_LIST);
    //createChildNode(DECLARATION);//创建声明子节点，当前节点为DECLARATION
    declaration();
    //returnToParentNode();//当前节点为DECLARATION_LIST
    while(isType()){
        //createChildNode(DECLARATION);//创建声明子节点，当前节点为DECLARATION
        declaration();
        //returnToParentNode();//当前节点为DECLARATION_LIST
    }
    returnToParentNode();
}

void Syntax_Analyzer::declaration(){
    if(isType()){
        createChildNode(sa::VAR_DECLARATION);//建立声明子节点(当前节点为VAR_DECLARATION)
        //createChildNode(TYPE_SPECIFIER);//建立类型规范子节点(当前节点为TYPE_SPECIFIER)
        type_specifier();//call
        //returnToParentNode();//当前节点为VAR_DECLARATION
        
        createChildNode(sa::ID);//创建ID节点
        currentNode->value=(getCurrentToken()).stringVal;
        match(la::ID);//match
        returnToParentNode();//当前节点为VAR_DECLARATION
        //returnToParentNode();
        
       //var();
        if(is_match("(")){
            currentNode->type=sa::FUN_DECLARATION;
            match("(");
            //createChildNode(PARAMS);
            params();
            //returnToParentNode();
            match(")");
            compound_stmt();
            //returnToParentNode();
        }
        else{
            if(is_match(";")){
                currentNode->type=sa::VAR_DECLARATION;
                //match(";");
            }
            else if(is_match("[")){
                currentNode->type=sa::VAR_DECLARATION;
                match("[");
                createChildNode(NUM);
                currentNode->value=(getCurrentToken()).stringVal;
                match(la::INT);
                returnToParentNode();
                match("]");
            }
            if(is_match("=")){
                match("=");
                expression();
            }
            while(is_match(",")){
                match(",");
                //var();
                createChildNode(sa::ID);//创建ID节点
                currentNode->value=(getCurrentToken()).stringVal;
                match(la::ID);//match
                returnToParentNode();//当前节点为VAR_DECLARATION
                if(is_match("=")){
                    match("=");
                    expression();
                }
            }
            match(";");
        }
        
        returnToParentNode();
        
    }
    //currentNode=currentNode->parent;
    
}

void Syntax_Analyzer::type_specifier(){
    createChildNode(sa::TYPE_SPECIFIER);
    currentNode->value=getCurrentToken().stringVal;
    if(is_match("int")){
        match("int");
    }
    else if(is_match("float")){
        match("float");
    }
    else if(is_match("string")){
        match("string");
    }
    else if(is_match("char")){
        match("char");
    }
    else if(is_match("void")){
        match("void");
    }
    else{
        error();
    }
    returnToParentNode();
}

void Syntax_Analyzer::params(){
    createChildNode(sa::PARAMS);
    if(is_match("void")){
        match("void");
        currentNode->value="void";
    }
    else{
        param_list();
    }
    returnToParentNode();
}



void Syntax_Analyzer::param_list(){
    createChildNode(sa::PARAM_LIST);
    param();
    while(getCurrentToken().stringVal==","){
        match(",");
        param();
    }
    returnToParentNode();
}

void Syntax_Analyzer::statement_list(){
    createChildNode(sa::STATEMENT_LIST);
    while(isStatement()){
        statement();
    }
    returnToParentNode();
}

void Syntax_Analyzer::param(){
    createChildNode(sa::PARAM);

    type_specifier();
    createChildNode(sa::ID);
    currentNode->value=getCurrentToken().stringVal;
    match(la::ID);
    returnToParentNode();
    if(getCurrentToken().stringVal=="["){
        match("[");
        match("]");
    }
    returnToParentNode();
}

void Syntax_Analyzer::statement(){
    createChildNode(sa::STATEMENT);

    if(is_match(la::ID)||is_match(la::INT|la::FLOAT)||is_match("(")||is_match(";")){
        expression_stmt();
    }
    else if(is_match("{")){
        compound_stmt();
    }
    else if(is_match("if")){
        selection_stmt();
    }
    else if(is_match("while")){
        while_iteration_stmt();
    }
    else if(is_match("for")){
        for_iteration_stmt();
    }
    else if(is_match("return")){
        return_stmt();
    }
    else{
        error();
    }

    returnToParentNode();
}

void Syntax_Analyzer::expression_stmt(){
    createChildNode(sa::EXPRESSION_STMT);
    if(!is_match(";")){
        expression();
    }
    match(";");
    returnToParentNode();
}

void Syntax_Analyzer::compound_stmt(){
    createChildNode(sa::COMPOUND_STMT);

    match("{");
    local_declaractions();
    statement_list();
    match("}");

    returnToParentNode();
}

void Syntax_Analyzer::local_declaractions(){
    createChildNode(sa::LOCAL_DECLARACTIONS);

    while(isType()){
        var_declaration();
    }

    returnToParentNode();
}

void Syntax_Analyzer::var_declaration(){
    createChildNode(sa::VAR_DECLARATION);

    type_specifier();
    /*
    createChildNode(sa::ID);
    currentNode->value=getCurrentToken().stringVal;
    match(la::ID);
    returnToParentNode();
    if(is_match("[")){
        match("[");
        createChildNode(NUM);
        currentNode->value=getCurrentToken().stringVal;
        match(la::INT);
        returnToParentNode();
        match("]");
    }
    */
    //var();
    createChildNode(sa::ID);//创建ID节点
        currentNode->value=(getCurrentToken()).stringVal;
        match(la::ID);//match
        returnToParentNode();//当前节点为VAR_DECLARATION
    if(is_match("=")){
        match("=");
        expression();
    }
    while(is_match(",")){
        match(",");
        //var();
        createChildNode(sa::ID);//创建ID节点
        currentNode->value=(getCurrentToken()).stringVal;
        match(la::ID);//match
        returnToParentNode();//当前节点为VAR_DECLARATION
        if(is_match("=")){
            match("=");
            expression();
        }
    }

    match(";");
    returnToParentNode();
}

void Syntax_Analyzer::selection_stmt(){
    createChildNode(sa::SELECTION_STMT);

    match("if");
    match("(");
    expression();
    match(")");
    statement();
    if(is_match("else")){
        match("else");
        statement();
    }

    returnToParentNode();
}

void Syntax_Analyzer::while_iteration_stmt(){
    createChildNode(sa::WHILE_ITERATION_STMT);
    match("while");
    match("(");
    expression();
    match(")");
    statement();
    returnToParentNode();
}

void Syntax_Analyzer::for_iteration_stmt(){
    createChildNode(sa::FOR_ITERATION_STMT);
    match("for");
    match("(");
    expression_stmt();
    expression_stmt();
    if(!is_match(")")){
        expression();
    }
    match(")");
    statement();
    returnToParentNode();
}

void Syntax_Analyzer::return_stmt(){
    createChildNode(sa::RETURN_STMT);
    
    match("return");
    expression_stmt();

    returnToParentNode();
}

void Syntax_Analyzer::expression(){
    createChildNode(sa::EXPRESSION);
    if(is_match(la::ID)){
        createChildNode(sa::SIMPLE_EXPRESSION);
        var();
        if(is_match("=")){
            TreeNode *p=currentNode->leftChild;
            returnToParentNode();
            delete currentNode->leftChild;
            currentNode->leftChild=p;
            p->parent=currentNode;
            match("=");
            expression();
        }
        else{
            TreeNode *p=currentNode->leftChild;
            currentNode->leftChild=NULL;
            createChildNode(sa::ADDITIVE_EXPRESSION);
            createChildNode(sa::TERM);
            createChildNode(sa::FACTOR);
            currentNode->leftChild=p;
            p->parent=currentNode;
            returnToParentNode();
            while(isMulop()){
                mulop();
                factor();
            }
            returnToParentNode();
            while(isAddop()){
                addop();
                term();
            }
            returnToParentNode();
            if(isRelop()){
                relop();
                additive_expression();
            }
            returnToParentNode();
        }
    }
    else{
        simple_expression();
    }
    returnToParentNode();
}

void Syntax_Analyzer::var(){
    createChildNode(sa::VAR);

    createChildNode(sa::ID);
    currentNode->value=getCurrentToken().stringVal;
    match(la::ID);
    returnToParentNode();
    if(is_match("[")){
        match("[");
        expression();
        match("]");
    }
    returnToParentNode();

}

void Syntax_Analyzer::simple_expression(){
    createChildNode(sa::SIMPLE_EXPRESSION);
    additive_expression();
    if(isRelop()){
        relop();
        additive_expression();
    }
    returnToParentNode();
}

void Syntax_Analyzer::additive_expression(){
    createChildNode(sa::ADDITIVE_EXPRESSION);
    if(isAddop()){
        addop();
    }
    term();
    while(isAddop()){
        addop();
        term();
    }
    returnToParentNode();
}

void Syntax_Analyzer::term(){
    createChildNode(sa::TERM);

    factor();
    while(isMulop()){
        mulop();
        factor();
    }

    returnToParentNode();
}

void Syntax_Analyzer::factor(){
    createChildNode(sa::FACTOR);
    if(is_match("(")){
        match("(");
        expression();
        match(")");
    }
    else if(is_match(la::INT|la::FLOAT|la::CHAR|la::STRING)){
        if(is_match(la::STRING)){
            createChildNode(sa::STRING);
        }else if(is_match(la::INT|la::CHAR)){
            createChildNode(sa::INT);
        }else if(is_match(la::FLOAT)){
            createChildNode(sa::FLOAT);
        }
        currentNode->value=getCurrentToken().stringVal;
        match(la::INT|la::FLOAT|la::CHAR|la::STRING);
        returnToParentNode();
    }
    else if(is_match(la::ID)){
        createChildNode(sa::CALL);
        createChildNode(sa::ID);
        currentNode->value=getCurrentToken().stringVal;
        match(la::ID);
        returnToParentNode();
        if(is_match("(")){
            match("(");
            args();
            match(")");
        }
        else if(is_match("[")){
            currentNode->type=sa::VAR;
            match("[");
            expression();
            match("]");
        }
        else{
            currentNode->type=sa::VAR;
        }
        returnToParentNode();
    }
    returnToParentNode();
}

void Syntax_Analyzer::relop(){
    createChildNode(sa::RELOP);
    currentNode->value=getCurrentToken().stringVal;
    match(RELOP);
    returnToParentNode();
}

void Syntax_Analyzer::addop(){
    createChildNode(sa::ADDOP);
    currentNode->value=getCurrentToken().stringVal;
    match(ADDOP);
    returnToParentNode();
}

void Syntax_Analyzer::mulop(){
    createChildNode(sa::MULOP);
    currentNode->value=getCurrentToken().stringVal;
    match(MULOP);
    returnToParentNode();
}

void Syntax_Analyzer::call(){
    createChildNode(sa::CALL);
    createChildNode(sa::ID);
    currentNode->value=getCurrentToken().stringVal;
    match(la::ID);
    returnToParentNode();
    match("(");
    args();
    match(")");
    returnToParentNode();
}

void Syntax_Analyzer::args(){
    createChildNode(sa::ARGS);
    if(!is_match(")")){
        arg_list();
    }
    returnToParentNode();
}

void Syntax_Analyzer::arg_list(){
    createChildNode(sa::ARG_LIST);

    expression();
    while(is_match(",")){
        match(",");
        expression();
    }
    returnToParentNode();
}

bool Syntax_Analyzer::match(Type_type t){
    if(t&la::INT){
        if(getCurrentToken().tockenType==la::CONST&&getCurrentToken().constType==la::INT){
            getNextToken();
            return true;
        }
    }
    if(t&la::FLOAT){
        if(getCurrentToken().tockenType==la::CONST&&getCurrentToken().constType==la::FLOAT){
            getNextToken();
            return true;
        }
    }
    if(t&la::STRING){
        if(getCurrentToken().tockenType==la::CONST&&getCurrentToken().constType==la::STRING){
            getNextToken();
            return true;
        }
    }
    if(t&VOID){
        if(getCurrentToken().tockenType==la::CONST&&getCurrentToken().constType==la::VOID){
            getNextToken();
            return true;
        }
    }
    if(t&la::CHAR){
        if(getCurrentToken().tockenType==la::CONST&&getCurrentToken().constType==la::CHAR){
            getNextToken();
            return true;
        }
    }
    error();
}

bool Syntax_Analyzer::is_match(Type_type t){
    if(t&la::INT){
        if(getCurrentToken().tockenType==la::CONST&&getCurrentToken().constType==la::INT){
            return true;
        }
    }
    if(t&la::FLOAT){
        if(getCurrentToken().tockenType==la::CONST&&getCurrentToken().constType==la::FLOAT){
            return true;
        }
    }
    if(t&la::STRING){
        if(getCurrentToken().tockenType==la::CONST&&getCurrentToken().constType==la::STRING){
            return true;
        }
    }
    if(t&VOID){
        if(getCurrentToken().tockenType==la::CONST&&getCurrentToken().constType==la::VOID){
            return true;
        }
    }
    if(t&la::CHAR){
        if(getCurrentToken().tockenType==la::CONST&&getCurrentToken().constType==la::CHAR){
            return true;
        }
    }
    
    return false;
}

bool Syntax_Analyzer::match(la::TockenType t){
    if(getCurrentToken().tockenType==t){
        getNextToken();
        return true;
    }
    else{
        cout<<"[ERROR]: The expected token is";
        switch(t){
            case la::RESERVED: cout<<"reserved"; break;
            case la::OPERATOR: cout<<"operator"; break;
            case la::SEPARATE: cout<<"separate"; break;
            case la::ID: cout<<"id"; break;
            case la::CONST: cout<<"const"; break;
            case la::ERROR: cout<<"error"; break;
            case la::PREPROCESS: cout<<"preprocess"; break;
        }
        cout<<" , and the actual token is "<<getCurrentToken().stringVal<<"."<<endl;
    }
}

bool Syntax_Analyzer::match(string s){
    if(getCurrentToken().stringVal==s){
        getNextToken();
        return true;
    }
    else{
        
        cout<<"[ERROR]: The expected token is"<<s;
        cout<<" , and the actual token is "<<getCurrentToken().stringVal<<"."<<endl;
        error();
    }
}

bool Syntax_Analyzer::is_match(la::TockenType t){
    if(getCurrentToken().tockenType==t){
        return true;
    }
    return false;
}

bool Syntax_Analyzer::is_match(string s){
    if(getCurrentToken().stringVal==s){
        return true;
    }
    return false;
}

bool Syntax_Analyzer::match(Op_type t){
    switch(t){
        case ADDOP:
            if(isAddop()){
                getNextToken();
                return true;
            }
            else{
                cout<<"[ERROR]: The expected token is"<<getTypeName(t);
                cout<<" , and the actual token is "<<getCurrentToken().stringVal<<"."<<endl;
                error();
            }
            break;
        case MULOP:
            if(isMulop()){
                getNextToken();
                return true;
            }
            else{
                cout<<"[ERROR]: The expected token is"<<getTypeName(t);
                cout<<" , and the actual token is "<<getCurrentToken().stringVal<<"."<<endl;
                error();
            }
            break;
        case RELOP:
            if(isRelop()){
                getNextToken();
                return true;
            }
            else{
                cout<<"[ERROR]: The expected token is"<<getTypeName(t);
                cout<<" , and the actual token is "<<getCurrentToken().stringVal<<"."<<endl;
                error();
            }
            break;
    }

    return false;
}
