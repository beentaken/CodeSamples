#ifndef CONCRETECALLBACK_H
#define CONCRETECALLBACK_H
#include "abstract-callback.h"
#include <vector>

namespace CS330 {

    //a very simple implementation of AbstractCallback interface
    //both function just print a static message
    class ConcreteCallback1 : public AbstractCallback {
		public:
			void OnFind(size_t pos);
			void OnDone();
	};

    //a more useful implementation of AbstractCallback interface
    //OnFind will print pattern and its position and will keep of the 
    //  number of matches
    //OnDone will print total number of matches
	template <typename Symbol>
		class ConcreteCallback2 : public AbstractCallback {
			public:
				ConcreteCallback2(const std::vector<Symbol> & _pattern);
				void OnFind(size_t pos);
				void OnDone();
			private:
				const std::vector<Symbol> pattern;
				size_t count;
				void Print(const std::vector<Symbol> & v);
		};

}
#include "concrete-callback.cpp"
#endif
