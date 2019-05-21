#include"Lexical_Analyzer.h"
#include"Syntax_Analyzer.h"
#include "util.h"
#include<iostream>
using namespace std;
int main(){
    Syntax_Analyzer syntax_Analyzer("test.txt");
    TreeNode* root=syntax_Analyzer.getSyntaxTree();
    showNode(root);
    getDig(root);
    return 0;
}