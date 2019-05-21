#include<cstdio>
#include<cctype>
#include<string>
#include<fstream>
#include<iostream>
#include"Lexical_Analyzer.h"
using namespace std;
using namespace la;
Lexical_Analyzer::Lexical_Analyzer(string filename){
    fs.open(filename.c_str(),fstream::in);
    if(fs.fail()){
        cout<<"failed to open source code file!"<<endl;
        return;
    }
}

string Lexical_Analyzer::panic_mode(){
    string buffer;
    char c=fs.get();
    while(!fs.eof()){
        if(!isalpha(c)||!isdigit(c)){
            fs.putback(c);
             cout<<(char)8;
            return buffer;
        }
        else{
            buffer+=c;
            c=fs.get();
        }
    }
    return buffer;
}

TockenRecord Lexical_Analyzer::get_next_tocken(){
    int status=0;
    string buffer;
    TockenRecord tocken;
    while(!fs.eof()){
        char c=fs.get();
        cout<<c;
        switch(status){
            case 0:
                if(isdigit(c)){
                    if(c=='0') status=1;
                    else status=2;
                    buffer+=c;
                    break;
                }
                else if(isalpha(c)){
                    status=7;
                    buffer+=c;
                    break;
                }
                else if(isspace(c)){
                    status=0;
                    break;
                }
                switch(c){
                    case '.':
                        status=3;
                        buffer+=c;
                        break;
                    case '/':
                        status=10;
                        buffer+=c;
                        break;
                    case '#':
                        status=14;
                        buffer+=c;
                        break;
                    case '\'':
                        status=15;
                        buffer+=c;
                        break;
                    case '"':
                        status=17;
                        buffer+=c;
                        break;
                    case '*':  case '%':case '(': case')':case'^': case '[':case ']':
                        tocken.tockenType=OPERATOR;
                        tocken.stringVal=c;
                        return tocken;
                    case ';':case '{':case '}':case ',':
                        tocken.tockenType=SEPARATE;
                        tocken.stringVal=c;
                        return tocken;
                    case 0:
                        tocken.tockenType=RESERVED;
                        tocken.stringVal="EOF";
                        return tocken;
                    case '=':case '<':case '>':case '!':
                        status=18;
                        buffer+=c;
                        break;
                    case '+': 
                        status=19;
                        buffer+=c;
                        break;
                    case '-':
                        status=20;
                        buffer+=c;
                        break;
                        
                }
                break;
            case 1:
                if(isdigit(c)){
                    status=2;
                    buffer+=c;
                    break;
                }
                else if(c=='e'||c=='E'){
                    status=5;
                    buffer+=c;
                    break;
                }
                else{
                    if(c=='x'||c=='X'){
                        status=8;
                        buffer+=c;
                        break;
                    }
                    else if(c=='.'){
                        status=3;
                        buffer+=c;
                        break;
                    }
                    else if(isalpha(c)){
                        string error_msg="Variables cannot begin with numbers : ";
                        buffer=error_msg+buffer+c;
                        status=PANIC_MODE;
                        break;
                    }
                    else{
                        fs.putback(c);
                         cout<<(char)8;
                        tocken.stringVal=buffer;
                        tocken.tockenType=CONST;
                        tocken.constType=INT;
                        return tocken;
                    }
                }
                break;
            case 2:
                if(isdigit(c)){
                    status=2;
                    buffer+=c;
                    break;
                }
                else if(c=='B'||c=='b'||c=='O'||c=='o'||c=='D'||c=='d'||c=='H'||c=='h'){
                    buffer+=c;
                    tocken.stringVal=buffer;
                    tocken.tockenType=CONST;
                    tocken.constType=INT;
                    return tocken;
                }
                else if(c=='.'){
                    buffer+=c;
                    status=3;
                    break;
                }
                else if(c=='e'||c=='E'){
                    status=5;
                    buffer+=c;
                    break;
                }
                else if(isspace(c)){
                    fs.putback(c);
                     cout<<(char)8;
                    tocken.stringVal=buffer;
                    tocken.tockenType=CONST;
                    tocken.constType=INT;
                    return tocken;
                }
                else if(isalpha(c)){
                    //ERROR,Panic mode
                    string error_msg="Variables cannot begin with numbers : ";
                    buffer=error_msg+buffer+c;
                    status=PANIC_MODE;
                    break;
                }
                else{
                    fs.putback(c);
                     cout<<(char)8;
                    tocken.stringVal=buffer;
                    tocken.tockenType=CONST;
                    tocken.constType=INT;
                    return tocken;
                }
                break;
            case 3:
                if(isdigit(c)){
                    status=4;
                    buffer+=c;
                    break;
                }
                else{
                    string error_msg="Variables cannot begin with numbers : ";
                    buffer=error_msg+buffer+c;
                    status=PANIC_MODE;
                    break;
                }
                break;
            case 4:
                if(isdigit(c)){
                    status=4;
                    buffer+=c;
                    break;
                }
                else if(c=='e'||c=='E'){
                    status=5;
                    buffer+=c;
                    break;
                }
                else if(isalpha(c)){
                    buffer+=c;
                    status=PANIC_MODE;
                    break;
                }
                else{
                    fs.putback(c);
                     cout<<(char)8;
                    tocken.stringVal=buffer;
                    tocken.tockenType=CONST;
                    tocken.constType=FLOAT;
                    return tocken;
                }
                break;
            case 5:
                if(isdigit(c)){
                    status=6;
                    buffer+=c;
                    break;
                }
                else{
                    string error_msg="The exponential part of the scientific notation must be  a decimal integer : ";
                    buffer=error_msg+buffer+c;
                    status=PANIC_MODE;
                    break;
                }
                break;
            case 6:
                if(isdigit(c)){
                    buffer+=c;
                    status=6;
                    break;
                }
                else if(isalpha(c)||c=='.'){
                    string error_msg="The exponential part of the scientific notation must be  a decimal integer : ";
                    buffer=error_msg+buffer+c;
                    status=PANIC_MODE;
                    break;
                }
                else{
                    fs.putback(c);
                     cout<<(char)8;
                    tocken.tockenType=CONST;
                    tocken.constType=FLOAT;
                    tocken.stringVal=buffer;
                    return tocken;
                }
                break;
            
            case 7:
                if(isalnum(c)||c=='_'){
                    for(int i=0;i<(sizeof(Reserved_words)/sizeof(string));i++){
                        if(buffer+c==Reserved_words[i]){
                            status=9;
                            buffer+=c;
                            break;
                        }
                    }
                    if(status!=9){
                        buffer+=c;
                        status = 7;
                    }
                }
                else{
                    //if(buffer=="")
                    fs.putback(c);
                    cout<<(char)8;
                    tocken.stringVal=buffer;
                    tocken.tockenType=ID;
                    return tocken;
                }
                break;
            case 8:
                if(isdigit(c)||c>='a'&&c<='f'||c>='A'&&c<='F'){
                    status=8;
                    buffer+=c;
                    break;
                }
                else{
                    if(c!='H'&&c!='h'){ fs.putback(c); cout<<(char)8;}
                    else buffer+=c;
                    tocken.stringVal=buffer;
                    tocken.tockenType=CONST;
                    tocken.constType=INT;
                    return tocken;
                }
            case 9:
                if(isspace(c)){
                    tocken.tockenType=RESERVED;
                    tocken.stringVal=buffer;
                    return tocken;
                }
                else if(isalnum(c)){
                    status=7;
                    buffer+=c;
                }
                else{
                    fs.putback(c);
                     cout<<(char)8;
                    tocken.tockenType=RESERVED;
                    tocken.stringVal=buffer;
                    return tocken;
                }

            case 10:
                if(c=='/'){
                    status=11;
                    buffer+=c;
                    break;
                }
                else if(c=='*'){
                    status=12;
                    buffer+=c;
                    break;
                }
                else{
                    fs.putback(c);
                     cout<<(char)8;
                    tocken.tockenType=OPERATOR;
                    tocken.stringVal=buffer;
                    return tocken;
                }
            case 11:
                if(c=='\n'||!c){
                    status=0;
                    buffer="";
                    break;
                }
                else{
                    status=11;
                    break;
                }
            case 12:
                if(c=='*'){
                    status=13;
                    break;
                }
                else{
                    status=12;
                    break;
                }
            case 13:
                if(c=='/'){
                    status=0;
                    buffer="";
                    break;
                }
                else{
                    status=12;
                    break;
                }
            case 14:
                if(c=='\n'||!c){
                    tocken.tockenType=PREPROCESS;
                    tocken.stringVal=buffer;
                    return tocken;
                }
                else{
                    buffer+=c;
                    status=14;
                    break;
                }
            case 15:
                if(c=='\''){
                    status=PANIC_MODE;
                    string error_msg="Character constants have and have only one letter :";
                    buffer=error_msg+buffer+c;
                    break;
                }
                else{
                    status=16;
                    buffer+=c;
                    break;
                }
            case 16:
                if(c=='\''){
                    buffer+=c;
                    tocken.tockenType=CONST;
                    tocken.stringVal=buffer;
                    tocken.constType=CHAR;
                    return tocken;
                }
                else{
                    status=QUOTE_PANIC_MODE;
                    string error_msg="Character constants have and have only one letter :";
                    buffer=error_msg+buffer+c;
                    break;
                }
            case 17:
                if(c=='"'){
                    buffer+=c;
                    tocken.tockenType=CONST;
                    tocken.constType=STRING;
                    tocken.stringVal=buffer;
                    return tocken;
                }
                else{
                    buffer+=c;
                    status=17;
                    break;
                }
            case 18:
                if(c=='='){
                    buffer+=c;
                    tocken.tockenType=OPERATOR;
                    tocken.stringVal=buffer;
                    return tocken;
                }
                else{
                    fs.putback(c);
                     cout<<(char)8;
                    tocken.tockenType=OPERATOR;
                    tocken.stringVal=buffer;
                    return tocken;
                }
            case 19:
                if(c=='+'){
                    buffer+=c;
                    tocken.tockenType=OPERATOR;
                    tocken.stringVal=buffer;
                    return tocken;
                }
                else{
                    fs.putback(c);
                     cout<<(char)8;
                    tocken.tockenType=OPERATOR;
                    tocken.stringVal=buffer;
                    return tocken;
                }
            case 20:
                if(c=='-'){
                    buffer+=c;
                    tocken.tockenType=OPERATOR;
                    tocken.stringVal=buffer;
                    return tocken;
                }
                else{
                    fs.putback(c);
                     cout<<(char)8;
                    tocken.tockenType=OPERATOR;
                    tocken.stringVal=buffer;
                    return tocken;
                }
            case QUOTE_PANIC_MODE:
                if(c!='\''&&c!=';'&&c!='\n'){
                    status=QUOTE_PANIC_MODE;
                    buffer+=c;
                    break;
                }
                else{
                    buffer+=c;
                    tocken.tockenType=ERROR;
                    tocken.stringVal=buffer;
                    return tocken;
                }
            case PANIC_MODE:
                if(isalnum(c)){
                    status=PANIC_MODE;
                    buffer+=c;
                    break;
                }
                else{
                    fs.putback(c);
                     cout<<(char)8;
                    tocken.tockenType=ERROR;
                    tocken.stringVal=buffer;
                    return tocken;
                }
        }
    }
}