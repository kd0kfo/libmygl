#include "plane.h"

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
    header = new DString(*(rhs.getHeader()));
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

template <class T> void Plane<T>::setHeader(const DString& bean)
{
  if(header == 0)
    header = new DString();
  
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
    header = new DString(*(rhs.getHeader()));
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

template <class T> T Plane<T>::getValue(const int x, const int y) const
{

  if(x < 0 || y < 0)
    {
      throw DavidException(DString("These Coordinates (") + Double(x).toDString() + DString(",") + Double(y).toDString() + DString(") are out side of the plane which has dimensions ") + Double(*rows).toDString() + DString(" by ") + Double(*columns).toDString() + ". Negative Values are not allowed",DavidException::PLANE_OUT_OF_BOUNDS_ERROR_CODE);
    }
	
  if(x > *rows || y > *columns)
    {
      throw DavidException(DString("These Coordinates (") + Double(x).toDString() + DString(",") + Double(y).toDString() + DString(") are out side of the plane which has dimensions ") + Double(*rows).toDString() + DString(" by ") + Double(*columns).toDString(),DavidException::PLANE_OUT_OF_BOUNDS_ERROR_CODE);
    }
  if(planeArray.size() == 0)
    throw DavidException("Plane Array is empty.");
  return planeArray[y+x*(*columns)];
}

template <class T> void Plane<T>::setValue(int x, int y, T& value)
{

    if(x < 0 || y < 0)
    {
      throw DavidException(DString("These Coordinates (") + Double(x).toDString() + DString(",") + Double(y).toDString() + DString(") are out side of the plane which has dimensions ") + Double(*rows).toDString() + DString(" by ") + Double(*columns).toDString() + ". Negative Values are not allowed",DavidException::PLANE_OUT_OF_BOUNDS_ERROR_CODE);
    }
	
  if(x > *rows || y > *columns)
    {
      throw DavidException(DString("These Coordinates (") + Double(x).toDString() + DString(",") + Double(y).toDString() + DString(") are out side of the plane which has dimensions ") + Double(*rows).toDString() + DString(" by ") + Double(*columns).toDString(),DavidException::PLANE_OUT_OF_BOUNDS_ERROR_CODE);
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

template <> bool Plane<utils::DStack<DString> >::write(const char * fileName, bool useGrid, bool blackOrWhite, char whiteSymbol)
{
  throw DavidException("Cannot write a DStack. Maybe later.",DavidException::DSTACK_ERROR_CODE);
}

template <> bool Plane<utils::DStack<Double> >::write(const char * fileName, bool useGrid, bool blackOrWhite, char whiteSymbol)
{
  throw DavidException("Cannot write a DStack. Maybe later.",DavidException::DSTACK_ERROR_CODE);
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

template <class T> void Plane<T>::draw(const DString& fileName, bool blackAndWhite, bool useGrid, int gridSpace,double bgRed, double bgGreen, double bgBlue) throw (DavidException)
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
					  image(i+1,j+1)->Red = currentValue;
					else
					  image(i+1,j+1)->Red = bgRed;
					break;
				      case 1:
					if(!isBackground)
					  image(i+1,j+1)->Green = currentValue;
					else
					  image(i+1,j+1)->Green = bgGreen;
					break;
				      case 2:
					if(!isBackground)
					  image(i+1,j+1)->Blue = currentValue;
					else
					  image(i+1,j+1)->Blue = bgBlue;
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

	if(!image.WriteToFile(fileName.toCharArray()))
	  throw DavidException(DString("Could not write ") + fileName,"EasyBMP Error",DavidException::IO_ERROR_CODE);
	

	std::cout << fileName+" has been written" << std::endl;
#endif	
	return;
}

template <> void Plane<utils::DStack<DString> >::draw(const DString& fileName, bool blackAndWhite, bool useGrid, int gridSpace, double bgRed, double bgGreen, double bgBlue) throw (DavidException)
{
  throw DavidException();
}

template <> void Plane<utils::DStack<Double> >::draw(const DString& fileName, bool blackAndWhite, bool useGrid, int gridSpace, double bgRed, double bgGreen, double bgBlue) throw (DavidException)
{
  throw DavidException();
}


template <class T> T Plane<T>::getMaxValue()
{
  T returnMe = getValue(0,0);
  for(int i = 0;i < *rows; i++)
    for(int j = 0;j < *columns;j++)
      if(getValue(i,j) > returnMe)
	returnMe = getValue(i,j);
  
  return returnMe;
}

template <> utils::DStack<DString> Plane<utils::DStack<DString> >::getMaxValue()
{
  throw DavidException("Maximum value makes no sense with a DStack",DavidException::DSTACK_ERROR_CODE);
}

template <> utils::DStack<Double> Plane<utils::DStack<Double> >::getMaxValue()
{
  throw DavidException("Maximum value makes no sense with a DStack",DavidException::DSTACK_ERROR_CODE);
}

template <class T> T Plane<T>::getMinValue()
{
  T returnMe = getValue(0,0);
  for(int i = 0;i < *rows; i++)
    for(int j = 0;j < *columns;j++)
      if(getValue(i,j) < returnMe)
	returnMe = getValue(i,j);
  
  return returnMe;
}

template <> utils::DStack<DString> Plane<utils::DStack<DString> >::getMinValue()
{
  throw DavidException("Maximum value makes no sense with a DStack",DavidException::DSTACK_ERROR_CODE);
}

template <> utils::DStack<Double> Plane<utils::DStack<Double> >::getMinValue()
{
  throw DavidException("Maximum value makes no sense with a DStack",DavidException::DSTACK_ERROR_CODE);
}


template <class T> bool Plane<T>::includeCurve(bool useCurve,double x, double y,double semimajor,double ellipticity, double Red, double Green, double Blue)
{
  return useCurve;

}

template <class T> T Plane<T>::getTotalValue()
{
	T returnMe;

	for(int i = 0;i< *rows -1;i++)
	  for(int j = 0;j<*columns -1;j++)
	    returnMe += this->getValue(i,j);

	return returnMe;

}


template <class T> bool Plane<T>::savePlane(const DString& rhs){return savePlane(rhs.toCharArray());}

template <class T> bool Plane<T>::savePlane(const char * fileName)
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
	  DString tempString;
	  for(int i = 0;i< *rows;i++)
	    for(int j = 0;j<*columns;j++)
		{
			outputT = getValue(i,j);
			tempString = outputT.toDString(stream);
			file << tempString << endl;
			stream.str("");
			stream.clear();
		}
	  return true;
	}
	else
	  return false;

}

template <> bool Plane<utils::DStack<DString> >::savePlane(const char * fileName)
{
  throw DavidException("DStacks cannot be written maybe later",DavidException::DSTACK_ERROR_CODE);
}

template <> bool Plane<utils::DStack<Double> >::savePlane(const char * fileName)
{
  throw DavidException("DStacks cannot be written maybe later",DavidException::DSTACK_ERROR_CODE);
}

template <class T>  Plane<T> * Plane<T>::readPlane(const char * fileName){DString bean(fileName); return Plane<T>::readPlane(bean);}

template <class T> Plane<T> * Plane<T>::readPlane(const DString& fileName)
{

	using namespace std;
	fstream file (fileName.toCharArray(), ios::in);
	if (file.is_open())
	{
		int tmpR,tmpC;
		char * curr = new char[150];
		DString string = "#";
		DString oldheader;
		
		while(string.charAt(0) == '#')
		  {
		    if(!string.equals("#")){oldheader += string+"\n";}
		    file.getline(curr,150);
		    
		    string = curr;
		  }
		tmpR = Double(string).toInt();

		string = "#";
		while(string.charAt(0) == '#')
		  {
		    if(!string.equals("#")){oldheader += string+"\n";}
		    file.getline(curr,150);
		    string = curr;
		  }
		tmpC = Double(string).toInt();

		T empty = 0.0;
		Plane<T> * newPlanePointer = new Plane<T>(tmpR,tmpC,empty);
		
		int counter = 0;
		int endCount = newPlanePointer->numberOfRows();
		int percentFinished = 0;
		VERBOSE_PRINT("Reading Plane:");
		char testChar;
		for(int i = 0;i< newPlanePointer->numberOfRows();i++)
			for(int j = 0;j< newPlanePointer->numberOfColumns();j++)
			{
			  counter++;
			  string = "#";
			  while(string.charAt(0) == '#')
			    {
			      if(!string.equals("#")){oldheader += string + "\n";}
			      file.getline(curr,150);
			      string = curr;
			    }
			  
			  T _t = T::parseDString(string);
			  newPlanePointer->setValue(i,j,_t);
			  
			  if((i*100/endCount) >= (percentFinished+5))
				{
					percentFinished = (int) i*100/endCount;
					VERBOSE_PRINT("Percent finished: ");
					VERBOSE_PRINT(percentFinished);
				}
			  
			}
		
		VERBOSE_PRINT("parsing complete");

		if(oldheader != "")
		  newPlanePointer->setHeader(oldheader.substring(0,oldheader.length()-1));

		if(counter != tmpR*tmpC)
		  throw DavidException("Not all or too few of the plane elements have been read.", DavidException::IO_ERROR_CODE);

		return newPlanePointer;

	}
	else
	  throw DavidException(DString("File Read Error:")+fileName);
}

template <> Plane<utils::DStack<DString> > * Plane<utils::DStack<DString> >::readPlane(const DString& fileName)
{
  throw DavidException("DStacks cannot be saved.", DavidException::DSTACK_ERROR_CODE);
}

template <> Plane<utils::DStack<Double> > * Plane<utils::DStack<Double> >::readPlane(const DString& fileName)
{
  throw DavidException("DStacks cannot be saved.", DavidException::DSTACK_ERROR_CODE);
}

template <class T> Plane<Double> * Plane<T>::bmpToPlane(const DString& fileName){return bmpToPlane(fileName.getString());}

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
      DString errorMessage = "Number of Columns don't match: 2 planes (";
      errorMessage += Double(left->numberOfRows()).toDString() + "x" + Double(left->numberOfColumns()).toDString();
      errorMessage += ") and (";
      errorMessage += Double(right->numberOfRows()).toDString() + "x" + Double(right->numberOfColumns()).toDString() + ")";
      throw DavidException(errorMessage);
    }

  if(left->numberOfRows() != right->numberOfRows())
    {
      DString errorMessage = "Number of Rows don't match: 2 planes (";
      errorMessage += Double(left->numberOfRows()).toDString() + "x" + Double(left->numberOfColumns()).toDString();
      errorMessage += ") and (";
      errorMessage += Double(right->numberOfRows()).toDString() + "x" + Double(right->numberOfColumns()).toDString() + ")";
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

template <> Plane<utils::DStack<DString> > * Plane<utils::DStack<DString> >::subtractPlanes(Plane<utils::DStack<DString> > * left, Plane<utils::DStack<DString> > * right)
{
  throw DavidException("DStacks cannont be subtracted from each other", DavidException::DSTACK_ERROR_CODE);
}

template <> Plane<utils::DStack<Double> > * Plane<utils::DStack<Double> >::subtractPlanes(Plane<utils::DStack<Double> > * left, Plane<utils::DStack<Double> > * right)
{
  throw DavidException("DStacks cannont be subtracted from each other", DavidException::DSTACK_ERROR_CODE);
}

template class Plane<Double>;
#include "libdnstd/Complex.h"
template class Plane<math::Complex>;
#include "libdnstd/DStack.h"
template class Plane<utils::DStack<Double> >;
#include "libdnstd/DHashMap.h"
