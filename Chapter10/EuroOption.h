#include "PRNG.h"

template<class T>
class EuropeanCall {
public:
        /* @Brief Class constructor */
        EuropeanCall(T _S, T _R, T _Y, T _Sig, T _Strike, T _time);
        /* @Brief Function to simulate using Monte Carlo, overwrite later */
        void simulate(RNG generator);
        /* @Brief Function to print option price and other variables to screen */
        void variables();
	/* @Brief Function to load variables to tape */
        void tapeLoad();
	/* @Brief Payoff function */
	void payoff();
	/* @Brief Function to price option */
	void pricer(RNG generator, const size_t N, Tape tape);
	/* @Brief Intialisation function */
	void init();
//protected:
        const T s0; /* Spot price */
        const T r; /* Interest rate */
        const T y; /* Dividend rate */
        const T sigma; /* Volatility */
        const T k; /* Strike */
        const T t; /* Time */
        T payout; /* Payoff */
	std::vector<T> path;
	T dt;
	T dx;
	std::vector<double> rngs; 
};