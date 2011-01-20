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


