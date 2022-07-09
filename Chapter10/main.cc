#include <memory>
#include <string>
#include <queue>
#include <cmath>
#include <iostream>
#include "AAD.h"
//#include "Option.h"
#include "EuroOption.h"
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
	globalTape.clear();
        /* S0,  r ,  y ,sigma,  K , t */
	Number S0{100};
	Number r{0.01};
	Number y{0.00};
	Number sigma{0.1};
	Number K{105};
	Number t{0.5};

	/* Putting variables on tape */
	S0.putOnTape();
	r.putOnTape();
	sigma.putOnTape();
	K.putOnTape();
	t.putOnTape();
	
	/* Option constructor */
	EuropeanCall<Number> Call{S0, r, y, sigma, K, t};
	//Call.init();
	//AsianCall<Number> Call{S0, r, y, sigma, K, t};
	/* Mark tape to be able to rewind to */
	const size_t N = 100;//'000;
	RNG generator{43121};
	Call.pricer(generator, 1, globalTape);
	globalTape.mark();
	for(auto i=0; i<N; ++i){
		globalTape.rewindToMark();
		Call.pricer(generator, 1, globalTape);
		Call.result.propagateToMark();
		Call.payout += Call.result.value();
	}
	Call.payout /= (N*exp(Call.r*Call.t));
		Call.variables();
	//(Call.payout).propagateToMark();
	(Call.payout).propagateMarkToStart();
	//Call.payout.propagateToStart();
	auto Delta = (Call.s0).adjoint();
	auto Vega = (Call.sigma).adjoint();
	auto Rho = (Call.r).adjoint();
	auto Theta = (Call.t).adjoint();
	std::cout << "Delta = " << Delta << std::endl;
	std::cout << "Vega = "  << Vega  << std::endl;
	std::cout << "Rho = "   << Rho   << std::endl;
	std::cout << "Theta = " << Theta << std::endl; 
        return 0;
}
