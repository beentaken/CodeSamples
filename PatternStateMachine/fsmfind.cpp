namespace CS330
{
    template <typename Symbol>
    FSMFind<Symbol>::FSMFind( const std::vector<Symbol>& _pattern) :
    pattern(_pattern),
	states(std::vector<State<Symbol>*>()),
	vecOnFind(std::vector<AbstractCallback*>()),
	vecOnDone(std::vector<AbstractCallback*>())
    {
		//Create states
        const std::vector<Symbol> empty_symbol;
		State<Symbol>* empty_state = new State<Symbol>(empty_symbol);
		states.push_back(empty_state);
		
		//Every other state
		for(unsigned i = 1; i < pattern.size(); ++i)
		{
			std::vector<Symbol> sub;
			
			for(unsigned j = 0; j < i; ++j)
				sub.push_back(pattern[j]);
			
			State<Symbol>* state = new State<Symbol>(sub,
										            false,
										            states[0]);
			states.push_back(state);
		}
		
		//End state
		State<Symbol>* end_state = new State<Symbol>(pattern,
													 true,
													 states[0]);
		states.push_back(end_state);
		
        CreateTransitions();
    }
    
    template <typename Symbol>
    FSMFind<Symbol>::~FSMFind()
    {
        for(unsigned i = 0; i < states.size(); ++i)
			delete states[i];
    }
    
    template <typename Symbol>
    void FSMFind<Symbol>::RegisterOnFind(AbstractCallback* p_cb)
    {
        vecOnFind.push_back(p_cb);
    }
    
    template <typename Symbol>
    void FSMFind<Symbol>::RegisterOnDone(AbstractCallback* p_cb)
    {
        vecOnDone.push_back(p_cb);
    }
    
    template <typename Symbol>
    void FSMFind<Symbol>::Find(const std::vector<Symbol> & text)
    {	
		State<Symbol>* state = states[0];
	
        for(unsigned i = 0; i < text.size(); ++i)
		{
			state = const_cast<State<Symbol>*>(state->NextState(text[i]));
			
			if(state->IsTerminal())
			{
				for(unsigned j = 0; j < vecOnFind.size(); ++j)
					vecOnFind[j]->OnFind(i);
			}
		}
		
		for(unsigned i = 0; i < vecOnDone.size(); ++i)
			vecOnDone[i]->OnDone();
    }
    
    template <typename Symbol>
    std::vector<Symbol> FSMFind<Symbol>::substring(const std::vector<Symbol> & str, size_t pos, size_t length) const
    {
        std::vector<Symbol> sub;
		
		for(unsigned i = 0; i < length; i++)
			sub.push_back(str[pos + i]);
		
		return sub;
    }
    
    template <typename Symbol>
    void FSMFind<Symbol>::CreateTransitions()
    {
		std::vector<Symbol> unique_sym;
		unique_sym.push_back(pattern[0]);
		
		//Loop and add all unique symbols
		for(unsigned i = 1; i < pattern.size(); ++i)
		{
			if(std::find(unique_sym.begin(), unique_sym.end(), pattern[i]) == unique_sym.end())
				unique_sym.push_back(pattern[i]);
		}
		
		for(unsigned i = 0; i < states.size(); ++i)
		{
			//make a substring with certain amount of letters from pattern
			std::vector<Symbol> sub = states[i]->Name();
			
			for(unsigned j = 0; j < unique_sym.size(); ++j)
			{
				//Add one of the unique letters onto the currecnt substring
				sub.push_back(unique_sym[j]);
				
				//Check if this is the next transition
				if(i != states.size() - 1 && sub == states[i + 1]->Name())
				{
					states[i]->AddTransition(unique_sym[j], states[i + 1]);
					sub.pop_back();
					break;
				}
				else
				{
					//Loop through and see if there is a transition in here
					for(unsigned k = 1; k < sub.size(); ++k)
					{
						std::vector<Symbol> str = substring(sub, k, sub.size() - k);
						
						if(states[str.size()]->Name() == str)
						{
							states[i]->AddTransition(unique_sym[j], states[str.size()]);
							break;
						}
					}
				}
				
				sub.pop_back();
			}
		}
    }
}