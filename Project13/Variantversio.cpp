#include <iostream>
#include <iomanip>
#include <memory>
#include <vector>
#include <sstream>
//#include <list>
#include <chrono>
#include <thread>
#include <variant>
#include <string>
#include "ASTnode.cpp"
#include <limits>

#include <type_traits>



#include <windows.h>

//Status : Working 
// TODO simplify some type checks are redundant some methods will check whether a variant contains a type return that type then the calling function has to chkeck again 


class Number {



public:
	double number;
	void wai() const {
		std::cout << "Number:" << (*this)();
	}
	

	Number() = delete;
	Number(double def) : number(def) {};

	double operator()() const {
		return number;
	}


};
class L_P  {
public:
	void wai()const  {
		std::cout << "L_P" << std::endl;
	}
	
};
class R_P {
public:
	void wai() const {
		std::cout << "R_P";
	}
	
};
class Plus  {
public:
	void wai() const {
		std::cout << "Plus" ;
	}
	Number operator()(const Number& a,const Number& b) const {
		return Number(a() + b());
	}
	
};
class Minus  {
public:
	void wai() const {
		std::cout << "Minus" ;
	}
	Number operator()(const Number& a,const Number& b) const {
		return Number(a() - b());
	}
	
};
class Times {
public:
	void wai() const {
		std::cout << "Times" ;
	}
	Number operator()(const Number& a,const Number& b) const{
		return Number (a() * b());
	}
	
};
class Divide  {
public:
	void wai() const {
		std::cout << "Divide" ;
	}

	Number operator()(const Number & a,const Number & b) const {
		return Number(a() / b());
	}
	
};
class End  {
public:
	const End & operator()()const{
		return *this;
	}

	void wai() const {
		std::cout << "End" ;
	}
};
class Blank {};


using Token = std::variant<L_P, R_P, Plus, Minus, Times, Divide, Number, End,Blank>;






//void print_tokens(std::vector<Token>& a) {
//	for (auto& i : a) {
//		if (L_P* pval = std::get_if<L_P>(&i)) {
//			pval->wai();
//		}
//		else if (R_P* pval = std::get_if<R_P>(&i))
//		{
//			pval->wai();
//		}
//		else if (Plus* pval = std::get_if<Plus>(&i)) {
//			pval->wai();
//		}
//		else if (Minus* pval = std::get_if<Minus>(&i)) {
//			pval->wai();
//		}
//		else if (Times* pval = std::get_if<Times>(&i)) {
//			pval->wai();
//		}
//		else if (Divide* pval = std::get_if<Divide>(&i)) {
//			pval->wai();
//		}
//		else if (Number* pval = std::get_if<Number>(&i)) {
//			pval->wai();
//		}
//		else if (End* pval = std::get_if<End>(&i)) {
//			pval->wai();
//		}
//
//
//	}
//}





class Printer {                                                		   //helper class
public:																   //helper class
	void operator()(Token& i) {                                 	   //helper class
		if (std::holds_alternative<L_P>(i)) {						   //helper class
			std::get<L_P>(i).wai();									   //helper class
		}															   //helper class
		else if (std::holds_alternative<R_P>(i)) {					   //helper class
			std::get<R_P>(i).wai();									   //helper class
		}															   //helper class
		else if (std::holds_alternative<Plus>(i)) {					   //helper class
			std::get<Plus>(i).wai();								   //helper class
		}															   //helper class
		else if (std::holds_alternative<Minus>(i)) {				   //helper class
			std::get<Minus>(i).wai();								   //helper class
		}															   //helper class
		else if (std::holds_alternative<Times>(i)) {				   //helper class
			std::get<Times>(i).wai();								   //helper class
		}															   //helper class
		else if (std::holds_alternative<Divide>(i)) {				   //helper class
			std::get<Divide>(i).wai();								   //helper class
		}															   //helper class
		else if (std::holds_alternative<Number>(i)) {				   //helper class
			std::get<Number>(i).wai();								   //helper class
		}															   //helper class
		else if (std::holds_alternative<End>(i)) {					   //helper class
			std::get<End>(i).wai();									   //helper class
		}															   //helper class
	}																   //helper class
																	   //helper class
																	   //helper class
};																	   //helper class



class Expresion {
private:
	ASTnode<Token,Printer> my_tree;
	std::vector<Token> tokens;
	std::string str;
	size_t curr_token;

public:
	Expresion() = delete;
	Expresion(std::string& str) :str(str), tokens(),my_tree(End()),curr_token(0) {
		tokenize(str);
	};

	ASTnode<Token, Printer>& get_AST() {
		return my_tree;
	}

	std::vector<Token>& get_tokens() {
		return tokens;
	}

	std::string& get_str_rep() {
		return str;
	}


	void tokenize(std::string& token_str) {
		std::stringstream s(token_str);
		char c;
		double tmp_f;

		while (s >> std::ws && !s.eof()) {
			c = s.peek();  // Check next char without consuming

			// Handle operators/parentheses first
			if (c == '(' || c == ')' || c == '+' || c == '-' || c == '*' || c == '/') {
				s >> c;  // Now consume the operator
				if (c == '(')      tokens.emplace_back(L_P());
				else if (c == ')') tokens.emplace_back(R_P());
				else if (c == '+') tokens.emplace_back(Plus());
				else if (c == '-') tokens.emplace_back(Minus());
				else if (c == '*') tokens.emplace_back(Times());
				else if (c == '/') tokens.emplace_back(Divide());
				continue;  // Skip number parsing
			}

			// If not an operator, try reading a number
			if (s >> tmp_f) {
				tokens.emplace_back(Number(tmp_f));
			}
			else {
				// Handle invalid tokens (e.g., letters)
				s.clear();
				s >> c;  // Discard the invalid char
				throw std::runtime_error("Invalid token: " + std::string(1, c));
			}
		}
		tokens.emplace_back(End());

		
	}
	

	Number eval() {
		return eval_helper(my_tree);
	}

	Number eval_helper(ASTnode<Token, Printer>& t) {
		if (std::holds_alternative<Number>(t.get_Content())) {
			return std::get<Number>(t.get_Content());
		}
		else if(std::holds_alternative<Plus>(t.get_Content())) {
			if (t.has_left() && t.has_right()) {
				Plus p = std::get<Plus>(t.get_Content());
				return p(eval_helper(t.get_left()),eval_helper(t.get_right()));
				
				
			}
			else {
				throw std::runtime_error("missing term");
			}

		}else if (std::holds_alternative<Minus>(t.get_Content())) {
			if (t.has_left() && t.has_right()) {
				Minus p = std::get<Minus>(t.get_Content());
				return p(eval_helper(t.get_left()), eval_helper(t.get_right()));


			}
			else {
				throw std::runtime_error("missing term");
			}

		}else if (std::holds_alternative<Times>(t.get_Content())) {
			if (t.has_left() && t.has_right()) {
				Times p = std::get<Times>(t.get_Content());
				return p(eval_helper(t.get_left()), eval_helper(t.get_right()));


			}
			else {
				throw std::runtime_error("missing factor");
			}

		}else if (std::holds_alternative<Divide>(t.get_Content())) {
			if (t.has_left() && t.has_right()) {
				Divide p = std::get<Divide>(t.get_Content());
				return p(eval_helper(t.get_left()), eval_helper(t.get_right()));


			}
			else {
				throw std::runtime_error("missing factor");
			}

		}
		else {
			throw std::runtime_error("unexpected token");
		}
	}
	

	
	ASTnode<Token,Printer>& AST_make() {
		//std::cout << "AST_make was called " << std::endl;
		parse_exp(my_tree);
		return my_tree;

	}


	void  parse_exp(ASTnode<Token,Printer>& tree) {
		//std::cout << " parse_exp was called " << std::endl;
		
		parse_term(tree);//
		
		parse_exp_prime(tree);

		
	}

	void parse_exp_prime(ASTnode<Token,Printer>& tree) {
		//std::cout << " parse_exp_prime was called " << std::endl;
		Token t = parse_operator_add();


		if (std::holds_alternative<Plus>(t)) {
			ASTnode<Token, Printer> tmp{ End() };

			tmp.push_left(End());
			tmp.get_left() = std::move(tree);
			tree = std::move(tmp);


			tree = std::get<Plus>(t);
			tree.push_right(End());
			parse_term(tree.get_right());

			parse_exp_prime(tree);
		}
		else if (std::holds_alternative<Minus>(t)) {
			ASTnode<Token, Printer> tmp{ End() };

			tmp.push_left(End());
			tmp.get_left() = std::move(tree);
			tree = std::move(tmp);

			tree = std::get<Minus>(t);
			tree.push_right(End());
			parse_term(tree.get_right());

			parse_exp_prime(tree);
		}
		else if (std::holds_alternative<End>(t)) {
			return;
		}
		else {
			throw std::runtime_error("recived unexpected token");
		}


		

		
	}
	
	void parse_term(ASTnode<Token,Printer>& tree) {
		//std::cout << " parse_term was called " << std::endl;
		
		parse_factor(tree);
		parse_term_prime(tree);
	}
	
	
	void parse_term_prime(ASTnode<Token,Printer>& tree) {
		//std::cout << " parse_term_prime was called " << std::endl;
		Token t = parse_operator_mult();

		
		if (std::holds_alternative<Times>(t)) {
			ASTnode<Token, Printer> tmp{ End() };

			tmp.push_left(End());
			tmp.get_left() = std::move(tree);
			tree = std::move(tmp);


			tree = std::get<Times>(t);
			tree.push_right(End());
			parse_factor(tree.get_right());
			parse_term_prime(tree);
		}
		else if (std::holds_alternative<Divide>(t)) {
			ASTnode<Token, Printer> tmp{ End() };

			tmp.push_left(End());
			tmp.get_left() = std::move(tree);
			tree = std::move(tmp);

			tree = std::get<Divide>(t);
			tree.push_right(End());
			parse_factor(tree.get_right());
			parse_term_prime(tree);
		}
		else if(std::holds_alternative<End>(t)){
			//std::cout << " parse_term_prime returned " << std::endl;
			return;
		}
		else {
			throw std::runtime_error("recived unexpected token");
		}


		

		
		//std::cout << " parse_term_prime returned " << std::endl;
	}





	void parse_factor(ASTnode<Token,Printer>& tree) { // argument is where in the tree you must lool into 
		//std::cout << "parse_factor was called " << std::endl;
		if (consume_lp()) {
			parse_exp(tree);

			if (!consume_rp()) {
				throw std::runtime_error("missing right parenthesis after token index: " + std::to_string(curr_token));
			};
			
		}
		else {
			 tree = parse_number();
			
		}


	}





	Token parse_operator_add() {
		//std::cout << "parse_operator_add was called " << std::endl;
		if (std::holds_alternative<Plus>(tokens[curr_token])) {
			Plus t = std::get<Plus>(tokens[curr_token]);
			curr_token++;
			return t;
		}if (std::holds_alternative<Minus>(tokens[curr_token])) {
			Minus t = std::get<Minus>(tokens[curr_token]);
			curr_token++;
			return t;
		}
		else {
			return End();
		}
	}

	Token parse_operator_mult() {
		//std::cout << "parse_operator_mult was called " << std::endl;
		if (std::holds_alternative<Times>(tokens[curr_token])) {
			Times t = std::get<Times>(tokens[curr_token]);
			curr_token++;
			return t;
		}if (std::holds_alternative<Divide>(tokens[curr_token])) {
			Divide t = std::get<Divide>(tokens[curr_token]);
			curr_token++;
			return t;
		}
		else {
			return End();
		}
	}

	Number parse_number() {
		//std::cout << "parse_number was called " << std::endl;
		if (std::holds_alternative<Number>(tokens[curr_token])) {
			Number tmp = std::get<Number>(tokens[curr_token]);
			curr_token++;
			return  tmp;
		}
		else {
			throw std::runtime_error("Token was not a number");
		}
	}

	bool consume_lp() {
		//std::cout << "consume_lp was called " << std::endl;
		if (std::holds_alternative<L_P>(tokens[curr_token])) {
			curr_token++;
			return true;
		}
		else {
			return false;
		}
	}

	bool consume_rp() {
		//std::cout << "consume_rp was called " << std::endl;
		if (std::holds_alternative<R_P>(tokens[curr_token])) {
			curr_token++;
			return true;
		}
		else {
			return false;
		}

	}


};




size_t ASTnode<Token, Printer >::node_size = 0;
int main()
{
	//_setmode(_fileno(stdout), _O_U8TEXT);

	//ASTnode<Token,Printer> my_tree{End()};

	//std::locale::global(std::locale("en_US.UTF-8"));
	Printer p;



	

	std::string str;
	
	//const End & t = std::get<End>(my_tree.get_Content());
	//t.wai();
	//L_P, R_P, Plus,Minus,Times,Divide,Number,End
	
	

	//print_tokens(e.get_tokens());
	
	while(true){

	std::cout << "q to quit otherwise input expression" << std::endl;
	std::cout << ">>>";
	char c = std::cin.peek();
	
	if (c == 'q') break;




	
	std::getline(std::cin,str);
	


	std::cout << str << std::endl;

	try{
	Expresion e(str);
	e.AST_make().print_tree(p);

	Number result(e.eval());
	std::cout << std::endl;
	std::cout  <<  e.get_str_rep() <<" = " << result() <<  std::endl;

	}
	catch (std::runtime_error& e) {
		std::cout << e.what() << std::endl;
	}

	}

	//std::cout << ASTnode<Token, Printer>::get_node_size() << std::endl;
	
}

