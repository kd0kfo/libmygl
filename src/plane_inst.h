/**
 * 
 * This file is part of libmygl, a C++ library for gravitational lensing
 * calculations. 
 *
 * Any use of of this library must cite my PhD thesis,
 * Coss, D., "Weak Shear Study of Galaxy Clusters by Simulated Gravitational
 * Lensing", PhD Thesis, 2010
 * which may be found at http://thesis.davecoss.com/Coss_dissertation.pdf
 *
 * Copyright 2007, 2010 David Coss, PhD
 *
 * libmygl is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * libmygl is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with libmygl.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef PLANE_INST_H
#define PLANE_INST_H 1

#ifndef __WIN__
#include "EasyBMP/EasyBMP.h"
#endif

#include "libdnstd/DavidException.h"
#include "libdnstd/Double.h"

#include <netcdf.h>

#include <fstream>
#include <sstream>

#include <iostream>
#include <vector>


/**
 * Plane Handler.
 * This class controls the data which is a 2 dimensional array, a plane.
 * It holds the array data and has methods to alter, draw, and write the plane data.
 * This template must be instantiated for the specificl program's use, eg template class Plane<Double>
 * 
 * EasyBMP is used for drawing.
 * Created By David Coss, 2007, 2010
 */
template<class T> class Plane
{
public:
  typedef T data_type;

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
	const T& getValue(const int x, const int y) const;///< gets the value at (x,y).
	void setValue(int x, int y, T value);///< setsthe value at (x,y) to value.
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
	void draw(const std::string& fileName, bool blackAndWhite = false, bool useGrid = false, int gridSpace = 1000,double bgRed = 0, double bgGreen = 0, double bgBlue = 0) throw (DavidException);
	
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
	T getMaxValue()const;

	/**
	 * Give the minimum value in the plane
	 */
	T getMinValue()const;

	/**
	 * Draws a curve of given specifications.
	 */
	bool includeCurve(bool useCurve,double x, double y, double semimajor,double ellipticity,double Red = 0, double Green = 255, double Blue = 0);
	
	/**
	 * Gives the sum of all values in the plane.
	 */
	T getTotalValue()const;

	/**
	 * Saves the plane data as a text file.
	 * Use this method to save the plane to be used again.
	 * @param fileName const char* string for the file's name
	 * @return bool true if the save was successful.
	 * @see readPlane(const char * fileName, Plane<T> * newPlanePointer)
	 */
	bool savePlane(const char * fileName, bool verbose = false);

	/**
	 * Saves the plane data as a text file.
	 * Use this method to save the plane to be used again.
	 * @param rhs std::string string for the file's name
	 * @return bool true if the save was successful.
	 */
	bool savePlane(const std::string& rhs, bool verbose = false);

	bool writeCDF(const std::string& filename, bool verbose = false);
	bool writeCDF(const char* filename, bool verbose = false);


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
	static Plane<T> * readPlane(const std::string& fileName);

	static Plane<T>* loadCDF(const std::string& filename, bool verbose = false);
	static Plane<T>* loadCDF(const char* filename, bool verbose = false);
	
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
	static Plane<Double> * bmpToPlane(const std::string& fileName);
	
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
	 * Gives the pointer to the std::string header
	 */
	std::string const * const getHeader() const{return header;}

	/**
	 * Sets the std::string header value
	 */
	void setHeader(const std::string& newHeader);
	
	/**
	 * Removes the header, if it exists
	 */
	void removeHeader(){delete header;header = 0;}

private:
	int * rows;
	int * columns;
	std::vector<T> planeArray;
	int numberOfCurves;
	std::string * header;//Plane info if provided, otherwise null pointer

		  

};

template <class T> Plane<T>::Plane()
{
  rows = new int;
  columns = new int;
  *rows = 1;
  *columns = 1;
  planeArray = std::vector<T>(1);
  header = 0;

  numberOfCurves = 0;

}

template <class T> Plane<T>::Plane(const Plane<T>& rhs)
{ 
  rows = new int;
  columns = new int;
  *rows = rhs.numberOfRows();
  *columns = rhs.numberOfColumns();
  planeArray = std::vector<T>(*rows * *columns);
  
  for(int i = 0;i<*rows;i++)
    for(int j = 0;j<*columns;j++)
      {
			T getTheValue = rhs.getValue(i,j);
			setValue(i,j,getTheValue);
      }

  if(rhs.getHeader() != 0)
    header = new std::string(*(rhs.header));
  else
    header = 0;

}
  

template <class T> Plane<T>::Plane(int _rows, int _columns, T _initialValue)
{
  if(_rows < 0 || _columns < 0)
    throw DavidException("Non negative dimensions are required",DavidException::INVALID_ARGUMENT_ERROR_CODE);

    header = 0;
    rows = new int;
  columns = new int;
  *rows = _rows;
  *columns = _columns;
  planeArray = std::vector<T>(_rows*_columns);

  for(int i=0;i<_rows;i++)
  {
		for(int j=0;j<_columns;j++)
		{
			planeArray[j+i*_columns] = _initialValue;
		}
 }
  
  numberOfCurves = 0;
  
  
}

template <class T> Plane<T>::Plane(std::vector<T> array, int _rows, int _columns)
{
  if(_rows < 0 || _columns < 0)
    throw DavidException("Non negative dimensions are required",DavidException::INVALID_ARGUMENT_ERROR_CODE);
  

  header = 0;
  planeArray = array;
  rows = new int;
  columns = new int;
  *rows = _rows;
  *columns = _columns;
}

template <class T> Plane<T>::~Plane()
{
  delete header;
  delete this->rows;
  delete this->columns;
  this->rows = 0;
  this->columns = 0;
  this->header = 0;
}

template <class T> void Plane<T>::setHeader(const std::string& bean)
{
  if(header == 0)
    header = new std::string();
  
  *header = bean;
}

template <class T> int Plane<T>::numberOfRows() const {return *rows;}

template <class T> int Plane<T>::numberOfColumns() const {return *columns;}

template <class T> int * Plane<T>::getDimensions() const
{
  int * returnMe = new int[2];

  returnMe[0] = this->numberOfRows();
  returnMe[1] = this->numberOfColumns();
  return returnMe;
}

template <class T> Plane<T>& Plane<T>::operator=(const Plane<T>& rhs)
{
  rows = new int;
  columns = new int;
  *rows = rhs.numberOfRows();
  *columns = rhs.numberOfColumns();
  planeArray = std::vector<T>(*rows * *columns);
  
  for(int i = 0;i<*rows;i++)
    for(int j = 0;j<*columns;j++)
      {
		T theStuff = rhs.getValue(i,j);
		setValue(i,j,theStuff);
      }
  
  if(rhs.getHeader() != 0)
    {
      header = new std::string;
      *header = *rhs.getHeader();
    }
  else
    header = 0;
      
  return *this;
}

template <class T> void Plane<T>::populate(const T& value)
{
	T newValue = value;
	for(int i = 0;i< this->numberOfRows();i++)
		for(int j = 0;j<this->numberOfColumns();j++)
			this->setValue(i,j,newValue);
}

template <class T> const T& Plane<T>::getValue(const int x, const int y) const
{

  if(x < 0 || y < 0)
    {
      throw DavidException(std::string("These Coordinates (") + Double(x).str() + std::string(",") + Double(y).str() + std::string(") are out side of the plane which has dimensions ") + Double(*rows).str() + std::string(" by ") + Double(*columns).str() + ". Negative Values are not allowed",DavidException::PLANE_OUT_OF_BOUNDS_ERROR_CODE);
    }
	
  if(x > *rows || y > *columns)
    {
      throw DavidException(std::string("These Coordinates (") + Double(x).str() + std::string(",") + Double(y).str() + std::string(") are out side of the plane which has dimensions ") + Double(*rows).str() + std::string(" by ") + Double(*columns).str(),DavidException::PLANE_OUT_OF_BOUNDS_ERROR_CODE);
    }
  if(planeArray.size() == 0)
    throw DavidException("Plane Array is empty.");
  return planeArray[y+x*(*columns)];
}

template <class T> void Plane<T>::setValue(int x, int y, T value)
{

    if(x < 0 || y < 0)
    {
      throw DavidException(std::string("These Coordinates (") + Double(x).str() + std::string(",") + Double(y).str() + std::string(") are out side of the plane which has dimensions ") + Double(*rows).str() + std::string(" by ") + Double(*columns).str() + ". Negative Values are not allowed",DavidException::PLANE_OUT_OF_BOUNDS_ERROR_CODE);
    }
	
  if(x > *rows || y > *columns)
    {
      throw DavidException(std::string("These Coordinates (") + Double(x).str() + std::string(",") + Double(y).str() + std::string(") are out side of the plane which has dimensions ") + Double(*rows).str() + std::string(" by ") + Double(*columns).str(),DavidException::PLANE_OUT_OF_BOUNDS_ERROR_CODE);
    }
  if(planeArray.size() == 0)
    throw DavidException("Plane Array is empty.");

  planeArray[y+x*(*columns)] = value;

  //return temp;
}

template <class T> bool Plane<T>::write(const char * fileName, bool useGrid, bool blackOrWhite, char whiteSymbol)
{
  using namespace std;

  ofstream myfile(fileName, ios::out);

  if(myfile.is_open())
    {
		if(useGrid)
		{
			for(int i = 0;i<(*columns);i++)
			{
				myfile << i % 10;
			}
		}
		myfile << endl;

      for(int i = 0;i< (*rows); i++)
	  {
		if(useGrid)
			myfile << i % 10;
		for(int j = 0; j< (*columns);j++)
		{
		  if(j != 0)
			  myfile << "," ;
		  if(!blackOrWhite)
		  {
			  myfile << "\"" << this->getValue(i,j) << "\"";
		  }
		  else
		  {
			if(this->getValue(i,j) == 0.0)
				myfile << " ";
			else
				myfile << whiteSymbol;
		  }
	  }
	  myfile << endl;
	}
      myfile.close();
    }
  else
    return false;

  return true;
}


template <class T> std::vector<T> Plane<T>::getPlaneArray()
{
	std::vector<T> returnMe((*rows) * (*columns));
  for(int i = 0; i< ((*rows) * (*columns));i++)
    returnMe[i] = planeArray[i];
  
  return returnMe;

}

template <class T> bool Plane<T>::isInSource(int x, int y, T value)
{
	return (this->getValue(x,y) == value);
}

template <class T> void Plane<T>::draw(const std::string& fileName, bool blackAndWhite, bool useGrid, int gridSpace,double bgRed, double bgGreen, double bgBlue) throw (DavidException)
{
  #ifndef __USE_BOINC__
	BMP image;

	image.SetSize(*rows,*columns);
	image.SetBitDepth(24);
	
	double currentValue = 0;//used for pixel color assignment loop
	bool isBackground = false;//used to determine if pixel is a background color
	for(int i = 0;i<*rows -1;i++)
	{
		for(int j = 0;j< *columns-1;j++)
		{

		  if(((i % gridSpace == 0) || (j % gridSpace == 0)) && i != 0 && j != 0 && useGrid)
			{
				image(i+1,j+1)->Red = 255;
				image(i+1,j+1)->Green = 0;
				image(i+1,j+1)->Blue = 0;
			}
			else
			{

				Double curr = getValue(i,j);
				for(int currIndex = 0;currIndex < 3;currIndex++)
				  {
				    
				    if(curr.getValue(currIndex) > 255)
				    {
				      1;DEBUG_PRINT("Warning: Setting pixel value to more than 255.");
				    }					   
				  }
				for(int k = 0;k<3;k++)
				  {
				    currentValue = curr.getValue(k);
				    isBackground = (curr.modulus() == 0);
				    switch(k)
				      {
				      case 0:
					if(!isBackground)
					  image(i+1,j+1)->Red = (int)currentValue;
					else
					  image(i+1,j+1)->Red = (int)bgRed;
					break;
				      case 1:
					if(!isBackground)
					  image(i+1,j+1)->Green = (int)currentValue;
					else
					  image(i+1,j+1)->Green = (int)bgGreen;
					break;
				      case 2:
					if(!isBackground)
					  image(i+1,j+1)->Blue = (int)currentValue;
					else
					  image(i+1,j+1)->Blue =(int) bgBlue;
					break;
				      default:
					break;
				      }
				  }
			}
			
		}
	}//end image writing for loop

	
	if(blackAndWhite && image.TellBitDepth() < 16)
		CreateGrayscaleColorTable(image);

	if(!image.WriteToFile(fileName.c_str()))
	  throw DavidException(std::string("Could not write ") + fileName,"EasyBMP Error",DavidException::IO_ERROR_CODE);
	

	std::cout << fileName+" has been written" << std::endl;
#endif	
	return;
}

template <class T> T Plane<T>::getMaxValue()const
{
  T returnMe = getValue(0,0);
  for(int i = 0;i < *rows; i++)
    for(int j = 0;j < *columns;j++)
      if(getValue(i,j) > returnMe)
	returnMe = getValue(i,j);
  
  return returnMe;
}

template <class T> T Plane<T>::getMinValue()const
{
  T returnMe = getValue(0,0);
  for(int i = 0;i < *rows; i++)
    for(int j = 0;j < *columns;j++)
      if(getValue(i,j) < returnMe)
	returnMe = getValue(i,j);
  
  return returnMe;
}

template <class T> bool Plane<T>::includeCurve(bool useCurve,double x, double y,double semimajor,double ellipticity, double Red, double Green, double Blue)
{
  return useCurve;

}

template <class T> T Plane<T>::getTotalValue()const
{
	T returnMe;

	for(int i = 0;i< *rows -1;i++)
	  for(int j = 0;j<*columns -1;j++)
	    returnMe += this->getValue(i,j);

	return returnMe;

}


template <class T> bool Plane<T>::savePlane(const std::string& rhs, bool verbose){return savePlane(rhs.c_str());}

template <class T> bool Plane<T>::savePlane(const char * fileName, bool verbose)
{
	using namespace std;
	
	fstream file (fileName, ios::out);
	
	if (file.is_open())
	{
	  if(this->header != 0)
	    {
	      DEBUG_PRINT(*(this->header));
	      file << *(this->header) << endl;
	    }
	  file << *rows << endl;
	  file << *columns << endl;

	  T outputT;
	  using namespace std;
	  ostringstream stream;
	  std::string tempString;
	  float percentFinished = 0.0;
	  if(verbose)
	    printf("Saving Plane:\n");
	  
	  for(int i = 0;i< *rows;i++)
	    {
	      for(int j = 0;j<*columns;j++)
		{
		  outputT = getValue(i,j);
		  tempString = outputT.str(stream);
		  file << tempString << endl;
		  stream.str("");
		  stream.clear();
		}
	      if(verbose && (i*100/(*rows)) >= (percentFinished+5))
		{
		  percentFinished = (int) i*100/(*rows);
		  printf("Percent finished: %f\n",percentFinished);
		}
	    }
	  return true;
	}
	else
	  return false;

}

template<class T> bool Plane<T>::writeCDF(const std::string& filename, bool verbose){return writeCDF(filename.c_str(),verbose);}

template<class T> bool Plane<T>::writeCDF(const char* filename, bool verbose)
{
  int retval, ncid, x_dimid, y_dimid, dimids[2], varid;
  double *temparray = NULL;

  if(filename == NULL)
    return false;
  
  temparray = (double*)malloc(sizeof(double)*(*this->rows)*(*this->columns));
  if(temparray == NULL)
    throw DavidException("Could not allocate memory for writeCDF");
  
  if((retval = nc_create(filename, NC_CLOBBER, &ncid)))
    {
      free(temparray);
      std::string error = "Error creating NetCDF file in writeCDF.";
      error += nc_strerror(retval);
      throw DavidException(error);
    }
  
  /* Define the dimensions. NetCDF will hand back an ID for each. */
   if ((retval = nc_def_dim(ncid, "x", *this->rows, &x_dimid)))
     {
       free(temparray);
       std::string error = "Error defining x dimension for NetCDF file in writeCDF.";
       error += nc_strerror(retval);
       throw DavidException(error);
     }
   if ((retval = nc_def_dim(ncid, "y", *this->columns, &y_dimid)))
     {
       free(temparray);
       std::string error = "Error defining y dimension for NetCDF file in writeCDF.";
       error += nc_strerror(retval);
       throw DavidException(error);
     }

   /* The dimids array is used to pass the IDs of the dimensions of
    * the variable. */
   dimids[0] = x_dimid;
   dimids[1] = y_dimid;

   /* Define the variable. The type of the variable in this case is
    * NC_INT (4-byte integer). */
   if ((retval = nc_def_var(ncid, "data", NC_INT, 2, dimids, &varid)))
     {
       free(temparray);
       std::string error = "Defining data for NetCDF file in writeCDF.";
       error += nc_strerror(retval);
       throw DavidException(error);
     }

   /* End define mode. This tells netCDF we are done defining
    * metadata. */
   if ((retval = nc_enddef(ncid)))
     {
       free(temparray);
       std::string error = "Error ending define mode for NetCDF file in writeCDF.";
       error += nc_strerror(retval);
       throw DavidException(error);
     }

   Double tmp;
   for(int i = 0, size = (*this->rows)*(*this->columns);i< size;i++)
     {
       tmp = this->planeArray[i];
       temparray[i] = tmp.doubleValue();
     }

   /* Write the pretend data to the file. Although netCDF supports
    * reading and writing subsets of data, in this case we write all
    * the data in one operation. */
   if ((retval = nc_put_var_double(ncid, varid, temparray)))
     {
       free(temparray);
       std::string error = "Error Writing NetCDF file in writeCDF.";
       error += nc_strerror(retval);
       throw DavidException(error);
     }

   /* Close the file. This frees up any internal netCDF resources
    * associated with the file, and flushes any buffers. */
   if ((retval = nc_close(ncid)))
     {
       free(temparray);
       std::string error = "Error closing NetCDF file in writeCDF.";
       error += nc_strerror(retval);
       throw DavidException(error);
     }

   return true;
  
}

template <class T>  Plane<T> * Plane<T>::readPlane(const std::string& fileName){return Plane<T>::readPlane(fileName.c_str());}

template <class T> Plane<T> * Plane<T>::readPlane(const char * fileName)
{

	using namespace std;
	fstream file (fileName, ios::in);
	if (file.is_open())
	{
		int tmpR,tmpC;
		char curr[150];
		std::string string = "#";
		std::string oldheader;
		
		while(string.at(0) == '#')
		  {
		    if(string != "#"){oldheader += string+"\n";}
		    file.getline(curr,150);
		    
		    string = curr;
		  }
		tmpR = Double(string).toInt();

		string = "#";
		while(string.at(0) == '#')
		  {
		    if(string != "#"){oldheader += string+"\n";}
		    file.getline(curr,150);
		    string = curr;
		  }
		tmpC = Double(string).toInt();

		T empty = 0.0;
		Plane<T> * newPlanePointer = new Plane<T>(tmpR,tmpC,empty);
		
		int counter = 0;
		int endCount = newPlanePointer->numberOfRows();
		float percentFinished = 0.;
		printf("Reading Plane:\n");
		char testChar;
		for(int i = 0;i< newPlanePointer->numberOfRows();i++)
			for(int j = 0;j< newPlanePointer->numberOfColumns();j++)
			{
			  counter++;
			  string = "#";
			  while(string.at(0) == '#')
			    {
			      if(string != "#"){oldheader += string + "\n";}
			      file.getline(curr,150);
			      string = curr;
			    }
			  
			  T _t = T::parseString(string);
			  newPlanePointer->setValue(i,j,_t);
			  
			  if((i*100/endCount) >= (percentFinished+5))
				{
					percentFinished = (int) i*100/endCount;
					printf("Percent finished: %f\n",percentFinished);
				}
			  
			}
		
		printf("Parsing complete\n");

		if(oldheader != "")
		  newPlanePointer->setHeader(oldheader.substr(0,oldheader.size()-1));

		if(counter != tmpR*tmpC)
		  throw DavidException("Not all or too few of the plane elements have been read.", DavidException::IO_ERROR_CODE);

		return newPlanePointer;

	}
	else
	  throw DavidException(std::string("File Read Error:")+fileName);
}

template <class T>  Plane<T>* Plane<T>::loadCDF(const std::string& fileName, bool verbose){return Plane<T>::loadCDF(fileName.c_str());}

template <class T> Plane<T>* Plane<T>::loadCDF(const char* filename, bool verbose)
{
  throw DavidException("IMPLEMENT LOAD CDF");
}

template <class T> Plane<Double> * Plane<T>::bmpToPlane(const std::string& fileName){return bmpToPlane(fileName.c_str());}

template <class T> Plane<Double> * Plane<T>::bmpToPlane(const char * fileName)
{

#ifndef __WIN__
  BMP image;

  image.ReadFromFile(fileName);

  int width = image.TellWidth();
  int height  = image.TellHeight();

  Double zero(0.0);

  Plane<Double> * newPlane = new Plane<Double>(width,height,zero);
  
  for(int i = 0; i<width-1;i++)
    for(int j = 0;j<height-1;j++)
      {
		double value = 0;
		double value2 = 0;
		double value3 = 0;
		//	DEBUG_PRINT(i+1);
		//	DEBUG_PRINT(j+1);
		value += (int) image(i+1,j+1)->Red;
		value2 += (int) image(i+1,j+1)->Green;
		value3 += (int) image(i+1,j+1)->Blue;
		
		Double newOne(value,value2,value3);
		newPlane->setValue(i,j,newOne);
      }
  return newPlane;
#else
  throw DavidException("Without EasyBMP, a plane cannot be created from a bitmap",DavidException::INVALID_ARGUMENT_ERROR_CODE);
  return 0;
#endif

}

template<class T> Plane<T> * Plane<T>::addPlanes(Plane<T> * left, Plane<T> * right)
{

  if(left->numberOfColumns() != right->numberOfColumns())
    {
      std::string errorMessage = "Number of Columns don't match: 2 planes (";
      errorMessage += Double(left->numberOfRows()).str() + "x" + Double(left->numberOfColumns()).str();
      errorMessage += ") and (";
      errorMessage += Double(right->numberOfRows()).str() + "x" + Double(right->numberOfColumns()).str() + ")";
      throw DavidException(errorMessage);
    }

  if(left->numberOfRows() != right->numberOfRows())
    {
      std::string errorMessage = "Number of Rows don't match: 2 planes (";
      errorMessage += Double(left->numberOfRows()).str() + "x" + Double(left->numberOfColumns()).str();
      errorMessage += ") and (";
      errorMessage += Double(right->numberOfRows()).str() + "x" + Double(right->numberOfColumns()).str() + ")";
      throw DavidException(errorMessage);
    }

  int rows = left->numberOfRows();
  int columns = left->numberOfColumns();

  T newT;
  Plane<T> * returnMe = new Plane<T>(rows, columns, newT);


  for(int i = 0; i<rows;i++)
    for(int j = 0; j<columns;j++)
      {
	T temp(left->getValue(i,j)+right->getValue(i,j));
	returnMe->setValue(i,j,temp);
      }

  return returnMe;

}

template <class T> Plane<T> * Plane<T>::subtractPlanes(Plane<T> * left, Plane<T> * right)
{
    if(left->numberOfColumns() != right->numberOfColumns())
    throw DavidException("Number of Columns don't match");

  if(left->numberOfRows() != right->numberOfRows())
    throw DavidException("Number of Rows don't match");

  int rows = left->numberOfRows();
  int columns = left->numberOfColumns();

  T newT;
  Plane<T> * returnMe = new Plane<T>(rows, columns, newT);
  
  for(int i = 0; i<rows;i++)
    for(int j = 0; j<columns;j++)
      {
	T temp = left->getValue(i,j)-right->getValue(i,j);
	returnMe->setValue(i,j,temp);
      }

  return returnMe;
}

template <class T> Plane<T> * Plane<T>::transpose()
{
  T curr;

  Plane<T> * returnMe = new Plane<T>(this->numberOfColumns(),this->numberOfRows(),curr);

  for(int i = 0;i<*rows;i++)
    for(int j = 0;j<*columns;j++)
      {
	curr = this->getValue(i,j);
	returnMe->setValue(j,i,curr);
      }

  return returnMe;
}


#endif
