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
#ifndef GLELLIPSE_H
#define GLELLIPSE_H 1

#include "densityprofile.h"


/** \mainpage Documentation of the Lensing Software.
 * Copyright David Coss 2010
 * Copying and distributing are permitted under the terms of the GNU Public License version 3(see COPYING or http://www.gnu.org/licenses/gpl.txt).
 */

/**
 * General Lensing Algorithm.
 * This class handles lensing information and calculations.
 * Copyright David Coss 2010
 * Copying and distributing are permitted under the terms of the GNU Public License version 3(see COPYING or http://www.gnu.org/licenses/gpl.txt).
 */
class GLAlgorithm
{

 public:

  /**
   * Default Constructor.
   * All pointers set to null.
   */
  GLAlgorithm();

  /**
   * Parameter based constructor.
   * This constructor should be used if the deflection angles have not yet been calculated.
   * @param observationParameters Set of General paramters (eg speed of light, solar mass).
   * @param lensParameters Lens Parameters (eg size, location).
   * @param sourceParameters Source parameters.
   * @param density DensityProfile (usually to be calculated by this constructor).
   * @param _sourcePlane Plane containing the sources.
   * @param glellipseBounds int array that includes the pixel bounds if only a subset is calculated.
   * @param offset 2-D array that provides for an offset of the center of the grid. Values should be pixel units.
   */
  GLAlgorithm(const struct general_parameters *observationParameters, const struct lens_parameters *lensParameters,const struct source_parameters *sourceParameters, DensityProfile * density, Plane<Double> * _sourcePlane, int * glellipseBounds = 0, double * offset = 0);

  /**
   * Lens Density and Source based constructor.
   * This is used if the density profile has already been calculated.
   * @param density DensityProfile of the lens.
   * @param sourceParameters Parameters of the source.
   * @param _sourcePlane The sources.
   * @param glellipseBounds int array that includes the pixel bounds if only a subset is calculated.
   * @param offset 2-D array that provides for an offset of the center of the grid. Values should be pixel units.
   */
  GLAlgorithm(DensityProfile * density, const struct source_parameters * sourceParameters, Plane<Double> * _sourcePlane, int * glellipseBounds = 0, double * offset = 0);

  /**
   * Deflection angle array based constructor.
   * This constructor should be used the most. It is suggested (for speed of simulation) that he deflection angles
   * be calculated, saved, and then reused as need. This construct provides deflection planes, via newDeflectionMap, 
   * to be used once calculated. Note: the 2 dimensional deflections are stored as Complex numbers with
   * the horizontal and vertical deflections as the Real and Imaginary Parts, respectively.
   * @param newDeflectionMap math::Complex Deflections Plane
   * @param observationParameters General observation Parameters.
   * @param sourceParameters Source Parameters.
   * @param _sourcePlane Source Plane.
   */
  GLAlgorithm(Plane<math::Complex> * newDeflectionMap,const struct general_parameters *observationParameters,const struct source_parameters * sourceParameters, Plane<Double> * _sourcePlane);

  /**
   * Destructor.
   * This destructor only sets the pointers to null, since they may be used in other
   * parts of the program. So if they should be deleted, delete the outside of glalgorithm.
   */
  ~GLAlgorithm();

  /**
   * Gives the point in the source frame the point (x,y) would be lensed to.
   * @param x horizontal position in pixels from corner (zero indexed).
   * @param y vertical position in pixels from corner (zero indexed).
   */
  double * newLocation(double x, double y);

  /**
   * Gives the amount of deflection.
   * The Deflection is given as Complex numbers with
   * the horizontal and vertical deflections as the Real and Imaginary Parts, respectively.
   * 
   * offset is a 2-D vector containing the x and y offsets respectively. If offset = 0, then there is no offset. So offset can be null. Offset is a pixel offset (ie must match x and y's units).
   *
   * @param x horizontal position in pixels from corner (zero indexed).
   * @param y vertical position in pixels from corner (zero indexed).
   * @param (x,y) offset of the center in pixels.
   * @return math::Complex
   */
  math::Complex deflection(double x,double y, double * offset);
	
  /**
   * Checks to see whether the point (x,y) maps to a source, if so, returns it's mass.
   * x and y in arcseconds.
   * @param x horizontal location
   * @param y vertical location.
   * @return Double mass at source location.
   */
  Double mapsToSource(double x, double y);

  static double abs(const double x){return sqrt(x*x);}///< Absolute value of x
	
  /**
   * Reads the source plane and give the mass in that space.
   * @param x horizontal position.
   * @param y vertical position.
   * @return double mass at that position.
   */
  double massInElement(const double x, const double y) const;

  /**
   * Determines whether there is lens mass at the point (x,y).
   * @param x horizontal position.
   * @param y vertical position.
   * @return bool.
   */
  bool isLensMassElement(const double x, const double y) const;

  /**
   * Creates a plane creating the deflection angles.
   * If the plane was created correctly, true is returned.
   * X and Y bounds were added to allow for parallel computing. The deflection map can be calculated a portion
   * at a time. Then those portions may be added together to form a final grid.
   *
   * @return bool.
   * @see getDeflectionPlane()
   */
  bool createDeflectionMapPlane(int _leftBound = -1, int _rightBound = -1, int _lowerBound = -1, int _upperBound = -1, double * offset = 0);

  /**
   * Returns a pointer to the deflection plane.
   * @return Plane<math::Complex>*
   * @see createDeflectionMapPlane()
   */
  Plane<math::Complex> * getDeflectionPlane(){return deflectionMap;}

 private:
  const struct general_parameters *observationParameters;
  const struct lens_parameters *lensParameters;
  const struct source_parameters *sourceParameters;
  DensityProfile *massDensity;
  Plane<math::Complex> *deflectionMap;
  Plane<Double> *sourcePlane;
};

#endif //GLELLIPSE_H


