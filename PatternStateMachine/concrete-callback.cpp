namespace CS330 {
	void ConcreteCallback1::OnFind(size_t pos) {
		std::cout << "Found pattern at " << pos << std::endl;
	}
	void ConcreteCallback1::OnDone() {
		std::cout << "------DONE---------" << std::endl;
	}

	template <typename Symbol>
	ConcreteCallback2<Symbol>::ConcreteCallback2
		(const std::vector<Symbol> & _pattern)  
		: pattern(_pattern), count(0)
		{ }

	template <typename Symbol>
	void ConcreteCallback2<Symbol>::OnFind(size_t pos) {
		++count;
		std::cout << "Found pattern ";
		Print(pattern);
		std::cout << " at " << pos-pattern.size()+1 << "-" << pos << std::endl;
	}

	template <typename Symbol>
	void ConcreteCallback2<Symbol>::OnDone() {
		std::cout << "Pattern ";
		Print(pattern);
		std::cout << " found " << count << " times\n";
	}

	template <typename Symbol>
	void ConcreteCallback2<Symbol>::Print(const std::vector<Symbol> & v) {
		for (typename std::vector<Symbol>::const_iterator it=v.begin();
				it!=v.end();
				++it)
		{
			std::cout << *it << " ";
		}
	}
}
