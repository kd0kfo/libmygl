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
