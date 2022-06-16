//class RNG;
//template <class T>
//class Option;
//template<class T>
//class EuropeanCall;

#include <iostream>
#include "PRNG.h"
#include "Option.h"

template class Option<double>;
template class EuropeanCall<double>;

int main(){
        EuropeanCall<double> EuroCall{100,0.05,0.00,0.2,105,1};
        RNG generator{1234};
	EuroCall.simulate(generator, 100000);
        EuroCall.price();
	std::cout << "Normal Random Number = " << generator.gaussian() << "\n";
	return 0;
}
