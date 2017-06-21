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

// g++-mp-5 -O2 -Wall -std=c++11 -I./../../ -I/opt/local/include rchisq_test.cpp -o rchisq.test -framework Accelerate

#include "armadillo"
#include "stats.hpp"

int main()
{
    double dof = 3;
    double chisq_mean = dof;
    double chisq_var = 2*dof;
    double chisq_rand = stats::rchisq(dof);

    std::cout << "chisq rv: " << chisq_rand << std::endl;

    int n = 100000;
    arma::vec chisq_vec = stats::rchisq(n,dof);

    std::cout << "chisq rv mean: " << arma::mean(chisq_vec) << ". Should be close to: " << chisq_mean << std::endl;
    std::cout << "chisq rv variance: " << arma::var(chisq_vec) << ". Should be close to: " << chisq_var << std::endl;

    return 0;
}
