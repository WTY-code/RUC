#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstring>
#include <cstdlib>

#define END -1                    //结束字符种别码
#define ERROR -2                  //错误字符种别码
#define INITIAL -3                //初始种别码
#define CodeMaxLength 10000       //待编译源代码最大字符数
#define KeyWordNum 8              //关键字个数
#define KeyWordMaxLength 20       //单个关键字最大字符数
#define OperatorNum 25            //界符及运算符个数
#define OperatorMaxLength 2       //单个界符及操作符最大字符数
#define IdentifierNum 1000        //自定义标识符最大个数
#define IdentifierMaxLength 30    //单个自定义标识符最大字符数

using namespace std;

typedef struct //词的结构，二元组
{

    int type; //单词种别
    int word;//单词在对应表中的指针
}WORD;

static char KeyWord[KeyWordNum][KeyWordMaxLength] = {/*0*/ "break",
                                                     /*1*/ "continue",
                                                     /*2*/ "else",
                                                     /*3*/ "float",
                                                     /*4*/ "if",
                                                     /*5*/ "int",
                                                     /*6*/ "return",
                                                     /*7*/ "while"};
static char Operator[OperatorNum][OperatorMaxLength+1] = {/*8*/ "+",
                                                        /* 9*/ "-",
                                                        /*10*/ "*",
                                                        /*11*/ "/",
                                                        /*12*/ "%",
                                                        /*13*/ "!",
                                                        /*14*/ "?",
                                                        /*15*/ ",",
                                                        /*16*/ ":",
                                                        /*17*/ ";",
                                                        /*18*/ "=",
                                                        /*19*/ ">",
                                                        /*20*/ "<",
                                                        /*21*/ "(",
                                                        /*22*/ ")",
                                                        /*23*/ "{",
                                                        /*24*/ "}",
                                                        /*25*/ "||",
                                                        /*26*/ "&&",
                                                        /*27*/ "==",
                                                        /*28*/ "!=",
                                                        /*29*/ "<=",
                                                        /*30*/ ">=",
                                                        /*31*/ "++",
                                                        /*32*/ "--"};

static char Identifier[IdentifierNum][IdentifierMaxLength] = {""};/*1000 自定义标识符*/ /*1001 常数*/

//对源码预处理，从文件读入数组，并去掉注释内容
void Preprocess(char raw[], int codelength)
{
    char temp[CodeMaxLength];
    int count = 0;
    for (int i = 0; i <= codelength-1; i++)
    {
        if (raw[i] == '/'&&raw[i+1] == '/')
        {//若为单行注释“//”,则去除注释后面的东西，直至遇到回车换行
            while (raw[i] != '\n')
            {
                i++;
            }
        }
        if (raw[i] == '/'&&raw[i + 1] == '*')
        {//若为多行注释“/* 。。。*/”则去除该内容
            i += 2;
            while (raw[i] != '*' || raw[i + 1] != '/')
            {
                i++;
                if (raw[i] == '\0')
                {
                    cout<<"error in annotation: '*/' not found. exit..."<<endl;
                    exit(0);
                }
            }
            i += 2;//跳过“*/”
        }
        if (raw[i]!='\n' && raw[i]!='\t' && raw[i] !='\v' && raw[i]!='\r')
        {//若出现无用字符，略过
            temp[count++] = raw[i];
        }
    }
    temp[count] = '\0';
    strcpy(raw, temp);
}

bool IsDigit(char digit)
{
    if (digit>='0' && digit<='9')
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool IsLetter(char letter)
{
    if (letter>='a' && letter<='z' || letter>='A' && letter<='Z' || letter=='_')
    {
        return true;
    }
    else
    {
        return false;
    }
}

int SearchKeyWord(char KeyWord[KeyWordNum][KeyWordMaxLength], char token[])
{
    for (int i = 0; i < KeyWordNum; i++)
    {
        if (strcmp(KeyWord[i], token) == 0)
        {//若成功查找，则返回种别码
            return i;
        }
    }
    return 1000;//查找不成功返回1000，为自定义标识符
}

void Scan(WORD* oneword, char code[], char token[], int &p_code)
{
    int i=0, p_token = 0;//p_token为token[]的指针，收集有用字符
    char ch;
    ch = code[p_code];
    while (ch == ' ')
    {//过滤空格
        p_code++;
        ch = code[p_code];
    }
    for (i = 0; i<IdentifierMaxLength-1; i++)
    {//每次使用前对token初始化
        token[i] = '\0';
    }
    if (IsLetter(code[p_code]))
    {//开头为字母
        token[p_token++] = code[p_code];//收集
        p_code++;
        while (IsLetter(code[p_code]) || IsDigit(code[p_code]))
        {//后跟字母或数字
            token[p_token++] = code[p_code];//收集
            p_code++;
        }
        token[p_token] = '\0';
        oneword->type = SearchKeyWord(KeyWord, token);//查表找到种别码
        return;
    }
    else if (IsDigit(code[p_code]))
    {//首字符为数字
        //处理不规范标识符命名,如012au
        int tool = p_code;
        while(code[tool]!='\0' && IsDigit(code[tool]))
        {
            tool++;
        }
        if (IsLetter(code[tool]))
        {
            printf("error: nonstandard identifier\n");
            oneword->type = INITIAL;
            while(IsLetter(code[tool]))
            {
                tool++;
            }
            p_code = tool;
            return;
        }
        while (IsDigit(code[p_code]))
        {//后跟数字
            token[p_token++] = code[p_code];//收集
            p_code++;
        }//此时p_code指向下次将要开始的位置
        token[p_token] = '\0';
        oneword->type = 1001;//常数种别码
        return;
    }
    else if (ch == '*' || ch == '/' || ch == '%' || ch == '\?' || ch == ',' || ch == ':'
             || ch == ';' || ch == '(' || ch == ')'  || ch == '{' || ch == '}')
    {//若为运算符或者界符（不涉及超前搜索），查表得到结果
        token[0] = code[p_code];
        token[1] = '\0';//此时token为单字符串
        for (i = 0; i<OperatorNum; i++)
        {//查运算符界符表
            if (strcmp(token, Operator[i]) == 0)
            {
                oneword->type = KeyWordNum + i;//界符的种别码在关键词之后，因此数组标号加关键词个数为种别码
                break;
            }
        }
        p_code++;
        return;
    }
    else  if (code[p_code] == '+')
    {//+,++
        p_code++;//超前搜索
        if (code[p_code] == '+')
        {
            oneword->type = 31;
        }
        else
        {
            p_code--;
            oneword->type = 8;
        }
        p_code++;
        return;
    }
    else  if (code[p_code] == '-')
    {//-,--
        p_code++;//超前搜索
        if (code[p_code] == '-')
        {
            oneword->type = 32;
        }
        else
        {
            p_code--;
            oneword->type = 9;
        }
        p_code++;
        return;
    }
    else  if (code[p_code] == '<')
    {//<,<=
        p_code++;//超前搜索
        if (code[p_code] == '=')
        {
            oneword->type = 29;
        }
        else
        {
            p_code--;
            oneword->type = 20;
        }
        p_code++;
        return;
    }
    else  if (code[p_code] == '>')
    {//>,>=
        p_code++;//超前搜索
        if (code[p_code] == '=')
        {
            oneword->type = 30;
        }
        else
        {
            p_code--;
            oneword->type = 19;
        }
        p_code++;
        return;
    }
    else  if (code[p_code] == '=')
    {//=,==
        p_code++;//超前搜索
        if (code[p_code] == '=')
        {
            oneword->type = 27;
        }
        else
        {
            p_code--;
            oneword->type = 18;
        }
        p_code++;
        return;
    }
    else  if (code[p_code] == '!')
    {//!,!=
        p_code++;//超前搜索
        if (code[p_code] == '=')
        {
            oneword->type = 28;
        }
        else
        {
            oneword->type = 13;
            p_code--;
        }
        p_code++;
        return;
    }
    else  if (code[p_code] == '&')
    {//&&
        p_code++;
        if (code[p_code] == '&')
        {
            oneword->type = 26;
        }
        else
        {
            p_code--;
        }
        p_code++;
        return;
    }
    else if (code[p_code] == '|')
    {//||
        p_code++;
        if (code[p_code] == '|')
        {
            oneword->type = 25;
        }
        else
        {
            p_code--;
        }
        p_code++;
        return;
    }
    else  if (code[p_code] == '\0')
    {//结束符
        oneword->type = END;//种别码为-1，表示正常结束
        return;
    }
    else
    {//不能被以上词法分析识别，则出错。
        oneword->type = ERROR;//种别码为-2，表示出错
        p_code++;
        return;
    }
}

//词法分析程序，参数为源程序txt文件名
void LexicalAnalysis(char sourcecodefile[]){
    char code[CodeMaxLength],token[IdentifierMaxLength];
    int p_code = 0;//源代码指针
    int codelength = 0;//源代码总字符数
    WORD* oneword = new WORD;//当前识别的单词
    oneword->type = INITIAL;//设置种别码的初始值-3
    FILE *fp, *fp1;//fp为源代码文件，fp1为结果文件
    if ((fp = fopen(sourcecodefile, "r")) == NULL){//打开源程序
        cout << "open sourcecode file error!";
        exit(0);
    }
    if ((fp1 = fopen("lexical.txt", "w")) == NULL){//打开结果文件
        cout << "open result file error!";
        exit(0);
    }
    code[p_code] = fgetc(fp);
    while (code[p_code] != EOF){//将源程序读入code[]数组
        p_code++;
        code[p_code] = fgetc(fp);
    }
    code[p_code] = '\0';
    codelength = p_code+1;
    fclose(fp);
    cout << "RAW CODE:" << endl;
    cout << code << endl<<endl;
    Preprocess(code,codelength);
    cout << "AFTER PREPROCESS:" << endl;
    cout << code << endl<<endl;
    p_code = 0;
    while(oneword->type != END){//遇到结束符之前逐个扫描单词，根据种别码执行相应动作
        Scan(oneword,code,token,p_code);
        if (oneword->type == 1000)
        {//标识符
            for (int i = 0; i<IdentifierNum; i++){
                //插入标识符表中
                if (strcmp(Identifier[i], token) == 0){//已在表中
                    oneword->word = i;
                    printf("<1000, %s>\n", token);
                    fprintf(fp1, "<1000, %d>\n", oneword->word);//将种别码和在对应表中入口指针写入结果文件
                    break;
                }
                if (strcmp(Identifier[i], "") == 0){//查找空间
                    strcpy(Identifier[i], token);
                    oneword->word = i;
                    printf("<1000, %s>\n",token);
                    fprintf(fp1, "<1000, %d>\n", oneword->word);//将种别码和在对应表中入口指针写入结果文件
                    break;
                }
            }
        }
        else if (oneword->type >= 0 && oneword->type <= KeyWordNum-1)
        {//关键字
            oneword->word = oneword->type;
            printf("<%d, %s>\n", oneword->type, token);
            fprintf(fp1, "<%d, %d>\n", oneword->type, oneword->word);//将种别码和在对应表中入口指针写入结果文件
        }
        else if (oneword->type == 1001)
        {//const 常数
            int digit = atoi(token);
            //float digit = atof(token);
            oneword->word = digit;
            printf("<%d(const), %d>\n",1001, digit);
            fprintf(fp1, "<%d, %d>\n",1001, oneword->word);//将种别码和在对应表中入口指针写入结果文件
        }
        else if (oneword->type >= KeyWordNum && oneword->type < KeyWordNum+OperatorNum)
        {//界符及运算符
            oneword->word = oneword->type-KeyWordNum;
            printf("<%d, %s>\n", oneword->type, Operator[oneword->word]);
            fprintf(fp1, "<%d, %d>\n", oneword->type, oneword->word);//将种别码和在对应表中入口指针写入结果文件
        }
        else if(oneword->type == ERROR){
            printf("token: %c error!\n",code[p_code-1]);//报错
            oneword->type = INITIAL;
            continue;
        }
    }
    fclose(fp1);
    printf("\n---- LEXICAL ANALYSIS END----\n");
    return;
}
char programfile[] = "test.txt";
int main()
{
    LexicalAnalysis(programfile);
}
