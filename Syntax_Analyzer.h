#ifndef SYNTAX_ANALYZER_H
#define SYNTAX_ANALYZER_H
#include<cstdio>
#include<fstream>
#include<cctype>
#include<string>
#include<queue>
#include"Lexical_Analyzer.h"
using namespace std;

namespace sa
{
    typedef enum{
        PROGRAM,DECLARATION_LIST,DECLARATION,VAR_DECLARATION,TYPE_SPECIFIER,
        FUN_DECLARATION,PARAMS,PARAM_LIST,PARAM,COMPOUND_STMT,LOCAL_DECLARACTIONS,
        STATEMENT_LIST,STATEMENT,EXPRESSION_STMT,SELECTION_STMT,WHILE_ITERATION_STMT,
        FOR_ITERATION_STMT,RETURN_STMT,EXPRESSION,VAR,SIMPLE_EXPRESSION,RELOP,ADDITIVE_EXPRESSION,
        ADDOP,TERM,MULOP,FACTOR,CALL,ARGS,ARG_LIST,ID,NUM,STRING,INT,FLOAT
    }Syntax_type;
} // namespace name

using namespace sa;

typedef la::ConstType Add_on_type;

typedef Syntax_type Op_type;

typedef int Type_type;

class TreeNode{
public:
    TreeNode* leftChild;
    TreeNode* rightSibling;
    TreeNode* parent;
    Syntax_type type;
    string value;
    TreeNode():leftChild(NULL),rightSibling(NULL),parent(NULL){};
};

class Syntax_Analyzer{
protected:
    Lexical_Analyzer lexiacl_analyzer;
    string filename;
    TockenRecord currentToken;
    TreeNode* root;
    TreeNode* currentNode;
    bool errorFlag;
    string errorMsg;
    queue<TockenRecord> buffer;

    bool isType();
    bool isStatement();
    bool isMulop();
    bool isAddop();
    bool isRelop();
    TockenRecord getCurrentToken();
    void getNextToken();

    void error();



    void createChildNode(Syntax_type);  //在当前节点下创建一个子节点，并将游标移动到这个节点
    void createSiblingNode(Syntax_type);    //为当前节点创建一个右兄弟，并将游标移动到这个节点
    void returnToParentNode();  //将游标移动到当前节点的父节点

    void program();
    void declaration_list();
    void declaration();
    void var_declaration();
    void type_specifier();
    void fun_declaration();
    void params();
    void param_list();
    void param();
    void compound_stmt();
    void local_declaractions();
    void statement_list();
    void statement();
    void expression_stmt();
    void selection_stmt();
    void while_iteration_stmt();
    void for_iteration_stmt();
    void return_stmt();
    void expression();
    void var();
    void simple_expression();
    void relop();
    void additive_expression();
    void addop();
    void term();
    void mulop();
    void factor();
    void call();
    void args();
    void arg_list();
    bool match(la::TockenType);
    bool match(string);
    bool match(Type_type);
    bool match(Op_type);

    bool is_match(la::TockenType);
    bool is_match(string);
    bool is_match(Type_type);
public:
    Syntax_Analyzer(string fname):lexiacl_analyzer(fname),filename(fname),errorFlag(false){currentNode = root = new TreeNode;getNextToken();};
    TreeNode* getSyntaxTree();
};
#endif