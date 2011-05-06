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
#include "shearimage.h"

ShearImage::ShearImage(Plane<Double>& newConvergence, Plane<math::Complex>& newPlane)
{

  if(newConvergence.numberOfRows() != newPlane.numberOfRows() || newConvergence.numberOfColumns() != newPlane.numberOfColumns())
    throw DavidException("The dimensions of the convergence plane must equal that of the shear map");


  convergence = new Plane<Double>(newConvergence);
  shearMap = new Plane<math::Complex>(newPlane);
}

ShearImage::ShearImage(const ShearImage& rhs)
{
  convergence = new Plane<Double>(rhs.getConvergence());
  shearMap = new Plane<math::Complex>(rhs.getShearMap());
}

ShearImage::~ShearImage()
{
  delete convergence;
  delete shearMap;
}

Plane<math::Complex> ShearImage::getShearMap() const
{
  return *shearMap;
}

Plane<Double> ShearImage::getConvergence() const
{
  return *convergence;
}

Double ShearImage::distortPixel(Double& original, const int x, const int y) const throw (DavidException)
{

  double kappa = convergence->getValue(x,y).doubleValue();
  double gamma = shearMap->getValue(x,y).modulus();
  
  double a = 1.0 - kappa - gamma;
  if(a < 0)
    a *= -1;

  a = original.getValue(1)/a;
  double b =1 - kappa + gamma;
  if(b<0)
    b *= -1;

  b = (1-original.getValue(0))*original.getValue(1)/b;

  double gamma_x = shearMap->getValue(x,y).getRealPart();
  double gamma_y = shearMap->getValue(x,y).getImaginaryPart();
  double angle = (gamma_x/gamma_y)+sqrt((gamma_x/gamma_y)*(gamma_x/gamma_y)+1);
  angle = atan(angle);

  Double returnMe(1-b/a,a,angle);

  return returnMe;
}

Plane<Double> * ShearImage::createDistortedImage(const Plane<Double> * original) const throw (DavidException)
{
  int rows = original->numberOfRows();
  int columns = original->numberOfColumns();
  Double cZero(0.0);
  Plane<Double> *  returnMe = new Plane<Double>(rows, columns, cZero);
  
  for(int i = 0;i<rows;i++)
    for(int j = 0;j<columns;j++)
      {
	Double curr = original->getValue(i,j);
	if(curr.getValue(0) != 0 || curr.getValue(1) != 0)
	  try
	    {
	      Double newValue = distortPixel(curr,i,j);
	      returnMe->setValue(i,j,newValue);
	    }
	  catch(DavidException de)
	    {
	      delete returnMe;
	      returnMe = 0;
	      throw de;
	    }
      }
  
  return returnMe;
}
