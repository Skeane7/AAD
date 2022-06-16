/*
 * @Author Shane Keane
 * @File Option.cc
 * @Brief File containing class functions for European and Asian options
 * @Version 1.0
 * @Date 09-06-2022
 */

#include "AAD.h"
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
/*
template<typename T>
T Option<T>::payoff(T x) {
	return x;
}
*/
template<typename T>
void Option<T>::price()
{
	std::cout << "Spot Price = " << S0 << ", Interest rate (r) = " << r << ", Dividend Yield = " << y << "\n" << "Volatility = " << sigma << ", Strike Price = " << K << ", Time(years) = " << t << "\n";
	std::cout << "Option price = " << payout << "\n";
}


/*
template<typename T>
EuropeanCall<T>::EuropeanCall(T S, T R, T Y, T Sig, T Strike, T time){
	Option{S, R, Y, Sig, Strike, time};
}
*/
template <typename T>
T Option<T>::payoff(T x){
	return max(x-K, T{0});
}

/*
template<typename T>
void Option<T>::simulate(RNG generator, const size_t Npaths){
	auto rngs = generator.gaussian(Npaths);
	T St;
	for(auto z : rngs){
	St = S0*exp((r-0.5*pow(sigma, 2))*t + sigma*sqrt(t)*z);
	payout += payoff(St);
	}
	payout/=(Npaths*(1+r));
}
*/

template<typename T>
void Option<T>::simulate(RNG generator, const size_t Npaths1, size_t Ndays){
	auto Ndays2 = Ndays*t.value();
	auto rngs = generator.gaussian(Npaths1*Ndays2);
	T St{0};
	T power{0};
	T deltaT = 1/(t*Ndays);
	T x = (r-pow(sigma, 2)/2)*deltaT;
	T sqrtDT = sigma*sqrt(deltaT);
	for(auto i=0;i<Npaths1;++i){
		for(auto day=0;day<Ndays2;++day){
			power += x + rngs[i*Ndays2+day]*sqrtDT;
			St += exp(power); 
		}
		power = 0;
		St *= (S0*deltaT);
		payout += payoff(St);
		St = 0;
	}
	payout /= (Npaths1*(1+r));
}

template class Option<Number>;
//template class EuropeanCall<double>;
