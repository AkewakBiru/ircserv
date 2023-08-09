#include <iostream>
#include <vector>
#include <sstream>
#include <string>

//" \t\r\v\n\f"

// std::vector<std::string> parseInput(char const *data, std::string const &delim)
// {
// 	std::vector<std::string> _res;
// 	std::stringstream stream(data);
// 	std::string val;

// 	while (std::getline(stream, val))
// 	{
// 		size_t start = 0;
// 		size_t end = val.find_first_of(delim);

// 		while (end != std::string::npos)
// 		{
// 			if (end != start)
// 				_res.push_back(val.substr(start, end - start));
// 			start = end + 1;
// 			end = val.find_first_of(delim, start);
// 		}

// 		if (start < val.length())
// 			_res.push_back(val.substr(start));
// 	}
// 	return (_res);
// }

#include <typeinfo>

void printer(int *a)
{
	std::printf("%d\n", *a);
}
int main()
{
	// char const data[1024] = "this	is a test\nanother";
	// std::vector<std::string> res = parseInput(data, " \t\r\v\n\f");
	// for (std::vector<std::string>::iterator a=res.begin(); a!=res.end(); ++a)
	// {
	// 	std::cout << *a << std::endl;
	// }
	std::vector<int> a;
	a.push_back(1);
	a.push_back(2);
	a.push_back(3);
	a.push_back(4);
	printer(a.begin().base());
	// for (std::vector<int>::iterator it=a.begin(); it!=a.end(); it++)
	// 	std::cout << *it << "\t";
	// std::cout << *(a.begin()) << std::endl;
	// std::cout << typeid(a.begin().base()).name() << "\n";
}
