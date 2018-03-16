namespace CS330 {
////////////////////////////////////////////////////////////
	template <typename Symbol>
	BruteForceFind<Symbol>::BruteForceFind(const std::vector<Symbol>& _pattern) 
	: pattern(_pattern)
	{ }

////////////////////////////////////////////////////////////
	template <typename Symbol>
	void BruteForceFind<Symbol>::Find(const std::vector<Symbol> & text) {
		if (text.size()<pattern.size()) return; 

		/* Searching */
		unsigned j=0;
		for (   ;j <= text.size()-pattern.size(); ++j) {
			unsigned i=0;
			for (   ;i < pattern.size() && pattern[i] == text[i + j]; ++i)
			{} //empty body

			if (i >= pattern.size())
				std::cout << "BruteForceFind: pattern at position " 
					<< j << std::endl;
		}
	}
}
