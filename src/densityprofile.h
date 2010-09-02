#ifndef DENSITYPROFILE_CPP
#define DENSITYPROFILE_CPP

#include <iostream>

#ifdef __DEBUG__
#define DEBUG_PRINT(x) std::cout << x << std::endl; 
#else 
#define DEBUG_PRINT
#endif
#ifdef __VERBOSE__
#define VERBOSE_PRINT(x) std::cout << x << std::endl; 
#else 
#define VERBOSE_PRINT(x)
#endif

#include "Double.h"


#include "plane.h"

/**
 * Density Profile Handler.
 * This Class handles information pertaining to the Deflector, including mass and shape
 * Created By David Coss, 2007
 */
class DensityProfile
{

public:
  /**
   * Default Constructor.
   * This is the default constructor. By default all members(pointers) are set to null.
   */
	DensityProfile();
	
	/**
	 * Main Constructor.
	 * This is the main constructor to be used.
	 * @param newPlane The lensing plane, where the elements of the plain are mass values.
	 * @param lensParameters Parameters of the lens
	 * @see ray_trace_ellipse for simulation setup
	 * @param observationParameters General Parameters
	 */
	DensityProfile(Plane<Double> * newPlane, double * lensParameters, double * observationParameters);

	/**
	 * Destructor.
	 */
	~DensityProfile();
	
	/**
	 * Sets the lens plane values.
	 * @param newPlane new Lens plane.
	 * @return Plane<Double> Old plane value. NOTE: if you don't delete it, it won't be deleted(eg memory leak). Enjoy!
	 */
	Plane<Double> * setPlane(Plane<Double> * newPlane);//Returns Old Plane, DELETE IT!!

	/**
	 * Gets a pointer to the lens plane.
	 * DOES NOT Call drawPlane() to be sure that the plane is up to date. You need to do that.
	 * So it can return 0
	 *
	 * @see drawPlane()
	 */
	Plane<Double> * getPlane();
	
	/**
	 * Determines whether there is mass at a given point, (x,y).
	 * @param x double horizontal value.
	 * @param y double vertical value.
	 * @return bool
	 */
	bool isMassAtPoint(const double x, const double y) const;
	
	/**
	 * Returns the mass at the point, (x,y), in solar masses.
	 *
	 * If a Plane<Double> was provided for the mass density,
	 * x and y must represent the pixel coordinates of the 
	 * position in the Plane array (zero indexed). If, on
	 * the other hand, no Plane<Double> is provided, the 
	 * mass at the point will becalculated based on the 
	 * provided elliptical density information given, where
	 * x and y are in pixel values from the center of the 
	 * grid.
	 *
	 * @param x double horizontal value
	 * @param y double vertical value.
	 * @return double mass value
	 */
	double massAtPoint(const double x, const double y) const;

	/**
	 * Determines whether or not the Plane which contains the mass at each point is created.
	 */
	bool densityProfilePlaneIsDefined() const{ return lensPlane != 0;}
	
	/**
	 * Creates the Plane which contains the mass at each point.
	 */
	void drawPlane();
	
	bool clearAllFields();///< deletes everything. sets all pointers equal to zero. BE CAREFUL!!!
	double * getObservationParameters(){return observationParameters;}
	double * getLensParameters(){return lensParameters;}

private:
	Plane<Double> * lensPlane;
	double * observationParameters;
	double * lensParameters;


};

#endif

