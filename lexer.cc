/*
 * Copyright (C) Rida Bazzi, 2016
 *
 * Do not share this file with anyone
 */
#include <iostream>
#include <istream>
#include <vector>
#include <string>
#include <cctype>

#include "lexer.h"
#include "inputbuf.h"

using namespace std;

string reserved[] = { "END_OF_FILE",
    "IF", "WHILE", "DO", "THEN", "PRINT",
    "PLUS", "MINUS", "DIV", "MULT",
    "EQUAL", "COLON", "COMMA", "SEMICOLON",
    "LBRAC", "RBRAC", "LPAREN", "RPAREN",
    "NOTEQUAL", "GREATER", "LESS", "LTEQ", "GTEQ",
    "DOT", "NUM", "ID", "ERROR", "REALNUM", "BASE08NUM", "BASE16NUM" // TODO: Add labels for new token types here (as string)
};

#define KEYWORDS_COUNT 5
string keyword[] = { "IF", "WHILE", "DO", "THEN", "PRINT" };

void Token::Print()
{
    cout << "{" << this->lexeme << " , "
         << reserved[(int) this->token_type] << " , "
         << this->line_no << "}\n";
}

LexicalAnalyzer::LexicalAnalyzer()
{
    this->line_no = 1;
    tmp.lexeme = "";
    tmp.line_no = 1;
    tmp.token_type = ERROR;
}

bool LexicalAnalyzer::SkipSpace()
{
    char c;
    bool space_encountered = false;

    input.GetChar(c);
    line_no += (c == '\n');

    while (!input.EndOfInput() && isspace(c)) {
        space_encountered = true;
        input.GetChar(c);
        line_no += (c == '\n');
    }

    if (!input.EndOfInput()) {
        input.UngetChar(c);
    }
    return space_encountered;
}

bool LexicalAnalyzer::IsKeyword(string s)
{
    for (int i = 0; i < KEYWORDS_COUNT; i++) {
        if (s == keyword[i]) {
            return true;
        }
    }
    return false;
}

TokenType LexicalAnalyzer::FindKeywordIndex(string s)
{
    for (int i = 0; i < KEYWORDS_COUNT; i++) {
        if (s == keyword[i]) {
            return (TokenType) (i + 1);
        }
    }
    return ERROR;
}

// Token LexicalAnalyzer::ScanNumber()
// {
//     char c;
//     char c2;
//     char c3;
//     char c4;
//     char c5;

//     input.GetChar(c);
//     if (isdigit(c)) {
//         if (c == '0') {
//             tmp.lexeme = "0";
//         } else {
//             tmp.lexeme = "";
//             while (!input.EndOfInput() && isdigit(c)) {
//                 tmp.lexeme += c;
//                 input.GetChar(c);
//             }
//             if (!input.EndOfInput()) {
//                 input.UngetChar(c);
//             }
//         }
//         // TODO: You can check for REALNUM, BASE08NUM and BASE16NUM here!        
//         tmp.token_type = NUM;
//         tmp.line_no = line_no;

//         // if (!input.EndOfInput())
//         // {
//             input.GetChar(c);
//             // if (!input.EndOfInput())
//             // {
//                 input.GetChar(c2);
            
            
        
        

//         if (c == '.')
//         {
//             //might be a REALNUM. So, After DOT check for another digits
//             //Rule : REALNUM = NUM DOT digit digit*

//             if (isdigit(c2))
//             {
//                 //it is a REALNUM according to longest preference rule
//                 tmp.lexeme += c;
//                 tmp.lexeme += c2;

//                //start for seeing for the digits
//                while (!input.EndOfInput() && isdigit(c)) {
//                    tmp.lexeme += c;
//                    input.GetChar(c);
//                 }
//                 if (!input.EndOfInput()) {
//                    input.UngetChar(c);
//                 }
//                 //end

//                 tmp.token_type = REALNUM;
//                 tmp.line_no = line_no;

//             }else{
//                 //it's not a REALNUM so unget them
//                 //Ungetting in a reverse order as mentioned
//                 input.UngetChar(c2);
//                 input.UngetChar(c);
//             }
            
//         }
        


//         return tmp;
//     } else {
//         if (!input.EndOfInput()) {
//             input.UngetChar(c);
//         }
//         tmp.lexeme = "";
//         tmp.token_type = ERROR;
//         tmp.line_no = line_no;
//         return tmp;
//     }
// }


Token LexicalAnalyzer::ScanNumber()
{
    char c;
    char ch1 ,ch2 , ch3, ch4;
    string str1;

    input.GetChar(c);
    if (isdigit(c)) {
        if (c == '0') {
            tmp.lexeme = "0";
        } else {
            tmp.lexeme = "";
            while (!input.EndOfInput() && isdigit(c)) {
                tmp.lexeme += c;
                input.GetChar(c);
            }
            if (!input.EndOfInput()) {
                input.UngetChar(c);
            }
        }
        // TODO: You can check for REALNUM, BASE08NUM and BASE16NUM here!
        
        //REALNUM
        input.GetChar(ch1);
        if (ch1 == '.')
        {
            //Here, the input may be REALNUM
            input.GetChar(ch2);
            if (isdigit(ch2))
            {
                //is a REALNUM
                tmp.lexeme = tmp.lexeme + ch1;
                tmp.lexeme = tmp.lexeme + ch2;
                input.GetChar(ch3);
                while(!input.EndOfInput() && isdigit(ch3)){
                    tmp.lexeme = tmp.lexeme + ch3;
                    input.GetChar(ch3);
                }
                if (!input.EndOfInput())
                {
                    input.UngetChar(ch3);
                }
                tmp.token_type = REALNUM;
                tmp.line_no = line_no;
                return tmp;
            }else{
                //not a REALNUM
                input.UngetChar(ch2);
                input.UngetChar(ch1);
            }
        }else{
             input.UngetChar(ch1);
        }

        //BASE08NUM
        input.GetChar(ch1);
        input.GetChar(ch2);
        input.GetChar(ch3);
        if ((ch1 == 'x') && (ch2 == '0') && (ch3 == '8'))
        {
            str1 = tmp.lexeme;
            //checking if this string is a valid BASE08NUM
            //We can Check because 
            //NUM       = ((pdigit . (digit)*) + 0)
            //BASE08NUM = ((pdigit8 . (digit8)*) + 0) (x) (08)
            //Here the BASE08NUM's numbers are nothing but the Subset of NUM plus x08
            //Hence, traverse through the string and check it
            if (str1 == "0")
            {
                tmp.lexeme = tmp.lexeme + ch1 + ch2 + ch3;
                tmp.token_type = BASE08NUM;
                tmp.line_no = line_no;
                return tmp;
            }else{
                int flag = 0;
                for (int i = 0; i < str1.length(); i++)
                {
                    if (((str1[i] >= '0') && (str1[i] <= '7')))
                    {
                        //if true then we need to continue
                        continue;
                    }else{
                        //we need to mark the flag as one and break as it's not BASE08NUM
                        flag = 1;
                        break;
                    }
                } 

                //according to flag make a decision
                if (!flag)
                {
                    //Is a BASE08NUM
                    tmp.lexeme = tmp.lexeme + ch1 + ch2 + ch3;
                    tmp.token_type = BASE08NUM;
                    tmp.line_no = line_no;
                    return tmp;
                }else{
                    //Not a BASE08NUM
                    input.UngetChar(ch3);
                    input.UngetChar(ch2);
                    input.UngetChar(ch1);
                }
                
            }
            
        }else{
            input.UngetChar(ch3);
            input.UngetChar(ch2);
            input.UngetChar(ch1);
        }


        //BASE16NUM
        //Here, only pdigit and digit x16 will be present (No Letters and only Numbers)
        //We will check for even Letters in the further code
        //Idea: For BASE16NUM containing A,B,C, etc , it should start with a Number or else those numbers which will
              //start with alphabets will be counted as IDs because IDs should be given preference according to the order.
              //Hence, here, we can also check for 
        input.GetChar(ch1);
        input.GetChar(ch2);
        input.GetChar(ch3);
        if ((ch1 == 'x') && (ch2 == '1') && (ch3 == '6'))
        {
            str1 = tmp.lexeme;
            
            tmp.lexeme = tmp.lexeme + ch1 + ch2 + ch3;
            tmp.token_type = BASE16NUM;
            tmp.line_no = line_no;
            return tmp;
            
        }else{
            input.UngetChar(ch3);
            input.UngetChar(ch2);
            input.UngetChar(ch1);
        }


        //we can check for A,B,C,D,E,F over here.
        //The previous tmp lexeme should only be of numbers in this case.
        //The zero case is handled above.
        string tempstring = "";  //initializing empty string
        input.GetChar(c);
        if(c >= 'A' && c <= 'F'){

             while (!input.EndOfInput() && ( isdigit(c) || (c >= 'A' && c <= 'F'))) {
                tempstring += c;
                input.GetChar(c);
            }
            if (!input.EndOfInput()) {
                input.UngetChar(c);
            }

            input.GetChar(ch1);
            input.GetChar(ch2);
            input.GetChar(ch3);

            if ((ch1 == 'x') && (ch2 == '1') && (ch3 == '6'))
            {
                tmp.lexeme = tmp.lexeme + tempstring + ch1 + ch2 + ch3;
                tmp.token_type = BASE16NUM;
                tmp.line_no = line_no;
                return tmp;
            }else{
                input.UngetChar(ch3);
                input.UngetChar(ch2);
                input.UngetChar(ch1);
                for (int i = tempstring.length() - 1; i >= 0; i--)
                {
                    input.UngetChar(tempstring[i]);
                }
                
            }
            
        }else{
            input.UngetChar(c);
        }


        
        tmp.token_type = NUM;
        tmp.line_no = line_no;
        return tmp;
    } else {
        if (!input.EndOfInput()) {
            input.UngetChar(c);
        }
        tmp.lexeme = "";
        tmp.token_type = ERROR;
        tmp.line_no = line_no;
        return tmp;
    }
}


Token LexicalAnalyzer::ScanIdOrKeyword()
{
    char c;
    input.GetChar(c);

    if (isalpha(c)) {
        tmp.lexeme = "";
        while (!input.EndOfInput() && isalnum(c)) {
            tmp.lexeme += c;
            input.GetChar(c);
        }
        if (!input.EndOfInput()) {
            input.UngetChar(c);
        }
        tmp.line_no = line_no;
        if (IsKeyword(tmp.lexeme))
            tmp.token_type = FindKeywordIndex(tmp.lexeme);
        else
            tmp.token_type = ID;
    } else {
        if (!input.EndOfInput()) {
            input.UngetChar(c);
        }
        tmp.lexeme = "";
        tmp.token_type = ERROR;
    }
    return tmp;
}

// you should unget tokens in the reverse order in which they
// are obtained. If you execute
//
//    t1 = lexer.GetToken();
//    t2 = lexer.GetToken();
//    t3 = lexer.GetToken();
//
// in this order, you should execute
//
//    lexer.UngetToken(t3);
//    lexer.UngetToken(t2);
//    lexer.UngetToken(t1);
//
// if you want to unget all three tokens. Note that it does not
// make sense to unget t1 without first ungetting t2 and t3
//
TokenType LexicalAnalyzer::UngetToken(Token tok)
{
    tokens.push_back(tok);;
    return tok.token_type;
}

Token LexicalAnalyzer::GetToken()
{
    char c;

    // if there are tokens that were previously
    // stored due to UngetToken(), pop a token and
    // return it without reading from input
    if (!tokens.empty()) {
        tmp = tokens.back();
        tokens.pop_back();
        return tmp;
    }

    SkipSpace();
    tmp.lexeme = "";
    tmp.line_no = line_no;
    input.GetChar(c);
    switch (c) {
        case '.':
            tmp.token_type = DOT;
            return tmp;
        case '+':
            tmp.token_type = PLUS;
            return tmp;
        case '-':
            tmp.token_type = MINUS;
            return tmp;
        case '/':
            tmp.token_type = DIV;
            return tmp;
        case '*':
            tmp.token_type = MULT;
            return tmp;
        case '=':
            tmp.token_type = EQUAL;
            return tmp;
        case ':':
            tmp.token_type = COLON;
            return tmp;
        case ',':
            tmp.token_type = COMMA;
            return tmp;
        case ';':
            tmp.token_type = SEMICOLON;
            return tmp;
        case '[':
            tmp.token_type = LBRAC;
            return tmp;
        case ']':
            tmp.token_type = RBRAC;
            return tmp;
        case '(':
            tmp.token_type = LPAREN;
            return tmp;
        case ')':
            tmp.token_type = RPAREN;
            return tmp;
        case '<':
            input.GetChar(c);
            if (c == '=') {
                tmp.token_type = LTEQ;
            } else if (c == '>') {
                tmp.token_type = NOTEQUAL;
            } else {
                if (!input.EndOfInput()) {
                    input.UngetChar(c);
                }
                tmp.token_type = LESS;
            }
            return tmp;
        case '>':
            input.GetChar(c);
            if (c == '=') {
                tmp.token_type = GTEQ;
            } else {
                if (!input.EndOfInput()) {
                    input.UngetChar(c);
                }
                tmp.token_type = GREATER;
            }
            return tmp;
        default:
            if (isdigit(c)) {
                input.UngetChar(c);
                return ScanNumber();
            } else if (isalpha(c)) {
                input.UngetChar(c);
                return ScanIdOrKeyword();
            } else if (input.EndOfInput())
                tmp.token_type = END_OF_FILE;
            else
                tmp.token_type = ERROR;

            return tmp;
    }
}

int main()
{
    LexicalAnalyzer lexer;
    Token token;

    token = lexer.GetToken();
    token.Print();
    while (token.token_type != END_OF_FILE)
    {
        token = lexer.GetToken();
        token.Print();
    }
}