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

#include "plane.h"
#include <math.h>
#include <iostream>


#ifdef __DEBUG__
#define DEBUG_PRINT(x) std::cout << x std::endl;
#else
#define DEBUG_PRINT(x)
#endif

#include "libndstd/DList.h"
#include "libndstd/Complex.h"
#include "libndstd/Double.h"

#include "spl.h"
#include "Cosmology.h"


/**
 * General Shear Algorithm for use with SPL.
 * This class handles lensing information and calculations.
 * Created By David Coss, 2009
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
	 * This constructor should be used if the shear quantities have not yet been calculated.
	 * @param observationParameters Set of General paramters (eg speed of light, solar mass).
	 * @param lensParameters Lens Parameters (eg size, location).
	 * @param sourceParameters Source parameters.
	 * @param density DList<Double> particle distribution for use with SPL
	 */
	GLShear(double * observationParameters, double * lensParameters, double * sourceParameters, DList<Double> * density, Plane<Double> * _sourcePlane, int * glellipseBounds = 0, double * offset = 0);

	/**
	 * Lens Density and Source based constructor.
	 * This is used if the density profile has already been calculated.
	 * @param density DensityProfile of the lens.
	 * @param sourceParameters Parameters of the source.
	 * @param _sourcePlane The sources.
	 */
	GLShear(DList<Double> * density, double * _observationParameters, double * lensParameters, double * sourceParameters, Plane<Double> * _sourcePlane, int * glellipseBounds = 0, double * offset = 0, bool createShearMap = true, double windowValue = 1,DList<Double> * optionalSource = 0);

	/**
	 * Shear Map based constructor.
	 * This constructor should be used the most. It is suggested (for speed of simulation) that the shear
	 * be calculated, saved, and then reused as need. This construct provides shear planes, via newShearMap, 
	 * to be used once calculated. Note: the 2 dimensional shear is stored as Complex numbers with
	 * the horizontal and vertical shear as the Real and Imaginary Parts, respectively.
	 * @param newShearMap Complex Shear Map
	 * @param observationParameters General observation Parameters.
	 * @param sourceParameters Source Parameters.
	 * @param _sourcePlane Source Plane.
	 */
	GLShear::GLShear(DList<Double> * newShearMap, double * observationParameters, double * sourceParameters, Plane<Double> * _sourcePlane);

	/**
	 * Destructor.
	 * This destructor only sets the pointers to null, since they may be used in other
	 * parts of the program. So if they should be deleted, delete the outside of glalgorithm.
	 */
	~GLShear();

	/**
	 * Gives the amount of shear
	 * The Shear is given as Complex numbers with
	 * the horizontal and vertical deflections as the Real and Imaginary Parts, respectively.
	 *
	 * offset is a 2-D vector containing the x and y offsets respectively. If offset = 0, then there is no offset. So offset can be null. Offset is a pixel offset (ie must match x and y's units).
	 *
	 * @param x horizontal position.
	 * @param y vertical position.
	 * @return Complex
	 */
	math::Complex GLShear::shear(double x,double y,double * offset = 0);
	
	/**
	 * Determines whether the location is at the Einstein Radius.
	 * @return bool.
	 */
	bool isAtEinsteinRadius(double x, double y){return false;}
	
	static double abs(const double x){return sqrt(x*x);}///< Absolute value of x
	
	/**
	 * Creates a plane creating the shear values.
	 * If the plane was created correctly, true is returned.
	 * X and Y bounds were added to allow for parallel computing. The shear map can be calculated a portion
	 * at a time. Then those portions may be added together to form a final grid.
	 *
	 * @return bool.
	 * @see getShearMap()
	 */
	bool createShearMapPlane(double * offset = 0);

	/**
	 * Returns a pointer to the shear map plane.
	 * @return Plane<math::Complex>*
	 * @see createShearMapPlane()
	 */
	DList<Double> * getShearPlane(){return shearMap;}

	/**
	 * Returns a pointer to a new convergence plane.
	 * Note: the point is not stored anywhere else. Therefore, you must delete it if you ever want to see that memory again.
	 */
	void createConvergencePlane(DList<Double> * convergence, double * offset);

	double conv(double x,double y,double * offset = 0);

private:
	double * observationParameters;
	double * lensParameters;
	double * sourceParameters;
	double windowValue;
	DList<Double> * massDensity;
	DList<Double> * shearMap;
	Plane<Double> * sourcePlane;
};


#endif

