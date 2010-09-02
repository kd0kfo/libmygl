#ifndef GLSHEAR_CPP
#define GLSHEAR_CPP

#ifndef _WIN32
#import <iostream>
#endif

#ifdef __DEBUG__
#define DEBUG_PRINT(x) std::cout << x << std::endl; 
#else 
#define DEBUG_PRINT
#endif

#include <math.h>
#include <iostream>

#include "Double.h"
#include "Complex.h"

#include "densityprofile.h"
#include "Cosmology.h"



/** \mainpage Documentation of the Lensing Software.
 * Created By David Coss, 2007
 */

/**
 * General Lensing Algorithm.
 * This class handles lensing information and calculations.
 * Created By David Coss, 2007
 */
class GLShear
{

public:

	/**
	 * Default Constructor.
	 * All pointers set to null.
	 */
	GLShear();

	/**
	 * Parameter based constructor.
	 * This constructor should be used if the deflection angles have not yet been calculated.
	 * @param observationParameters Set of General paramters (eg speed of light, solar mass).
	 * @param lensParameters Lens Parameters (eg size, location).
	 * @param sourceParameters Source parameters.
	 * @param density DensityProfile (usually to be calculated by this constructor).
	 * @param _sourcePlane Plane containing the sources.
	 */
	GLShear(double * observationParameters, double * lensParameters, double * sourceParameters, DensityProfile * density, Plane<Double> * _sourcePlane, int * glellipseBounds = 0, double * offset = 0);

	/**
	 * Lens Density and Source based constructor.
	 * This is used if the density profile has already been calculated.
	 * @param density DensityProfile of the lens.
	 * @param sourceParameters Parameters of the source.
	 * @param _sourcePlane The sources.
	 */
	GLShear(DensityProfile * density, double * sourceParameters, Plane<Double> * _sourcePlane, int * glellipseBounds = 0, double * offset = 0);

	/**
	 * Deflection angle array based constructor.
	 * This constructor should be used the most. It is suggested (for speed of simulation) that he deflection angles
	 * be calculated, saved, and then reused as need. This construct provides deflection planes, via newDeflectionMap, 
	 * to be used once calculated. Note: the 2 dimensional deflections are stored as Complex numbers with
	 * the horizontal and vertical deflections as the Real and Imaginary Parts, respectively.
	 * @param newDeflectionMap Complex Deflections Plane
	 * @param observationParameters General observation Parameters.
	 * @param sourceParameters Source Parameters.
	 * @param _sourcePlane Source Plane.
	 */
	GLShear(Plane<math::Complex> * newDeflectionMap, double * observationParameters, double * sourceParameters, Plane<Double> * _sourcePlane);

	/**
	 * Destructor.
	 * This destructor only sets the pointers to null, since they may be used in other
	 * parts of the program. So if they should be deleted, delete the outside of glalgorithm.
	 */
	~GLShear();

	/**
	 * Gives the point in the source frame the point (x,y) would be lensed to.
	 * @param x horizontal position in pixels from corner (zero indexed).
	 * @param y vertical position in pixels from corner (zero indexed).
	 */
	double * newLocation(double x, double y);

	/**
	 * Gives the amount of deflection.
	 * The Deflection is given as Complex numbers with
	 * the horizontal and vertical deflections as the Real and Imaginary Parts, respectively.
	 * 
	 * offset is a 2-D vector containing the x and y offsets respectively. If offset = 0, then there is no offset. So offset can be null. Offset is a pixel offset (ie must match x and y's units).
	 *
	 * @param x horizontal position in pixels from corner (zero indexed).
	 * @param y vertical position in pixels from corner (zero indexed).
	 * @return Complex
	 */
	math::Complex shear(double x,double y, double * offset);
	
	/**
	 * Checks to see whether the point (x,y) maps to a source, if so, returns it's mass.
	 * x and y in arcseconds.
	 * @param x horizontal location
	 * @param y vertical location.
	 * @return Double mass at source location.
	 */
	Double mapsToSource(double x, double y);

	/**
	 * Determines whether the location is at the Einstein Radius.
	 * @return bool.
	 */
	bool isAtEinsteinRadius(double x, double y){return false;}
	
	static double abs(const double x){return sqrt(x*x);}///< Absolute value of x
	
	/**
	 * Reads the source plane and give the mass in that space.
	 * @param x horizontal position.
	 * @param y vertical position.
	 * @return double mass at that position.
	 */
	double massInElement(const double x, const double y) const;

	/**
	 * Determines whether there is lens mass at the point (x,y).
	 * @param x horizontal position.
	 * @param y vertical position.
	 * @return bool.
	 */
	bool isLensMassElement(const double x, const double y) const;

	/**
	 * Creates a plane creating the deflection angles.
	 * If the plane was created correctly, true is returned.
	 * X and Y bounds were added to allow for parallel computing. The deflection map can be calculated a portion
	 * at a time. Then those portions may be added together to form a final grid.
	 *
	 * @return bool.
	 * @see getDeflectionPlane()
	 */
	bool createShearMapPlane(int _leftBound = -1, int _rightBound = -1, int _lowerBound = -1, int _upperBound = -1, double * offset = 0);

	/**
	 * Returns a pointer to the deflection plane.
	 * @return Plane<math::Complex>*
	 * @see createDeflectionMapPlane()
	 */
	Plane<math::Complex> * getShearPlane(){return shearMap;}

private:
	double * observationParameters;
	double * lensParameters;
	double * sourceParameters;
	DensityProfile * massDensity;
	Plane<math::Complex> * shearMap;
	Plane<Double> * sourcePlane;
};

#endif

