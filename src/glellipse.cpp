#ifndef GLALGORITHM_CPP
#define GLALGORITHM_CPP

#ifndef _WIN32
#import <iostream>
#endif


#include "glellipse.h"

GLAlgorithm::GLAlgorithm()
{
  this->observationParameters = 0;
  this->lensParameters = 0;
  this->sourceParameters = 0;
  massDensity = 0;
  deflectionMap = 0;
  sourcePlane = 0;

}

GLAlgorithm::GLAlgorithm(double * observationParameters, double * lensParameters, double * sourceParameters, DensityProfile * density, Plane<Double> * _sourcePlane, int * glellipseBounds, double * offset)
{
  this->observationParameters = observationParameters;
  this->lensParameters = lensParameters;
  this->sourceParameters = sourceParameters;
  massDensity = density;//DO NOT DELETE!!! WILL BE DELETED EXTERNALLY!!!!
  deflectionMap = 0;
  if(glellipseBounds == 0)
    createDeflectionMapPlane();
  else
    createDeflectionMapPlane(glellipseBounds[0], glellipseBounds[1], glellipseBounds[2], glellipseBounds[3], offset);
  sourcePlane = _sourcePlane;
}

GLAlgorithm::GLAlgorithm(DensityProfile * density, double * newSourceParameters, Plane<Double> * _sourcePlane, int * glellipseBounds, double * offset)
{
  observationParameters = density->getObservationParameters();
  lensParameters = density->getLensParameters();
  sourceParameters = newSourceParameters;
  massDensity = density;
  deflectionMap = 0;
  if(glellipseBounds == 0)
    createDeflectionMapPlane();
  else
    createDeflectionMapPlane(glellipseBounds[0], glellipseBounds[1], glellipseBounds[2], glellipseBounds[3], offset);
  sourcePlane = _sourcePlane;
}

GLAlgorithm::GLAlgorithm(Plane<math::Complex> * newDeflectionMap, double * observationParameters, double * sourceParameters, Plane<Double> * _sourcePlane)
{
  this->observationParameters = observationParameters;
  this->lensParameters = 0;
  this->sourceParameters = sourceParameters;
  massDensity = 0;
  deflectionMap = newDeflectionMap;
  sourcePlane = _sourcePlane;
}


GLAlgorithm::~GLAlgorithm()
{
  //Don't delete params as they are created out side of this class(see constructor)
  observationParameters = 0;
  lensParameters = 0;
  sourceParameters = 0;
  massDensity = 0;
  //delete deflectionMap;
  //deflectionMap = 0;
  sourcePlane = 0;
}

//x and y are in pixels
//Gives the *arcsencond* location that this grid point lenses to.
double * GLAlgorithm::newLocation(double x, double y)
{

  double * returnMe = new double[2];

  double N = observationParameters[0];//Number of pixels

  double X,Y;
  X = (x-N/2)*observationParameters[1];//Size of one pixel in arcseconds
  Y = (y-N/2)*observationParameters[1];//Size of one pixel in arcseconds
  

  int i,j;
  math::Complex bending = deflectionMap->getValue(Double(x).toInt(),Double(y).toInt());

  returnMe[0]  = X + bending.getRealPart();//plus sign because alphas contain 
  returnMe[1]  = Y + bending.getImaginaryPart();//the negative sign to point toward mass

  return returnMe;
}

//x,y positions in pixels from corner (zero indexed).
//offset is the offset of the point x=0 and y=0
math::Complex GLAlgorithm::deflection(double x,double y, double * offset)
{
  double * returnMe = new double[2];
  returnMe[0] = 0;//x deflection in arcseconds;
  returnMe[1] = 0;//y deflection in arcseconds;

  double c,Gn,N,pixelsize;
  try
    {
      //try is used to in case parameters are accidentally not loaded.
      c = observationParameters[5];
      Gn = observationParameters[2];
      N = observationParameters[0];
      pixelsize = Cosmology::arcsecondsToCentimeters(observationParameters[1],lensParameters[2]);//Pixelsize in centimeters
    }
  catch(...)
    {
      throw DavidException("Speed of light not supplied in parameters");
    }
	
  double centiX = x*pixelsize;
  double centiY = y*pixelsize;

  if(offset != 0)
    {
      centiX += offset[0]*pixelsize;
      centiY += offset[1]*pixelsize;
    }
	
  int leftBound = (int)-N/2;
  int rightBound = (int)N/2;
  int lowerBound = (int)-N/2;
  int upperBound = (int)N/2;
  /**/

  for(int i = leftBound; i < rightBound;i++)
    {
      for(int j = lowerBound;j< upperBound;j++)
	{
	  if(isLensMassElement(i+(N/2),j+(N/2)))
	    {
	      double solarMasses = massInElement(i+(N/2),j+(N/2));
	      double M = observationParameters[3]*solarMasses;
	      double xi = sqrt( (i*pixelsize-centiX)*(i*pixelsize-centiX) + (j*pixelsize-centiY)*(j*pixelsize-centiY) );//measured in centimeters

	      double deflection = (xi != 0) ? (206265)*4*Gn*(M)/(c*c*xi) : 0;///deflection is measured in arcseconds
	      double angle = (i-x == 0) ? (D_PI/2.0) : atan(abs(j- y)/abs(i-x));//
	      if(angle == D_PI/2.0 && (j-y != 0))
		{
		  //DEBUG_PRINT("90 deg");
		  returnMe[1] += deflection*((j-y)/abs(j-y));
		}
	      else if((i-x) != 0 && (j-y) != 0)
		{
		  //DEBUG_PRINT("Not 90 deg");
		  returnMe[0] += deflection*((i-x)/abs(i-x))*cos(angle);
		  returnMe[1] += deflection*((j-y)/abs(j-y))*sin(angle);
		}
	      else if((i-x) != 0 && (j-y) == 0)
		{
		  returnMe[0] += deflection*((i-x)/abs(i-x));
		}
	    }
	}
    }
  

  math::Complex returnC(returnMe[0],returnMe[1]);
  delete [] returnMe;
  returnMe = 0;

  return returnC;
	
}

//x and y in pixels
//If the location does map to a source, the light intensity is returned
//If not, zero is returned.
Double GLAlgorithm::mapsToSource(double x, double y)
{
  double pixelsize = observationParameters[1];
  double * newCoords = newLocation(x,y);//in arcseconds
  Double returnMe;

  if(sourcePlane == 0)
    {
      double a = sourceParameters[3]*pixelsize;
      double e = sourceParameters[4];
      double b = a*(1-e);
      double xCenter = sourceParameters[0]*pixelsize;
      double yCenter = sourceParameters[1]*pixelsize;
	    
      double ellipse = pow(newCoords[0]-xCenter,2)/pow(a,2)+pow(newCoords[1]-yCenter,2)/pow(b,2);
      bool maps = (ellipse <= 1);

      if(maps)
	returnMe.setValue(0,255);
	      
    }
  else
    {
      int N = (int)observationParameters[0];
      int i,j;
      i = Double(newCoords[0]/pixelsize + (N/2)).toInt();
      j = Double(newCoords[1]/pixelsize + (N/2)).toInt();
	    
      if(abs(i) > N || abs(j) > N || i < 0 || j < 0)
	{
	  std::cout << i << " " << j << std::endl;
	  throw DavidException("Out of Range",DavidException::PLANE_OUT_OF_BOUNDS_ERROR_CODE);
	}
      else
	{
	  returnMe = sourcePlane->getValue(i,j);
	}
    }


  delete [] newCoords;
  newCoords = 0;

  return returnMe;
}

double GLAlgorithm::massInElement(const double x, const double y) const
{
  if(massDensity != 0)
    {
      return massDensity->massAtPoint(x, y);
    }
  else
    {
      return 0;/**/
    }
}

bool GLAlgorithm::isLensMassElement(const double x, const double y) const
{
  if(massDensity != 0)
    return massDensity->isMassAtPoint(x,y);
  else 
    return false;/**/
}

bool GLAlgorithm::createDeflectionMapPlane(int _leftBound, int _rightBound, int _lowerBound, int _upperBound, double * offset)
{


  using namespace math;

  if(observationParameters == 0 || lensParameters == 0 || sourceParameters == 0 || massDensity  == 0)
    {
      return false;
    }

  int N = (int)observationParameters[0];

  if(deflectionMap != 0)
    {
      delete deflectionMap;
    }

  math::Complex zero(0,0);

  deflectionMap = new Plane<math::Complex>(N,N,zero);

  int percentFinished = 0;

  int leftBound, rightBound, lowerBound, upperBound;

  leftBound = (_leftBound < 0) ? 0 : _leftBound;
  rightBound = (_rightBound < 0) ? N : _rightBound;
  lowerBound = (_lowerBound < 0) ? 0 : _lowerBound;
  upperBound = (_upperBound < 0) ? N : _upperBound;
	

  VERBOSE_PRINT("Creating Deflection Map: ");

  for(int i = leftBound;i< rightBound;i++)
    for(int j = lowerBound; j<upperBound;j++)
      {
	math::Complex value = this->deflection(Double(i-(N/2)).toInt(),Double(j-(N/2)).toInt(), offset);
	deflectionMap->setValue(i,j,value);

	if(((i-leftBound)*100/(rightBound-leftBound)) >= (percentFinished+5))
	  {
	    percentFinished = (int) (i-leftBound)*100/(rightBound-leftBound);
	    VERBOSE_PRINT("Percent finished: ");
	    VERBOSE_PRINT(percentFinished);
	  }
      }

  return deflectionMap;

}


#endif
