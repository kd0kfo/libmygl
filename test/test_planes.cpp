#include "plane.h"

#include <cstdio>

#include <netcdf.h>


void write_plane()
{
  math::Complex init(12,34);
  Plane<math::Complex> complex_plane(5,5,init);
  complex_plane.savePlane("test.plane");
  printf("Saved plane.\n");

}

void read_plane()
{
  Plane<math::Complex> empty_plane;
  empty_plane.readPlane("test.plane");
  printf("Read plane.\n");

}

void write_cdf()
{
  math::Complex init(12,34);
  Plane<math::Complex> complex_plane(5,5,init);
  
  complex_plane.writeCDF("test.nc");
  printf("Write NetCDF\n");
}

void load_cdf()
{
  Plane<math::Complex> empty_plane;
  empty_plane.loadCDF("test.nc");
  printf("Loaded NetCDF\n");
}

int main(int argc, char **argv)
{
  write_plane();
  read_plane();
  write_cdf();
  load_cdf();
  return 0;
  
}
