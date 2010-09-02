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

GLShear::GLShear(double * observationParameters, double * lensParameters, double * sourceParameters, DList<Double> * density, Plane<Double> * _sourcePlane, int * glellipseBounds, double * offset)
{
  this->observationParameters = observationParameters;
  this->lensParameters = lensParameters;
  this->sourceParameters = sourceParameters;
  massDensity = density;//DO NOT DELETE!!! WILL BE DELETED EXTERNALLY!!!!
  shearMap = 0;
  if(glellipseBounds == 0)
    createShearMapPlane();
  else
    createShearMapPlane(offset);
  sourcePlane = _sourcePlane;
}

GLShear::GLShear(DList<Double> * density, double * _observationParameters, double * _lensParameters, double * newSourceParameters, Plane<Double> * _sourcePlane, int * glellipseBounds, double * offset, bool createShearMap, double windowValue, DList<Double> * optionalSource)
{
  observationParameters = _observationParameters;
  lensParameters = _lensParameters;
  sourceParameters = newSourceParameters;
  massDensity = density;
  this->windowValue = windowValue;
  shearMap = optionalSource;
  if(createShearMap && glellipseBounds == 0)
    createShearMapPlane();
  else if(createShearMap && glellipseBounds != 0)
    createShearMapPlane(offset);

  sourcePlane = _sourcePlane;
}

GLShear::GLShear(DList<Double> * newShearMap, double * observationParameters, double * sourceParameters, Plane<Double> * _sourcePlane)
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

//x,y positions in arcseconds
//arg is the direction of shear, either x component or y component
math::Complex GLShear::shear(double x,double y,double * offset)
{
  math::Complex returnMe(0.0,0.0);
	
  double N = observationParameters[0];
  double pixelsize = Cosmology::arcsecondsToCentimeters(observationParameters[1],lensParameters[2]);//arcseconds to centimeters
  double centiX = x*pixelsize;
  double centiY = y*pixelsize;

  if(offset != 0)
    {
      if(sizeof(offset)/sizeof(double) != 2)
	throw DavidException("I need both an X and Y offset", DavidException::INVALID_ARGUMENT_ERROR_CODE);

      centiX += offset[0]*pixelsize;
      centiY += pixelsize*offset[1];
    }

  double c;
  try
    {
      /**
       * speed of light should be provied in parameters file.
       * This way units are "guaranteed" to match.
       */
      c = observationParameters[5];
    }
  catch(...)
    {
      throw DavidException("Speed of light not supplied in parameters");
    }
  double Gn = observationParameters[2];
  double Sigma_critical = Cosmology::criticalDensity(lensParameters[2],sourceParameters[2]);//grams per centimeter^2
	
	
  DNode<Double> * keepThePlace = massDensity->getCurr();//Just in case(?)

  while(massDensity->getCurr() != 0)
    {
      Double curr = massDensity->getCurr()->getItemCopy();
      double i = curr.getValue(1);
      double j = curr.getValue(2);
      double M = observationParameters[3]*curr.getValue(0);//mass in grams
      double xi = (i*pixelsize-centiX)*(i*pixelsize-centiX) + (j*pixelsize-centiY)*(j*pixelsize-centiY);//measured in centimeters
      xi = sqrt(xi);

      double normalize = Sigma_critical;//grams per square centimeter
	      
      double angle = atan2((j- y),(i-x));

      if(xi != 0)
	{
	  /**
	   * Use SPL
	   */
	  returnMe +=  SPL::shear(M,xi,angle,normalize,windowValue*pixelsize);

	  //Try out bending angle then delete this part
	  //returnMe += math::Complex(SPL::bendingAngle(solarMasses,xi,normalize,windowValue*pixelsize),0.0);

	  /**/
	  /**
	   * Don't use SPL
	   *
	   double coeff = 4*Gn*M/pow(xi,4);
	   returnMe += math::Complex(coeff*((i-x)*(i-x)-(j-y)*(j-y)),coeff*(2*(i-x)*(j-y)));
	   /**/
	}
      massDensity->forward();
    }

  massDensity->setCurr(keepThePlace);
  
  return returnMe;
	
}

bool GLShear::createShearMapPlane(double * offset)
{


  using namespace math;

  if(observationParameters == 0 || lensParameters == 0 || sourceParameters == 0 || massDensity  == 0)
    {
      return false;
    }

  int N = observationParameters[0];

  if(shearMap == 0)
    throw DavidException("I was not given a list of points to calculate shear.", DavidException::INVALID_ARGUMENT_ERROR_CODE);

  int percentFinished = 0;
  int currentIndex = 0;
  int totalPoints = shearMap->size();

  VERBOSE_PRINT("Creating Shear Map: ");

  DNode<Double> * saveSpot = shearMap->getCurr();
  shearMap->reset();
  double x,y,a,b;
  DNode<Double> * curr = shearMap->getCurr();
  Double newItem;

  if(offset != 0)
    {
      a = offset[0];
      b = offset[1];
    }
  else
    a = b = 0;

  double saveWindow = this->windowValue;
  while(shearMap->getCurr() != 0)
    {
      this->windowValue = curr->getItem()->getValue(0);
      x = curr->getItem()->getValue(1) + a;
      y = curr->getItem()->getValue(2) + b;
      math::Complex value = this->shear(x,y,offset);
      newItem = Double(x,y,value.modulus());
      curr->setItem(newItem);
      currentIndex++;
      if(100*currentIndex/totalPoints >= (percentFinished+5))
	{
	  percentFinished = (int) (currentIndex)*100/(totalPoints);
	  VERBOSE_PRINT("Percent finished: ");
	  VERBOSE_PRINT(percentFinished);
	}
      shearMap->forward();
      curr = shearMap->getCurr();
    }

  shearMap->setCurr(saveSpot);
  this->windowValue = saveWindow;
  return shearMap;

}

void GLShear::createConvergencePlane(DList<Double> * convergence, double * offset)
{

  using namespace math;

  if(convergence == 0)
    {
      DEBUG_PRINT("Null convergence pointer");
      convergence = new DList<Double>;
    }

  if(observationParameters == 0 || lensParameters == 0 || sourceParameters == 0 || massDensity  == 0)
    throw DavidException("I need the parameters to compute the convergence.",DavidException::INVALID_ARGUMENT_ERROR_CODE);
    

  int N = observationParameters[0];

  int percentFinished = 0;
  int currentIndex = 0;
  int totalPoints = convergence->size();


  VERBOSE_PRINT("Creating Convergence: ");

  DNode<Double> * saveSpot = convergence->getCurr();
  convergence->reset();
  double x,y,a,b;
  DNode<Double> * curr = convergence->getCurr();
  Double newItem;

  if(offset != 0)
    {
      a = offset[0];
      b = offset[1];
    }
  else
    a = b = 0;

  double saveWindow = this->windowValue;
  while(convergence->getCurr() != 0)
    {
      this->windowValue = curr->getItem()->getValue(0);
      x = curr->getItem()->getValue(1) + a;
      y = curr->getItem()->getValue(2) + b;
      double value = this->conv(x,y,offset);
      newItem = Double(x,y,value);
      curr->setItem(newItem);
      currentIndex++;

      if(100*currentIndex/totalPoints >= (percentFinished+5))
	{
	  percentFinished = (int) (currentIndex)*100/(totalPoints);
	  VERBOSE_PRINT("Percent finished: ");
	  VERBOSE_PRINT(percentFinished);
	}
      convergence->forward();
      curr = convergence->getCurr();
    }
  
  convergence->setCurr(saveSpot);
  this->windowValue = saveWindow;
  return;

}


//x,y positions in arcseconds
double GLShear::conv(double x,double y,double * offset)
{
  double returnMe = 0.0;
  double N = observationParameters[0];
  double pixelsize = Cosmology::arcsecondsToCentimeters(1,lensParameters[2]);//arcseconds to centimeters
  double centiX = x*pixelsize;
  double centiY = y*pixelsize;

  if(offset != 0)
    {
      if(sizeof(offset)/sizeof(double) != 2)
	throw DavidException("I need both an X and Y offset", DavidException::INVALID_ARGUMENT_ERROR_CODE);

      centiX += offset[0]*pixelsize;
      centiY += pixelsize*offset[1];
    }

  double c;
  try
    {
      /**
       * speed of light should be provied in parameters file.
       * This way units are "guaranteed" to match.
       */
      c = observationParameters[5];
    }
  catch(...)
    {
      throw DavidException("Speed of light not supplied in parameters");
    }

  double Sigma_critical = Cosmology::criticalDensity(lensParameters[2],sourceParameters[2]);
	
  DNode<Double> * keepThePlace = massDensity->getCurr();//Just in case(?)

  while(massDensity->getCurr() != 0)
    {
      Double curr = massDensity->getCurr()->getItemCopy();
      double i = curr.getValue(1);
      double j = curr.getValue(2);
      double M = observationParameters[3]*curr.getValue(0);//Mass in grams
      double xi = (i*pixelsize-centiX)*(i*pixelsize-centiX) + (j*pixelsize-centiY)*(j*pixelsize-centiY);//measured in centimeters
      xi = sqrt(xi);
      
      //std::cout << "distance: " << xi << " window: " << windowValue*pixelsize << std::endl;
      returnMe +=  SPL::smoothConvergence(M,xi,0,Sigma_critical,windowValue*pixelsize);//angle is not really needed.
      
      massDensity->forward();
    }
  massDensity->setCurr(keepThePlace);
  return returnMe;
	
}
