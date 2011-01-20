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

