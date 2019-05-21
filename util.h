#ifndef UTIL_H
#define UTIL_H
#include"Syntax_Analyzer.h"
void showNode(TreeNode*);
void getDig(TreeNode*);
string getTypeName(TreeNode*);
string getTypeName(sa::Syntax_type);
void dfs(TreeNode*);
string getTyoeName(sa::Syntax_type type);
#endif