/*	Augustine Collins
*	Systems Program and Design Compiler:
*		Part 1 | Lexical Analyzer
*	Parser
*		Part 1 | Parsing Declarations
*	9/16/2021
*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
#include <stack>

using namespace std;

struct entry {
	string name;
	int type;
	int value;
	int size;
	int elType;
};

struct quadruples {
	string oper;
	string op1;
	string op2;
	string res;
};

struct list {
	string value;
	int type;
};

string filename("sourceProgram.txt");

int STsize = 0;
int hashVal = 0;
vector<entry> ST[100];
entry hashEntry;
vector<list> quads;
list bundle;
vector<quadruples> quadrupleList;
quadruples quadlist;

struct token {
	int type;
	int value;
};

token insertToken;
vector<token> tokenList;

int constFlag = 0;
int intFlag = 0;
int charFlag = 0;
int flag = 0;
int startingPos = 0;
int start;
int ending;
int parenthflag;

int mul = 0;
int dv = 0;
int par = 0;
int add = 0;
int sub = 0;
int linestart;
int lineend;
int parcnt = 0;
int mulcnt = 0;
int addcnt = 0;
int subcnt = 0;

stack<string> tempvars;
int tempFlag = 0;

bool stLookup() {
	for (int i = 0; i < ST[hashVal].size(); i++) {
		if (hashEntry.name == ST[hashVal][i].name) {
			insertToken.type = ST[hashVal][i].type;
			insertToken.value = ST[hashVal][i].value;
			tokenList.push_back(insertToken);
			flag = 1;
			break;
		}
	}
	if (flag == 1) {
		flag = 0;
		return true;
	}
	else {
		ST[hashVal].push_back(hashEntry);
		STsize++;
		insertToken.type = hashEntry.type;
		insertToken.value = hashEntry.value;
		tokenList.push_back(insertToken);
		return false;
	}
}

bool tokenListChecker(int type, int ogValue, int newVal) {
	for (int i = 0; i < tokenList.size(); i++) {
		if (tokenList[i].type == type && tokenList[i].value == ogValue) {
			tokenList[i].value = newVal;
		}
	}
	return true;
}

bool ADDtemps() {
	string temp = "temp";
	temp.append(to_string(tempFlag));
	tempFlag = tempFlag + 1;
	tempvars.push(temp);
	return true;
}

bool DELtemps() {
	while (!tempvars.empty()) {
		tempvars.pop();
	}
	tempFlag = 0;
	return true;
}

int lexicalAnylizer()
{
	char ch;
	int count = 0;
	string token = "";
	int tokenValue = 0;

	string keywords[19] = { "else", "endl", "for", "goto", "int", "if", "include", "iostream", "main", "namespace",
		"return", "std", "using", "void", "char", "const" ,"while", "cin", "cout" };



	//NAME
	const int INT = 101;	//integer
	const int INC = 102;	//integer constant
	const int INV = 103;	//integer variable
	const int CHA = 104;	//character
	const int CHC = 105;	//character constant
	const int CHV = 106;	//character variable
	const int STR = 107;	//string
	const int STC = 108;	//string constant
	const int STV = 109;	//string variable
	const int RSW = 111;	//Reserved Word
	const int OPR = 112;	//Operator
	const int PUN = 113;	//Punctuation
	const int CAR = 114;	//Char Array
	const int IAR = 115;	//Int Array

	//RESERVED WORDS
	const int MN = 201;	//main
	const int VD = 202;	//void
	const int IF = 203;	//if
	const int WH = 204;	//while
	const int CT = 205; //cout
	const int RT = 206;	//return
	const int IN = 207;	//include
	const int US = 208;	//using
	const int NS = 209;	//namespace

	//OPERATORS
	const int PS = 301;	//+
	const int PP = 302;	//++
	const int SB = 303;	//-
	const int SS = 304;	//--
	const int ML = 305; //*
	const int DV = 306;	///
	const int GT = 307; //>
	const int LT = 308; //<
	const int DE = 309; //==
	const int NE = 310; //!=
	const int LE = 311; //<=
	const int GE = 312; //>=
	const int NT = 313; //!
	const int DA = 314; //&&
	const int OR = 315; //||
	const int EQ = 316; //=

	//PUNCTUATION
	const int SM = 401;	//;
	const int LP = 402;	//(
	const int RP = 403;	//)
	const int LB = 404;	//[
	const int RB = 405;	//]
	const int CM = 406; //,
	const int PR = 407; //.
	const int LC = 408;	//{
	const int RC = 409; //}
	const int HT = 410; //#
	const int DQ = 411; //"
	const int SQ = 412; //'


	const char space = 1, digit = 2, letter = 4, punctuation = 8, oper = 16;
	unsigned int chars[256] = { 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,
		1, 16, 8, 8, 0, 0, 16, 8, 8, 8, 16, 16, 8, 16, 8, 16, 2,2,2,2,2,2,2,2,2,2,
		0, 8, 16, 16, 16, 0, 0, 4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,
		8, 0, 8, 0, 0, 0, 4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4, 8, 16,
		8, 0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0 };

	//C:/Users/Augustine/OneDrive/Desktop/sourceprogram.txt" || C:/Users/jcoln/OneDrive/Desktop/sourceProgram.txt
	ifstream input_file("C:/Users/jcoln/OneDrive/Desktop/sourceProgram.txt");
	if (!input_file.is_open()) {
		cerr << "Could not open the file - '"
			<< filename << "'" << endl;
		return EXIT_FAILURE;
	}

	input_file.get(ch);

	//cout << "********** SCANNER OUTPUT ***********" << endl;

	while (!input_file.eof()) {	//If not end of file
		if (chars[ch] == 1) {	//finds blanks
			token = "";
			hashVal = 0;
			input_file.get(ch);
		}
		else {
			switch (chars[ch]) {
				//********************************************************************************************************************
			case 8:						//PUNCTUATION
				token = token + ch;	//If present, print entry in hash table, else add to hash table and print.
				switch (ch) {
				case ';':
					constFlag = 0;
					intFlag = 0;
					charFlag = 0;
					//cout << 113 << '\t' << int(ch) << '\t' << ch << endl;
					insertToken.type = 113;
					insertToken.value = int(ch);
					tokenList.push_back(insertToken);
					token = ch;
					bundle.value = token;
					bundle.type = 113;
					quads.push_back(bundle);
					break;
				case '(':
					//cout << 113 << '\t' << int(ch) << '\t' << ch << endl;
					insertToken.type = 113;
					insertToken.value = int(ch);
					tokenList.push_back(insertToken);
					token = ch;
					bundle.value = token;
					bundle.type = 113;
					quads.push_back(bundle);
					break;
				case ')':
					//cout << 113 << '\t' << int(ch) << '\t' << ch << endl;
					insertToken.type = 113;
					insertToken.value = int(ch);
					tokenList.push_back(insertToken);
					token = ch;
					bundle.value = token;
					bundle.type = 113;
					quads.push_back(bundle);
					break;
				case '[':
					//cout << 113 << '\t' << int(ch) << '\t' << ch << endl;
					insertToken.type = 113;
					insertToken.value = int(ch);
					tokenList.push_back(insertToken);
					token = ch;
					bundle.value = token;
					bundle.type = 113;
					quads.push_back(bundle);
					break;
				case ']':
					//cout << 113 << '\t' << int(ch) << '\t' << ch << endl;
					insertToken.type = 113;
					insertToken.value = int(ch);
					tokenList.push_back(insertToken);
					token = ch;
					bundle.value = token;
					bundle.type = 113;
					quads.push_back(bundle);
					break;
				case ',':
					//cout << 113 << '\t' << int(ch) << '\t' << ch << endl;
					insertToken.type = 113;
					insertToken.value = int(ch);
					tokenList.push_back(insertToken);
					token = ch;
					bundle.value = token;
					bundle.type = 113;
					quads.push_back(bundle);
					break;
				case '.':
					//cout << 113 << '\t' << int(ch) << '\t' << ch << endl;
					insertToken.type = 113;
					insertToken.value = int(ch);
					tokenList.push_back(insertToken);
					token = ch;
					bundle.value = token;
					bundle.type = 113;
					quads.push_back(bundle);
					break;
				case '{':
					//cout << 113 << '\t' << int(ch) << '\t' << ch << endl;
					insertToken.type = 113;
					insertToken.value = int(ch);
					tokenList.push_back(insertToken);
					token = ch;
					bundle.value = token;
					bundle.type = 113;
					quads.push_back(bundle);
					break;
				case '}':
					//cout << 113 << '\t' << int(ch) << '\t' << ch << endl;
					insertToken.type = 113;
					insertToken.value = int(ch);
					tokenList.push_back(insertToken);
					token = ch;
					bundle.value = token;
					bundle.type = 113;
					quads.push_back(bundle);
					break;
				case '#':
					//cout << 113 << '\t' << int(ch) << '\t' << ch << endl;
					insertToken.type = 113;
					insertToken.value = int(ch);
					tokenList.push_back(insertToken);
					token = ch;
					bundle.value = token;
					bundle.type = 113;
					quads.push_back(bundle);
					break;
				case '"':
					token = "";
					hashVal = 0;
					count++;
					while (count != 0) {
						input_file.get(ch);
						if (ch != '"') {
							token = token + ch;
							hashVal = token.length() * 2 % 10;
						}
						else {
							count--;
						}
					}
					hashEntry.name = token;
					hashEntry.type = 107;
					hashEntry.value = token.length() % 10;		//VALUE LINE
					hashVal = token.length() * 2 % 10;
					if (stLookup() == false) {
						ST[hashVal].push_back(hashEntry);
						STsize++;
					}
					//cout << "107" << '\t' << token.length() % 10 << '\t' << token << endl;
					bundle.value = token;
					bundle.type = 107;
					quads.push_back(bundle);
					break;
				default:
					if (int(ch) == 39 && chars[input_file.peek()] == 4) {	//If next value is single character or number
						input_file.get(ch);
						insertToken.type = 104;
						insertToken.value = int(ch);
						tokenList.push_back(insertToken);
						//cout << "104" << '\t' << int(ch) << '\t' << ch << endl;
						token = ch;
						bundle.type = 104;
						bundle.value = token;
						quads.push_back(bundle);
					}
					break;
				}
				input_file.get(ch);
				token = "";
				break;
				//********************************************************************************************************************
			case 16:								//OPERATORS
				token = token + ch;
				hashVal = token.length() * 2 % 10;
				switch (ch) {
				case '+':
					if (input_file.peek() == int('+')) {
						//cout << 112 << '\t' << int(ch + input_file.peek()) << '\t' << ch << ch << endl;
						insertToken.type = 112;
						insertToken.value = int(ch) * 2;
						tokenList.push_back(insertToken);
						token = ch;
						bundle.value = token;
						bundle.type = 112;
						quads.push_back(bundle);
						input_file.get(ch);
					}
					else {
						//cout << 112 << '\t' << int(ch) << '\t' << ch << endl;
						insertToken.type = 112;
						insertToken.value = int(ch);
						tokenList.push_back(insertToken);
						token = ch;
						bundle.value = token;
						bundle.type = 112;
						quads.push_back(bundle);
					}
					break;
				case '-':
					if (input_file.peek() == int('-')) {
						//cout << 304 << '\t' << int(ch + input_file.peek()) << '\t' << ch << ch << endl;
						insertToken.type = 304;
						insertToken.value = int(ch) * 2;
						tokenList.push_back(insertToken);
						token = ch;
						bundle.value = token;
						bundle.type = 112;
						quads.push_back(bundle);
						input_file.get(ch);
					}
					else {
						//cout << 112 << '\t' << int(ch) << '\t' << ch << endl;
						insertToken.type = 112;
						insertToken.value = int(ch);
						tokenList.push_back(insertToken);
						token = ch;
						bundle.value = token;
						bundle.type = 112;
						quads.push_back(bundle);
					}
					break;
				case '*':
					//cout << 112 << '\t' << int(ch) << '\t' << ch << endl;
					insertToken.type = 112;
					insertToken.value = int(ch);
					tokenList.push_back(insertToken);
					token = ch;
					bundle.value = token;
					bundle.type = 112;
					quads.push_back(bundle);
					break;
				case '/':
					if (input_file.peek() == '/') {
						while (ch != '\n') {
							input_file.get(ch);
						}
						break;
					}
					if (input_file.peek() == int('*')) {	//Checks if start of a multi-line comment
						while (ch != '*' || input_file.peek() != int('/')) {
							input_file.get(ch);
						}
						input_file.get(ch);
					}
					else {
						//cout << 112 << '\t' << int(ch) << '\t' << ch << endl;
						insertToken.type = 112;
						insertToken.value = int(ch);
						tokenList.push_back(insertToken);
						token = ch;
						bundle.value = token;
						bundle.type = 112;
						quads.push_back(bundle);
					}
					break;
				case '>':
					if (input_file.peek() == int('>')) {
						//cout << 112 << '\t' << int(ch + input_file.peek()) << '\t' << ch << ch << endl;
						insertToken.type = 112;
						insertToken.value = int(ch) * 2;
						tokenList.push_back(insertToken);
						token = ch;
						token = token + ch;
						bundle.value = token;
						bundle.type = 112;
						quads.push_back(bundle);
						input_file.get(ch);
					}
					else if (input_file.peek() == int('=')) {
						//cout << 112 << '\t' << int(ch + input_file.peek()) << '\t' << ch << '=' << endl;
						insertToken.type = 112;
						insertToken.value = int(ch) + int(input_file.peek());
						tokenList.push_back(insertToken);
						token = ch;
						bundle.value = token;
						bundle.type = 112;
						quads.push_back(bundle);
						input_file.get(ch);
					}
					else {
						//cout << 112 << '\t' << int(ch) << '\t' << ch << endl;
						insertToken.type = 112;
						insertToken.value = int(ch);
						tokenList.push_back(insertToken);
						token = ch;
						bundle.value = token;
						bundle.type = 112;
						quads.push_back(bundle);
					}
					break;
				case '<':
					if (input_file.peek() == int('<')) {
						//cout << 112 << '\t' << int(ch + input_file.peek()) << '\t' << ch << ch << endl;
						insertToken.type = 112;
						insertToken.value = int(ch) * 2;
						tokenList.push_back(insertToken);
						token = ch;
						token = token + ch;
						bundle.value = token;
						bundle.type = 112;
						quads.push_back(bundle);
						input_file.get(ch);
					}
					else {
						//cout << 112 << '\t' << int(ch) << '\t' << ch << endl;
						insertToken.type = 112;
						insertToken.value = int(ch);
						tokenList.push_back(insertToken);
						token = ch;
						bundle.value = token;
						bundle.type = 112;
						quads.push_back(bundle);
					}
					break;
				case '!':
					if (input_file.peek() == int('=')) {
						//cout << 112 << '\t' << int(ch + input_file.peek()) << '\t' << ch << char(input_file.peek()) << endl;
						insertToken.type = 112;
						insertToken.value = int(ch) + int(input_file.peek());
						tokenList.push_back(insertToken);
						token = ch;
						bundle.value = token;
						bundle.type = 112;
						quads.push_back(bundle);
						input_file.get(ch);
					}
					else {
						//cout << 112 << '\t' << int(ch) << '\t' << ch << endl;
						insertToken.type = 112;
						insertToken.value = int(ch);
						tokenList.push_back(insertToken);
						token = ch;
						bundle.value = token;
						bundle.type = 112;
						quads.push_back(bundle);
					}
					break;
				case '&':
					if (input_file.peek() == int('&')) {
						//cout << 112 << '\t' << int(ch + ch) << '\t' << ch << ch << endl;
						insertToken.type = 112;
						insertToken.value = int(ch) * 2;
						tokenList.push_back(insertToken);
						token = ch;
						bundle.value = token;
						bundle.type = 112;
						quads.push_back(bundle);
						input_file.get(ch);
					}
					else {
						cout << "ILLEGAL CHARACTER" << endl;
					}
					break;
				case '|':
					if (input_file.peek() == int('|')) {
						//cout << 112 << '\t' << int(ch + ch) << '\t' << ch << ch << endl;
						insertToken.type = 112;
						insertToken.value = int(ch) * 2;
						tokenList.push_back(insertToken);
						token = ch;
						bundle.value = token;
						bundle.type = 112;
						quads.push_back(bundle);
						input_file.get(ch);
					}
					else {
						cout << "ILLEGAL CHARACTER" << endl;
					}
					break;
				case '=':
					if (input_file.peek() == int('=')) {
						//cout << 112 << '\t' << int(ch + input_file.peek()) << '\t' << ch << ch << endl;
						insertToken.type = 112;
						insertToken.value = int(ch) * 2;
						tokenList.push_back(insertToken);
						token = ch;
						bundle.value = token;
						bundle.type = 112;
						quads.push_back(bundle);
						input_file.get(ch);
					}
					else {
						//cout << 112 << '\t' << int(ch) << '\t' << ch << endl;
						insertToken.type = 112;
						insertToken.value = int(ch);
						tokenList.push_back(insertToken);
						token = ch;
						bundle.value = token;
						bundle.type = 112;
						quads.push_back(bundle);
					}
					break;
				default:
					cout << "NOT CODED YET. SORRY :(" << endl;
					break;
				}
				token = "";
				hashVal = 0;
				input_file.get(ch);
				break;
				//********************************************************************************************************************
			case 2:							//DIGITS
				while (chars[ch] == 2) {
					token = token + ch;
					input_file.get(ch);
				}
				//cout << "101" << '\t' << token << endl;
				insertToken.type = 101;
				insertToken.value = stoi(token);
				tokenValue = 0;
				tokenList.push_back(insertToken);
				bundle.value = token;
				bundle.type = 101;
				quads.push_back(bundle);
				break;
				//********************************************************************************************************************
			case 4:							//LETTERS
				while (chars[input_file.peek()] != 16 && chars[input_file.peek()] != 1 && chars[input_file.peek()] != 8 && input_file.peek() != int('\n')) {
					token = token + ch;
					input_file.get(ch);
				}
				if (chars[input_file.peek()] == 4) {
					while (input_file.peek() == int(']')) {
						token = token + ch;
						input_file.get(ch);
					}
				}
				token = token + ch;
				hashEntry.name = token;

				if (constFlag == 1 && intFlag == 1) {
					hashEntry.type = 102;
				}
				else if (constFlag == 1 && charFlag == 1) {
					hashEntry.type = 105;
				}
				//PROBLEM AREA
				else if (constFlag == 0 && intFlag == 1) {
					hashEntry.type = 103;
				}
				//END PROBLEM AREA
				else if (constFlag == 0 && charFlag == 1) {
					hashEntry.type = 106;
				}


				for (int i = 0; i < token.length(); i++) {
					tokenValue = int(token.at(i)) * 3.1415 + tokenValue;
				}
				tokenValue = tokenValue % 155;
				hashEntry.value = tokenValue;		//VALUE LINE
				hashVal = token.length() * 3.24984 + (int(token.at(0)) % 3);
				hashEntry.value = tokenValue + (hashVal * 2);		//VALUE LINE
				for (int i = 0; i < 18; i++) {
					if (token == keywords[i]) {
						hashEntry.type = 111;
						break;
					}
				}
				stLookup();

				//DEFINING TYPE SECTION
				if (token == "const") {
					//cout << 111 << '\t' << tokenValue + (hashVal * 2) << '\t' << token << endl;
					bundle.value = token;
					bundle.type = 111;
					quads.push_back(bundle);
					constFlag = 1;
					input_file.get(ch);
					break;
				}
				else if (token == "int")
				{
					//cout << 111 << '\t' << tokenValue + (hashVal * 2) << '\t' << token << endl;
					bundle.value = token;
					bundle.type = 111;
					quads.push_back(bundle);
					intFlag = 1;
					input_file.get(ch);
					break;
				}
				else if (token == "char") {
					//cout << 111 << '\t' << tokenValue + (hashVal * 2) << '\t' << token << endl;
					bundle.value = token;
					bundle.type = 111;
					quads.push_back(bundle);
					charFlag = 1;
					input_file.get(ch);
					break;
				}

				if (constFlag == 1 && intFlag == 1) {
					//cout << 102 << '\t' << tokenValue + (hashVal * 2) << '\t' << token << endl;
					bundle.value = token;
					bundle.type = 102;
					quads.push_back(bundle);
				}
				else if (constFlag == 1 && charFlag == 1) {
					//cout << 105 << '\t' << tokenValue + (hashVal * 2) << '\t' << token << endl;
					bundle.value = token;
					bundle.type = 105;
					quads.push_back(bundle);
				}
				else if (constFlag == 0) {
					if (intFlag == 1) {
						//cout << hashEntry.type << '\t' << tokenValue + (hashVal * 2) << '\t' << token << endl;
						bundle.value = token;
						bundle.type = 103;
						quads.push_back(bundle);
						input_file.get(ch);
						break;
					}
					if (charFlag == 1) {
						//cout << 106 << '\t' << tokenValue + (hashVal * 2) << '\t' << token << endl;
						bundle.value = token;
						bundle.type = 106;
						quads.push_back(bundle);
						input_file.get(ch);
						break;
					}
					else {
						//cout << 111 << '\t' << tokenValue + (hashVal * 2) << '\t' << token << endl;
						bundle.value = token;
						bundle.type = 111;
						quads.push_back(bundle);
					}
				}

				input_file.get(ch);
				break;
			case 0:
				if (ch == '\t' || ch == '\n') {
					input_file.get(ch);
				}
				else {
					cout << "ERROR" << endl;
					input_file.get(ch);
				}
			}
		}
		hashVal = 0;
		tokenValue = 0;
		token = "";
	}

	input_file.close();

	//cout << endl;
	//cout << endl;
	//cout << "********** SYMBOL TABLE **********" << endl;
	//cout << "hashVal" << '\t' << setw(20) << left << "name" << setw(20) << left << "type" << setw(20) << left << "value" << setw(20) << left
	//	<< "size" << setw(20) << left << "elType" << endl;

	////Print entire vector of array
	//for (int i = 0; i <= STsize; i++) {
	//	for (int t = 0; t < ST[i].size(); t++) {
	//		cout << i << '\t' << setw(20) << left << ST[i][t].name << setw(20) << left << ST[i][t].type << setw(20) << left << ST[i][t].value << setw(20)
	//			<< left << ST[i][t].size << setw(20) << left << ST[i][t].elType << endl;
	//	}
	//}

	return EXIT_SUCCESS;
}

//PARSER CODE ***********************************************************************************************************************
int parser() {

	for (int i = 0; i < tokenList.size(); i++) {

		//break when main
		if (tokenList[i].type == 111 && tokenList[i].value == 106) {
			i = tokenList.size() - 1;
		}

		switch (tokenList[i].type) {
		case 101:	//Integer
			if (tokenList[i].type != 112 && tokenList[i + 1].type != 113) {
				cout << "ERROR: EXPECTING OPERATOR OR PUNCTUATION at Token: " << i + 1 << endl;
			}
			break;
		case 102:	//Integer Constant
			if (tokenList[i + 1].type == 112 && tokenList[i + 1].value == 61) {
				if (tokenList[i + 2].type == 101 || tokenList[i + 2].type == 102 || tokenList[i + 2].type == 103) {
					//WORKING SYMBOL TABLE ACCESSOR
					for (int s = 0; s < 100; s++) {
						for (int a = 0; a < ST[s].size(); a++) {
							if (ST[s][a].value == tokenList[i].value && ST[s][a].type == tokenList[i].type) {
								ST[s][a].value = tokenList[i + 2].value;
								tokenListChecker(tokenList[i].type, tokenList[i].value, tokenList[i + 2].value);
							}
						}
					}
				}
				else {
					cout << "Error: Cannot assign to type const int at: token  " << i + 1 << endl;
				}
			}
			break;
		case 103:	//Integer Variable
			if (tokenList[i + 1].type == 112) {
				if (tokenList[i + 1].value != 61) {
					cout << "ERROR" << endl;
				}
			}
			if (tokenList[i + 1].type != 113) {
				if (tokenList[i + 1].value != 61) {
					cout << "ERROR" << endl;
				}
			}
			if (tokenList[i + 1].type == 113) {
				if ((tokenList[i + 1].type != 113 && tokenList[i + 1].value != 59) && tokenList[i + 2].type != 101 && tokenList[i + 2].type != 102 && tokenList[i + 2].type != 103) {
					cout << "ERROR: Expecting integer or integer variable: token " << i + 1 << endl;
					break;
				}
			}
			if (tokenList[i + 1].type == 112 && tokenList[i + 1].value == 61) {
				if (tokenList[i + 2].type == 101 || tokenList[i + 2].type == 102 || tokenList[i + 2].type == 103 || tokenList[i + 2].type == 105) {
					//WORKING SYMBOL TABLE ACCESSOR
					for (int s = 0; s < 100; s++) {
						for (int a = 0; a < ST[s].size(); a++) {
							if (ST[s][a].value == tokenList[i].value && ST[s][a].type == tokenList[i].type) {
								ST[s][a].value = tokenList[i + 2].value;
								tokenListChecker(tokenList[i].type, tokenList[i].value, tokenList[i + 2].value);
							}
						}
					}
				}
				else {
					cout << "ERROR: Cannot assign to type int at: token " << i + 1 << endl;
				}
			}
			break;
		case 104:	//Char character
			if (tokenList[i].type != 112 && tokenList[i + 1].type != 113) {
				cout << "ERROR" << endl;
			}
			break;
		case 105:	//Character constant
			if (tokenList[i + 1].type == 112 && tokenList[i + 1].value == 61) {
				if (tokenList[i + 2].type == 104 || tokenList[i + 2].type == 105 || tokenList[i + 2].type == 106) {
					//WORKING SYMBOL TABLE ACCESSOR
					for (int s = 0; s < 100; s++) {
						for (int a = 0; a < ST[s].size(); a++) {
							if (ST[s][a].value == tokenList[i].value && ST[s][a].type == tokenList[i].type) {
								ST[s][a].value = tokenList[i + 2].value;
								tokenListChecker(tokenList[i].type, tokenList[i].value, tokenList[i + 2].value);
							}
						}
					}
				}
				else {
					cout << "ERROR: Cannot assign to type const char at: token " << i + 1 << endl;
				}
			}

			break;
		case 106:	//Char variable
			if (tokenList[i + 1].type == 112 && tokenList[i + 1].value == 61) {
				if (tokenList[i + 2].type == 104 || tokenList[i + 2].type == 105 || tokenList[i + 2].type == 106) {
					//WORKING SYMBOL TABLE ACCESSOR
					for (int s = 0; s < 100; s++) {
						for (int a = 0; a < ST[s].size(); a++) {
							if (ST[s][a].value == tokenList[i].value && ST[s][a].type == tokenList[i].type) {
								ST[s][a].value = tokenList[i + 2].value;
								tokenListChecker(tokenList[i].type, tokenList[i].value, tokenList[i + 2].value);
							}
						}
					}
				}
				else {
					cout << "ERROR: Cannot assign to type char at: token " << i + 1 << endl;
				}
			}
			break;
		case 111:
			switch (tokenList[i].value) {
			case 126:	//int
				if ((tokenList[i + 1].type != 103 && tokenList[i + 1].type != 102) && tokenList[i + 1].value != 153 && (tokenList[i + 1].type != 111 && tokenList[i + 1].value != 106))
				{
					cout << "ERROR: Invalid Integer Variable: token " << i + 2 << endl;
				}
				break;
			case 56:	//const
				if ((tokenList[i + 1].type != 111 && tokenList[i + 1].value != 3) && (tokenList[i + 1].type == 111 && tokenList[i + 1].value == 14)) {	//int
					cout << "ERROR: Const can only be followed by int or char: token " << i + 2 << endl;
				}
				break;
			case 196:	//include
				if (tokenList[i + 1].type != 112 && tokenList[i + 1].value != 60) {
					cout << "ERROR: Must follow include with < at token: " << i + 2 << endl;
				}
				break;
			case 138:	//iostream
				if (tokenList[i + 1].type != 112 && tokenList[i + 1].value != 62) {
					cout << "ERROR: Must follow with > at token: " << i + 2 << endl;
				}
				break;
			case 81:	//std
				if (tokenList[i + 1].type != 113 && tokenList[i + 1].value != 59)
				{
					cout << "ERROR: Must follow std with ;: token " << i + 2 << endl;
				}
				break;
			case 106:	//main
				if (tokenList[i + 1].type != 113 && tokenList[i + 1].value != 40)
				{
					cout << "ERROR: Must follow main with (: token " << i + 2 << endl;
				}
				break;
			case 69:	//namespace
				if (tokenList[i + 1].type != 111 && tokenList[i + 1].value != 11) {
					cout << "ERROR: Expecting std at token: " << i + 2 << endl;
				}
				break;
			case 132:	//Bad return code
				if (tokenList[i + 1].type != 101 || (tokenList[i + 1].value != 0 && tokenList[i + 1].value != 1)) {
					cout << "ERROR: BAD RETURN CODE" << endl;
				}
				break;
			case 129:	//std
				if (tokenList[i + 1].type != 113 && tokenList[i + 1].value != 59) {
					cout << "ERROR: Semi-colon expected at token: " << i + 2 << endl;
				}
				break;
			case 52:	//using
				if (tokenList[i + 1].type != 111 && tokenList[i + 1].value != 9) {
					cout << "ERROR: Expecting namespace at: token " << i + 2 << endl;
				}
				break;
			case 83:	//char
				if (tokenList[i + 1].type != 105 && tokenList[i + 1].type != 106) {
					cout << "ERROR: Expecting char variable or char constant: token " << i + 2 << endl;
					break;
				}
				break;
			case 14:	//char variable
				if (tokenList[i + 1].type != 106) {
					cout << "ERROR: Expecting constant char variable: token " << i + 2 << endl;
				}
				break;
			case 170:	//while
				if (tokenList[i + 1].type != 113 && tokenList[i + 1].value != 40)
				{
					cout << "ERROR: Must follow while with (: token " << i + 2 << endl;
				}
				break;
			default:
				cout << "NOT CODED: " << tokenList[i].type << '\t' << tokenList[i].value << endl;
				break;
			}
			break;
		case 112:	//Operator
			if (tokenList[i].value == 60) {
				break;
			}
			if (tokenList[i].value == 62) {
				break;
			}
			break;
		case 113:	//Punctuation
			if (tokenList[i].value == 59) {	//Semi-colon
				break;
			}
			if (tokenList[i].value == 44) {
				if (tokenList[i + 1].type != 102 && tokenList[i + 1].type != 103 && tokenList[i + 1].type != 106) {
					cout << "ERROR: Semicolon excepted at: token " << i + 2 << endl;
					break;
				}
			}

			if (tokenList[i].value == 91) {
				if (tokenList[i + 1].type == 101 || tokenList[i + 1].type == 102 || tokenList[i + 1].type == 103) {
					if (tokenList[i + 2].value != 93) {
						cout << "ERROR: Missing Bracket at: Token " << i + 2 << endl;
						break;
					}
					//WORKING SYMBOL TABLE ACCESSOR
					for (int s = 0; s < 100; s++) {
						for (int a = 0; a < ST[s].size(); a++) {
							if (ST[s][a].value == tokenList[i - 1].value && ST[s][a].type == tokenList[i - 1].type) {
								ST[s][a].size = tokenList[i + 1].value;
								if (tokenList[i - 1].type == 102 || tokenList[i - 1].type == 103) {
									ST[s][a].elType = 115;
								}
								else {
									ST[s][a].elType = 114;
								}
							}
						}
					}
				}
				else {
					cout << "ERROR: ILLEGAL ARRAY VALUE" << endl;
				}
			}
			break;
		default:
			cout << tokenList[i].type << " " << tokenList[i].value << endl;
			break;
		}
	}

	//Parser Part 2

	for (int i = 0; i < quads.size(); i++) {
		if (quads[i].value == "main") {
			startingPos = i + 4;
			break;
		}
	}

	string operation;
	string op1;
	string op2;
	string res;

	//Loop for all lines in main
	for (int i = startingPos; i < quads.size(); i++) {
		if (quads[i].value == "=") {
			linestart = i;
			while (quads[i].value != ";") {
				if (quads[i].value == "(") {
					par = par + 1;
				}
				if (quads[i].value == "*") {
					mul = mul + 1;
				}
				if (quads[i].value == "/") {
					dv = dv + 1;
				}
				if (quads[i].value == "+") {
					add = add + 1;
				}
				if (quads[i].value == "-") {
					sub = sub + 1;
				}
				i++;
				lineend = i;
			}
			lineend = i;


			//Loop for all lines in main
			for (int i = linestart; i < lineend; i++) {
				if (quads[i].value == "=") {
					//DRIVER for line-by-line
					while (quads[i].value != ";") {
						//IF (
						while (par != 0) {
							if (par <= 1) {
								if (quads[i].value != "(") {
									i++;
								}
								else {
									operation = quads[i + 2].value;
									if (quads[i - 1].value == ")") {
										if (!tempvars.empty()) {
											op1 = tempvars.top();
											tempvars.pop();
										}
										else {
											ADDtemps();
											op1 = tempvars.top();
											tempvars.pop();
										}
									}
									else if (quads[i + 1].value == "(") {
										if (!tempvars.empty()) {
											op2 = tempvars.top();
											tempvars.pop();
										}
										else {
											ADDtemps();
											op2 = tempvars.top();
											tempvars.pop();
										}
									}
									if (operation == "*") {
										mul = mul - 1;
									}
									else if (operation == "/") {
										dv = dv - 1;
									}
									else if (operation == "+") {
										add = add - 1;
									}
									else if (operation == "-") {
										sub = sub - 1;
									}
									op1 = quads[i + 1].value;
									if (quads[i + 3].value == "(") {
										op2 = tempvars.top();
										tempvars.pop();
									}
									else {
										op2 = quads[i + 3].value;
									}
									ADDtemps();
									res = tempvars.top();
									quadlist.oper = operation;
									quadlist.op1 = op1;
									quadlist.op2 = op2;
									quadlist.res = res;
									quadrupleList.push_back(quadlist);
									i = linestart;	//Line reset
									operation = ""; op1 = ""; op2 = ""; res = "";
									parcnt = 0;
									par = par - 1;
								}
							}
							else {
								while (parcnt != par) {
									i++;
									if (quads[i].value == "(") {
										parcnt = parcnt + 1;
									}
								}
								operation = quads[i + 2].value;
								if (quads[i - 1].value == ")") {
									if (!tempvars.empty()) {
										op1 = tempvars.top();
										tempvars.pop();
									}
									else {
										ADDtemps();
										op1 = tempvars.top();
										tempvars.pop();
									}
								}
								else if (quads[i + 1].value == "(") {
									if (!tempvars.empty()) {
										op2 = tempvars.top();
										tempvars.pop();
									}
									else {
										ADDtemps();
										op2 = tempvars.top();
										tempvars.pop();
									}
								}
								if (operation == "*") {
									mul = mul - 1;
								}
								else if (operation == "/") {
									dv = dv - 1;
								}
								else if (operation == "+") {
									add = add - 1;
								}
								else if (operation == "-") {
									sub = sub - 1;
								}
								op1 = quads[i + 1].value;
								if (quads[i + 3].value == "(") {
									op2 = tempvars.top();
									tempvars.pop();
								}
								else {
									op2 = quads[i + 3].value;
								}
								ADDtemps();
								res = tempvars.top();
								quadlist.oper = operation;
								quadlist.op1 = op1;
								quadlist.op2 = op2;
								quadlist.res = res;
								quadrupleList.push_back(quadlist);
								i = linestart;	//Line reset
								operation = ""; op1 = ""; op2 = ""; res = "";
								par = par - 1;
								parcnt = 0;
							}
						}
						//IF *
						while (mul != 0) {
							if (mul <= 1) {
								if (quads[i].value != "*") {
									i++;
								}
								else {
									mul = mul - 1;
									operation = quads[i].value;
									if (quads[i - 1].value == ")" || quads[i + 1].value == "(") {
										if (quads[i - 1].value == ")") {
											if (!tempvars.empty()) {
												op1 = tempvars.top();
												tempvars.pop();
											}
											else {
												ADDtemps();
												op1 = tempvars.top();
												tempvars.pop();
											}
										}
										if (quads[i + 1].value == "(") {
											if (!tempvars.empty()) {
												op2 = tempvars.top();
												tempvars.pop();
											}
											else {
												ADDtemps();
												op2 = tempvars.top();
												tempvars.pop();
											}
										}
									}
									else {
										if (!tempvars.empty()) {
											op2 = tempvars.top();
											tempvars.pop();
										}
										else {
											op2 = quads[i + 1].value;
										}
										if (!tempvars.empty()) {
											op1 = tempvars.top();
											tempvars.pop();
										}
										else {
											op1 = quads[i - 1].value;
										}
										ADDtemps();
										res = tempvars.top();
										quadlist.oper = operation;
										quadlist.op1 = op1;
										quadlist.op2 = op2;
										quadlist.res = res;
										quadrupleList.push_back(quadlist);
										i = linestart;	//Line reset
										operation = ""; op1 = ""; op2 = ""; res = "";
										break;
									}
									if (op1.empty()) {
										op1 = quads[i - 1].value;
									}
									if (op2.empty()) {
										op2 = quads[i + 1].value;
									}
									ADDtemps();
									res = tempvars.top();
									quadlist.oper = operation;
									quadlist.op1 = op1;
									quadlist.op2 = op2;
									quadlist.res = res;
									quadrupleList.push_back(quadlist);
									i = linestart;	//Line reset
									operation = ""; op1 = ""; op2 = ""; res = "";
								}
							}
							else {
								while (mulcnt != mul) {
									i++;
									if (quads[i].value == "*") {
										mulcnt = mulcnt + 1;
									}
								}
								operation = quads[i].value;
								if (quads[i - 1].value == ")") {
									if (!tempvars.empty()) {
										op1 = tempvars.top();
										tempvars.pop();
									}
									else {
										ADDtemps();
										op1 = tempvars.top();
										tempvars.pop();
									}
								}
								else if (quads[i + 1].value == "(") {
									if (!tempvars.empty()) {
										op2 = tempvars.top();
										tempvars.pop();
									}
									else {
										ADDtemps();
										op2 = tempvars.top();
										tempvars.pop();
									}
								}
								op1 = quads[i - 1].value;
								if (quads[i + 1].value == "(") {
									op2 = tempvars.top();
									tempvars.pop();
								}
								else {
									if (!tempvars.empty()) {
										op2 = tempvars.top();
										tempvars.pop();
									}
									else {
										op2 = quads[i + 1].value;
									}
								}
								ADDtemps();
								res = tempvars.top();
								quadlist.oper = operation;
								quadlist.op1 = op1;
								quadlist.op2 = op2;
								quadlist.res = res;
								quadrupleList.push_back(quadlist);
								i = linestart;	//Line reset
								operation = ""; op1 = ""; op2 = ""; res = "";
								mul = mul - 1;
								mulcnt = 0;
							}
						}
						//IF /
						while (dv != 0) {
							if (quads[i].value != "/") {
								i++;
							}
							else {
								dv = dv - 1;
								operation = quads[i].value;
								if (quads[i - 1].value == ")" || quads[i + 1].value == "(") {
									if (quads[i - 1].value == ")") {
										if (!tempvars.empty()) {
											op1 = tempvars.top();
											tempvars.pop();
										}
										else {
											ADDtemps();
											op1 = tempvars.top();
											tempvars.pop();
										}
									}
									else if (quads[i + 1].value == "(") {
										if (!tempvars.empty()) {
											op2 = tempvars.top();
											tempvars.pop();
										}
										else {
											ADDtemps();
											op2 = tempvars.top();
											tempvars.pop();
										}
									}
								}
								else {
									op1 = quads[i - 1].value;
									op2 = quads[i + 1].value;
								}
								if (op1.empty()) {
									op1 = quads[i - 1].value;
								}
								if (op2.empty()) {
									op2 = quads[i + 1].value;
								}
								ADDtemps();
								res = tempvars.top();
								quadlist.oper = operation;
								quadlist.op1 = op1;
								quadlist.op2 = op2;
								quadlist.res = res;
								quadrupleList.push_back(quadlist);
								i = linestart;	//Line reset
								operation = ""; op1 = ""; op2 = ""; res = "";
							}
						}
						//IF +
						while (add != 0) {
							if (add <= 1) {
								if (quads[i].value != "+") {
									i++;
								}
								else {
									add = add - 1;
									operation = quads[i].value;
									if (quads[i + 1].value == "(") {
										op2 = tempvars.top();
										tempvars.pop();
									}
									else {
										op2 = quads[i + 1].value;
									}
									if (quads[i - 1].value == ")") {
										op1 = tempvars.top();
										tempvars.pop();
									}
									else {
										op1 = quads[i - 1].value;
									}
									if (!tempvars.empty()) {
										op2 = tempvars.top();
										tempvars.pop();
									}
									ADDtemps();
									res = tempvars.top();
									quadlist.oper = operation;
									quadlist.op1 = op1;
									quadlist.op2 = op2;
									quadlist.res = res;
									quadrupleList.push_back(quadlist);
									i = linestart;	//Line reset
									operation = ""; op1 = ""; op2 = ""; res = "";
								}
							}
							else {
								while (addcnt != add) {
									i++;
									if (quads[i].value == "+") {
										addcnt = addcnt + 1;
									}
								}
								operation = quads[i].value;
								if (quads[i - 1].value == ")") {
									if (!tempvars.empty()) {
										op1 = tempvars.top();
										tempvars.pop();
									}
									else {
										ADDtemps();
										op1 = tempvars.top();
										tempvars.pop();
									}
								}
								else if (quads[i + 1].value == "(") {
									if (!tempvars.empty()) {
										op2 = tempvars.top();
										tempvars.pop();
									}
									else {
										ADDtemps();
										op2 = tempvars.top();
										tempvars.pop();
									}
								}
								op1 = quads[i - 1].value;
								if (quads[i + 1].value == "(") {
									op2 = tempvars.top();
									tempvars.pop();
								}
								else {
									if (!tempvars.empty()) {
										op2 = tempvars.top();
										tempvars.pop();
									}
									else {
										op2 = quads[i + 1].value;
									}
								}
								ADDtemps();
								res = tempvars.top();
								quadlist.oper = operation;
								quadlist.op1 = op1;
								quadlist.op2 = op2;
								quadlist.res = res;
								quadrupleList.push_back(quadlist);
								i = linestart;	//Line reset
								operation = ""; op1 = ""; op2 = ""; res = "";
								add = add - 1;
								addcnt = 0;
							}
						}
						//IF -
						while (sub != 0) {
							if (sub <= 1) {
								if (quads[i].value != "-") {
									i++;
								}
								else {
									sub = sub - 1;
									operation = quads[i].value;

									if (quads[i + 1].value == "-") {

									}

									else if (quads[i + 1].value == "(") {
										op2 = tempvars.top();
										tempvars.pop();
									}
									else {
										op2 = quads[i + 1].value;
									}
									if (quads[i - 1].value == ")") {
										op1 = tempvars.top();
										tempvars.pop();
									}
									else {
										op1 = quads[i - 1].value;
									}
									if (!tempvars.empty()) {
										op2 = tempvars.top();
										tempvars.pop();
									}
									ADDtemps();
									res = tempvars.top();
									quadlist.oper = operation;
									quadlist.op1 = op1;
									quadlist.op2 = op2;
									quadlist.res = res;
									quadrupleList.push_back(quadlist);
									i = linestart;	//Line reset
									operation = ""; op1 = ""; op2 = ""; res = "";
								}
							}
							else {
								while (subcnt != sub) {
									i++;
									if (quads[i].value == "-") {
										subcnt = subcnt + 1;
									}
								}
								operation = quads[i].value;
								if (quads[i - 1].value == ")") {
									if (!tempvars.empty()) {
										op1 = tempvars.top();
										tempvars.pop();
									}
									else {
										ADDtemps();
										op1 = tempvars.top();
										tempvars.pop();
									}
								}
								else if (quads[i + 1].value == "(") {
									if (!tempvars.empty()) {
										op2 = tempvars.top();
										tempvars.pop();
									}
									else {
										ADDtemps();
										op2 = tempvars.top();
										tempvars.pop();
									}
								}
								op1 = quads[i - 1].value;
								if (quads[i + 1].value == "(" && !tempvars.empty()) {
									op2 = tempvars.top();
									tempvars.pop();
								}
								else {
									if (!tempvars.empty()) {
										op2 = tempvars.top();
										tempvars.pop();
									}
									else {
										op2 = quads[i + 1].value;
									}
								}
								ADDtemps();
								res = tempvars.top();
								quadlist.oper = operation;
								quadlist.op1 = op1;
								quadlist.op2 = op2;
								quadlist.res = res;
								quadrupleList.push_back(quadlist);
								i = linestart;	//Line reset
								operation = ""; op1 = ""; op2 = ""; res = "";
								sub = sub - 1;
								subcnt = 0;
							}
						}

						//Equals Case
						if (par == 0 && mul == 0 && dv == 0 && add == 0 && sub == 0) {
							if (quads[linestart - 1].value == "]") {
								op1 = quads[linestart - 2].value;
								operation = quads[linestart].value;
								op2 = "";
								ADDtemps();
								res = tempvars.top();
								quadlist.oper = operation;
								quadlist.op1 = op1;
								quadlist.op2 = op2;
								quadlist.res = res;
								quadrupleList.push_back(quadlist);
								operation = ""; op1 = ""; op2 = ""; res = "";
							}
							if (!tempvars.empty()) {
								op1 = tempvars.top();
							}
							else {
								op1 = quads[linestart + 1].value;
							}

							if (quads[linestart - 1].value == "]") {
								operation = "[]=";
								op1 = quads[linestart + 1].value;
								op2 = tempvars.top();
								tempvars.pop();
								res = quads[linestart - 4].value;
								quadlist.oper = operation;
								quadlist.op1 = op1;
								quadlist.op2 = op2;
								quadlist.res = res;
								quadrupleList.push_back(quadlist);
								operation = ""; op1 = ""; op2 = ""; res = "";
								break;
							}
							else {
								operation = quads[linestart].value;
								op2 = " ";
								res = quads[linestart - 1].value;
								quadlist.oper = operation;
								quadlist.op1 = op1;
								quadlist.op2 = op2;
								quadlist.res = res;
								quadrupleList.push_back(quadlist);
								operation = ""; op1 = ""; op2 = ""; res = "";
								break;
							}
						}
						i++;
					}
				}
			}
		}

		else if (quads[i].value == "if" || quads[i].value == "while") {
		if (quads[i+1].value == "(") {
			linestart = i;
			while (quads[i].value != ")") {
				if (quads[i].value == "*") {
					mul = mul + 1;
				}
				if (quads[i].value == "/") {
					dv = dv + 1;
				}
				if (quads[i].value == "+") {
					add = add + 1;
				}
				if (quads[i].value == "-") {
					sub = sub - 1;
				}
				i++;
				lineend = i;
			}
			lineend = i;
			//Loop for all lines in main
			for (int i = linestart; i < lineend; i++) {
				if (quads[i].value == "=") {
					//DRIVER for line-by-line
					while (quads[i].value != ";") {
						//IF *
						while (mul != 0) {
							if (mul <= 1) {
								if (quads[i].value != "*") {
									i++;
								}
								else {
									mul = mul - 1;
									operation = quads[i].value;
									if (quads[i - 1].value == ")" || quads[i + 1].value == "(") {
										if (quads[i - 1].value == ")") {
											if (!tempvars.empty()) {
												op1 = tempvars.top();
												tempvars.pop();
											}
											else {
												ADDtemps();
												op1 = tempvars.top();
												tempvars.pop();
											}
										}
										if (quads[i + 1].value == "(") {
											if (!tempvars.empty()) {
												op2 = tempvars.top();
												tempvars.pop();
											}
											else {
												ADDtemps();
												op2 = tempvars.top();
												tempvars.pop();
											}
										}
									}
									else {
										if (!tempvars.empty()) {
											op2 = tempvars.top();
											tempvars.pop();
										}
										else {
											op2 = quads[i + 1].value;
										}
										if (!tempvars.empty()) {
											op1 = tempvars.top();
											tempvars.pop();
										}
										else {
											op1 = quads[i - 1].value;
										}
										ADDtemps();
										res = tempvars.top();
										quadlist.oper = operation;
										quadlist.op1 = op1;
										quadlist.op2 = op2;
										quadlist.res = res;
										quadrupleList.push_back(quadlist);
										i = linestart;	//Line reset
										operation = ""; op1 = ""; op2 = ""; res = "";
										break;
									}
									if (op1.empty()) {
										op1 = quads[i - 1].value;
									}
									if (op2.empty()) {
										op2 = quads[i + 1].value;
									}
									ADDtemps();
									res = tempvars.top();
									quadlist.oper = operation;
									quadlist.op1 = op1;
									quadlist.op2 = op2;
									quadlist.res = res;
									quadrupleList.push_back(quadlist);
									i = linestart;	//Line reset
									operation = ""; op1 = ""; op2 = ""; res = "";
								}
							}
							else {
								while (mulcnt != mul) {
									i++;
									if (quads[i].value == "*") {
										mulcnt = mulcnt + 1;
									}
								}
								operation = quads[i].value;
								if (quads[i - 1].value == ")") {
									if (!tempvars.empty()) {
										op1 = tempvars.top();
										tempvars.pop();
									}
									else {
										ADDtemps();
										op1 = tempvars.top();
										tempvars.pop();
									}
								}
								else if (quads[i + 1].value == "(") {
									if (!tempvars.empty()) {
										op2 = tempvars.top();
										tempvars.pop();
									}
									else {
										ADDtemps();
										op2 = tempvars.top();
										tempvars.pop();
									}
								}
								op1 = quads[i - 1].value;
								if (quads[i + 1].value == "(") {
									op2 = tempvars.top();
									tempvars.pop();
								}
								else {
									if (!tempvars.empty()) {
										op2 = tempvars.top();
										tempvars.pop();
									}
									else {
										op2 = quads[i + 1].value;
									}
								}
								ADDtemps();
								res = tempvars.top();
								quadlist.oper = operation;
								quadlist.op1 = op1;
								quadlist.op2 = op2;
								quadlist.res = res;
								quadrupleList.push_back(quadlist);
								i = linestart;	//Line reset
								operation = ""; op1 = ""; op2 = ""; res = "";
								mul = mul - 1;
								mulcnt = 0;
							}
						}
						//IF /
						while (dv != 0) {
							if (quads[i].value != "/") {
								i++;
							}
							else {
								dv = dv - 1;
								operation = quads[i].value;
								if (quads[i - 1].value == ")" || quads[i + 1].value == "(") {
									if (quads[i - 1].value == ")") {
										if (!tempvars.empty()) {
											op1 = tempvars.top();
											tempvars.pop();
										}
										else {
											ADDtemps();
											op1 = tempvars.top();
											tempvars.pop();
										}
									}
									else if (quads[i + 1].value == "(") {
										if (!tempvars.empty()) {
											op2 = tempvars.top();
											tempvars.pop();
										}
										else {
											ADDtemps();
											op2 = tempvars.top();
											tempvars.pop();
										}
									}
								}
								else {
									op1 = quads[i - 1].value;
									op2 = quads[i + 1].value;
								}
								if (op1.empty()) {
									op1 = quads[i - 1].value;
								}
								if (op2.empty()) {
									op2 = quads[i + 1].value;
								}
								ADDtemps();
								res = tempvars.top();
								quadlist.oper = operation;
								quadlist.op1 = op1;
								quadlist.op2 = op2;
								quadlist.res = res;
								quadrupleList.push_back(quadlist);
								i = linestart;	//Line reset
								operation = ""; op1 = ""; op2 = ""; res = "";
							}
						}
						//IF +
						while (add != 0) {
							if (add <= 1) {
								if (quads[i].value != "+") {
									i++;
								}
								else {
									add = add - 1;
									operation = quads[i].value;
									if (quads[i + 1].value == "(") {
										op2 = tempvars.top();
										tempvars.pop();
									}
									else {
										op2 = quads[i + 1].value;
									}
									if (quads[i - 1].value == ")") {
										op1 = tempvars.top();
										tempvars.pop();
									}
									else {
										op1 = quads[i - 1].value;
									}
									if (!tempvars.empty()) {
										op2 = tempvars.top();
										tempvars.pop();
									}
									ADDtemps();
									res = tempvars.top();
									quadlist.oper = operation;
									quadlist.op1 = op1;
									quadlist.op2 = op2;
									quadlist.res = res;
									quadrupleList.push_back(quadlist);
									i = linestart;	//Line reset
									operation = ""; op1 = ""; op2 = ""; res = "";
								}
							}
							else {
								while (addcnt != add) {
									i++;
									if (quads[i].value == "+") {
										addcnt = addcnt + 1;
									}
								}
								operation = quads[i].value;
								if (quads[i - 1].value == ")") {
									if (!tempvars.empty()) {
										op1 = tempvars.top();
										tempvars.pop();
									}
									else {
										ADDtemps();
										op1 = tempvars.top();
										tempvars.pop();
									}
								}
								else if (quads[i + 1].value == "(") {
									if (!tempvars.empty()) {
										op2 = tempvars.top();
										tempvars.pop();
									}
									else {
										ADDtemps();
										op2 = tempvars.top();
										tempvars.pop();
									}
								}
								op1 = quads[i - 1].value;
								if (quads[i + 1].value == "(") {
									op2 = tempvars.top();
									tempvars.pop();
								}
								else {
									if (!tempvars.empty()) {
										op2 = tempvars.top();
										tempvars.pop();
									}
									else {
										op2 = quads[i + 1].value;
									}
								}
								ADDtemps();
								res = tempvars.top();
								quadlist.oper = operation;
								quadlist.op1 = op1;
								quadlist.op2 = op2;
								quadlist.res = res;
								quadrupleList.push_back(quadlist);
								i = linestart;	//Line reset
								operation = ""; op1 = ""; op2 = ""; res = "";
								add = add - 1;
								addcnt = 0;
							}
						}
						//IF -
						while (sub != 0) {
							if (sub <= 1) {
								if (quads[i].value != "-") {
									i++;
								}
								else {
									sub = sub - 1;
									operation = quads[i].value;

									if (quads[i + 1].value == "-") {

									}

									else if (quads[i + 1].value == "(") {
										op2 = tempvars.top();
										tempvars.pop();
									}
									else {
										op2 = quads[i + 1].value;
									}
									if (quads[i - 1].value == ")") {
										op1 = tempvars.top();
										tempvars.pop();
									}
									else {
										op1 = quads[i - 1].value;
									}
									if (!tempvars.empty()) {
										op2 = tempvars.top();
										tempvars.pop();
									}
									ADDtemps();
									res = tempvars.top();
									quadlist.oper = operation;
									quadlist.op1 = op1;
									quadlist.op2 = op2;
									quadlist.res = res;
									quadrupleList.push_back(quadlist);
									i = linestart;	//Line reset
									operation = ""; op1 = ""; op2 = ""; res = "";
								}
							}
							else {
								while (subcnt != sub) {
									i++;
									if (quads[i].value == "-") {
										subcnt = subcnt + 1;
									}
								}
								operation = quads[i].value;
								if (quads[i - 1].value == ")") {
									if (!tempvars.empty()) {
										op1 = tempvars.top();
										tempvars.pop();
									}
									else {
										ADDtemps();
										op1 = tempvars.top();
										tempvars.pop();
									}
								}
								else if (quads[i + 1].value == "(") {
									if (!tempvars.empty()) {
										op2 = tempvars.top();
										tempvars.pop();
									}
									else {
										ADDtemps();
										op2 = tempvars.top();
										tempvars.pop();
									}
								}
								op1 = quads[i - 1].value;
								if (quads[i + 1].value == "(" && !tempvars.empty()) {
									op2 = tempvars.top();
									tempvars.pop();
								}
								else {
									if (!tempvars.empty()) {
										op2 = tempvars.top();
										tempvars.pop();
									}
									else {
										op2 = quads[i + 1].value;
									}
								}
								ADDtemps();
								res = tempvars.top();
								quadlist.oper = operation;
								quadlist.op1 = op1;
								quadlist.op2 = op2;
								quadlist.res = res;
								quadrupleList.push_back(quadlist);
								i = linestart;	//Line reset
								operation = ""; op1 = ""; op2 = ""; res = "";
								sub = sub - 1;
								subcnt = 0;
							}
						}

						//Equals Case
						if (par == 0 && mul == 0 && dv == 0 && add == 0 && sub == 0) {
							if (quads[linestart - 1].value == "]") {
								op1 = quads[linestart - 2].value;
								operation = quads[linestart].value;
								op2 = "";
								ADDtemps();
								res = tempvars.top();
								quadlist.oper = operation;
								quadlist.op1 = op1;
								quadlist.op2 = op2;
								quadlist.res = res;
								quadrupleList.push_back(quadlist);
								operation = ""; op1 = ""; op2 = ""; res = "";
							}
							if (!tempvars.empty()) {
								op1 = tempvars.top();
							}
							else {
								op1 = quads[linestart + 1].value;
							}

							if (quads[linestart - 1].value == "]") {
								operation = "[]=";
								op1 = quads[linestart + 1].value;
								op2 = tempvars.top();
								tempvars.pop();
								res = quads[linestart - 4].value;
								quadlist.oper = operation;
								quadlist.op1 = op1;
								quadlist.op2 = op2;
								quadlist.res = res;
								quadrupleList.push_back(quadlist);
								operation = ""; op1 = ""; op2 = ""; res = "";
								break;
							}
							else {
								operation = quads[linestart].value;
								op2 = " ";
								res = quads[linestart - 1].value;
								quadlist.oper = operation;
								quadlist.op1 = op1;
								quadlist.op2 = op2;
								quadlist.res = res;
								quadrupleList.push_back(quadlist);
								operation = ""; op1 = ""; op2 = ""; res = "";
								break;
							}
						}
						i++;
					}
				}
			}
		}
	}
		




	if (quads[i].value == "cout") {
		operation = quads[i].value;
		while (quads[i].value != ";") {
			if (quads[i].value == "<<" || quads[i].value == "cout") {
				i++;
			}
			else {
				op1 = quads[i].value;
				if (quads[i].type == 107) {
					quadlist.oper = operation;
					quadlist.op1 = op1;
					quadlist.op2 = op2;
					quadlist.res = res;
					quadrupleList.push_back(quadlist);
				}
				else {
					quadlist.oper = operation;
					quadlist.op1 = op1;
					quadlist.op2 = op2;
					quadlist.res = res;
					quadrupleList.push_back(quadlist);
				}
				i++;
			}
		}
	}

	if (quads[i].value == "cin") {
		operation = quads[i].value;
		while (quads[i].value != ";") {
			if (quads[i].value == ">>") {
				i++;
			}
			else {
				op1 = quads[i].value;
				quadlist.oper = operation;
				quadlist.op1 = op1;
				quadlist.op2 = op2;
				quadlist.res = res;
				quadrupleList.push_back(quadlist);
				i++;
			}
		}
	}


		DELtemps();
		par = 0;
		mul = 0;
		dv = 0;
		add = 0;
		sub = 0;
	}


	for (int i = 0; i < quadrupleList.size(); i++) {
		cout << quadrupleList[i].oper << " " << quadrupleList[i].op1 << " " << quadrupleList[i].op2 << " " << quadrupleList[i].res << endl;
	}
	return 0;
}



int main() {
	lexicalAnylizer();
	parser();
	return 0;
}