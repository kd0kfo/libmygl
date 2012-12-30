cmake_minimum_required(VERSION 2.8)

project(MYGL CXX)
set(MYGL_MAJOR_VERSION 2)
set(MYGL_MINOR_VERSION 3)
set(MYGL_REVISION 0)
set(AUTHOR_EMAIL david@davecoss.com)

option(DNSTD_DIR "Include libdnstd" OFF)
if(DNSTD_DIR)
   include_directories(${DNSTD_DIR}/include)
   link_directories(${DNSTD_DIR}/lib)
endif(DNSTD_DIR)

option(NETCDF_DIR "Include netcdf" OFF)
if(NETCDF_DIR)
   include_directories(${NETCDF_DIR}/include)
   link_directories(${NETCDF_DIR}/lib)
endif(NETCDF_DIR)

execute_process(COMMAND git rev-parse HEAD OUTPUT_FILE ${PROJECT_BINARY_DIR}/build_git_sha.out)
file(STRINGS ${PROJECT_BINARY_DIR}/build_git_sha.out BUILD_GIT_SHA)
execute_process(COMMAND date OUTPUT_FILE ${PROJECT_BINARY_DIR}/build_time.out)
file(STRINGS ${PROJECT_BINARY_DIR}/build_time.out BUILD_TIME)
configure_file("${PROJECT_SOURCE_DIR}/src/version.cpp.in" "${PROJECT_SOURCE_DIR}/src/version.cpp")

add_library(mygl ${PROJECT_SOURCE_DIR}/src/Cosmology.cpp ${PROJECT_SOURCE_DIR}/src/densityprofile.cpp ${PROJECT_SOURCE_DIR}/src/glellipse.cpp ${PROJECT_SOURCE_DIR}/src/shearimage.cpp ${PROJECT_SOURCE_DIR}/src/planecreator.cpp ${PROJECT_SOURCE_DIR}/src/EasyBMP/EasyBMP.cpp ${PROJECT_SOURCE_DIR}/src/plane_inst.cpp ${PROJECT_SOURCE_DIR}/src/version.cpp)
target_link_libraries(mygl dnstd)


install(TARGETS mygl DESTINATION lib)
FILE(GLOB headers "${PROJECT_SOURCE_DIR}/src/*.h")
FILE(GLOB easybmp_headers "${PROJECT_SOURCE_DIR}/src/EasyBMP/*.h")
install(FILES ${headers} DESTINATION include/libmygl)
install(FILES ${easybmp_headers} DESTINATION include/libmygl/EasyBMP)
