// ====================================================================
// This file is part of FlexibleSUSY.
//
// FlexibleSUSY is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published
// by the Free Software Foundation, either version 3 of the License,
// or (at your option) any later version.
//
// FlexibleSUSY is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with FlexibleSUSY.  If not, see
// <http://www.gnu.org/licenses/>.
// ====================================================================

// File generated at Sat 27 Aug 2016 12:40:25

#ifndef NSM_INFO_H
#define NSM_INFO_H

#include <iosfwd>

namespace flexiblesusy {

namespace NSM_info {
   enum Particles : unsigned {VG, Hp, Fv, Ah, Fd, Fu, Fe, hh, VWp, VP, VZ,
      NUMBER_OF_PARTICLES};

   enum Masses : unsigned {MVG, MHp, MFv_1, MFv_2, MFv_3, MAh, MFd_1, MFd_2,
      MFd_3, MFu_1, MFu_2, MFu_3, MFe_1, MFe_2, MFe_3, Mhh_1, Mhh_2, MVWp, MVP,
      MVZ, NUMBER_OF_MASSES};

   enum Parameters : unsigned {g1, g2, g3, Lambda2, Lambda3, Lambda1, Yu0_0,
      Yu0_1, Yu0_2, Yu1_0, Yu1_1, Yu1_2, Yu2_0, Yu2_1, Yu2_2, Yd0_0, Yd0_1, Yd0_2,
      Yd1_0, Yd1_1, Yd1_2, Yd2_0, Yd2_1, Yd2_2, Ye0_0, Ye0_1, Ye0_2, Ye1_0, Ye1_1
      , Ye1_2, Ye2_0, Ye2_1, Ye2_2, Lambda5, Lambda4, mS2, mH2, vH, vS,
      NUMBER_OF_PARAMETERS};

   enum Mixings : unsigned {ReVd0_0, ImVd0_0, ReVd0_1, ImVd0_1, ReVd0_2,
      ImVd0_2, ReVd1_0, ImVd1_0, ReVd1_1, ImVd1_1, ReVd1_2, ImVd1_2, ReVd2_0,
      ImVd2_0, ReVd2_1, ImVd2_1, ReVd2_2, ImVd2_2, ReUd0_0, ImUd0_0, ReUd0_1,
      ImUd0_1, ReUd0_2, ImUd0_2, ReUd1_0, ImUd1_0, ReUd1_1, ImUd1_1, ReUd1_2,
      ImUd1_2, ReUd2_0, ImUd2_0, ReUd2_1, ImUd2_1, ReUd2_2, ImUd2_2, ReVu0_0,
      ImVu0_0, ReVu0_1, ImVu0_1, ReVu0_2, ImVu0_2, ReVu1_0, ImVu1_0, ReVu1_1,
      ImVu1_1, ReVu1_2, ImVu1_2, ReVu2_0, ImVu2_0, ReVu2_1, ImVu2_1, ReVu2_2,
      ImVu2_2, ReUu0_0, ImUu0_0, ReUu0_1, ImUu0_1, ReUu0_2, ImUu0_2, ReUu1_0,
      ImUu1_0, ReUu1_1, ImUu1_1, ReUu1_2, ImUu1_2, ReUu2_0, ImUu2_0, ReUu2_1,
      ImUu2_1, ReUu2_2, ImUu2_2, ReVe0_0, ImVe0_0, ReVe0_1, ImVe0_1, ReVe0_2,
      ImVe0_2, ReVe1_0, ImVe1_0, ReVe1_1, ImVe1_1, ReVe1_2, ImVe1_2, ReVe2_0,
      ImVe2_0, ReVe2_1, ImVe2_1, ReVe2_2, ImVe2_2, ReUe0_0, ImUe0_0, ReUe0_1,
      ImUe0_1, ReUe0_2, ImUe0_2, ReUe1_0, ImUe1_0, ReUe1_1, ImUe1_1, ReUe1_2,
      ImUe1_2, ReUe2_0, ImUe2_0, ReUe2_1, ImUe2_1, ReUe2_2, ImUe2_2, ZH0_0, ZH0_1,
      ZH1_0, ZH1_1, ZZ0_0, ZZ0_1, ZZ1_0, ZZ1_1, NUMBER_OF_MIXINGS};

   enum Input_parameters : unsigned {Lambda1Input, Lambda2Input, Lambda3Input,
      Lambda4Input, Lambda5Input, vSInput, Qin, NUMBER_OF_INPUT_PARAMETERS};

   extern const double normalization_g1;
   extern const double normalization_g2;
   extern const double normalization_g3;

   extern const unsigned particle_multiplicities[NUMBER_OF_PARTICLES];
   extern const char* particle_names[NUMBER_OF_PARTICLES];
   extern const char* particle_latex_names[NUMBER_OF_PARTICLES];
   extern const char* parameter_names[NUMBER_OF_PARAMETERS];
   extern const char* particle_mixing_names[NUMBER_OF_MIXINGS];
   extern const char* input_parameter_names[NUMBER_OF_INPUT_PARAMETERS];
   extern const char* model_name;
   extern const bool is_low_energy_model;
   extern const bool is_supersymmetric_model;

   void print(std::ostream&);
}

} // namespace flexiblesusy

#endif
