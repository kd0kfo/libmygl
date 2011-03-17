#ifndef PLANECREATOR_CPP
#define PLANECREATOR_CPP

#include "libdnstd/DavidException.h"
#include "libdnstd/Double.h"

#include "plane.h"

#include <iostream>
#include <math.h>

namespace utils{

	template<class T> class PlaneCreator{

	public:

		/**
		 * Main (only) Constructor
		 * Creates a plane (pointer) which has dimensions of Nx by Ny and has the specified background
		 */
		PlaneCreator(int Nx, int Ny, T backGround);
		
		/**
		 * Destructor
		 */
		~PlaneCreator(){;}

		/**
		 * Adds an Ellipse with the specified parameters
		 *
		 * @param semimajorLength Length fo the semimajor axis
		 * @param eccentricity e=1-b/a, a = semimajor axis length b = semiminor axis length
		 * @param centerX Horizontal Center location
		 * @param centerY Vertical Center location
		 * @param objectInPlane
		 */
		void addEllipse(double semimajorLength, double eccentricity, int centerX, int centerY, T objectInPlane);
		
		/**
		 * Returns the plane of the object.
		 *
		 * @returns Plane<T>
		 */
		Plane<T> * getPlane();

		/**
		 * Manually sets the plane to the provided plane.
		 * The old plane is returned
		 *
		 * @param width int
		 * @param height int
		 * @param newPlane Plane<T>
		 * @returns OldPlane Plane<T> (could be null!!)
		 */
		Plane<T> * setPlane(int width, int height,Plane<T> * newPlane);
	      
		
	private:
		Plane<T> * mainPlane;
		int Nx, Ny;
	};


};

#endif

