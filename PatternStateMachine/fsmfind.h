#ifndef FSMFIND_H
#define FSMFIND_H
#include <vector>
#include "abstract-callback.h"
#include <algorithm>

// this is a SAMPLE - driver only uses public API
// You can change anything you want in the private section.

namespace CS330 {
	template <typename Symbol>
		class FSMFind {
			public:
				FSMFind( const std::vector<Symbol>& _pattern); 
				~FSMFind();
                // client registers handlers
				void RegisterOnFind(AbstractCallback* p_cb);
				void RegisterOnDone(AbstractCallback* p_cb);
                // main functionality
				void Find(const std::vector<Symbol> & text);
			private:
                FSMFind(const FSMFind&) = delete;
                FSMFind& operator=(const FSMFind&) = delete;
				////////////////////////////////////////////////////////////
				std::vector<Symbol> 
					substring(const std::vector<Symbol> & str, size_t pos, size_t length) const;
                // preprocessing - called in the end of constructor
				void CreateTransitions();
			private:
				std::vector<Symbol> const      pattern;
				std::vector<State<Symbol>*>    states;
				std::vector<AbstractCallback*> vecOnFind;
				std::vector<AbstractCallback*> vecOnDone;
		};
}
#include "fsmfind.cpp"
#endif
