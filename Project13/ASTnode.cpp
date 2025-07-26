

#include <iostream>
#include <iomanip>
#include <memory>
#include <vector>
#include <sstream>
#include <list>
#include <utility>




/*
template<typename T>
class ASTnode {

public:


	ASTnode() :Content(), left(), right() {
	
	
	};
	ASTnode(const T& a) : Content(std::make_unique<T>(a)), left(), right() {}; // C++ allows references to temporaries as long as they are const 
	ASTnode(const ASTnode<T>& other) = delete;
	ASTnode(ASTnode<T> && other) = default;

	~ASTnode() = default;
	// opearot overloading -----------------------------------------------------------------------

	ASTnode<T>& operator=(ASTnode<T>&& o) noexcept = default;

	//printing -----------------------------------------------------------------------
	void treePrint() {
		printASTtree(*this);
	}

	


	//accesing ---------------------------------------------------------------

	

	auto& make_left() {
		left = std::make_unique<ASTnode>();
		//left->make_content();
		return *left;

	}


	auto& make_left(const T & a) {
		left = std::make_unique<ASTnode>();
		//left->make_content(a);
		return *left;

	}

	ASTnode& make_right() {
		right = std::make_unique<ASTnode>();
		//right->make_content();
		return *right;

	}

	ASTnode& make_right(const T & a) {
		right = std::make_unique<ASTnode>();
		//right->make_content(a);
		return *right;

	}
	

	

	auto& make_content() {
		Content = std::make_unique<T>();
		return Content;
	}
	
	auto& make_content(const T& a) {
		Content = std::make_unique<T>(a);
		return Content;
	}

	

	auto& acces_left_ptr() {
		
		return left;
	}
	auto& acces_right_ptr() {
		
		return right;
	}
	auto& acces_content_ptr() {
		
		return Content;
	}
	auto& acces_content() {
		if (!Content) {
			make_content();
		}
		return *Content;
	}

	auto& acces_left() {
		if (!left) {
			make_left();
		}
		return *left;
	}
	auto& acces_right() {
		if (!right) {
			make_right();
		}
		return *right;
	}


	private:

		//helper functions ----------------------------------------------------------------
		void printASTtree(ASTnode<int>& obj) {

			if (!obj.acces_content_ptr()) {
				return;
			}
			else
				if (!obj.acces_left_ptr() && !obj.acces_right_ptr()) {
					printChildrenandParent(obj);
				}
				else
					if (!obj.acces_left_ptr()) {
						printChildrenandParent(obj);
						printASTtree(obj.acces_right());
					}
					else
						if (!obj.acces_right_ptr()) {
							printChildrenandParent(obj);
							printASTtree(obj.acces_left());
						}
						else {


							printChildrenandParent(obj);
							printASTtree(obj.acces_left());
							printASTtree(obj.acces_right());



						}




		}
		void printChildrenandParent(ASTnode<int>& obj) {
			
			if (!obj.acces_content_ptr()) return; // do nothing if no content to print 
			if (!obj.acces_left_ptr() && !obj.acces_right_ptr()) {
				std::cout << *obj.acces_content_ptr() << std::endl;
				std::cout << "x  " << "x" << std::endl;
				return;
			}
			if (!obj.acces_left_ptr() && obj.acces_right_ptr()->acces_content_ptr()) {
				std::cout << *obj.acces_content_ptr() << std::endl;
				std::cout << "x  " << *obj.acces_right_ptr()->acces_content_ptr() << std::endl;
				return;
			}
			if (!obj.acces_right_ptr() && obj.acces_left_ptr()->acces_content_ptr()) {
				std::cout << *obj.acces_content_ptr() << std::endl;
				std::cout << *obj.acces_left_ptr()->acces_content_ptr() << "   x" << std::endl;
				return;
			}
			std::cout << *obj.acces_content_ptr() << std::endl;
			std::cout << *obj.acces_left_ptr()->acces_content_ptr() << " " << *obj.acces_right_ptr()->acces_content_ptr() << std::endl;

		}

	// members -------------------------------------------------------------------
	std::unique_ptr<T> Content;
	std::unique_ptr<ASTnode> left;
	std::unique_ptr<ASTnode> right;

};

*/

/*
int main() {

	ASTnode<int> a(0);
	ASTnode<int> b(0);

	*a.acces_left_ptr() = std::move(b);

	std::cout << a.acces_left_ptr()->acces_content() << std::endl;







}
*/



// Note printer is a helper functor that give some T will print it
template<typename T,class Printer>
class ASTnode {
	private:

	static size_t node_size;

	T * Content;
	ASTnode* left;
	ASTnode* right;

public:

	static size_t get_node_size(){
		return node_size;
	}

	ASTnode() = delete;
	ASTnode(const T& init) : Content(new T(init)), left(), right() { node_size++; }; // C++ allows references to temporaries as long as they are const 
	ASTnode(T&& init) : Content(new T(std::move(init))), left(), right() { node_size++; };
	ASTnode(const ASTnode<T,Printer>& other) : Content(new T(*other.Content)), left(other.left ? new ASTnode<T,Printer>(*other.left) : nullptr), right(other.right ? new ASTnode<T,Printer>(*other.right) : nullptr) { node_size++; };
	ASTnode(ASTnode<T,Printer>&& other) noexcept: Content(other.Content), right(other.right), left(other.left) { // No need to delete content since we are building the object it doesent have anything in it yet 
		
		
		other.Content = nullptr;
		other.right = nullptr;
		other.left = nullptr;
	};

	~ASTnode() {
		delete Content;
		delete left;
		delete right;
		node_size--;
		//std::cout << get_node_size() << std::endl;
	
	}
	// opearot overloading -----------------------------------------------------------------------

	ASTnode<T,Printer>& operator=(ASTnode<T,Printer>&& other) noexcept {
		if (this != &other) {
			delete Content;
			delete right;
			delete left;

			Content = other.Content;
			right = other.right;
			left = other.left;

			other.Content = nullptr;
			other.left = nullptr;
			other.right = nullptr;

		}
		return *this;
	};

	
	ASTnode<T,Printer>& operator=(const ASTnode<T,Printer>& other) {
		*Content = *other.Content;
		return *this;
		 
	}

	ASTnode<T,Printer>& operator=(const T& other) {
		*Content = other;
		return *this;
	}

	void print_tree(Printer& p){
		print_tree_helper(p);
	}
	
	void print_tree_helper(Printer& p, const std::string& prefix = "", bool isLeft = false)
	{
		// Print the right subtree first (top of the visual tree)
		if (has_right()) {
			get_right().print_tree_helper(p, prefix + (isLeft ? "|   " : "    "), false);
		}

		// Print this node
		std::cout << prefix;
		std::cout << (isLeft ? "\\-- " : "/-- ");
		p(get_Content());
		std::cout << std::endl;

		// Print the left subtree (bottom of the visual tree)
		if (has_left()) {
			get_left().print_tree_helper(p, prefix + (isLeft ? "    " : "|   "), true);
		}
	}

	

	ASTnode<T,Printer>& push_left(ASTnode<T,Printer>&& other) {
		if (!has_left()) {
			left = new ASTnode<T,Printer>(other);
		}
		else {
			throw std::runtime_error("left already exists");
		}
		return *left;
	}

	ASTnode<T,Printer>& push_right(ASTnode<T,Printer>&& other) {
		if (!has_right()) {
			right = new ASTnode<T,Printer>(other);
		}
		else {
			throw std::runtime_error("right already exists");
		}
		return *right;
	}


	ASTnode<T,Printer>& push_left(const T& other) {
		if (!has_left()) {
			left = new ASTnode<T,Printer>(other);
		}
		else {
			throw std::runtime_error("left already exists");
		}
		return *left;
	}

	ASTnode<T,Printer>& push_right(const T& other) {
		if (!has_right()) {
			right = new ASTnode<T,Printer>(other);
		}
		else {
			throw std::runtime_error("right already exists");
		}
		return *right;
	}

	ASTnode<T,Printer>& get_left() {
		if (has_left()) {
			return *left;
		}
		else {
			throw std::runtime_error("left does not exist");
		}
	
	}

	ASTnode<T,Printer>& get_right() {
		if (has_right()) {
			return *right;
		}
		else {
			throw std::runtime_error("right does not exist");
		}

	}

	

	bool has_right() const {
		return right;
	}

	bool has_left() const {
		return left;
	}


	T& get_Content(){
		return *Content;
	}

	//no need since the tree must be initialized;
	//bool has_content() const {return Content;}


	
};




