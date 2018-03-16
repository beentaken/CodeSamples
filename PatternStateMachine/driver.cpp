#include <iostream>
#include <ctime>
#include <cstdlib>
#include <vector>
#include "concrete-callback.h"
#include "state.h"
#include "bruteforcefind.h"
#include "fsmfind.h"

#define xTIME

////////////////////////////////////////////////////////////
std::vector<char> string2vector(const std::string & str) {
	std::vector<char> v;
	for (unsigned i=0; i<str.size();++i) {
		v.push_back(str[i]);
	}
	return v;
}
////////////////////////////////////////////////////////////
//user-defined character class based on c-strings, 
//that is each character is a pair "aa","ab", ....
#include <fstream>
class MyCharacter {
	char c1,c2;
	public:
		MyCharacter(char _c1,char _c2) : c1(_c1),c2(_c2) {}
		bool operator==(const MyCharacter & rhs) const {
			return c1==rhs.c1 && c2==rhs.c2;
		}
		bool operator<(const MyCharacter & rhs) const {//required by map
			if (c1==rhs.c1) return c2<rhs.c2;
			else return c1<rhs.c1;
		}
        //friend function may be implemented in class scope
        friend std::ostream& operator<<(std::ostream& os, const MyCharacter & object) {
            os << "("<<object.c1<<","<<object.c2<<")";
            return os;
        }
};
////////////////////////////////////////////////////////////


void test0() {
	typedef char Symbol;
	std::vector<Symbol> pattern(string2vector("abac"));
	std::vector<Symbol> 
		text(string2vector("abababababacacacacacacacacacacababababacacac"));

//	CS330::BruteForceFind<Symbol> bfpf(pattern);
//    std::cout << "Using brute-force find:\n";
//	bfpf.Find(text);

	//        Finite State Machine for pattern "abac"
	//              +-a-+
	//              |   |
	//           a   \ /   b      a       c
	//        0 ----> 1 ----> 2----> 3 ----> 4
	//                ^       ^      |       |
	//                |       +--b---+       |
	//                |              |       a
	//                +---a----------+       |
	//                ^                      |
	//                +----------------------+
	//		 all other arrows go to state 0.
	CS330::AbstractCallback* 
		p_cb2 = new CS330::ConcreteCallback2<Symbol>(pattern);
	CS330::FSMFind<Symbol> pf(pattern);
	//callback classes for OnFind and OnDone
	pf.RegisterOnFind(p_cb2);
	pf.RegisterOnDone(p_cb2);
    std::cout << "Using FSM find:\n";
	pf.Find(text);
	delete p_cb2;
}

void test1() {
	typedef char Symbol;
	std::vector<Symbol> pattern(string2vector("acaca"));
	std::vector<Symbol> 
		text( string2vector("abababababacacacacacacacacacacababababacacac") );
	//abababababacacacacacacacacacacababababacacac  
	//1         -----                  
	//2           -----                    
	//3             -----                      
	//4               -----                    
	//5                 -----                   
	//6                   -----                  
	//7                     -----
	//8                       -----
	//9                         -----
	//10                                    -----

//	CS330::BruteForceFind<Symbol> bfpf(pattern);
//    std::cout << "Using brute-force find:\n";
//	bfpf.Find(text);


	//		 Finite State Machine for pattern "acaca"
	//           +---+
	//           | a |
	//        a   \ /   c      a       c       a
	//     0 ----> 1 ----> 2----> 3 ----> 4 ----> 5
	//     ^                      |       ^       |
	//     |                      a       |       |
	//     |                      |       +--c----+
	//     +----------------------+               |
	//     ^                                      a
	//     |                                      |
	//     +--------------------------------------+
	CS330::AbstractCallback* p_cb1 = new CS330::ConcreteCallback1();
	CS330::AbstractCallback* 
		p_cb2 = new CS330::ConcreteCallback2<Symbol>(pattern);
	CS330::FSMFind<Symbol> pf(pattern);
	//2 different callback classes for OnFind and 1 for on done OnDone
	pf.RegisterOnFind(p_cb1);
	pf.RegisterOnFind(p_cb2);
	pf.RegisterOnDone(p_cb2);
    std::cout << "Using FSM find:\n";
	pf.Find(text);
	delete p_cb1;
	delete p_cb2;
}

void test2() {
	std::string t;
	for (int i=0;i<1<<20;++i) { t+="ab"; }
	t += "ac";

	typedef char Symbol;
	std::vector<Symbol> pattern(string2vector("abac"));
	std::vector<Symbol> text(string2vector(t));

//	CS330::BruteForceFind<Symbol> bfpf(pattern);
//#ifdef TIME
//	std::clock_t start, end;
//	start = std::clock();
//#endif
//    std::cout << "Using brute-force find:\n";
//	bfpf.Find(text);
//#ifdef TIME
//	end = std::clock();
//	std::cout << "Brute-force time " << (end - start) << std::endl;
//#endif

	CS330::AbstractCallback* p_cb = new CS330::ConcreteCallback2<Symbol>(pattern);
	CS330::FSMFind<Symbol> pf(pattern);
	//same callback classe for OnFind and OnDone
	pf.RegisterOnFind(p_cb);
	pf.RegisterOnDone(p_cb);
#ifdef TIME
	start = std::clock();
#endif
    std::cout << "Using FSM find:\n";
	pf.Find(text);
#ifdef TIME
	end = std::clock();
	std::cout << "FSM time " << (end - start) << std::endl;
#endif
	
	delete p_cb;
}

void test3() {
	std::string t;
	for (int i=0;i<1<<20;++i) { t+="acab"; }
	t += "acaca";

	typedef char Symbol;
	std::vector<Symbol> pattern(string2vector("acaca"));
	std::vector<Symbol> text(string2vector(t));

//	CS330::BruteForceFind<Symbol> bfpf(pattern);
//#ifdef TIME
//	std::clock_t start, end;
//	start = std::clock();
//#endif
//    std::cout << "Using brute-force find:\n";
//	bfpf.Find(text);
//#ifdef TIME
//	end = std::clock();
//	std::cout << "Brute-force time " << (end - start) << std::endl;
//#endif

	CS330::AbstractCallback* p_cb = new CS330::ConcreteCallback2<Symbol>(pattern);
	CS330::FSMFind<Symbol> pf(pattern);
	pf.RegisterOnFind(p_cb);

#ifdef TIME
	start = std::clock();
#endif
    std::cout << "Using FSM find:\n";
	pf.Find(text);
#ifdef TIME
	end = std::clock();
	std::cout << "FSM time " << (end - start) << std::endl;
#endif
	delete p_cb;
}

////////////////////////////////////////////////////////////
void test4() {
	std::string t;
	for (int i=0;i<1<<20;++i) { 
		t+=97 + static_cast<char>( std::rand()%(122-97) ); //add a random character 
		// from 'a' to 'y'
	}
	t += "zisisabiiiiigpatternzisisabiiiiigpatternz";

	typedef char Symbol;
	std::vector<Symbol> pattern(string2vector("zisisabiiiiigpatternz"));
	std::vector<Symbol> text(string2vector(t));

//	CS330::BruteForceFind<Symbol> bfpf(pattern);
//#ifdef TIME
//	std::clock_t start, end;
//	start = std::clock();
//#endif
//	bfpf.Find(text);
//#ifdef TIME
//	end = std::clock();
//	std::cout << "Brute-force time " << (end - start) << std::endl;
//#endif

	CS330::AbstractCallback* p_cb = new CS330::ConcreteCallback2<Symbol>(pattern);
	CS330::FSMFind<Symbol> pf(pattern);
	pf.RegisterOnFind(p_cb);
#ifdef TIME
	start = std::clock();
#endif
	pf.Find(text);
#ifdef TIME
	end = std::clock();
	std::cout << "FSM time " << (end - start) << std::endl;
#endif
	delete p_cb;

}

////////////////////////////////////////////////////////////
void test5() {
	typedef MyCharacter Symbol;
	std::vector<Symbol> pattern;
	pattern.push_back( MyCharacter('a','a') );
	pattern.push_back( MyCharacter('b','t') );
	std::vector<Symbol> text;
	text.push_back( MyCharacter('a','b') );//0
	text.push_back( MyCharacter('b','a') );//1
	text.push_back( MyCharacter('b','a') );//2
	text.push_back( MyCharacter('a','a') );//3 -- match
	text.push_back( MyCharacter('b','t') );//4 -- match
	text.push_back( MyCharacter('b','a') );//5
	text.push_back( MyCharacter('b','a') );//6

	CS330::AbstractCallback* p_cb2 = new CS330::ConcreteCallback2<Symbol>(pattern);
	CS330::FSMFind<Symbol> pf(pattern);
	pf.RegisterOnFind(p_cb2);
	pf.RegisterOnDone(p_cb2);
	pf.Find(text);
	delete p_cb2;
}


/////////////////////////////////////////////////V///////////
////////////////////////////////////////////////////////////
void test6();
void (*pTests[])(void) = { 
	test0,test1,test2,test3,test4,test5,test6
};
void test6() {
	//for (size_t i = 0; i<sizeof(pTests)/sizeof(pTests[0])-1; ++i) pTests[i](); //to slow for DrMemory
    test0();
    test1();
    test5();
}
////////////////////////////////////////////////////////////

#include <cstdio> //sscanf
int main(int argc, char ** argv) {
    if (argc >1) {
        int test = 0;
        std::sscanf(argv[1],"%i",&test);
        try {
            pTests[test]();
        } catch( const char* msg) {
            std::cerr << msg << std::endl;
        }
    }

    return 0;
}
