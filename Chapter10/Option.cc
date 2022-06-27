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
#include <numeric>

template<typename T>
Option<T>::Option(T _S, T _R, T _Y, T _Sig, T _Strike, T _time):
			s0{_S}, r{_R},y{_Y}, sigma{_Sig}, k{_Strike}, t{_time}{
		std::cout << "Option Construtor \n";
		T payout{0};
}

template<typename T>
void Option<T>::price()
{
	std::cout << "Spot Price = " << s0 << ", Interest rate (r) = " << r << ", Dividend Yield = " << y << "\n" << "Volatility = " << sigma << ", Strike Price = " << k << ", Time(years) = " << t << "\n";
	std::cout << "Option price = " << payout << "\n";
}


template<typename T>
EuropeanCall<T>::EuropeanCall(T _S, T _R, T _Y, T _Sig, T _Strike, T _time):
	Option<T>{_S, _R, _Y, _Sig, _Strike, _time}{}

template<typename T>
AsianCall<T>::AsianCall(T _S, T _R, T _Y, T _Sig, T _Strike, T _time):
        Option<T>{_S, _R, _Y, _Sig, _Strike, _time}{}

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
/*
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
*/
template<typename T>
std::vector<std::vector<T>> Option<T>::simulate(RNG generator, const size_t N){
	auto Ndays = 255*t;
	auto rngs = generator.gaussian(N, Ndays);
	std::vector<std::vector<T>> paths(N, std::vector<T>(Ndays));
	for(auto i=0;i<N;++i){
		paths[i][0] = s0;
		for(auto j=1;j<Ndays;++j){
			paths[i][j] = paths[i][j-1]*(1 + this->r/Ndays + this->sigma*sqrt(1/Ndays)*rngs[i][j-1]);
		}
	}		
	return paths;
}

template<typename T>
void EuropeanCall<T>::payoff(std::vector<std::vector<T>> paths, const size_t N){
	for(auto i=0;i<N;i++){
		this->payout += max((paths[i].back())-(this->k),0.0);
	}
	this->payout /= (N*(1+this->r));
}


template<typename T>
void AsianCall<T>::payoff(std::vector<std::vector<T>> paths, const size_t N){
        T average{0};
	for(auto i=0;i<N;++i){
		average = std::accumulate(paths[i].begin(), paths[i].end(), 0);
               	this->payout += max(average-(this->k),0.0);
		average = 0;
        }
        this->payout /= (N*255*(1+this->r));
}

template class Option<double>;
template class EuropeanCall<double>;
template class AsianCall<double>;
