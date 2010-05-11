#ifndef DENSITYPROFILE_CPP
#include "densityprofile.h"
#endif

DensityProfile::DensityProfile()
{
	lensPlane = 0;
	lensParameters = observationParameters = 0;

}

DensityProfile::DensityProfile(Plane<Double> * newPlane, double * newlensParameters, double * newobservationParameters)
{
	lensPlane = newPlane;
	lensParameters = newlensParameters;
	observationParameters = newobservationParameters;
	
}

DensityProfile::~DensityProfile()
{
	lensPlane = 0;
	lensParameters = observationParameters = 0;
}

Plane<Double> * DensityProfile::setPlane(Plane<Double> * newPlane)
{
	Plane<Double> * oldPlane = lensPlane;
	lensPlane = newPlane;
	return oldPlane;
}

Plane<Double> * DensityProfile::getPlane()
{
	return lensPlane;
}

bool DensityProfile::isMassAtPoint(const double x,const double y) const
{
	return massAtPoint(x,y) != 0;
}

double DensityProfile::massAtPoint(const double x, const double y) const
{
  if(lensPlane != 0)
    {
      //std::cout << "Actually used lensplane" << std::endl;
      return lensPlane->getValue(x,y).doubleValue();
    }

	if(observationParameters == 0 || lensParameters == 0)
	{
		VERBOSE_PRINT("ZERO CALL");
		return 0;
	}

	double N = observationParameters[0];
	double a = lensParameters[8];
	double e = lensParameters[9];
	double b = a*(1-e);

	double pi = 3.14159265;

	double pixelsize = observationParameters[1];

	double xCenter = lensParameters[0] + N/2;
	double yCenter = lensParameters[1] + N/2;
	double aDenom = a*a;
	double bDenom = b*b;

	double ellipse = (pow(x-xCenter,2)/aDenom) + (pow(y-yCenter,2)/bDenom);

	if(ellipse < 1)
	{
	  double volumeOfSample = pixelsize*pixelsize;
	  
		return lensParameters[6]*volumeOfSample;
	}
	
	//no mass
	return 0;
}

void DensityProfile::drawPlane()
{

  if(observationParameters == 0 || lensParameters == 0)
		return;
  int N = (int) observationParameters[0];

  Double cSucks(0.0);
  Plane<Double> * newPlane = new Plane<Double>(N,N,cSucks);
	if(lensPlane != 0)
	  {
	    delete lensPlane;
	    lensPlane = 0;
	  }

	for(int i = 0 ;i<N;i++)
	{
		for(int j = 0;j<N;j++)
		{
		  int x = i - (N/2);
		  int y = j - (N/2);
		  Double value(this->massAtPoint(x,y));
		  newPlane->setValue(i,j,value);
		}
	}
	
  lensPlane = newPlane;
}


bool DensityProfile::clearAllFields()
{
	DEBUG_PRINT("IN CLEAR");
	if(observationParameters != 0)
	  delete [] observationParameters;
	DEBUG_PRINT("This delete is bad");
	if(lensParameters != 0)
		delete [] lensParameters;
	DEBUG_PRINT("deleting lensplane");
	if(lensPlane != 0)
		delete lensPlane;

	observationParameters = lensParameters = 0;
	lensPlane = 0;

	return true;

}




