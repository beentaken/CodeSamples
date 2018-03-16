/*
Name: Zachery Lewis
Email: z.lewis@digipen.edu
Course: CS330
Semester: Spring
*/
#include "knapsack-dp.h"
#include <iostream>
#include <numeric>

int max(int a, int b)
{
	return (a > b)? a : b;
}

////////////////////////////////////////////////////////////
Item::Item( int const& weight, int const& value ) 
	: weight(weight), value(value) 
{
}

////////////////////////////////////////////////////////////
Item::Item( Item const& original ) 
	: weight(original.weight), value(original.value)
{
}

////////////////////////////////////////////////////////////
std::ostream& operator<< (std::ostream& os, Item const& item) {
	os << "(" << item.weight << " , " << item.value << ") ";
	return os;
}

////////////////////////////////////////////////////////////
std::istream& operator>> (std::istream& os, Item & item) {
	os >> item.weight >> item.value;
	return os;
}

////////////////////////////////////////////////////////////
typedef std::vector< std::vector<int> > Table; //2-dimensional table

////////////////////////////////////////////////////////////
//the returned value is a vector of indices
std::vector<int> knapsackDP( std::vector<Item> const& items, int const& W ) {
	int num_items = items.size();
	Table table;
	std::vector<int> temp;
	
	for(int n = 0; n <= num_items; ++n)
		temp.push_back(0);

	for(int w = 0; w <= W; ++w)
		table.push_back(temp);
	
	for(int n = 1; n <= num_items; ++n)
	{
		for(int w = 1; w <= W; ++w)
		{
			if( w >= items[n-1].weight)
				table[w][n] = max(table[w][n-1], table[w-items[n-1].weight][n-1] + items[n-1].value);
			else
				table[w][n] = table[w][n-1];
		}
	}

	//print final table - for debugging?
    //do not delete this code
    if ( num_items + W < 50 ) { //print only if table is not too big
        std::cout << "   ";
        for ( int n=0; n<=num_items; ++n) { std::cout << n << "     "; }
        std::cout << "  items\n        ";
        for ( int n=0; n<num_items; ++n) { std::cout << items[n].weight << "," << items[n].value<<"   "; }
        std::cout << "\n   ";
        for ( int n=0; n<=num_items; ++n) { std::cout << "------"; }
        std::cout << std::endl;

        for ( int w=0; w<=W; ++w) {
            std::cout << w << "| ";
            for ( int n=0; n<=num_items; ++n) {
                std::cout << table[w][n] << "     ";
            }
            std::cout << std::endl;
        }
    }
    //end do not delete this code 


	//figure out which items are in the bag based on the table
	std::vector<int> bag;
	int w = W;
	for(int n = num_items; n >= 0; n--)
	{
		if(table[w][n] == table[w][n-1])
			continue;
		
		bag.push_back(n- 1);
		
		w = w - items[n-1].weight;
		
		if(table[w][n-1] == 0)
			break;
	}
	
	return bag;
}

////////////////////////////////////////////////////////////
int valueBag( std::vector<Item> const& items, std::vector<int> const& bag ) {
	std::vector<int>::const_iterator it   = bag.begin(),
		                          it_e = bag.end();

	int accum = 0;
	//std::cout << "Bag ";
	for ( ; it != it_e; ++it) { 
		accum += items[ *it ].value; 
		//std::cout << *it << " ";
	}
	//std::cout << std::endl;
	return accum;
}

////////////////////////////////////////////////////////////
//prototype
//notice that auxiliary function returns value of the vector of items
//the actual vector is determined later from the table (similar to DP solution)
int knapsackRecMemAux( std::vector<Item> const&, int, int, Table& );

////////////////////////////////////////////////////////////
//function to kick start
std::vector<int> knapsackRecMem( std::vector<Item> const& items, int const& W ) {
	int num_items = items.size();
	Table table;
	
	std::vector<int> temp;
	
	for(int n = 0; n <= num_items; ++n)
		temp.push_back(0);

	for(int w = 0; w <= W; ++w)
		table.push_back(temp);
	
	
	for(int n = 1; n <= num_items; ++n)
	{
		for(int w = 1; w <= W; ++w)
		{
			table[w][n] = -1;
		}
	}
	
	knapsackRecMemAux(items, W, num_items, table);

	//print table - debugging?
    //do not delete this code
    if ( num_items + W < 50 ) { //print only if table is not too big
        std::cout << "   ";
        for ( int n=0; n<=num_items; ++n) { std::cout << n << "     "; }
        std::cout << "  items\n        ";
        for ( int n=0; n<num_items; ++n) { 
            std::cout << items[n].weight << "," << items[n].value<<"   "; 
        }
        std::cout << "\n   ";
        for ( int n=0; n<=num_items; ++n) { std::cout << "------"; }
        std::cout << std::endl;

        for ( int w=0; w<=W; ++w) {
            std::cout << w << "| ";
            for ( int n=0; n<=num_items; ++n) {
                std::cout << table[w][n] << "     ";
            }
            std::cout << std::endl;
        }
    }
    //end do not delete this code 

	//figure out which items are in the bag based on the table
	std::vector<int> bag;
	
	int w = W;
	for(int n = num_items; n >= 0; n--)
	{
		if(table[w][n] == table[w][n-1])
			continue;
		
		bag.push_back(n- 1);
		
		w = w - items[n-1].weight;
		
		if(table[w][n-1] == 0)
			break;
	}
	
	return bag;
}

////////////////////////////////////////////////////////////
//the real recursive function
int knapsackRecMemAux( std::vector<Item> const& items, int w, int index, Table& table ) 
{
	if(table[w][index] != -1)
		return table[w][index];
	
	if(index == 0 || w == 0)
		return 0;
	
	if(w >= items[index-1].weight)
	{
		table[w][index] = max(knapsackRecMemAux(items, w, index - 1, table), knapsackRecMemAux(items, w-items[index-1].weight, index - 1, table) + items[index-1].value);
		return table[w][index];
	}
	else
	{
		table[w][index] = knapsackRecMemAux(items, w, index - 1, table);
		return table[w][index];
	}
}
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
