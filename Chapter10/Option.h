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
	Option(T _S, T _R, T _Y, T _Sig, T _Strike, T _time);
	/* @Brief Function to calculate payoff, overwrite later */
	//virtual void payoff(std::vector<std::vector<T>> paths, const size_t N);
	/* @Brief Function to simulate using Monte Carlo, overwrite later */
	virtual std::vector<std::vector<T>> simulate(RNG generator, const size_t N);
	/* @Brief Function to print option price to screen */
	void price();
protected:	
	const T s0; /* Spot price */
	const T r; /* Interest rate */
	const T y; /* Dividend rate */
	const T sigma; /* Volatility */
	const T k; /* Strike */
	const T t; /* Time */
	T payout; /* Payoff */
};


template <class T>
class EuropeanCall : public Option<T> {
public:
	/* @Brief Class constructor */
	EuropeanCall(T _S, T _R, T _Y, T _Sig, T _Strike, T _time);
	/* @Brief Overwriting payoff function */
	void payoff(std::vector<std::vector<T>> paths, const size_t N);
};

template<class T>
class AsianCall: public Option<T> {
public:
	/* @Brief Class constructor */
	AsianCall(T _S, T _R, T _Y, T _Sig, T _Strike, T _time);
	/* @Brief Overwriting payoff function */
	void payoff(std::vector<std::vector<T>> paths, const size_t N);
};
