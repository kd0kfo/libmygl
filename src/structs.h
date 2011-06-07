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
#ifndef LIBMYGL_STRUCTS_H
#define LIBMYGL_STRUCTS_H 1

struct general_parameters{
  size_t ndim;// number of pixels in each dimension
  double arcsec_per_pixel;

  //Physical constants
  double G,solar_mass,distance_scale,c;

  bool showCriticalCurves;
  
};

struct lens_parameters{
  int xcenter,ycenter;// in pixels
  double redshift,velocity_dispersion,mass_density;
  double shear,semimajor_length/*arcsec*/,eccentricity;
};

struct source_parameters{
  int xcenter,ycenter;// in pixels
  double redshift;
  double semimajor_length/*arcsec*/,eccentricity;
  double orientation;// angle of semimajor-axis w.r.t. x-axis
  double flux;
};



#endif //LIBMYGL_STRUCTS_H
