#include "plane_inst.h"
//specializations

template <> Plane<utils::DStack<DString> > * Plane<utils::DStack<DString> >::subtractPlanes(Plane<utils::DStack<DString> > * left, Plane<utils::DStack<DString> > * right)
{
  throw DavidException("DStacks cannont be subtracted from each other", DavidException::DSTACK_ERROR_CODE);
}

template <> Plane<utils::DStack<Double> > * Plane<utils::DStack<Double> >::subtractPlanes(Plane<utils::DStack<Double> > * left, Plane<utils::DStack<Double> > * right)
{
  throw DavidException("DStacks cannont be subtracted from each other", DavidException::DSTACK_ERROR_CODE);
}

template <> bool Plane<utils::DStack<DString> >::write(const char * fileName, bool useGrid, bool blackOrWhite, char whiteSymbol)
{
  throw DavidException("Cannot write a DStack. Maybe later.",DavidException::DSTACK_ERROR_CODE);
}

template <> bool Plane<utils::DStack<Double> >::write(const char * fileName, bool useGrid, bool blackOrWhite, char whiteSymbol)
{
  throw DavidException("Cannot write a DStack. Maybe later.",DavidException::DSTACK_ERROR_CODE);
}

template <> void Plane<utils::DStack<DString> >::draw(const DString& fileName, bool blackAndWhite, bool useGrid, int gridSpace, double bgRed, double bgGreen, double bgBlue) throw (DavidException)
{
  throw DavidException();
}

template <> void Plane<utils::DStack<Double> >::draw(const DString& fileName, bool blackAndWhite, bool useGrid, int gridSpace, double bgRed, double bgGreen, double bgBlue) throw (DavidException)
{
  throw DavidException();
}

template <> utils::DStack<DString> Plane<utils::DStack<DString> >::getMinValue(){
  throw DavidException("Maximum value makes no sense with a DStack",DavidException::DSTACK_ERROR_CODE);
}

template <> utils::DStack<Double> Plane<utils::DStack<Double> >::getMinValue()
{
  throw DavidException("Maximum value makes no sense with a DStack",DavidException::DSTACK_ERROR_CODE);
}

template <> bool Plane<utils::DStack<DString> >::savePlane(const char * fileName)
{
  throw DavidException("DStacks cannot be written maybe later",DavidException::DSTACK_ERROR_CODE);
}

template <> bool Plane<utils::DStack<Double> >::savePlane(const char * fileName)
{
  throw DavidException("DStacks cannot be written maybe later",DavidException::DSTACK_ERROR_CODE);
}

template <> Plane<utils::DStack<DString> > * Plane<utils::DStack<DString> >::readPlane(const DString& fileName)
{
  throw DavidException("DStacks cannot be saved.", DavidException::DSTACK_ERROR_CODE);
}

template <> Plane<utils::DStack<Double> > * Plane<utils::DStack<Double> >::readPlane(const DString& fileName)
{
  throw DavidException("DStacks cannot be saved.", DavidException::DSTACK_ERROR_CODE);
}

template <> utils::DStack<DString> Plane<utils::DStack<DString> >::getMaxValue()
{
  throw DavidException("Maximum value makes no sense with a DStack",DavidException::DSTACK_ERROR_CODE);
}

template <> utils::DStack<Double> Plane<utils::DStack<Double> >::getMaxValue()
{
  throw DavidException("Maximum value makes no sense with a DStack",DavidException::DSTACK_ERROR_CODE);
}


//end specializations




