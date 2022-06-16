/* 
 * @Author Shane Keane
 * @File Option.h
 * @Brief File containing classes for European and Asian options
 * @Version 1.0
 * @Date 09-06-2022
 */

#include "PRNG.h"

template<class T>
class Option {
public: 
	/* @Brief Class constructor */
	Option(T S, T R, T Y, T Sig, T Strike, T time);
	/* @Brief Function to calculate payoff, overwrite later */
	virtual T payoff(T x);
	/* @Brief Function to simulate using Monte Carlo, overwrite later */
	virtual void simulate(RNG generator, const size_t Npaths, size_t Ndays);
	/* @Brief Function to print option price to screen */
	void price();
	
	T S0; /* Spot price */
	T r; /* Interest rate */
	T y; /* Dividend rate */
	T sigma; /* Volatility */
	T K; /* Strike */
	T t; /* Time */
	T payout; /* Payoff */
};


template <class T>
class EuropeanCall : public Option<T> {
public:
	/* @Brief Class constructor */
	EuropeanCall(T S, T R, T Y, T Sig, T Strike, T time);
	/* @Brief Overwriting payoff function */
	T payoff(T x);
	/* @Brief Overwriting function to simulate and price using MC */
	void simulate(RNG generator, const size_t Npaths);

};
/*
class AsianCall: public Option {

};
*/

