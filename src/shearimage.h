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
#ifndef SHEARIMAGE_CPP
#define SHEARIMAGE_CPP

#include "densityprofile.h"

/**
 * Creates a distorted image plane from given Shear and Convergence maps.
 *
 * Created by David Coss, 2008
 */
class ShearImage
{

 public:
  
  /**
   * Main Constructor
   *
   * @param newConvergence Plane<Double> for the given lens mass.
   * @param newShear Plane<Double> Shear Map for the given lens.
   */
  ShearImage(Plane<Double>& newConvergence, Plane<math::Complex>& newShear);

  /**
   * Copy Constructor
   */
  ShearImage(const ShearImage&);

  /**
   * Destructor
   * 
   */
  ~ShearImage();
  
  Plane<math::Complex> getShearMap() const;
  Plane<Double> getConvergence() const;
  
  /**
   * 1st Value of Double contains ellipticity and 2nd part contains semimajor axis length (e=1-b/a) and 3rd part contains the angle of the semi-major axis wrt the "horizontal" axis
   */
  Plane<Double> * createDistortedImage(const Plane<Double> * original) const  throw (DavidException);

  Double distortPixel(Double& original, int x, int y) const throw (DavidException);

 private:
  Plane<Double> * convergence;///<Convergence(mass density)
  Plane<math::Complex> * shearMap;///<Measure of 2D shear
  

};


#endif

