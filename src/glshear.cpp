#include "glshear.h"

GLShear::GLShear()
{
  this->observationParameters = 0;
  this->lensParameters = 0;
  this->sourceParameters = 0;
  massDensity = 0;
  shearMap = 0;
  sourcePlane = 0;

}

GLShear::GLShear(double * observationParameters, double * lensParameters, double * sourceParameters, DensityProfile * density, Plane<Double> * _sourcePlane, int * glellipseBounds, double * offset)
{
  this->observationParameters = observationParameters;
  this->lensParameters = lensParameters;
  this->sourceParameters = sourceParameters;
  massDensity = density;//DO NOT DELETE!!! WILL BE DELETED EXTERNALLY!!!!
  shearMap = 0;
  if(glellipseBounds == 0)
    createShearMapPlane();
  else
    createShearMapPlane(glellipseBounds[0], glellipseBounds[1], glellipseBounds[2], glellipseBounds[3], offset);
  sourcePlane = _sourcePlane;
}

GLShear::GLShear(DensityProfile * density, double * newSourceParameters, Plane<Double> * _sourcePlane, int * glellipseBounds, double * offset)
{
  observationParameters = density->getObservationParameters();
  lensParameters = density->getLensParameters();
  sourceParameters = newSourceParameters;
  massDensity = density;
  shearMap = 0;
  if(glellipseBounds == 0)
    createShearMapPlane();
  else
    createShearMapPlane(glellipseBounds[0], glellipseBounds[1], glellipseBounds[2], glellipseBounds[3], offset);
  sourcePlane = _sourcePlane;
}

GLShear::GLShear(Plane<math::Complex> * newShearMap, double * observationParameters, double * sourceParameters, Plane<Double> * _sourcePlane)
{
  this->observationParameters = observationParameters;
  this->lensParameters = 0;
  this->sourceParameters = sourceParameters;
  massDensity = 0;
  shearMap = newShearMap;
  sourcePlane = _sourcePlane;
}


GLShear::~GLShear()
{
  //Don't delete params as they are created out side of this class(see constructor)
  observationParameters = 0;
  lensParameters = 0;
  sourceParameters = 0;
  massDensity = 0;
  sourcePlane = 0;
}

//x and y are in pixels
//Gives the *arcsencond* location that this grid point lenses to.
double * GLShear::newLocation(double x, double y)
{

  double * returnMe = new double[2];

  double N = observationParameters[0];//Number of pixels

  double X,Y;
  X = (x-N/2)*observationParameters[1];//Size of one pixel in arcseconds
  Y = (y-N/2)*observationParameters[1];//Size of one pixel in arcseconds
  

  int i,j;
  math::Complex shearing = shearMap->getValue(Double(x).toInt(),Double(y).toInt());

  returnMe[0]  = X + shearing.getRealPart();//plus sign because alphas contain 
  returnMe[1]  = Y + shearing.getImaginaryPart();//the negative sign to point toward mass

  return returnMe;
}

//x,y positions in pixels from corner (zero indexed).
//offset is the offset of the point x=0 and y=0
math::Complex GLShear::shear(double x,double y, double * offset)
{
  double * returnMe = new double[2];
  returnMe[0] = 0;//x shear
  returnMe[1] = 0;//y shear

  double c,Gn,N,pixelsize,sigma_crit;
  try
    {
      /**
       * These parameters are need from the file.
       */
      c = observationParameters[5];
      Gn = observationParameters[2];
      N = observationParameters[0];
      sigma_crit =  Cosmology::criticalDensity(lensParameters[2],sourceParameters[2]);
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
  
  double solarMasses,M,xi,coeff;
  for(int i = leftBound; i < rightBound;i++)
    {
      for(int j = lowerBound;j< upperBound;j++)
	{
	  if(isLensMassElement(i+(N/2),j+(N/2)))
	    {


	      solarMasses = massInElement(i+(N/2),j+(N/2));
	      M = observationParameters[3]*solarMasses;//mass in grams
	      xi = sqrt( (i*pixelsize-centiX)*(i*pixelsize-centiX) + (j*pixelsize-centiY)*(j*pixelsize-centiY) );//measured in centimeters

	      coeff = (xi != 0) ? M/(D_PI*sigma_crit*pow(xi,4)) : 0;///coefficient in "radians" not "arcseconds"
	      returnMe[0] += coeff*(pow(j*pixelsize-centiY,2)-pow(i*pixelsize-centiX,2));
	      returnMe[1] += -2*coeff*(j*pixelsize-centiY)*(i*pixelsize-centiX);
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
Double GLShear::mapsToSource(double x, double y)
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

double GLShear::massInElement(const double x, const double y) const
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

bool GLShear::isLensMassElement(const double x, const double y) const
{
  if(massDensity != 0)
    return massDensity->isMassAtPoint(x,y);
  else 
    return false;/**/
}

bool GLShear::createShearMapPlane(int _leftBound, int _rightBound, int _lowerBound, int _upperBound, double * offset)
{


  using namespace math;

  if(observationParameters == 0 || lensParameters == 0 || sourceParameters == 0 || massDensity  == 0)
    {
      return false;
    }

  int N = (int)observationParameters[0];

  if(shearMap != 0)
    {
      delete shearMap;
    }

  math::Complex zero(0,0);

  shearMap = new Plane<math::Complex>(N,N,zero);

  int percentFinished = 0;

  int leftBound, rightBound, lowerBound, upperBound;

  leftBound = (_leftBound < 0) ? 0 : _leftBound;
  rightBound = (_rightBound < 0) ? N : _rightBound;
  lowerBound = (_lowerBound < 0) ? 0 : _lowerBound;
  upperBound = (_upperBound < 0) ? N : _upperBound;
	

  VERBOSE_PRINT("Creating Shear Map: ");

  for(int i = leftBound;i< rightBound;i++)
    for(int j = lowerBound; j<upperBound;j++)
      {
	math::Complex value = this->shear(Double(i-(N/2)).toInt(),Double(j-(N/2)).toInt(), offset);
	shearMap->setValue(i,j,value);

	if(((i-leftBound)*100/(rightBound-leftBound)) >= (percentFinished+5))
	  {
	    percentFinished = (int) (i-leftBound)*100/(rightBound-leftBound);
	    VERBOSE_PRINT("Percent finished: ");
	    VERBOSE_PRINT(percentFinished);
	  }
      }

  return shearMap;

}


