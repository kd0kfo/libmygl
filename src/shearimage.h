#ifndef SHEARIMAGE_CPP
#define SHEARIMAGE_CPP

#include "libdnstd/Double.h"
#include "libdnstd/Complex.h"

#include "densityprofile.h"
#include "plane.h"

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

