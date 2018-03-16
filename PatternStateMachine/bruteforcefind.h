#ifndef BRUTEFORCEFIND_H
#define BRUTEFORCEFIND_H
#include <vector>


namespace CS330 {
	template <typename Symbol>
		class BruteForceFind {
			public:
				BruteForceFind(const std::vector<Symbol>& _pattern);
			private:
				//copy and assignment are private and have NO implementation
				//this way client cannot use them (and shouldn't)
				BruteForceFind(const BruteForceFind&);
				BruteForceFind& operator=(const BruteForceFind&);
			public:
				void Find(const std::vector<Symbol> & text);
			private:
				const std::vector<Symbol> pattern;
		};

}
#include "bruteforcefind.cpp"
#endif
