#ifndef PLANE_H
#define PLANE_H 1

#include <vector>
#include "libdnstd/Complex.h"
#include "libdnstd/Double.h"
#include "libdnstd/DStack.h"

#include "plane_inst.h"

template class Plane<Double>;
template class Plane<math::Complex>;
#endif


