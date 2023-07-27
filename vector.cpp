#include <iostream>
#include <vector>

int main()
{
	std::vector<int> numbers;

	for (size_t i=0; i<10; i++)
	{
		// adds an element to the vector
		numbers.push_back(i);
	}

	// pops an element from the vector
	numbers.pop_back();

	// 
	for (std::vector<int>::iterator i=numbers.begin(); i != numbers.end(); ++i)
		std::cout << *i << "\n";

}