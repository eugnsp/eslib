#pragma once

namespace esu::au
{
// Mass
constexpr double gram = 1 / 9.10938e-28; // 1 gram (1 au = 1 me = 9.1e-28 g)
constexpr double kgram = 1e3 * gram;     // TODO : remove

// Length
constexpr double angstrom = 1 / .5291772; // 1 Angstrom (1 au = 1 Bohr = 0.529 A)
constexpr double metre = 1e10 * angstrom;
constexpr double cmetre = 1e-2 * metre; // TODO : remove
constexpr double nmetre = 1e-9 * metre; // TODO : remove

// Time
constexpr double second = 1 / 2.418884e-17; // 1 sec (1 au = 2.42... * 1e-17 sec)

// Energy
constexpr double rydberg = 1 / 2.;         // 1 Rydberg (1 Ha = 1 au = 2 Ry)
constexpr double evolt = 1 / 27.21139;     // 1 eV (1 Ha = 27.21 eV)
constexpr double kelvin = 1 / 3.157746e5;  // 1 Kelvin (1 Ha = 3.16e5 K)
constexpr double joule = 1 / 4.359744e-18; // 1 J (1 Ha = 4.36e-18 J)

// Potential and electric field
constexpr double volt = evolt;
constexpr double volt_over_cmetre = 1 / 5.142206e9; // 1 Volt/cm (1 Ha/Bohr = 5.14e9 V/cm)

// Charge
constexpr double coulomb = 1 / 1.602177e-19; // 1 Coulomb (1 proton = 1 au = 1.6e-19 C)
} // namespace esu::au
