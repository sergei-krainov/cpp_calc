#include <iostream>

using namespace std;

double number_value;
map<string,double>table;
string string_value;



enum Token_value {
	NAME, NUMBER, END,
	PLUS = '+', MINUS = '-', MUL = '*', DIV = '/',
	PRINT = ';', ASSIGN = '=', LP = '(', RP = ')'
};

Token_value curr_tok = PRINT;

Token_value get_token(void);
double expr(bool get);

double error(const string& s) {
	cerr << s << "\n";
	return 1;
}

double prim(bool get) {
	if (get) get_token();
	
	switch(curr_tok) {
	case NUMBER:
	{
		double v = number_value;
		get_token();
		return v;
	}
	case NAME:
	{
		double& v = table[string_value];
		if ( get_token() == ASSIGN ) v = expr(true);
		return v;
	}
	case MINUS:
	{
		return -prim(true);
	}
	case LP:
	{
		double e = expr(true);
		if ( curr_tok != RP ) return error(") expected");
		get_token();
		return e;
	}
	default:
	{
		return error("primary expected");
	}
	}
	
}

double term(bool get) {
	double left = prim(get);
	
	for (;;) {
		switch(curr_tok) {
		case MUL:
			left *= prim(true);
			break;
		case DIV:
			left /= prim(true);
			break;
		default:
			return left;
		}
		
	}
}

double expr(bool get) {
	double left = term(get);
	
	for (;;) {
		switch(curr_tok) {
		case PLUS:
			left += term(true);
			break;
		case MINUS:
			left -= term(true);
			break;
		default:
			return left;
		}
		
	}
}

Token_value get_token() {
	char c;
	
	do {
		if (!cin.get(c)) return curr_tok = END;
	} while( c != '\n' && isspace(c));
	
	switch(c) {
	case '\n':
	case ';':
		return curr_tok = Token_value(c);

	//case ';':
	case '*':
	case '/':
	case '+':
	case '-':
	case '(':
	case ')':
	case '=':
		return curr_tok = Token_value(c);

	case '0' : case '1' : case '2' : case '3' : case '4' : case '5':
	case '6' : case '7' : case '8' : case '9' : case '.':
		cin.putback(c);
		cin >> number_value;
		return curr_tok = NUMBER;
		
	default:
		if (isalpha(c)) {
			string_value = c;
			while (cin.get(c) && isalnum(c)) string_value.push_back(c);
			cin.putback(c);
			return curr_tok = NAME;
		}
		error("bad token");
		return curr_tok = PRINT;
	}
}

int main() {
	while(cin) {
		if (curr_tok == END) break;
		if (curr_tok == PRINT) continue;
		cout << expr(false) << endl;
	}
	
	return 0;
}
