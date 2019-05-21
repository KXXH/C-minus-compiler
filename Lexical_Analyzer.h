#ifndef LEXICAL_ANALYZER
#define LEXICAL_ANALYZER

#include<cstdio>
#include<fstream>
#include<cctype>
#include<string>
#define PANIC_MODE 999
#define QUOTE_PANIC_MODE 1000
using namespace std;

const string Reserved_words[]={
    "void","if","else","int","float","return","char","break","while","for"
};

namespace la
{
    typedef enum{
        RESERVED,OPERATOR,SEPARATE,ID,CONST,ERROR,PREPROCESS
    }TockenType;

    typedef enum{
        INT=1,FLOAT=2,CHAR=4,STRING=8,VOID=16
    }ConstType;
} // namespace la

using namespace la;


class TockenRecord{
    public:
    TockenType tockenType;
    ConstType constType;

    string stringVal;
    string errorMsg;
    double doubleVal;
};

class Lexical_Analyzer{
private:
    fstream fs;
public:
    Lexical_Analyzer(string filename);
    TockenRecord get_next_tocken();
    string panic_mode();
    bool eof(){return fs.eof();}
};

#endif