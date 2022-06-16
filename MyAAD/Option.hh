/* 
 * @Author Shane Keane
 * @File Option.h
 * @Brief File containing classes for European and Asian options
 * @Version 1.0
 * @Date 09-06-2022
 */

//#ifdef __OPTIONOBJECT__
//#define __OPTIONOBJECT__

#include "PRNG.h"
#include <iostream>
#include <cmath> 

template<typename T>
class Option {
public:
       		
	/* @Brief Class constructor */
	Option(T S, T R, T Y, T Sig, T Strike, T time):
		S0{S}, r{R},y{Y}, sigma{Sig}, K{Strike}, t{time}{
                std::cout << "Option Construtor \n";
                T payout{0};
	}
	/* @Brief Function to calculate payoff, overwrite later */
	virtual T payoff(T x);
	/* @Brief Function to simulate using Monte Carlo, overwrite later */
	virtual void simulate(RNG generator, const size_t Npaths);
	/* @Brief Function to print option price to screen */
	void price(){
		std::cout << "Option price = " << payout << "\n";	
	}

	const T S0; /* Spot price */
	const T r; /* Interest rate */
	const T y; /* Dividend rate */
	const T sigma; /* Volatility */
	const T K; /* Strike */
	const T t; /* Time */
	T payout; /* Payoff */
};


template <typename T>
class EuropeanCall : public Option<T>
{
public:
	/* @Brief Class constructor */
	EuropeanCall(T S, T R, T Y, T Sig, T Strike, T time):
		Option(S, R, Y, Sig, Strike, time){}
	/* @Brief Overwriting payoff function */
	T payoff(T x){
		if(x>K){
			return x-K;
		} else {
			return 0;
		}
	}	
	/* @Brief Overwriting function to simulate and price using MC */
	void simulate(RNG generator, const size_t Npaths){
		auto rngs = generator.gaussian(Npaths);
		T St;
		for(auto z : rngs){
			St = (S0)*std::exp((r-0.5*std::pow(sigma, 2))*t + sigma*std::sqrt(t)*z);
			payout += payoff(St);
		}
		payout/=Npaths;
	}

};


//template class Option<double>;
//template class EuropeanCall<double>;
/*
class AsianCall: public Option {

};
*/

//#endif
