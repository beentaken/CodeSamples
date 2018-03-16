namespace CS330
{
	template <typename Symbol>
	State<Symbol>::State(const std::vector<Symbol>& _name) :
	name(_name),
	isTerminal(false),
	transitions(std::map<Symbol,const State<Symbol>*>()),
	p_default_state(this)
	{ }
	
	template <typename Symbol>
	State<Symbol>::State(  const std::vector<Symbol>& _name, 
							bool _isTerminal, 
							const State* _p_default_target) :
							name(_name),
							isTerminal(_isTerminal),
							transitions(std::map<Symbol,const State<Symbol>*>()),
							p_default_state(_p_default_target)
	{ }
	
	template <typename Symbol>
	void State<Symbol>::AddTransition(const Symbol & sym, const State * p_target)
	{
		transitions[sym] = p_target;
	}
	
	template <typename Symbol>
	const State<Symbol>* State<Symbol>::NextState(const Symbol & sym) const
	{
		if(transitions.find(sym) == transitions.end())
			return p_default_state;
		else
			return transitions.find(sym)->second;
	}
	
	template <typename Symbol>
	void State<Symbol>::PrintDebug() const
	{
		Print(name);
	}
	
	template <typename Symbol>
	std::vector<Symbol> State<Symbol>::Name() const
	{
		return name;
	}
	
	template <typename Symbol>
	bool State<Symbol>::IsTerminal() const
	{
		return isTerminal;
	}
	
	template <typename Symbol>
	void State<Symbol>::Print(const std::vector<Symbol> & v) const
	{
		for(unsigned i = 0; i < v.size(); i++)
            std::cout << v[i] << std::endl;
	}
}