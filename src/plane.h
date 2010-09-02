#ifndef PLANE_CPP
#define PLANE_CPP


#include <fstream>
#include <sstream>

#ifndef __WIN__
#include "EasyBMP/EasyBMP.h"
#endif

#include "DavidException.h"
#include "DStack.h"

#include <iostream>

#ifdef __VERBOSE__
#define VERBOSE_PRINT(x) std::cout << x << std::endl; 
#else 
#define VERBOSE_PRINT(x)
#endif

#ifdef __DEBUG__
#define DEBUG_PRINT(x) std::cout << x << std::endl; 
#else 
#define DEBUG_PRINT
#endif

#include <vector>

#include "Double.h"




/**
 * Plane Handler.
 * This class controls the data which is a 2 dimensional array, a plane.
 * It holds the array data and has methods to alter, draw, and write the plane data.
 * This template must be instantiated for the specificl program's use, eg template class Plane<Double>
 * 
 * EasyBMP is used for drawing.
 * Created By David Coss, 2007
 */
template<class T> class Plane
{
  
public:

	/**
	 * Main (only) constructor.
	 * Alternatively, the plane can be created from a pre-existing array, via readPlane() and bmpToPlane()
	 * @param rows
	 * @param columns
	 * @param intialValue initial value of all of the array elements.
	 */
	Plane(int rows, int columns, T initialValue);

	Plane();
	Plane(const Plane<T>&);
	Plane(std::vector<T> oldTs, int rows, int columns);
	/**
	 * Destructor.
	 */
	~Plane();

	Plane<T>& operator=(const Plane<T>& rhs);
	  
	/**
	 * Fills the entire plane with the given value.
	 */
	void populate(const T& value);
	T getValue(const int x, const int y) const;///< gets the value at (x,y).
	void setValue(int x, int y, T& value);///< setsthe value at (x,y) to value.
	int numberOfRows() const;///< gives the number of rows as an int.
	int numberOfColumns() const;///< gives the number of columns as an int.
	int * getDimensions() const;///< gives rows and columns (in that order) in an array

	/**
	 * Draws the images as a bitmap file.
	 * 
	 * @see draw(const char * fileName,bool blackAndWhite = false, bool useGrid = false, int gridSpace = 100)
	 * @param fileName name of created file (files will be overwritten).
	 * @param blackAndWhite default = false.
	 * @param useGrid default = false.
	 * @param gridSpace default = 100 pixels.
	 * @param bgRed Background Red pixel value
	 * @param bgGreen Background Green pixel value
	 * @param bgBlue Background Blue pixel value
	 */
	void draw(const DString& fileName, bool blackAndWhite = false, bool useGrid = false, int gridSpace = 1000,double bgRed = 0, double bgGreen = 0, double bgBlue = 0) throw (DavidException);
	
	std::vector<T> getPlaneArray();///< returns a pointer to the plane's array.
	
	/**
	 * "Draws" the plane's data as ASCII text file. To save the plane as numerical text value, see savePlane(const char *fileName)
	 * whiteSymbol is the character to be used as "empty space."
	 */
	bool write(const char * fileName,bool addGrid = false, bool blackOrWhite = true, char whiteSymbol = '.');
	
	/**
	 * Determines if the given value is contained at (x,y).
	 * @param x
	 * @param y
	 * @param value
	 * @return bool
	 */
	bool isInSource(int x, int y, T value);
	
	/**
	 * Gives the maximum value in the plane.
	 */
	T getMaxValue();

	/**
	 * Give the minimum value in the plane
	 */
	T getMinValue();

	/**
	 * Draws a curve of given specifications.
	 */
	bool includeCurve(bool useCurve,double x, double y, double semimajor,double ellipticity,double Red = 0, double Green = 255, double Blue = 0);
	
	/**
	 * Gives the sum of all values in the plane.
	 */
	T getTotalValue();

	/**
	 * Saves the plane data as a text file.
	 * Use this method to save the plane to be used again.
	 * @param fileName const char* string for the file's name
	 * @return bool true if the save was successful.
	 * @see readPlane(const char * fileName, Plane<T> * newPlanePointer)
	 */
	bool savePlane(const char * fileName);

	/**
	 * Saves the plane data as a text file.
	 * Use this method to save the plane to be used again.
	 * @param rhs DString string for the file's name
	 * @return bool true if the save was successful.
	 */
	bool savePlane(const DString& rhs);

	/**
	 * Returns a pointer to a plane created based on a file.
	 * Note: if there is an error in the file being read, a DavidException is thrown.
	 *
	 * @param fileName.
	 * @return Plane<T>*
	 * @throws DavidException.
	 */
	static Plane<T> * readPlane(const char * fileName);
	
	/**
	 * Returns a pointer to a plane created based on a file.
	 * Note: if there is an error in the file being read, a DavidException is thrown.
	 *
	 * @param fileName.
	 * @return Plane<T>*
	 * @throws DavidException.
	 */
	static Plane<T> * readPlane(const DString& fileName);
	
	/**
	 * Returns a pointer to a plane created based on a bitmap file.
	 * The contents of the plane are the RGB contents of the bitmap.
	 * @param fileName
	 * @return Plane<T>*
	 */
	static Plane<Double> * bmpToPlane(const char * fileName);


	/**
	 * Returns a pointer to a plane created based on a bitmap file.
	 * The contents of the plane are the RGB contents of the bitmap.
	 * @param fileName
	 * @return Plane<T>*
	 */
	static Plane<Double> * bmpToPlane(const DString& fileName);
	
	/**
	 * Adds the values of the two arrays. Returns the Result.
	 * Note: Arrays must be same size.
	 * @param left
	 * @param right
	 * @return Plane<T> sum of left and right
	 */
	static Plane<T> * addPlanes(Plane<T> * left, Plane<T> * right);

	/**
	 * Subtracts the right plane from the left plane.
	 * Note: Arrays must be same size.
	 * @param left
	 * @param right
	 * @return Plane<T> difference of left and right
	 */
	static Plane<T> * subtractPlanes(Plane<T> * left, Plane<T> * right);

	/**
	 * Gives a pointer to a new plane equal to the transpose of this plane
	 *
	 */
	Plane<T> * transpose();

	/**
	 * Gives the pointer to the DString header
	 */
	DString const * const getHeader() const{return header;}

	/**
	 * Sets the DString header value
	 */
	void setHeader(const DString& newHeader);
	
	/**
	 * Removes the header, if it exists
	 */
	void removeHeader(){delete header;header = 0;}

private:
	int * rows;
	int * columns;
	std::vector<T> planeArray;
	int numberOfCurves;
	DString * header;//Plane info if provided, otherwise null pointer

		  

};


#endif


