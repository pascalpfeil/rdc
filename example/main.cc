
#include <cmath>
#include <iostream>
#include <vector>

#include "rdc/rdc.h"

int main() {
  const std::vector<double> x_values{1, 7, 1, 2, 1, 7, 4, 3, 4, 5};
  const std::vector<double> y_values{1, 7, 1, 2, 1, 7, 4, 3, 4, 5};

  std::cout << "rdc: " << rdc::rdc(x_values, y_values) << std::endl;
}