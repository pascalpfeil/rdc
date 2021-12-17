#include <gtest/gtest.h>

#include "rdc/rdc.h"

TEST(DataTypeTest, Double) {
  const std::vector<double> x_values{5.48813504e-01, 7.15189366e-01,
                                     6.02763376e-01, 5.44883183e-01};
  const std::vector<double> y_values{5.92880271e-01, 1.00636957e-02,
                                     4.75826196e-01, 7.08770391e-01};

  const double result = rdc::rdc(x_values, y_values);

  EXPECT_NEAR(result, 1, 0.00000001);
}

TEST(DataTypeTest, Int) {
  const std::vector<int> x_values{1, 7, 1, 2, 1, 7, 4, 3, 4, 5};
  const std::vector<int> y_values{1, 7, 1, 2, 1, 7, 4, 3, 4, 5};

  const double result = rdc::rdc(x_values, y_values);

  EXPECT_NEAR(result, 1, 0.000000000000001);
}

TEST(DataTypeTest, String) {
  const std::vector<std::string> x_values{"Hello", "World", "foo", "bar"};
  const std::vector<std::string> y_values{"Hello", "World", "fizz", "buzz"};

  const double result = rdc::rdc(x_values, y_values);

  EXPECT_NEAR(result, 1, 0.000000000000001);
}