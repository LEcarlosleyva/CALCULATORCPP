
#include <iostream>
#include <iomanip>
#include <memory>
#include <vector>
#include <sstream>
#include <list>
#include <chrono>
#include <thread>
#include "ASTnode.cpp"
//TODO implement tree  to create sintax tree 



class Token {

public:
	virtual void wai() {
		std::cout << " I am base" << std::endl;
	}

	virtual std::unique_ptr<Token> clone() = 0;
	virtual ~Token() = default;

};

class L_P : public Token {
public:
	void wai() {
		std::cout << " I am L_P" << std::endl;
	}
	std::unique_ptr<Token> clone() {
		return std::make_unique<L_P>(*this);
	}
};
class R_P : public Token {
public:
	void wai() {
		std::cout << " I am R_P" << std::endl;
	}
	std::unique_ptr<Token> clone() {
		return std::make_unique<R_P>(*this);
	}
};

class Plus : public Token {
public:
	void wai() {
		std::cout << " I am Plus" << std::endl;
	}
	std::unique_ptr<Token> clone() {
		return std::make_unique<Plus>(*this);
	}
};
class Minus : public Token {
public:
	void wai() {
		std::cout << " I am Minus" << std::endl;
	}
	std::unique_ptr<Token> clone() {
		return std::make_unique<Minus>(*this);
	}
};


class Times : public Token {
public:
	void wai() {
		std::cout << " I am Times" << std::endl;
	}
	std::unique_ptr<Token> clone() {
		return std::make_unique<Times>(*this);
	}
};

class Divide : public Token {
public:
	void wai() {
		std::cout << " I am Divide" << std::endl;
	}
	std::unique_ptr<Token> clone() {
		return std::make_unique<Divide>(*this);
	}
};

class Number : public Token {



public:
	void wai() {
		std::cout << " I am Number: " << get_Num() << std::endl;
	}
	int64_t number;
	Number(int64_t def = 0) : number(def) {};
	int64_t get_Num() const { return number; };
	std::unique_ptr<Token> clone() {
		return std::make_unique<Number>(*this);
	}

};


class End : public Token {
public:
	std::unique_ptr<Token> clone() {
		return std::make_unique<End>(*this);
	}
	void wai() {
		std::cout << " I am End" << std::endl;
	}
};




/*
class Grammar
{
public:

	Grammar() = delete;
	
	Grammar(std::string& token_str) : curr_indx(0),
		token_str(token_str), tokens(), ast(nullptr) {

		tokenize();
		Parse_Exp();
	};
	~Grammar() = default;


	void Parse_Exp() {
		std::cout << curr_indx << "Parse_Exp" << std::endl;
		
		Parse_Term();
		Parse_Exp_prime();
	}

	void Parse_Exp_prime() {
		std::cout << curr_indx << "Parse_Exp_prime" << std::endl;
		if (!Consume_Op_add()){
			return;
		}
		Parse_Term();
		Parse_Exp_prime();

	}

	bool Consume_Op_add() {
		std::cout << curr_indx << "Consume_Op_add" << std::endl;

		if (dynamic_cast<Plus*>(tokens[curr_indx].get())) {
			
			curr_indx++;
			return true;
		}
		else if (dynamic_cast<Minus*>(tokens[curr_indx].get())) {
			
			curr_indx++;
			return true;
		}
		else {
			return false;
		}
		


	}

	bool Consume_Op_mult() {

		std::cout << curr_indx << "Consume_Op_mult" << std::endl;
		if (dynamic_cast<Times*>(tokens[curr_indx].get())) {
			
			curr_indx++;
			return true;
		}
		else if (dynamic_cast<Divide*>(tokens[curr_indx].get())) {
			
			curr_indx++;
			return true;
		}
		else {
			return false;
		}
		


	}

	void Parse_Term() {
		std::cout << curr_indx << "Parse_term" << std::endl;
		Parse_Factor();
		Parse_term_prime();

	}

	void Parse_term_prime() {
		std::cout << curr_indx << "Parse_term_prim" << std::endl;
		if (!Consume_Op_mult()) {
			return;
		}
		Parse_Factor();
		Parse_term_prime();

	}

	void Parse_Factor() {
		std::cout << curr_indx << "Parse_Factor" << std::endl;
		if (!Consume_L_P()) {
			Parse_Number();
			return;
		}
		else {
			Parse_Exp();
		}
		if (!Consume_R_P()) {
			throw std::runtime_error("missing right parenthesis");
		}

	}


	bool Consume_L_P() {
		std::cout << curr_indx << "Consume_L_P" << std::endl;
		if (dynamic_cast<L_P*>(tokens[curr_indx].get())) {
			curr_indx++;
			
			return true;
		}
		else {
			
			return false;
		}
		

	}

	bool Consume_R_P() {
		std::cout << curr_indx << "Consume_R_P" << std::endl;
		if (dynamic_cast<R_P*>(tokens[curr_indx].get())) {
			curr_indx++;
			
			return true;
		}
		else {
			
			return false;
		}
		

	}

	void Parse_Number()  {
		std::cout << curr_indx << "Parse Number" << std::endl;
		 
		if (!dynamic_cast<Number*>(tokens[curr_indx].get())) {
			
			throw std::runtime_error("NAN");
		}
		else {
			
			curr_indx++;
		}
	}

	void tokenize() {
		std::stringstream s(token_str);
		char c;
		int64_t tmp_f;

		while (s >> std::ws && !s.eof()) {
			c = s.peek();  // Check next char without consuming

			// Handle operators/parentheses first
			if (c == '(' || c == ')' || c == '+' || c == '-' || c == '*' || c == '/') {
				s >> c;  // Now consume the operator
				if (c == '(')      tokens.push_back(std::make_unique<L_P>());
				else if (c == ')') tokens.push_back(std::make_unique<R_P>());
				else if (c == '+') tokens.push_back(std::make_unique<Plus>());
				else if (c == '-') tokens.push_back(std::make_unique<Minus>());
				else if (c == '*') tokens.push_back(std::make_unique<Times>());
				else if (c == '/') tokens.push_back(std::make_unique<Divide>());
				continue;  // Skip number parsing
			}

			// If not an operator, try reading a number
			if (s >> tmp_f) {
				tokens.push_back(std::make_unique<Number>(tmp_f));
			}
			else {
				// Handle invalid tokens (e.g., letters)
				s.clear();
				s >> c;  // Discard the invalid char
				throw std::runtime_error("Invalid token: " + std::string(1, c));
			}
		}
		tokens.push_back(std::make_unique<End>());
	}


	

	auto& get_tokens() {
		return tokens;
	}

private:
	
	
	ASTnode<Token*> ast;
	std::string token_str;
	uint64_t curr_indx;
	std::vector<std::unique_ptr<Token>> tokens;
	
};

*/
/*

void printChildrenandParent(std::unique_ptr<ASTnode<int>>& obj) {
	if (!obj) {
		return;
	}
	if (!obj->acces_left_ptr() && !obj->acces_right_ptr()) {
		return;
	} 
	if (!obj->acces_left_ptr()) {
		std::cout << obj->acces_content() << std::endl;
		std::cout << "x  " << obj->acces_right_ptr()->acces_content() << std::endl;
		return;
	} 
	if (!obj->acces_right_ptr()) {
		std::cout << obj->acces_content() << std::endl;
		std::cout << obj->acces_left_ptr()->acces_content() << "   x" << std::endl;
		return;
	}
	std::cout << obj->acces_content() << std::endl;
	std::cout << obj->acces_left_ptr()->acces_content() << " " << obj->acces_right_ptr()->acces_content() << std::endl;

}



void printASTtree(ASTnode<int> &obj) {
	
	if (!obj) {
		return;
	}
	else {


		printChildrenandParent(obj);
		printASTtree(obj->acces_left_ptr());
		printASTtree(obj->acces_right_ptr());

		

	}




}





int main() {

	std::unique_ptr<ASTnode<int>> a = std::make_unique<ASTnode<int>>(0);
	

	std::vector<int> fib;

	fibMake(fib, 6);


	for (const auto& i : fib) {
		std::cout << i << std::endl;
	}

	std::cout << "---------------------------" << std::endl;
	//std::cout << fib.size();
	treefibMake(fib, a, fib.size() - 1);
	printASTtree(a);
}

*/
/*
void fibMake(std::vector<int>& a, const uint64_t size) {
	a.push_back(1);
	a.push_back(1);
	for (uint64_t i = 2; i < size; i++) {
		a.push_back(a[i - 2] + a[i - 1]);
	}


}
*/
/*
class treeMake_fib {
public:


	void operator()(std::vector<int>& a, ASTnode<int>& tree) {
		tree = a[a.size() - 1];
		treefibMake(a, tree, a.size() - 1);
		
		
		return;
	}

	void treefibMake(std::vector<int>& a, ASTnode<int>& tree,int indx) {
		
		if (indx < 2 || a.empty()) {
			
			return ;
		}
		else {
			//std::cout << "indx= " << indx << std::endl;
			//std::cout << "a[" << indx - 1 << "]= " << a[indx - 1] << std::endl;
			//std::cout << "a[" << indx - 2 << "]= " << a[indx - 2] << std::endl;
			//std::this_thread::sleep_for(std::chrono::milliseconds(1000));
				tree.push_right(a[indx - 1]);
				tree.push_left( a[indx - 2]);
			
			
			
			

			treefibMake(a,tree.get_right(),indx - 1);
			treefibMake(a,tree.get_left(),indx - 2);
		}

	}



};

*/


//int main() {
//	
//	ASTnode<int> b(0);
//
//
//	std::vector<int> a;
//
//
//	fibMake(a,10);
//
//	//for (const auto& i : a) {
//	//	std::cout << i << std::endl;
//	//}
//	//
//
////	std::cout << "--------------------------------------" << std::endl;
//	//treeMake_fib my_fib_tree;
//
//	//my_fib_tree(a, b);
//
//	//b.print_tree();
//
//
//	//b.treePrint();
//	
//	//std::cout << b.acces_right().acces_right().acces_content() << std::endl;
//
//	//std::cout << !nullptr << std::endl;
//	/*
//
//	std::unique_ptr<int> a = std::make_unique<int>();
//
//	std::cout << *a << std::endl;
//	*/
//
////}