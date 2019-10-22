#pragma once

namespace esc::au
{
// Mass
constexpr double gram = 1 / 9.10938e-28; // 1 gram (1 au = 1 me = 9.1e-28 g)
constexpr double kgram = 1e3 * gram;	 // TODO : remove

// Length
constexpr double angstrom = 1 / .5291772; // 1 Angstrom (1 au = 1 Bohr = 0.529 A)
constexpr double metre = 1e10 * angstrom;
constexpr double cmetre = 1e-2 * metre; // TODO : remove
constexpr double nmetre = 1e-9 * metre; // TODO : remove

// Time
constexpr double second = 1 / 2.418884e-17; // 1 sec (1 au = 2.42... * 1e-17 sec)

// Energy
constexpr double rydberg = 1 / 2.;		   // 1 Rydberg (1 Ha = 1 au = 2 Ry)
constexpr double evolt = 1 / 27.21139;	   // 1 eV (1 Ha = 27.21 eV)
constexpr double kelvin = 1 / 3.157746e5;  // 1 Kelvin (1 Ha = 3.16e5 K)
constexpr double joule = 1 / 4.359744e-18; // 1 J (1 Ha = 4.36e-18 J)

// Potential and electric field
constexpr double volt = evolt;
constexpr double volt_over_cmetre = 1 / 5.142206e9; // 1 Volt/cm (1 Ha/Bohr = 5.14e9 V/cm)

// Charge
constexpr double coulomb = 1 / 1.602177e-19; // 1 Coulomb (1 proton = 1 au = 1.6e-19 C)

#define ESC_DEFINE_AU_UNIT(Name, factor)                                                           \
	inline constexpr double from_##Name(double x)                                                  \
	{                                                                                              \
		constexpr auto fac = (factor);                                                             \
		return fac * x;                                                                            \
	}                                                                                              \
                                                                                                   \
	inline constexpr double to_##Name(double x)                                                    \
	{                                                                                              \
		constexpr auto fac = 1. / (factor);                                                        \
		return fac * x;                                                                            \
	}                                                                                              \
                                                                                                   \
	inline namespace literals                                                                      \
	{                                                                                              \
	inline constexpr double operator"" _##Name(long double x)                                      \
	{                                                                                              \
		return from_##Name(static_cast<double>(x));                                                \
	}                                                                                              \
                                                                                                   \
	inline constexpr double operator"" _##Name(unsigned long long x)                               \
	{                                                                                              \
		return from_##Name(static_cast<double>(x));                                                \
	}                                                                                              \
	} // namespace literals

// Units of length

ESC_DEFINE_AU_UNIT(m, metre)
ESC_DEFINE_AU_UNIT(cm, cmetre)
ESC_DEFINE_AU_UNIT(nm, nmetre)
ESC_DEFINE_AU_UNIT(ang, angstrom)

// Units of inversed length

ESC_DEFINE_AU_UNIT(per_m, 1 / metre)
ESC_DEFINE_AU_UNIT(per_cm, 1 / cmetre)
ESC_DEFINE_AU_UNIT(per_nm, 1 / nmetre)
ESC_DEFINE_AU_UNIT(per_ang, 1 / angstrom)

// Units of time

ESC_DEFINE_AU_UNIT(sec, second)
ESC_DEFINE_AU_UNIT(msec, 1e-3 * second)
ESC_DEFINE_AU_UNIT(usec, 1e-6 * second)
ESC_DEFINE_AU_UNIT(nsec, 1e-9 * second)
ESC_DEFINE_AU_UNIT(psec, 1e-12 * second)

// Units of velocity

ESC_DEFINE_AU_UNIT(m_per_sec, metre / second)
ESC_DEFINE_AU_UNIT(cm_per_sec, cmetre / second)

// Units of concentration

ESC_DEFINE_AU_UNIT(per_cm2, 1 / (cmetre * cmetre))
ESC_DEFINE_AU_UNIT(per_cm3, 1 / (cmetre * cmetre * cmetre))

// Units of mass density

ESC_DEFINE_AU_UNIT(g_per_cm2, gram / (cmetre * cmetre))
ESC_DEFINE_AU_UNIT(ng_per_cm2, 1e-9 * gram / (cmetre * cmetre))

// Units of energy

ESC_DEFINE_AU_UNIT(ry, rydberg)
ESC_DEFINE_AU_UNIT(evolt, evolt)
ESC_DEFINE_AU_UNIT(mevolt, 1e-3 * evolt)
ESC_DEFINE_AU_UNIT(kelvin, kelvin)
ESC_DEFINE_AU_UNIT(joule, joule)

// Units of power

ESC_DEFINE_AU_UNIT(watt, joule / second)

// Units of potential

ESC_DEFINE_AU_UNIT(volt, volt)

// Units of current

ESC_DEFINE_AU_UNIT(amp, coulomb / second)
ESC_DEFINE_AU_UNIT(mamp, 1e-3 * coulomb / second)

// Units of electric field

ESC_DEFINE_AU_UNIT(volt_per_m, volt / metre)
ESC_DEFINE_AU_UNIT(volt_per_cm, volt / cmetre)
ESC_DEFINE_AU_UNIT(kvolt_per_m, 1e3 * volt / metre)
ESC_DEFINE_AU_UNIT(kvolt_per_cm, 1e3 * volt / cmetre)

// Units of mobility

ESC_DEFINE_AU_UNIT(cm2_per_volt_sec, cmetre* cmetre / (evolt * second))

// Unit of resistance

ESC_DEFINE_AU_UNIT(ohm, volt* second / coulomb)
ESC_DEFINE_AU_UNIT(mohm, 1e-3 * volt * second / coulomb)
ESC_DEFINE_AU_UNIT(kohm, 1e3 * volt * second / coulomb)
} // namespace esu::au
