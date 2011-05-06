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
#ifndef COSMOLOGY_CPP
#define COSMOLOGY_CPP

class Cosmology
{
public:

  /**
   * Gives the Distance(in Mpc) out to a redshift, z2, starting at redshift, z1. To measure from the current epoch, z1 = 0. 
   * omega is the density parameter of matter. 
   * h = hubble's constant.
   * Default values for omega and h are based on WMAP 5-year data
   */
  static double redshiftToDistance(double z1,double z2, double omega = 0.357, double h = 0.71);

  /**
   * R1 and R2 are functions used in the SEF 1999 distance calculation
   */
  static double R1(double z,double omega);

  /**
   * R1 and R2 are functions used in the SEF 1999 distance calculation
   */
  static double R2(double z,double omega);

  /**
   * 3 element array of coefficients for calculation of distances
   * cf Demianski et al 2003 A&A
   */
  static double* distanceCoefficients(double z, double omega_m = 0.3, double omega_lambda = 0.7, double omega_k = 0, double alpha = 0.8);

  /**
   * Calculates the chi function found in  Demianski et al 2003 A&A
   * Equation for Angular diameter distance between 2 redshifts
   * Gamma must be curve fitted based on CDM parameters and
   * matter homogeneity. alpha = 0.3 for this gamma = 6
   */
  static double demianskiChi(double z, double gamma = 6);

  /**
   * Used to convert arcseconds to h^-1 centimeters for a given redshift.
   *
   * @param arcseconds double angle in arcseconds
   * @param z  double redshift value
   * @param z0 double optional initial redshift. Default value is 0 for current epoch.
   * @param omega double value of the density parameter of matter. Default value taken from WMAP 5-year data.
   * @param h double Hubble constant. Default value taken from WMAP 5-year data.
   */
  static double arcsecondsToCentimeters(double arcseconds, double z, double z0 = 0, double omega = 0.357,double h = 0.71);

  /**
   * Calculates the Critical Density(2-D) for given redshifts.
   * In Grams per centimeter^2
   */
  static double criticalDensity(const double& z1, const double&z2);


  //Constants
  static const double speedOfLight;///<centimeters per second
  static const double newtonsConstant;///<cm^3 /g /s^2
  static const double solarMass;///<Solar mass in grams
  


};


  

#endif

