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
#include "Cosmology.h"
#include <math.h>
#include "libdnstd/DavidException.h"
#include "libdnstd/Double.h"

const double Cosmology::speedOfLight = 3e+10;//centimeters per second
const double Cosmology::newtonsConstant = .0000000667;//cm^3 /g /s^2
const double Cosmology::solarMass = 2e+33;//Solar mass in grams

double Cosmology::R1(double z,double omega)
{
  return (omega*z-omega+2)/pow(1+z,2.0);
}

double Cosmology::R2(double z,double omega)
{
  return sqrt(omega*z+1.0)/pow(z+1,2.0);
}

double Cosmology::redshiftToDistance(double z1, double z2, double omega, double h)
{
  double returnMe = 0;
  double hubbleLength = 3e3 /h;
  if(z1 == 0)
    {
      double * cs = distanceCoefficients(z2);//,omega,1-omega);//3 coefficients (cf .h file)
      returnMe = cs[0]*z2*z2+pow(1+cs[1]*z2+cs[2]*z2*z2,2);
      returnMe = z2/sqrt(returnMe);
      delete [] cs;cs = 0;
      return returnMe * hubbleLength;
    }
  else
    {
      double d_s, d_d,chi_d,chi_s;
      d_s = redshiftToDistance(0,z2,omega,h);
      d_d = redshiftToDistance(0,z1,omega,h);
      chi_d = demianskiChi(z1);
      chi_s = demianskiChi(z2);
      return (1+z1)*d_d*d_s*(chi_d-chi_s)/hubbleLength;
    }

  
}


double Cosmology::arcsecondsToCentimeters(double arcseconds, double z, double z0, double omega,double h)
{
  double returnMe = redshiftToDistance(z0,z,omega,h);//distance in Mpc
  returnMe *= 3.08568025e24;//convert to centimeters
  returnMe *= 4.84813681e-6*arcseconds;
  
  return returnMe;
}


double Cosmology::criticalDensity(const double& z1, const double&z2)
{

  double dl,dls,ds;
  dl = redshiftToDistance(0,z1);
  ds = redshiftToDistance(0,z2);
  dls = redshiftToDistance(z1,z2);

  return  (speedOfLight*speedOfLight/(4*D_PI*newtonsConstant))*(ds/(3.08568025e24*dl*dls));

}


double* Cosmology::distanceCoefficients(double z, double omega_m, double omega_lambda, double omega_k, double alpha)
{
  double * returnMe = new double[3];

  returnMe[0] = -1*pow(2*omega_k+3*omega_m,2)/4+(7*omega_k+(13+alpha)*omega_m)/2-(14*omega_k+(28+5*alpha)*omega_m)/(6*omega_k+7*omega_m+4*omega_lambda);

  returnMe[1] = 1+(2*omega_k+3*omega_m)/4;

  returnMe[2] = pow(2*omega_k+3*omega_m,2)-4*(5*omega_k+9*omega_m)+8*(14*omega_k+(28+5*alpha)*omega_m)/(6*omega_k+7*omega_m+4*omega_lambda);
  returnMe[2] = returnMe[2]/16;

  return returnMe;
}

double Cosmology::demianskiChi(double z, double gamma)
{
  return 1/(z+z*z/gamma);
}
