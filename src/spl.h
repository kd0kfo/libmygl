/**
 * 
 * This file is part of libmygl, a C++ library for gravitational lensing
 * calculations. 
 *
 * Any use of of this library must cite my PhD thesis,
 * Coss, D., "Weak Shear Study of Galaxy Clusters by Simulated Gravitational
 * Lensing", PhD Thesis, 2010
 * which may be found at http://thesis.davecoss.com/Coss_dissertation.pdf
 *
 * Copyright 2007, 2010 David Coss, PhD
 *
 * libmygl is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * libmygl is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with libmygl.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef SPL_CPP
#define SPL_CPP

#include "Cosmology.h"
#include "libdnstd/Double.h"
#include "libdnstd/Complex.h"
#include <math.h>
#include "libdnstd/DTree.h"
#include "libdnstd/DList.h"

/**
 * Class to handle calculations in smooth particle lensing.
 */
class SPL{

 public:
  /**
   * Smooths the convergence
   */
  static double smoothConvergence(const double& masses,const double& position,const double& angle,const double& Sigma_c,double window);


  /**
   * Calculates the Bending angle according to SPL
   */
  static double bendingAngle(const double& mass, const double& distance, const  double& Sigma_c, double window = 1);

  /**
   * Calculates Shear (Complex)
   */
  static math::Complex shear(const double& mass,const double& distance,const double& angle, const double& Sigma_c,double window = 1);


};


#endif

