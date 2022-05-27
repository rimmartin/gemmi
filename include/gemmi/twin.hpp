// Copyright 2022 Global Phasing Ltd.
//
// Twinning laws.

#ifndef GEMMI_TWIN_HPP_
#define GEMMI_TWIN_HPP_

#include <algorithm>     // for sort
#include <utility>       // for pair
#include "symmetry.hpp"  // for Op
#include "unitcell.hpp"  // for UnitCell
#include "cellred.hpp"   // for GruberVector

namespace gemmi {

namespace impl {

// Determination of the lattice symmetry is based on P.H. Zwart et al (2006)
// http://legacy.ccp4.ac.uk/newsletters/newsletter44/articles/explore_metric_symmetry.html
// which in turn is based on ideas from
// Le Page (1982) https://doi.org/10.1107/S0021889882011959
// Lebedev et al. (2006) https://doi.org/10.1107/S0907444905036759

struct TwoFoldData {
  std::int8_t matrix[9];
  std::int8_t ds_axis[3];  // 2-fold axis direction in direct space
  std::int8_t rs_axis[3];  // 2-fold axis direction in reciprocal space
};

// The template wrapper here is only to substitute C++17 inline variables
// https://stackoverflow.com/questions/38043442/how-do-inline-variables-work
template<class Dummy> struct TwoFold_ {
  static const TwoFoldData table[81];
};

// Two-fold twinning operators generated by cctbx/examples/reduced_cell_two_folds.py
// "Enumeration of the 81 2-fold symmetry operations possible for reduced cells."
template<class Dummy>
const TwoFoldData TwoFold_<Dummy>::table[81] = {
{{-1, -1, -1, 0, 0, 1, 0, 1, 0}, {-1, 1, 1}, {0, 1, 1}},
{{-1, -1, 0, 0, 1, 0, 0, -1, -1}, {1, -2, 1}, {0, 1, 0}},
{{-1, -1, 0, 0, 1, 0, 0, 0, -1}, {-1, 2, 0}, {0, 1, 0}},
{{-1, -1, 0, 0, 1, 0, 0, 1, -1}, {-1, 2, 1}, {0, 1, 0}},
{{-1, -1, 1, 0, 0, -1, 0, -1, 0}, {1, -1, 1}, {0, -1, 1}},
{{-1, 0, -1, 0, -1, -1, 0, 0, 1}, {-1, -1, 2}, {0, 0, 1}},
{{-1, 0, -1, 0, -1, 0, 0, 0, 1}, {-1, 0, 2}, {0, 0, 1}},
{{-1, 0, -1, 0, -1, 1, 0, 0, 1}, {-1, 1, 2}, {0, 0, 1}},
{{-1, 0, 0, -1, 0, -1, 1, -1, 0}, {0, -1, 1}, {1, -1, 1}},
{{-1, 0, 0, -1, 0, 1, -1, 1, 0}, {0, 1, 1}, {-1, 1, 1}},
{{-1, 0, 0, -1, 1, -1, 0, 0, -1}, {0, 1, 0}, {1, -2, 1}},
{{-1, 0, 0, -1, 1, 0, 0, 0, -1}, {0, 1, 0}, {-1, 2, 0}},
{{-1, 0, 0, -1, 1, 1, 0, 0, -1}, {0, 1, 0}, {-1, 2, 1}},
{{-1, 0, 0, 0, -1, -1, 0, 0, 1}, {0, -1, 2}, {0, 0, 1}},
{{-1, 0, 0, 0, -1, 0, -1, -1, 1}, {0, 0, 1}, {-1, -1, 2}},
{{-1, 0, 0, 0, -1, 0, -1, 0, 1}, {0, 0, 1}, {-1, 0, 2}},
{{-1, 0, 0, 0, -1, 0, -1, 1, 1}, {0, 0, 1}, {-1, 1, 2}},
{{-1, 0, 0, 0, -1, 0, 0, -1, 1}, {0, 0, 1}, {0, -1, 2}},
{{-1, 0, 0, 0, -1, 0, 0, 0, 1}, {0, 0, 1}, {0, 0, 1}},
{{-1, 0, 0, 0, -1, 0, 0, 1, 1}, {0, 0, 1}, {0, 1, 2}},
{{-1, 0, 0, 0, -1, 0, 1, -1, 1}, {0, 0, 1}, {1, -1, 2}},
{{-1, 0, 0, 0, -1, 0, 1, 0, 1}, {0, 0, 1}, {1, 0, 2}},
{{-1, 0, 0, 0, -1, 0, 1, 1, 1}, {0, 0, 1}, {1, 1, 2}},
{{-1, 0, 0, 0, -1, 1, 0, 0, 1}, {0, 1, 2}, {0, 0, 1}},
{{-1, 0, 0, 0, 0, -1, 0, -1, 0}, {0, -1, 1}, {0, -1, 1}},
{{-1, 0, 0, 0, 0, 1, 0, 1, 0}, {0, 1, 1}, {0, 1, 1}},
{{-1, 0, 0, 0, 1, -1, 0, 0, -1}, {0, 1, 0}, {0, -2, 1}},
{{-1, 0, 0, 0, 1, 0, 0, -1, -1}, {0, -2, 1}, {0, 1, 0}},
{{-1, 0, 0, 0, 1, 0, 0, 0, -1}, {0, 1, 0}, {0, 1, 0}},
{{-1, 0, 0, 0, 1, 0, 0, 1, -1}, {0, 2, 1}, {0, 1, 0}},
{{-1, 0, 0, 0, 1, 1, 0, 0, -1}, {0, 1, 0}, {0, 2, 1}},
{{-1, 0, 0, 1, 0, -1, -1, -1, 0}, {0, -1, 1}, {-1, -1, 1}},
{{-1, 0, 0, 1, 0, 1, 1, 1, 0}, {0, 1, 1}, {1, 1, 1}},
{{-1, 0, 0, 1, 1, -1, 0, 0, -1}, {0, 1, 0}, {-1, -2, 1}},
{{-1, 0, 0, 1, 1, 0, 0, 0, -1}, {0, 1, 0}, {1, 2, 0}},
{{-1, 0, 0, 1, 1, 1, 0, 0, -1}, {0, 1, 0}, {1, 2, 1}},
{{-1, 0, 1, 0, -1, -1, 0, 0, 1}, {1, -1, 2}, {0, 0, 1}},
{{-1, 0, 1, 0, -1, 0, 0, 0, 1}, {1, 0, 2}, {0, 0, 1}},
{{-1, 0, 1, 0, -1, 1, 0, 0, 1}, {1, 1, 2}, {0, 0, 1}},
{{-1, 1, -1, 0, 0, -1, 0, -1, 0}, {-1, -1, 1}, {0, -1, 1}},
{{-1, 1, 0, 0, 1, 0, 0, -1, -1}, {-1, -2, 1}, {0, 1, 0}},
{{-1, 1, 0, 0, 1, 0, 0, 0, -1}, {1, 2, 0}, {0, 1, 0}},
{{-1, 1, 0, 0, 1, 0, 0, 1, -1}, {1, 2, 1}, {0, 1, 0}},
{{-1, 1, 1, 0, 0, 1, 0, 1, 0}, {1, 1, 1}, {0, 1, 1}},
{{0, -1, -1, -1, 0, 1, 0, 0, -1}, {-1, 1, 0}, {-1, 1, 1}},
{{0, -1, -1, 0, -1, 0, -1, 1, 0}, {-1, 0, 1}, {-1, 1, 1}},
{{0, -1, 0, -1, 0, 0, -1, 1, -1}, {-1, 1, 1}, {-1, 1, 0}},
{{0, -1, 0, -1, 0, 0, 0, 0, -1}, {-1, 1, 0}, {-1, 1, 0}},
{{0, -1, 0, -1, 0, 0, 1, -1, -1}, {1, -1, 1}, {-1, 1, 0}},
{{0, -1, 1, -1, 0, -1, 0, 0, -1}, {-1, 1, 0}, {1, -1, 1}},
{{0, -1, 1, 0, -1, 0, 1, -1, 0}, {1, 0, 1}, {1, -1, 1}},
{{0, 0, -1, -1, -1, 1, -1, 0, 0}, {-1, 1, 1}, {-1, 0, 1}},
{{0, 0, -1, 0, -1, 0, -1, 0, 0}, {-1, 0, 1}, {-1, 0, 1}},
{{0, 0, -1, 1, -1, -1, -1, 0, 0}, {-1, -1, 1}, {-1, 0, 1}},
{{0, 0, 1, -1, -1, -1, 1, 0, 0}, {1, -1, 1}, {1, 0, 1}},
{{0, 0, 1, 0, -1, 0, 1, 0, 0}, {1, 0, 1}, {1, 0, 1}},
{{0, 0, 1, 1, -1, 1, 1, 0, 0}, {1, 1, 1}, {1, 0, 1}},
{{0, 1, -1, 0, -1, 0, -1, -1, 0}, {-1, 0, 1}, {-1, -1, 1}},
{{0, 1, -1, 1, 0, -1, 0, 0, -1}, {1, 1, 0}, {-1, -1, 1}},
{{0, 1, 0, 1, 0, 0, -1, -1, -1}, {-1, -1, 1}, {1, 1, 0}},
{{0, 1, 0, 1, 0, 0, 0, 0, -1}, {1, 1, 0}, {1, 1, 0}},
{{0, 1, 0, 1, 0, 0, 1, 1, -1}, {1, 1, 1}, {1, 1, 0}},
{{0, 1, 1, 0, -1, 0, 1, 1, 0}, {1, 0, 1}, {1, 1, 1}},
{{0, 1, 1, 1, 0, 1, 0, 0, -1}, {1, 1, 0}, {1, 1, 1}},
{{1, -1, -1, 0, -1, 0, 0, 0, -1}, {1, 0, 0}, {-2, 1, 1}},
{{1, -1, 0, 0, -1, 0, 0, 0, -1}, {1, 0, 0}, {-2, 1, 0}},
{{1, -1, 1, 0, -1, 0, 0, 0, -1}, {1, 0, 0}, {2, -1, 1}},
{{1, 0, -1, 0, -1, 0, 0, 0, -1}, {1, 0, 0}, {-2, 0, 1}},
{{1, 0, 0, -1, -1, 0, -1, 0, -1}, {-2, 1, 1}, {1, 0, 0}},
{{1, 0, 0, -1, -1, 0, 0, 0, -1}, {-2, 1, 0}, {1, 0, 0}},
{{1, 0, 0, -1, -1, 0, 1, 0, -1}, {2, -1, 1}, {1, 0, 0}},
{{1, 0, 0, 0, -1, 0, -1, 0, -1}, {-2, 0, 1}, {1, 0, 0}},
{{1, 0, 0, 0, -1, 0, 0, 0, -1}, {1, 0, 0}, {1, 0, 0}},
{{1, 0, 0, 0, -1, 0, 1, 0, -1}, {2, 0, 1}, {1, 0, 0}},
{{1, 0, 0, 1, -1, 0, -1, 0, -1}, {-2, -1, 1}, {1, 0, 0}},
{{1, 0, 0, 1, -1, 0, 0, 0, -1}, {2, 1, 0}, {1, 0, 0}},
{{1, 0, 0, 1, -1, 0, 1, 0, -1}, {2, 1, 1}, {1, 0, 0}},
{{1, 0, 1, 0, -1, 0, 0, 0, -1}, {1, 0, 0}, {2, 0, 1}},
{{1, 1, -1, 0, -1, 0, 0, 0, -1}, {1, 0, 0}, {-2, -1, 1}},
{{1, 1, 0, 0, -1, 0, 0, 0, -1}, {1, 0, 0}, {2, 1, 0}},
{{1, 1, 1, 0, -1, 0, 0, 0, -1}, {1, 0, 0}, {2, 1, 1}},
};

using TwoFold = TwoFold_<void>;

} // namespace impl

using OpObliquity = std::pair<Op, double>;

// Obliquity calculated here is the same as Le Page delta in cctbx.
// (tested against output of lebedev_2005_perturbation.py from cctbx)
inline double calculate_cos_obliquity(const UnitCell& reduced_cell,
                                      const Vec3& d_axis, const Vec3& r_axis) {
  // From the Le Page paper:
  //  tan(delta) = |t x tau| / |t . tau|  (i.e. delta is angle 0...90deg)
  //  t = L U   where U is direct-space axis
  //  tau = (L^T)^-1 h   where h is reciprocal-space axis
  //  L is orthogonalization matrix, although in a different order
  Vec3 t = reduced_cell.orth.mat.multiply(d_axis);
  Vec3 tau = reduced_cell.frac.mat.left_multiply(r_axis);
  // it's faster to calculate cos(delta) than tan(delta)
  return std::min(1.0, std::fabs(t.cos_angle(tau)));
}

// Reduced cell can be from GruberVector::get_cell() after Niggli reduction.
// max_obliq is max obliquity (delta) in degrees as defined in Le Page (1982).
inline std::vector<OpObliquity> find_lattice_2fold_ops(const UnitCell& reduced_cell,
                                                       double max_obliq) {
  std::vector<OpObliquity> ret;
  const double cos_max_obliq = std::cos(rad(max_obliq));
  for (const impl::TwoFoldData& row : impl::TwoFold::table) {
    Vec3 d_axis(row.ds_axis[0], row.ds_axis[1], row.ds_axis[2]);
    Vec3 r_axis(row.rs_axis[0], row.rs_axis[1], row.rs_axis[2]);
    double cos_delta = calculate_cos_obliquity(reduced_cell, d_axis, r_axis);
    if (cos_delta > cos_max_obliq) {
      constexpr int D = Op::DEN;
      Op op{{row.matrix[0] * D, row.matrix[1] * D, row.matrix[2] * D,
             row.matrix[3] * D, row.matrix[4] * D, row.matrix[5] * D,
             row.matrix[6] * D, row.matrix[7] * D, row.matrix[8] * D},
            {0, 0, 0}};
      ret.emplace_back(op, deg(std::acos(cos_delta)));
    }
  }
  std::sort(ret.begin(), ret.end(),
            [](const OpObliquity& a, const OpObliquity& b) { return a.second < b.second; });
  return ret;
}

// Reduced cell can be from GruberVector::get_cell() after Niggli reduction.
// max_obliq is max obliquity (delta) in degrees as defined in Le Page (1982).
// Returns lattice symmetry except inversion.
inline GroupOps find_lattice_symmetry_r(const UnitCell& reduced_cell, double max_obliq) {
  std::vector<OpObliquity> gen = find_lattice_2fold_ops(reduced_cell, max_obliq);
  std::vector<Op> genops;
  genops.reserve(gen.size());
  for (const OpObliquity& op_obl : gen)
    genops.push_back(op_obl.first);
  GroupOps go;
  go.sym_ops.push_back(Op::identity());
  go.cen_ops.push_back({0,0,0});
  // cf. GroupOps::add_missing_elements()
  if (!gen.empty())
    go.sym_ops.push_back(gen[0].first);
    // no need to try operator^2, we know it must be identity
  go.add_missing_elements_part2(genops, 24, true);
  return go;
}

// Returns lattice symmetry, but without inversion.
inline GroupOps find_lattice_symmetry(const UnitCell& cell, char centring,
                                      double max_obliq) {
  GruberVector gv(cell, centring, true);
  gv.niggli_reduce();
  UnitCell reduced = gv.get_cell();
  GroupOps gops = find_lattice_symmetry_r(reduced, max_obliq);
  gops.change_basis_forward(*gv.change_of_basis);
  return gops;
}

// Determine potential twinning operators.
// Returns all operators or only unique ones (coset representatives).
inline std::vector<Op> find_potential_twinning(const UnitCell& cell,
                                               const SpaceGroup* sg,
                                               double max_obliq,
                                               bool all_ops) {
  if (sg == nullptr)
    sg = &get_spacegroup_p1();
  GroupOps go = sg->operations();
  GroupOps lat_go = find_lattice_symmetry(cell, sg->centring_type(), max_obliq);
  if (!go.has_same_centring(lat_go))
    fail("find_potential_twinning(): internal error");
  std::vector<Op> ops;
  size_t sg_symop_count = go.sym_ops.size();
  for (const Op& op : lat_go.sym_ops)
    if (!go.find_by_rotation(op.rot) &&
        !go.find_by_rotation(op.negated_rot())) {
      ops.push_back(op);
      if (!all_ops)
        for (size_t i = 1; i < sg_symop_count; ++i)
          go.sym_ops.push_back(op * go.sym_ops[i]);
    }
  return ops;
}

} // namespace gemmi
#endif
