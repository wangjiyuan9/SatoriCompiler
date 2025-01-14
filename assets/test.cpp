#include "test.h"
#include "regexp.h"

set<string> keywords = {
	"auto", "break", "case", "char", "const", "continue", "default",
	"do", "double", "else", "enum", "extern", "float", "for", "goto",
	"if", "int", "long", "register", "return", "short", "signed",
	"sizeof", "static", "struct", "switch", "typedef", "union",
	"unsigned", "void", "volatile", "while"};

set<string> separators = {
	"(", ")", "[", "]", "{", "}", ",", ";", ":", ".",
	"&", "*", "+", "-", "~", "!", "/", "%", "<", ">",
	"^", "|", "=", "?", "#", "\\", "\"", "\`"};

void lexerMain()
{
	Lexer lexer;
	lexer.readLexerDef("./assets/lexer.def");
	// lexer.readCodeFile("./assets/code.cpp");
	lexer.readCodeFile("./assets/error.cpp");
	// lexer.readCodeFile("./assets/lexer.cpp");
	lexer.tokenize();
	lexer.printTokens();
}

void lexerTest()
{
	Lexer lexer;
	int id = 0;
	for (auto &keyword : keywords)
	{
		lexer.addKeywordId(keyword, id++);
	}
	for (auto &sep : separators)
	{
		lexer.addKeywordId(sep, id++);
	}
	lexer.addTokenType(BLANK, "[\\t\\n\\r\\f\\v ]+");
	lexer.addTokenType(IGNORE, "\"([\\x0a-\\x0d\\x20-x21\\0x23-\\x7e]|\\\\\")*\"");
	lexer.addTokenType(IGNORE, "/\\\*([\\x0a-\\x0d\\x20-\\x29\\x2b-\\x7e]*|\\*[\\x0a-\\x0d\\x20-\\x2e\\x30-\\x7e]*)*\\*/");
	lexer.addTokenType(IDENTIFIER, "[a-zA-Z_][a-zA-Z0-9_]*");
	lexer.addTokenType(SEPARATOR, "[\\+\\-\\*\\\\%=\\(\\){}\\[\\]<>;,.\\|&^!]");
	lexer.addTokenType(SEPARATOR, ">=|<=|!=|==|\\+\\+|\\-\\-|\\|\\||&&|\\*=|/=|\\+=|\\-=|%=|<<|>>");
	lexer.addTokenType(STRING, "\"([\\x0a-\\x0d\\x20-x21\\0x23-\\x7e]|\\\")*\"");
	lexer.addTokenType(MACRO, "#[a-zA-Z][a-zA-Z0-9]*");
	lexer.addTokenType(INCLUDE, "\"[a-zA-Z0-9_.]+\"|<[a-zA-Z0-9_.]+>");
	lexer.addTokenType(INTEGER, "(\\-|\\+|\\e)[0-9]+");
	lexer.addTokenType(REAL, "(\\-|\\+|\\e)[0-9]+\\.[0-9]+");
	lexer.addTokenType(CHARACTER, "\'[\\x0a-\\x0d\\x20-x21\\0x23-\\x7e]*\'");
	lexer.tokenize("int main() { int a = 1; int b = 2; int c = a + b; return 0; }");
	lexer.printTokens();
}

void nfaTest() // test for regexp2FA
{
	string raw = "";
	cin >> raw;
	cout << "input raw: " << raw << endl;
	Regexp2FA convertor = Regexp2FA(raw);
	convertor.regexpPreproc();
	convertor.tmpReg2postfix();
	convertor.postfix2FA();
	FiniteAutomaton nfa = convertor.getNFA();
	nfa.print();
	string word;
	while (true)
	{
		cout << "input word: ";
		cin >> word;
		cout << "inputted word: " << word << endl;
		viewer vWord = viewer(word);
		string result;
		bool res = nfa.accepts(vWord, result);
		cout << "match result: " << (res ? "yes" : "no") << endl;
		cout << "matched word: " << result << endl;
	}
}