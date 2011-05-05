#include "densityprofile.h"

DensityProfile::DensityProfile()
{
	lensPlane = NULL;
	lensParameters = NULL;
	observationParameters = NULL;

}

DensityProfile::DensityProfile(Plane<Double> **newPlane, const struct lens_parameters* newlensParameters, const struct general_parameters* newobservationParameters)
{
	lensPlane = newPlane;
	lensParameters = newlensParameters;
	observationParameters = newobservationParameters;
	
}

DensityProfile::~DensityProfile()
{
	lensPlane = NULL;
	lensParameters = NULL;
	observationParameters = NULL;
}

Plane<Double> * DensityProfile::setPlane(Plane<Double> * newPlane)
{
  Plane<Double> * oldPlane;
  if(lensPlane == NULL)
    throw DavidException("DensityProfile::setPlane: Uninitialized plane.");
  oldPlane = *lensPlane;
  *lensPlane = newPlane;
  return oldPlane;
}

Plane<Double>* DensityProfile::getPlane()
{
  if(lensPlane == NULL)
    return NULL;
  return *lensPlane;
}

bool DensityProfile::isMassAtPoint(const double x,const double y) const
{
	return massAtPoint(x,y) != 0;
}

double DensityProfile::massAtPoint(double x, double y) const
{

  if(observationParameters == 0 || lensParameters == 0)
    return 0.0;
  
  const double& N = observationParameters->ndim;
  if(lensPlane != NULL && *lensPlane != NULL)
    {
      return (*lensPlane)->getValue((int)x,(int)y).doubleValue();
    }

	const double& a = lensParameters->semimajor_length;
	const double& e = lensParameters->eccentricity;
	const double& pixelsize = observationParameters->arcsec_per_pixel;

	double b = a*(1-e);

	double pi = D_PI;//borrowed from Double.h

	double xCenter = lensParameters->xcenter + N/2;
	double yCenter = lensParameters->ycenter + N/2;
	double aDenom = a*a;
	double bDenom = b*b;

	double ellipse = (pow(x-xCenter,2)/aDenom) + (pow(y-yCenter,2)/bDenom);

	if(ellipse < 1)
	{
	  double volumeOfSample = pixelsize*pixelsize;
	  
	  return lensParameters->mass_density*volumeOfSample;
	}
	
	//no mass
	return 0;
}

void DensityProfile::drawPlane()
{
  Plane<Double> *newPlane;
  if(observationParameters == 0 || lensParameters == 0)
		return;
  const size_t& N = observationParameters->ndim;

  if(*lensPlane != NULL)
    delete *lensPlane;

  newPlane = new Plane<Double>(N,N,0.0);

  for(size_t i = 0 ;i<N;i++)
    {
      for(size_t j = 0;j<N;j++)
	{
	  Double value(this->massAtPoint(i,j));
	  newPlane->setValue(i,j,value);
	}
    }

  *lensPlane = newPlane;
}


bool DensityProfile::clearAllFields()
{
  if(lensPlane != NULL)
    delete *lensPlane;

  observationParameters = NULL;
  lensParameters = NULL;
  lensPlane = NULL;

  return true;

}




