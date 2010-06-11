COMPILER = g++ -fpermissive -w -D__VERBOSE__ -D__DEBUG__ -static -g
#COMPILER = c++ -fpermissive -w -D__USE_BOINC__ -D__VERBOSE__ -static
#COMPILER = c++ -fpermissive -w -D__VERBOSE__ -static

INCLUDES = -I../libdnstd/

all: libmygl.a


libmygl.a: Cosmology.o densityprofile.o glellipse.o shearimage.o plane.o planecreator.o spl.o glshear.o
	ar rcs $@ $^ ./EasyBMP/EasyBMP.o

clean:
	rm -f *.o *.exe *.out *stackdump* libmygl.a


.cpp.o: %.h
	${COMPILER} ${INCLUDES} -c $< 
