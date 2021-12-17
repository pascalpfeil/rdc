# RDC: Randomized Dependence Coefficient

The RDC is a measure of nonlinear dependence between two (possibly multidimensional) variables.

*Algorithm by:* David Lopez-Paz, Philipp Hennig, and Bernhard Schoelkopf ([2013 paper](https://papers.nips.cc/paper/5138-the-randomized-dependence-coefficient.pdf)).

*Code by:* Pascal Pfeil, inspired by Gary Doran's [python rdc implementation](https://github.com/garydoranjr/rdc).

## Usage

``` cpp
#include "rdc/rdc.h"

// ...

const std::vector<double> x_values{/* ... */};
const std::vector<double> y_values{/* ... */};

rdc::rdc(x_values, y_values) 
```

`x` and `y` are currently only supported to be 1-D vectors of any type that supports sorting with `std::sort`.
They have to be of the same size.

There are additional keyword parameters for `rdc` that correspond to parameters described in the paper.

### CMake

You can include `rdc` in your own CMake based project like this:

``` cmake
include(FetchContent)
set(RDC_BUILD_EXAMPLE OFF)
set(RDC_BUILD_TESTS OFF)
set(RDC_BUILD_BENCHMARKS OFF)
FetchContent_Declare(
    rdc
    GIT_REPOSITORY "https://github.com/pascalpfeil/rdc.git"
    GIT_TAG main
)
FetchContent_MakeAvailable(rdc)
FetchContent_GetProperties(rdc SOURCE_DIR RDC_INCLUDE_DIR)
include_directories(${RDC_INCLUDE_DIR})

target_link_libraries(your_target rdc)
```

### Header

The `rdc/rdc.h` header has just one dependency, [Eigen](https://eigen.tuxfamily.org/).
If you don't use CMake, you can also provide Eigen yourself and copy the header into your project. 
