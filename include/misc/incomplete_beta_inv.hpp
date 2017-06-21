/*################################################################################
  ##
  ##   Copyright (C) 2011-2017 Keith O'Hara
  ##
  ##   This file is part of the StatsLib C++ library.
  ##
  ##   StatsLib is free software: you can redistribute it and/or modify
  ##   it under the terms of the GNU General Public License as published by
  ##   the Free Software Foundation, either version 2 of the License, or
  ##   (at your option) any later version.
  ##
  ##   StatsLib is distributed in the hope that it will be useful,
  ##   but WITHOUT ANY WARRANTY; without even the implied warranty of
  ##   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  ##   GNU General Public License for more details.
  ##
  ################################################################################*/

/* 
 * inverse of the incomplete beta function
 *
 * Keith O'Hara
 * 06/15/2016
 *
 * This version:
 * 06/18/2017
 */

#ifndef _stats_incomplete_beta_inv_HPP
#define _stats_incomplete_beta_inv_HPP

inline
bool
incomplete_beta_inv(double alpha_par, double beta_par, double p, double& ret)
{
    //
    bool success = false;
    int iter_max = 1000;
    double err_tol = 1E-08;

    if (alpha_par == 1.0 && beta_par == 1.0) {
        ret = p;
        return true;
    }

    double value;
    double lbeta = std::lgamma(alpha_par) + std::lgamma(beta_par) - std::lgamma(alpha_par + beta_par); // log beta function value
    //
    if (alpha_par > 1.0 && beta_par > 1.0) {
        //
        // use 26.2.23 in Abramowitz and Stegun (1972 print)
        double p_term = (p > 0.5) ? std::log(1.0 - p) : std::log(p);
        double t_val = std::sqrt(-2.0*p_term);

        double c_0 = 2.515517, c_1 = 0.802853, c_2 = 0.010328;
        double d_0 = 1.0, d_1 = 1.432788, d_2 = 0.189269, d_3 = 0.001308;

        value = t_val - (c_0 + c_1*t_val + c_2*t_val*t_val)/(d_0 + d_1*t_val + d_2*t_val*t_val + d_3*t_val*t_val*t_val);

        if (p > 0.5) {
            value *= -1.0;
        }

        // use 26.5.22 in Abramowitz and Stegun (1972 print)
        double ab_term_1 = ( 1.0/(2*alpha_par - 1.0) + 1.0/(2*beta_par - 1.0) );
        double ab_term_2 = ( 1.0/(2*beta_par - 1.0) - 1.0/(2*alpha_par - 1.0) );

        double lambda = (value*value - 3.0)/6.0;
        double h_term = 2.0 / ab_term_1;
        double w_term = value * std::sqrt(h_term + lambda)/h_term - ab_term_2*(lambda + 5.0/6.0 -2.0/(3.0*h_term));
        
        value = alpha_par / (alpha_par + beta_par*std::exp(2.0*w_term));
    } else {
        double term_1 = std::pow(alpha_par/(alpha_par+beta_par),alpha_par) / alpha_par;
        double term_2 = std::pow(beta_par/(alpha_par+beta_par),beta_par) / beta_par;
        double s_val = term_1 + term_2;

        double check_val = term_1 / s_val;

        if (p <= check_val) {
            value = std::pow(p*s_val*alpha_par,1.0/alpha_par);
        } else {
            value = 1.0 - std::pow(p*s_val*beta_par,1.0/beta_par);
        }
    }

    if (value <= 0.0) {
        printf("incomplete_gamma_inv error: value <= 0 found.\n");
        ret = 0.0;
        return false;
    }
    //
    int iter = 0;
    double err_val, deriv_1, deriv_2, ratio_val_1, ratio_val_2, halley_direc=1.0, ib_val=1.0;

    while (std::abs(halley_direc) >= err_tol && iter < iter_max) {
        iter++;
        //
        incomplete_beta(alpha_par,beta_par,value,ib_val);
        err_val = ib_val - p; // err_val = f(x)
        
        deriv_1 = std::exp( (alpha_par-1.0)*std::log(value) + (beta_par-1.0)*std::log(1.0 - value) - lbeta ); // derivative of the incomplete gamma function w.r.t. x
        deriv_2 = deriv_1*((alpha_par - 1.0)/value - (beta_par - 1.0)/(1.0 - value));                         // second derivative of the incomplete gamma function w.r.t. x

        ratio_val_1 = err_val / deriv_1;
        ratio_val_2 = deriv_2 / deriv_1;

        halley_direc = ratio_val_1 / std::max( 0.8, std::min( 1.2, 1.0 - 0.5*ratio_val_1*ratio_val_2 ) );
        value -= halley_direc;
        //
        if (value <= 0.0) {
            value = 0.5*(value + halley_direc);
        }
    }
    //
    if (std::abs(halley_direc) < err_tol && iter < iter_max) {
        ret = value;
        success = true;
    }
    //
    return success;
}

#endif
