#include "planecreator.h"

namespace utils{

	template <class T> PlaneCreator<T>::PlaneCreator(int _Nx, int _Ny, T backGround)
	{
		Nx = _Nx;
		Ny = _Ny;

		mainPlane = new Plane<T>(Nx, Ny, backGround);

	}

	template <class T> void PlaneCreator<T>::addEllipse(double semimajorLength, double eccentricity, int centerX, int centerY, T objectInPlane)
	{
		
		if(mainPlane == 0)
			throw DavidException("For Some reason, the plane doesn't exist yet");

		double a = semimajorLength;
		double e = eccentricity;
		double b = a*(1-e);

		int leftBound = -1-((int) a)+centerX;
		int rightBound = 1+((int) a)+centerX;
		int lowerBound = -1-((int) b)+centerY;
		int upperBound = 1+((int) b)+centerY;
		
		if(abs(leftBound) > Nx/2 || abs(rightBound) > Nx/2 || abs(lowerBound) > Nx/2 || abs(upperBound) > Nx/2)
			throw DavidException("Ellipse out of Plane Bounds",DavidException::INVALID_ARGUMENT_ERROR_CODE);


		for(int i = leftBound ; i<rightBound;i++)
			for(int j = lowerBound ; j<upperBound;j++)
			{
				double ellipse = pow(i*1.0-centerX,2.0)/pow(a*1.0,2.0)+pow(j*1.0-centerY,2.0)/pow(b*1.0,2.0);
				if(ellipse <= 1)
					mainPlane->setValue((int) (i+Nx/2),(int) (j+Ny/2),objectInPlane);
			}
	}

	template <class T> Plane<T> * PlaneCreator<T>::getPlane()
	{
		return mainPlane;
	}

template <class T> Plane<T> * PlaneCreator<T>::setPlane(int width, int height,Plane<T> * newPlane)
{
  Plane<T> * oldie = mainPlane;
  mainPlane = newPlane;

  return mainPlane;
}



};


template class utils::PlaneCreator<Double>;
