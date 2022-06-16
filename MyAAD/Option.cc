/*
 * @Author Shane Keane
 * @File Option.cc
 * @Brief File containing class functions for European and Asian options
 * @Version 1.0
 * @Date 09-06-2022
 */


#include "Option.h"
#include <iostream>
#include <cmath> 

template<typename T>
Option<T>::Option(T S, T R, T Y, T Sig, T Strike, T time):
			S0{S}, r{R},y{Y}, sigma{Sig}, K{Strike}, t{time}{
		//S0{S};r{R};y{Y};sigma{Sig};K{Strike};t{time};
		std::cout << "Option Construtor \n";
		T payout{0};
}

template<typename T>
T Option<T>::payoff(T x) {
	return x;
}

template<typename T>
void Option<T>::price()
{
	std::cout << "Spot Price = " << S0 << ", Interest rate (r) = " << r << ", Dividend Yield = " << y << "\n" << "Volatility = " << sigma << ", Strike Price = " << K << ", Time(years) = " << t << "\n";
	std::cout << "Option price = " << payout << "\n";
}



template<typename T>
EuropeanCall<T>::EuropeanCall(T S, T R, T Y, T Sig, T Strike, T time){
	Option{S, R, Y, Sig, Strike, time};
}

template <typename T>
T EuropeanCall<T>::payoff(T x){
	if(x>this->K){
		return x-this->K;
	} else {
		return 0;
	}
}
template<typename T>
void EuropeanCall<T>::simulate(RNG generator, const size_t Npaths){
	auto rngs = generator.gaussian(Npaths);
	T St;
	for(auto z : rngs){
		//St = S0*exp((r-0.5*pow(sigma, 2))*t + sigma*sqrt(t)*z);
		St = (this->S0)*exp((this->r + this->sigma*z));
		payout += payoff(St);
	}
	payout/=Npaths;
}

template class Option<double>;
template class EuropeanCall<double>;
