#include <memory>
#include <string>
#include <queue>
#include <cmath>
#include <iostream>
#include "AAD.h"
#include "Option.h"
//#include "PRNG.h"

size_t Node::numAdj = 1;
bool Tape::multi = false;

Tape globalTape;
thread_local Tape* Number::tape = &globalTape;


/* Templated Black Scholes function with approx normal distribution */
template <class T>
/* x[S0, r, y, sigma, K, T] */
T f(T S0, T r, T y, T sigma, T K, T t) {
        //std::cout << "df \n";
        auto df = exp(-r*t);
        //std::cout << "F \n";
        auto F = S0*exp((r - y)*t);
        //std::cout << "std \n";
        auto std = sigma * sqrt(t);
        //std::cout << "d \n";
        auto d = log(F/K)/std;
        //std::cout << "d1,d2 \n";
        auto d1 = d+0.5*std, d2 = d1-std;
        //std::cout << "nd1 \n";
        auto nd1 = normalCdf(d1), nd2 = normalCdf(d2);
        //std::cout << "c \n";
        auto c = df*(F*nd1 - K*nd2);
        return c;
}


int main() {
		      /* S0,  r ,  y ,sigma,  K , t */
	Number S0{100};
	Number r{0.01};
	Number y{0.00};
	Number sigma{0.1};
	Number K{105};
	Number t{1.0};
	
	Option<Number> Call{S0, r, y, sigma, K, t};
/*	
	(Call.S0).putOnTape();
	(Call.r).putOnTape();
	(Call.y).putOnTape();
	(Call.sigma).putOnTape();
	(Call.K).putOnTape();
	(Call.t).putOnTape();
*/
	RNG generator{7065};

	Call.simulate(generator, 100000, 255);
	Call.price();

        (Call.payout).propagateToStart();
	auto Delta = (Call.S0).adjoint();
	auto Vega = (Call.sigma).adjoint();
	auto Rho = (Call.r).adjoint();
	auto Theta = (Call.t).adjoint();
	std::cout << "Delta = " << Delta << std::endl;
	std::cout << "Vega = "  << Vega  << std::endl;
	std::cout << "Rho = "   << Rho   << std::endl;
	std::cout << "Theta = " << Theta << std::endl; 
        return 0;
}
