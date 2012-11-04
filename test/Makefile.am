bin_PROGRAMS = test_planes

test_planes_SOURCES = test_planes.cpp 
test_planes_CPPFLAGS = -I../src $(DNSTD_CPPFLAGS) $(NETCDF_CPPFLAGS)
test_planes_LDFLAGS = -L../src $(DNSTD_LDFLAGS) $(NETCDF_LDFLAGS)
test_planes_LDADD = -lmygl $(DNSTD_LDADD) $(NETCDF_LDADD)

test: test_planes
	./test_planes