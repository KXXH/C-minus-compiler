#include"util.h"
#include"Syntax_Analyzer.h"
#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<queue>
#include<cstdlib>
using namespace std;


string id="0";

string getId(TreeNode* t){
    stringstream ss;
    ss<<(long long)t;
    string s;
    ss>>s;
    return s;
}


string getId(){
    if(*(id.end()-1)<'9'){
        //cout<<*(id.end()-1)<<endl;;
        id[id.length()-1]++;
    }
    else{
        id=id+"0";
    }
    return id;
}

string getTypeName(TreeNode*);
fstream fs;

void getDig(TreeNode* root){
    if(root->leftChild==NULL) return;
    
    fs.open("output.dot",fstream::out);
    fs<<"digraph G {\n"<<getId(root)<<" [label=\"root\"]\n";
    dfs(root->leftChild);
    fs<<"}";
    fs.close();
    system("dot -Tpng .\\output.dot -o out.png");
    system(".\\out.png");
}

void dfs(TreeNode* t){
    if(t==NULL) return;
    string parentId=getId(t->parent);
    string currentId=getId(t);
    fs<<currentId<<"[label=\""<<getTypeName(t);
    if(t->value.length()>0){
        if(t->type==sa::STRING){
            fs<<"("<<t->value.substr(1,t->value.length()-2)<<")";
        }else{
            fs<<"("<<t->value<<")";
        }
    }
    fs<<"\"]"<<endl;
    fs<<parentId<<" -> "<<currentId<<endl;
    if(t->leftChild!=NULL){
        dfs(t->leftChild);
        t=t->leftChild;
        while(t->rightSibling!=NULL){
            dfs(t->rightSibling);
            t=t->rightSibling;
        }
    }
    
    
}

string getTypeName(sa::Syntax_type type){
    switch(type){
        case sa::PROGRAM:
            return "program";
            break;
        case sa::DECLARATION_LIST:
            return "declaration-list";
            break;
        case sa::DECLARATION:
            return "declaration";
            break;
        case sa::VAR_DECLARATION:
            return "var-declaration";
            break;
        case sa::TYPE_SPECIFIER:
            return "type-specifier";
            break;
        case sa::FUN_DECLARATION:
            return "fun-dedeclaration";
            break;
        case sa::PARAMS:
            return "params";
            break;
        case sa::PARAM_LIST:
            return "param-list";
            break;
        case sa::PARAM:
            return "param";
            break;
        case sa::COMPOUND_STMT:
            return "compound-stmt";
            break;
        case sa::LOCAL_DECLARACTIONS:
            return "local-declaractions";
            break;
        case sa::STATEMENT_LIST:
            return "statement-list";
            break;
        case sa::STATEMENT:
            return "statement";
            break;
        case sa::EXPRESSION_STMT:
            return "expression-stmt";
            break;
        case sa::SELECTION_STMT:
            return "selection-stmt";
            break;
        case sa::WHILE_ITERATION_STMT:
            return "while-iteration-stmt";
            break;
        case sa::FOR_ITERATION_STMT:
            return "for-iteration-stmt";
            break;
        case sa::RETURN_STMT:
            return "return-stmt";
            break;
        case sa::EXPRESSION:
            return "expression";
            break;
        case sa::VAR:
            return "var";
            break;
        case sa::SIMPLE_EXPRESSION:
            return "simple-expression";
            break;
        case sa::RELOP:
            return "relop";
            break;
        case sa::ADDITIVE_EXPRESSION:
            return "additive-expression";
            break;
        case sa::ADDOP:
            return "addop";
            break;
        case sa::TERM:
            return "term";
            break;
        case sa::MULOP:
            return "mulop";
            break;
        case sa::FACTOR:
            return "factor";
            break;
        case sa::CALL:
            return "call";
            break;
        case sa::ARGS:
            return "args";
            break;
        case sa::ARG_LIST:
            return "arg-list";
            break;
        case sa::ID:
            return "id";
            break;
        case sa::NUM:
            return "num";
            break;
        case sa::INT:
            return "int";
            break;
        case sa::FLOAT:
            return "float";
            break;
        case sa::STRING:
            return "string";
            break;
        default:
            return "unknown";
            break;
    }
}

string getTypeName(TreeNode* t){
    switch(t->type){
        case sa::PROGRAM:
            return "program";
            break;
        case sa::DECLARATION_LIST:
            return "declaration-list";
            break;
        case sa::DECLARATION:
            return "declaration";
            break;
        case sa::VAR_DECLARATION:
            return "var-declaration";
            break;
        case sa::TYPE_SPECIFIER:
            return "type-specifier";
            break;
        case sa::FUN_DECLARATION:
            return "fun-dedeclaration";
            break;
        case sa::PARAMS:
            return "params";
            break;
        case sa::PARAM_LIST:
            return "param-list";
            break;
        case sa::PARAM:
            return "param";
            break;
        case sa::COMPOUND_STMT:
            return "compound-stmt";
            break;
        case sa::LOCAL_DECLARACTIONS:
            return "local-declaractions";
            break;
        case sa::STATEMENT_LIST:
            return "statement-list";
            break;
        case sa::STATEMENT:
            return "statement";
            break;
        case sa::EXPRESSION_STMT:
            return "expression-stmt";
            break;
        case sa::SELECTION_STMT:
            return "selection-stmt";
            break;
        case sa::WHILE_ITERATION_STMT:
            return "while-iteration-stmt";
            break;
        case sa::FOR_ITERATION_STMT:
            return "for-iteration-stmt";
            break;
        case sa::RETURN_STMT:
            return "return-stmt";
            break;
        case sa::EXPRESSION:
            return "expression";
            break;
        case sa::VAR:
            return "var";
            break;
        case sa::SIMPLE_EXPRESSION:
            return "simple-expression";
            break;
        case sa::RELOP:
            return "relop";
            break;
        case sa::ADDITIVE_EXPRESSION:
            return "additive-expression";
            break;
        case sa::ADDOP:
            return "addop";
            break;
        case sa::TERM:
            return "term";
            break;
        case sa::MULOP:
            return "mulop";
            break;
        case sa::FACTOR:
            return "factor";
            break;
        case sa::CALL:
            return "call";
            break;
        case sa::ARGS:
            return "args";
            break;
        case sa::ARG_LIST:
            return "arg-list";
            break;
        case sa::ID:
            return "id";
            break;
        case sa::NUM:
            return "num";
            break;
        case sa::INT:
            return "int";
            break;
        case sa::FLOAT:
            return "float";
            break;
        case sa::STRING:
            return "string";
            break;
        default:
            return "unknown";
            break;
    }
}

void showNode(TreeNode* t){
    if(t==NULL) return;
    cout<<"{\"type\":\"";
    switch(t->type){
        case sa::PROGRAM:
            cout<<"program";
            break;
        case sa::DECLARATION_LIST:
            cout<<"declaration-list";
            break;
        case sa::DECLARATION:
            cout<<"declaration";
            break;
        case sa::VAR_DECLARATION:
            cout<<"var-declaration";
            break;
        case sa::TYPE_SPECIFIER:
            cout<<"type-specifier";
            break;
        case sa::FUN_DECLARATION:
            cout<<"fun-dedeclaration";
            break;
        case sa::PARAMS:
            cout<<"params";
            break;
        case sa::PARAM_LIST:
            cout<<"param-list";
            break;
        case sa::PARAM:
            cout<<"param";
            break;
        case sa::COMPOUND_STMT:
            cout<<"compound-stmt";
            break;
        case sa::LOCAL_DECLARACTIONS:
            cout<<"local-declaractions";
            break;
        case sa::STATEMENT_LIST:
            cout<<"statement-list";
            break;
        case sa::STATEMENT:
            cout<<"statement";
            break;
        case sa::EXPRESSION_STMT:
            cout<<"expression-stmt";
            break;
        case sa::SELECTION_STMT:
            cout<<"selection-stmt";
            break;
        case sa::WHILE_ITERATION_STMT:
            cout<<"while-iteration-stmt";
            break;
        case sa::FOR_ITERATION_STMT:
            cout<<"for-iteration-stmt";
            break;
        case sa::RETURN_STMT:
            cout<<"return-stmt";
            break;
        case sa::EXPRESSION:
            cout<<"expression";
            break;
        case sa::VAR:
            cout<<"var";
            break;
        case sa::SIMPLE_EXPRESSION:
            cout<<"simple-expression";
            break;
        case sa::RELOP:
            cout<<"relop";
            break;
        case sa::ADDITIVE_EXPRESSION:
            cout<<"additive-expression";
            break;
        case sa::ADDOP:
            cout<<"addop";
            break;
        case sa::TERM:
            cout<<"term";
            break;
        case sa::MULOP:
            cout<<"mulop";
            break;
        case sa::FACTOR:
            cout<<"factor";
            break;
        case sa::CALL:
            cout<<"call";
            break;
        case sa::ARGS:
            cout<<"args";
            break;
        case sa::ARG_LIST:
            cout<<"arg-list";
            break;
        case sa::ID:
            cout<<"id";
            break;
        case sa::NUM:
            cout<<"num";
            break;
        case sa::INT:
            cout<<"int";
            break;
        case sa::FLOAT:
            cout<<"float";
            break;
        case sa::STRING:
            cout<<"string";
            break;
        default:
            cout<<"unknown";
            break;
    }
    cout<<"\",\"value\":\"";
    cout<<t->value;
    cout<<"\",\"child\":[";
    showNode(t->leftChild);
    if(t->leftChild!=NULL&&t->leftChild->rightSibling!=NULL){
        TreeNode* p=t->leftChild->rightSibling;
        while(p!=NULL){
            cout<<',';
            showNode(p);
            p=p->rightSibling;
        }
    }
    cout<<"]}";
}
