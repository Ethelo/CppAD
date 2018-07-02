/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-18 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

# include <limits>
# include <cppad/cppad.hpp>

bool new_dynamic(void)
{	bool ok = true;
	using CppAD::AD;
	using CppAD::NearEqual;
	double eps = 10. * std::numeric_limits<double>::epsilon();
	size_t n   = 3;

	// dynamic parameter vector
	CPPAD_TESTVECTOR(AD<double>) adynamic(n);
	for(size_t j = 0; j < n; ++j)
		adynamic[j] = 2.0;

	// domain space vector
	CPPAD_TESTVECTOR(AD<double>) ax(n);
	for(size_t j = 0; j < n; ++j)
		ax[j] = AD<double>(j + 1);

	// declare independent variables, dynamic parammeters, starting recording
	size_t abort_op_index = 0;
	bool   record_compare = true;
	CppAD::Independent(ax, abort_op_index, record_compare, adynamic);

	// range space vector
	CPPAD_TESTVECTOR(AD<double>) ay(n);
	int k = 0;
	ay[k] = double(-k-1)*(adynamic[k] + ax[k]) * (ax[k] + adynamic[k]);
	++k;
	ay[k] = double(-k-1)*(adynamic[k] - ax[k]) * (ax[k] - adynamic[k]);
	++k;
	ay[k] = double(-k-1)*(adynamic[k] * ax[k]) * (ax[k] * adynamic[k]);

	// create f: x -> y and stop tape recording
	CppAD::ADFun<double> f(ax, ay);

	// zero order forward mode
	CPPAD_TESTVECTOR(double) x(n), y(n);
	for(size_t j = 0; j < n; ++j)
		x[j] = double(j + 1);
	y    = f.Forward(0, x);
	double check;
	k = 0;
	check  = ( Value(adynamic[k]) + x[k] ) * ( x[k] + Value(adynamic[k]) );
	check *= double(-k-1);
	ok    &= NearEqual(y[k] , check, eps, eps);
	++k;
	check  = ( Value(adynamic[k]) - x[k] ) * ( x[k] - Value(adynamic[k]) );
	check *= double(-k-1);
	ok    &= NearEqual(y[k] , check, eps, eps);
	++k;
	check  = ( Value(adynamic[k]) * x[k] ) * ( x[k] * Value(adynamic[k]) );
	check *= double(-k-1);
	ok    &= NearEqual(y[k] , check, eps, eps);

	// change the dynamic parameter values
	CPPAD_TESTVECTOR(double) dynamic(n);
	for(size_t j = 0; j < n; j++)
		dynamic[j] = double(2 * j + 1);
	f.new_dynamic(dynamic);
	//
	y = f.Forward(0, x);
	k     = 0;
	check = double(-k-1)*( dynamic[k] + x[k] ) * ( x[k] + dynamic[k] );
	ok   &= NearEqual(y[k] , check, eps, eps);
	++k;
	check = double(-k-1)*( dynamic[k] - x[k] ) * ( x[k] - dynamic[k] );
	ok   &= NearEqual(y[k] , check, eps, eps);
	++k;
	check = double(-k-1)*( dynamic[k] * x[k] ) * ( x[k] * dynamic[k] );
	ok   &= NearEqual(y[k] , check, eps, eps);
	//
	return ok;
}