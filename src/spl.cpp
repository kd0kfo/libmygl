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
#include "spl.h"

double SPL::smoothConvergence(const double& mass,const double& distance,const double& angle,const double& Sigma_c,double window)
{
  return (mass/(2*D_PI*window*window*Sigma_c))*Double::e(-distance*distance/(2*window*window));
}

double SPL::bendingAngle(const double& mass, const double& distance, const double& Sigma_c, double window)
{

  double returnMe = mass/(D_PI*distance*Sigma_c);
  return returnMe*(Double::e(-1*pow(distance/window,2)/2)-1);

}

math::Complex SPL::shear(const double& mass, const double& distance,const double& angle, const double& Sigma_c,double window)
{
  double * shear = new double[2];
  double coeff;
  
  coeff = mass/(2*D_PI*window*window*Sigma_c);
  coeff *= (2*window*window+distance*distance)*exp(-0.5*pow(distance/window,2)) - 2*window*window;
  coeff *= 1/(distance*distance);

  shear[0] = coeff*cos(2*angle);
  shear[1] = coeff*sin(2*angle);

   math::Complex returnMe(shear[0],shear[1]);
  delete[] shear;
  return returnMe;
		     
}


