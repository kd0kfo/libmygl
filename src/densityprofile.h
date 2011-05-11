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
#ifndef DENSITYPROFILE_H
#define DENSITYPROFILE_H 1

#include <iostream>

#include "plane.h"
#include "structs.h"

class DensityProfile
{

 public:
  /**
   * Default Constructor.
   * This is the default constructor. By default all members(pointers) are set to null.
   */
  DensityProfile();
	
  /**
   * Main Constructor.
   * This is the main constructor to be used.
   * @param newPlane The lensing plane, where the elements of the plain are mass values.
   * @param lensParameters Parameters of the lens
   * @see ray_trace_ellipse for simulation setup
   * @param observationParameters General Parameters
   */
  DensityProfile(Plane<Double> **newPlane, const struct lens_parameters *lensParameters, const struct general_parameters *observationParameters);

  /**
   * Destructor.
   */
  ~DensityProfile();
	
  /**
   * Sets the lens plane values.
   * @param newPlane new Lens plane.
   * @return Plane<Double> Old plane value. NOTE: if you don't delete it, it won't be deleted(eg memory leak). Enjoy!
   */
  Plane<Double> * setPlane(Plane<Double> *newPlane);//Returns Old Plane, DELETE IT!!

  /**
   * Gets a pointer to the lens plane.
   * DOES NOT Call drawPlane() to be sure that the plane is up to date. You need to do that.
   * So it can return 0
   *
   * @see drawPlane()
   */
  Plane<Double> *getPlane();
	
  /**
   * Determines whether there is mass at a given point, (x,y).
   * @param x double horizontal value in pixels.
   * @param y double vertical value in pixels.
   * @see massAtPoint(const double x, const double y)
   * @return bool
   */
  bool isMassAtPoint(double x, double y) const;
	
  /**
   * Returns the mass at the point, (x,y), in solar masses.
   *
   * If a Plane<Double> was provided for the mass density,
   * x and y must represent the pixel coordinates of the 
   * position in the Plane array (zero indexed). If, on
   * the other hand, no Plane<Double> is provided, the 
   * mass at the point will becalculated based on the 
   * provided elliptical density information given, where
   * x and y are in pixel values from the center of the 
   * grid.
   *
   * @param x double horizontal value
   * @param y double vertical value.
   * @return double mass value
   */
  double massAtPoint(double x, double y) const;

  /**
   * Determines whether or not the Plane which contains the mass at each point is created.
   */
  bool densityProfilePlaneIsDefined() const{ return lensPlane != NULL && *lensPlane != NULL;}
	
  /**
   * Creates the Plane which contains the mass at each point.
   */
  void drawPlane();
	
  void clearAllFields();///< only deletes the lens plane, but sets all pointers equal to zero. BE CAREFUL!!!
  const struct general_parameters* getObservationParameters(){return observationParameters;}
  const struct lens_parameters* getLensParameters(){return lensParameters;}

 private:
  Plane<Double> **lensPlane;
  const struct general_parameters* observationParameters;
  const struct lens_parameters* lensParameters;


};

#endif

