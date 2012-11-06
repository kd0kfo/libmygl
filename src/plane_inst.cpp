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
#include "plane_inst.h"
#include "libdnstd/Complex.h"
//specializations

template<> bool Plane<Double>::writeCDF(const char* filename, bool verbose)
{
  int retval, ncid, x_dimid, y_dimid, dimids[2], realid, imagid, zid;
  double *temparray = NULL;

  if(filename == NULL)
    return false;
  
  temparray = (double*)malloc(sizeof(double)*(rows)*(columns));
  if(temparray == NULL)
    throw DavidException("Could not allocate memory for writeCDF");
  
  if((retval = nc_create(filename, NC_CLOBBER, &ncid)))
    {
      free(temparray);
      std::string error = "Error creating NetCDF file in writeCDF.\n";
      error += nc_strerror(retval);
      throw DavidException(error);
    }
  
  /* Define the dimensions. NetCDF will hand back an ID for each. */
   if ((retval = nc_def_dim(ncid, PLANE_XDIM_NAME, rows, &x_dimid)))
     {
       free(temparray);
       std::string error = "Error defining x dimension for NetCDF file in writeCDF.\n";
       error += nc_strerror(retval);
       throw DavidException(error);
     }
   if ((retval = nc_def_dim(ncid, PLANE_YDIM_NAME, this->columns, &y_dimid)))
     {
       free(temparray);
       std::string error = "Error defining y dimension for NetCDF file in writeCDF.\n";
       error += nc_strerror(retval);
       throw DavidException(error);
     }

   /* The dimids array is used to pass the IDs of the dimensions of
    * the variable. */
   dimids[0] = x_dimid;
   dimids[1] = y_dimid;

   if ((retval = nc_def_var(ncid, PLANE_REALDATA_NAME, NC_DOUBLE, 2, dimids, &realid)))
     {
       free(temparray);
       std::string error = "Defining real data for NetCDF file in writeCDF.\n";
       error += nc_strerror(retval);
       throw DavidException(error);
     }

   if ((retval = nc_def_var(ncid, PLANE_IMAGDATA_NAME, NC_DOUBLE, 2, dimids, &imagid)))
     {
       free(temparray);
       std::string error = "Defining imaginary data for NetCDF file in writeCDF.\n";
       error += nc_strerror(retval);
       throw DavidException(error);
     }

      if ((retval = nc_def_var(ncid, PLANE_ZDATA_NAME, NC_DOUBLE, 2, dimids, &zid)))
     {
       free(temparray);
       std::string error = "Defining data for NetCDF file in writeCDF.\n";
       error += nc_strerror(retval);
       throw DavidException(error);
     }



   /* End define mode. This tells netCDF we are done defining
    * metadata. */
   if ((retval = nc_enddef(ncid)))
     {
       free(temparray);
       std::string error = "Error ending define mode for NetCDF file in writeCDF.\n";
       error += nc_strerror(retval);
       throw DavidException(error);
     }

   Double tmp;
   for(int i = 0, size = (rows)*(columns);i< size;i++)
     {
       tmp = this->planeArray[i];
       temparray[i] = tmp.getValue(0);
     }

   /* Write the pretend data to the file. Although netCDF supports
    * reading and writing subsets of data, in this case we write all
    * the data in one operation. */
   if ((retval = nc_put_var_double(ncid, realid, temparray)))
     {
       free(temparray);
       std::string error = "Error Writing NetCDF file in writeCDF.\n";
       error += nc_strerror(retval);
       throw DavidException(error);
     }

   for(int i = 0, size = (rows)*(columns);i< size;i++)
     {
       tmp = this->planeArray[i];
       temparray[i] = tmp.getValue(1);
     }
   
   if ((retval = nc_put_var_double(ncid, imagid, temparray)))
     {
       free(temparray);
       std::string error = "Error Writing NetCDF file in writeCDF.\n";
       error += nc_strerror(retval);
       throw DavidException(error);
     }
   
   for(int i = 0, size = (rows)*(columns);i< size;i++)
     {
       tmp = this->planeArray[i];
       temparray[i] = tmp.getValue(2);
     }
   if ((retval = nc_put_var_double(ncid, zid, temparray)))
     {
       free(temparray);
       std::string error = "Error Writing NetCDF file in writeCDF.\n";
       error += nc_strerror(retval);
       throw DavidException(error);
     }
   

   /* Close the file. This frees up any internal netCDF resources
    * associated with the file, and flushes any buffers. */
   if ((retval = nc_close(ncid)))
     {
       free(temparray);
       std::string error = "Error closing NetCDF file in writeCDF.\n";
       error += nc_strerror(retval);
       throw DavidException(error);
     }

   printf("test2!!!\n");

   return true;
  
}

template<> bool Plane<math::Complex>::writeCDF(const char* filename, bool verbose)
{
  int retval, ncid, x_dimid, y_dimid, dimids[2], realid, imagid;
  double *temparray = NULL;

  if(filename == NULL)
    return false;
  
  temparray = (double*)malloc(sizeof(double)*(rows)*(columns));
  if(temparray == NULL)
    throw DavidException("Could not allocate memory for writeCDF");
  
  if((retval = nc_create(filename, NC_CLOBBER, &ncid)))
    {
      free(temparray);
      std::string error = "Error creating NetCDF file in writeCDF.\n";
      error += nc_strerror(retval);
      throw DavidException(error);
    }
  
  /* Define the dimensions. NetCDF will hand back an ID for each. */
   if ((retval = nc_def_dim(ncid, PLANE_XDIM_NAME, rows, &x_dimid)))
     {
       free(temparray);
       std::string error = "Error defining x dimension for NetCDF file in writeCDF.\n";
       error += nc_strerror(retval);
       throw DavidException(error);
     }
   if ((retval = nc_def_dim(ncid, PLANE_YDIM_NAME, this->columns, &y_dimid)))
     {
       free(temparray);
       std::string error = "Error defining y dimension for NetCDF file in writeCDF.\n";
       error += nc_strerror(retval);
       throw DavidException(error);
     }

   /* The dimids array is used to pass the IDs of the dimensions of
    * the variable. */
   dimids[0] = x_dimid;
   dimids[1] = y_dimid;

   if ((retval = nc_def_var(ncid, PLANE_REALDATA_NAME, NC_DOUBLE, 2, dimids, &realid)))
     {
       free(temparray);
       std::string error = "Error defining real data for NetCDF file in writeCDF.\n";
       error += nc_strerror(retval);
       throw DavidException(error);
     }

   if ((retval = nc_def_var(ncid, PLANE_IMAGDATA_NAME, NC_DOUBLE, 2, dimids, &imagid)))
     {
       free(temparray);
       std::string error = "Error defining imaginary data for NetCDF file in writeCDF.\n";
       error += nc_strerror(retval);
       throw DavidException(error);
     }


   /* End define mode. This tells netCDF we are done defining
    * metadata. */
   if ((retval = nc_enddef(ncid)))
     {
       free(temparray);
       std::string error = "Error ending define mode for NetCDF file in writeCDF.\n";
       error += nc_strerror(retval);
       throw DavidException(error);
     }

   math::Complex tmp;
   for(int i = 0, size = (rows)*(columns);i< size;i++)
     {
       tmp = this->planeArray[i];
       temparray[i] = tmp.getRealPart();
     }

   /* Write the pretend data to the file. Although netCDF supports
    * reading and writing subsets of data, in this case we write all
    * the data in one operation. */
   if ((retval = nc_put_var_double(ncid, realid, temparray)))
     {
       free(temparray);
       std::string error = "Error Writing NetCDF file in writeCDF.\n";
       error += nc_strerror(retval);
       throw DavidException(error);
     }

   for(int i =0, size = rows*columns;i<size;i++)
     {
       tmp = this->planeArray[i];
       temparray[i] = tmp.getImaginaryPart();
     }
   if ((retval = nc_put_var_double(ncid, imagid, temparray)))
     {
       free(temparray);
       std::string error = "Error Writing NetCDF file in writeCDF.\n";
       error += nc_strerror(retval);
       throw DavidException(error);
     }

   /* Close the file. This frees up any internal netCDF resources
    * associated with the file, and flushes any buffers. */
   if ((retval = nc_close(ncid)))
     {
       free(temparray);
       std::string error = "Error closing NetCDF file in writeCDF.\n";
       error += nc_strerror(retval);
       throw DavidException(error);
     }

   printf("test2!!!\n");

   return true;
  
}


//end specializations



