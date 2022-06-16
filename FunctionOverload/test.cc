#include <iostream>
#include <vector>
#include <cblas.h> 

int main() {
	std::vector<std::vector<double>> x = { {1.0,2.0}, {3.0,4.0}};
	auto a = x.begin();
	std::cout << *(*a).begin() << "\n";
	return 0;
}
