/*
MIT License

Copyright (c) 2021 Pascal Pfeil

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef RDC_H_
#define RDC_H_

#include <Eigen/Dense>
#include <cmath>
#include <complex>
#include <functional>
#include <iterator>
#include <random>
#include <vector>

using Eigen::MatrixXd;
using Eigen::VectorXcd;
using Eigen::VectorXd;

namespace rdc {

namespace {

template <typename ForwardIterator>
VectorXd rankdata_ordinal(ForwardIterator begin, const size_t size) {
  typedef typename std::iterator_traits<ForwardIterator>::value_type T;

  std::vector<std::pair<T, size_t>> vec_sort;
  vec_sort.reserve(size);
  for (size_t i = 0; i < size; i++) {
    vec_sort.emplace_back(begin[i], i);
  }
  std::sort(vec_sort.begin(), vec_sort.end());

  VectorXd res(size);
  for (size_t i = 0; i < size; i++) {
    res[vec_sort[i].second] = i + 1 /*one based*/;
  }

  return res;
}

MatrixXd covariance(const MatrixXd& m) {
  const MatrixXd centered = m.rowwise() - m.colwise().mean();
  return (centered.transpose() * centered) / (m.rows() - 1);
}

MatrixXd hstack(const MatrixXd& m1, const MatrixXd& m2) {
  MatrixXd stacked(m1.rows(), m1.cols() + m2.cols());
  stacked << m1, m2;
  return stacked;
}

VectorXcd canonical_correlations(const MatrixXd& C, unsigned k) {
  // Due to numerical issues, if k is too large,
  // then rank(fX) < k or rank(fY) < k, so we need
  // to find the largest k such that the eigenvalues
  // (canonical correlations) are real-valued.
  const unsigned k0 = k;
  unsigned lb = 1, ub = k;
  while (true) {
    const MatrixXd Cxx = C.block(/*startRow=*/0, /*startCol=*/0,
                                 /*blockRows=*/k, /*blockCols=*/k);
    const MatrixXd Cyy = C.block(/*startRow=*/k0, /*startCol=*/k0,
                                 /*blockRows=*/k, /*blockCols=*/k);
    const MatrixXd Cxy = C.block(/*startRow=*/0, /*startCol=*/k0,
                                 /*blockRows=*/k, /*blockCols=*/k);
    const MatrixXd Cyx = C.block(/*startRow=*/k0, /*startCol=*/0,
                                 /*blockRows=*/k, /*blockCols=*/k);

    const VectorXcd eigs = (Cxx.completeOrthogonalDecomposition().solve(Cxy) *
                            Cyy.completeOrthogonalDecomposition().solve(Cyx))
                               .eigenvalues();

    // Binary search if k is too large
    if (!((eigs.imag().array() == 0).all() && 0 <= eigs.real().minCoeff() &&
          eigs.real().maxCoeff() <= 1)) {
      ub--;
      k = (ub + lb) / 2;
      continue;
    }
    if (lb == ub) {
      return eigs;
    }
    lb = k;
    if (ub == lb + 1) {
      k = ub;
    } else {
      k = (ub + lb) / 2;
    }
  }
}

}  // namespace

template <typename ForwardIterator>
double rdc(
    ForwardIterator x_begin, ForwardIterator y_begin, const size_t size,
    unsigned k = 20, const double s = 1. / 6,
    const std::function<double(double)>& f = [](const double d) {
      return std::sin(d);
    }) {

  /// Estimation of Copula-Transformations
  const VectorXd cx = rdc::rankdata_ordinal(x_begin, size) / size;
  const VectorXd cy = rdc::rankdata_ordinal(y_begin, size) / size;

  /// Generation of Random Non-Linear Projections
  // Add a vector of ones so that w.x + b is just a dot product
  const VectorXd O = VectorXd::Ones(size);
  MatrixXd X = rdc::hstack(cx, O);
  MatrixXd Y = rdc::hstack(cy, O);
  // Random linear projections
  std::mt19937 gen(42);
  std::normal_distribution<> standard_normal;
  auto lambda = [&standard_normal, &gen](int) { return standard_normal(gen); };
  const MatrixXd Rx =
      (s / X.cols()) * MatrixXd::NullaryExpr(X.cols(), k, lambda);
  const MatrixXd Ry =
      (s / Y.cols()) * MatrixXd::NullaryExpr(Y.cols(), k, lambda);
  X *= Rx;
  Y *= Ry;
  // Apply non-linear function to random projections
  const MatrixXd fX = X.unaryExpr(f);
  const MatrixXd fY = Y.unaryExpr(f);

  /// Compute canonical correlations
  // Compute full covariance matrix
  const MatrixXd C = rdc::covariance(rdc::hstack(fX, fY));
  // Compute canonical correlations
  const VectorXcd eigs = rdc::canonical_correlations(C, k);

  return std::sqrt(eigs.real().maxCoeff());
}

template <typename T>
double rdc(
    const std::vector<T>& x, const std::vector<T>& y, unsigned k = 20,
    const double s = 1. / 6,
    const std::function<double(double)>& f = [](const double d) {
      return std::sin(d);
    }) {
  assert(x.size() == y.size());

  return rdc(x.begin(), y.begin(), x.size(), k, s, f);
}

}  // namespace rdc

#endif  // RDC_H_
