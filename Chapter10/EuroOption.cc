#include "AAD.h"
#include "EuroOption.h"
#include <iostream>
#include <cmath>
#include <numeric>


template<typename T>
EuropeanCall<T>::EuropeanCall(T _S, T _R, T _Y, T _Sig, T _Strike, T _time):
                        s0{_S}, r{_R},y{_Y}, sigma{_Sig}, k{_Strike}, t{_time}{
                std::cout << "Option Construtor \n";
                T payout{0};
		path.resize(int(t.value()*255));
}

template<typename T>
void EuropeanCall<T>::variables(){
        std::cout << "Spot Price = " << s0 << ", Interest rate (r) = " << r << ", Dividend Yield = " << y << "\n" << "Volatility = " << sigma << ", Strike Price = " << k << ", Time(years) = " << t << "\n";
        std::cout << "Option price = " << payout << "\n";
}

template<typename T>
void EuropeanCall<T>::init(){
	auto Ndays = t.value()*255;
	path.resize(Ndays);
	rngs.resize(Ndays);
	dt = r*t/Ndays;
        dx = sigma*sqrt(t/Ndays);
	//dt.putOnTape();
	//dx.putOnTape();	
}


template<typename T>
void EuropeanCall<T>::simulate(RNG generator){
        auto Ndays{t*255};
        rngs = generator.gaussian(Ndays.value());
        path[0]=T{1};
        for(auto j=1;j<int(Ndays.value());++j){
                path[j]=path[j-1]*(exp(dt) + dx*rngs[j]);
        }
}

template<typename T>
void EuropeanCall<T>::payoff(){
        payout += max((path.back()*s0)-k,T{0});
}


template<typename T>
void EuropeanCall<T>::pricer(RNG generator, const size_t N, Tape tape){
        for(auto i=0; i<N; ++i){
                tape.rewindToMark();
                simulate(generator);
                payoff();
                payout.propagateMarkToStart();
        }
        payout /= (N*(exp(r*t)));
}


template class EuropeanCall<Number>;
