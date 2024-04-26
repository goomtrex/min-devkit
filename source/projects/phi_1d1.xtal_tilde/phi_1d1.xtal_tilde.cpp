/// @file
/// @ingroup 	minexamples
/// @copyright	Copyright 2024 @goomtrex. All rights reserved.
/// @license    Use of this code is governed by the MIT License found in the `License.md` file.

//////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////

#include "c74_min.h"
using namespace c74::min;

#include <xtal/all.hh>
namespace x_a = xtal::algebra;
namespace x_o = xtal::occur;
namespace x_y = xtal::process;
namespace x_z = xtal::processor;


XTAL_ENV_(push)
////////////////////////////////////////////////////////////////////////////////

class phi_1d1 : public object<phi_1d1>, public sample_operator<0, 1>
{
public:// METADATA
	MIN_TAGS {"audio, oscillator"};
	MIN_AUTHOR {"Synthetic Methods"};
	MIN_RELATED {"phasor~, saw~"};
	MIN_DESCRIPTION {
		"A non-bandlimited <a href='https://en.wikipedia.org/wiki/Sawtooth_wave'>sawtooth wave</a> "
		"representing normalized <a href='https://en.wikipedia.org/wiki/Phase_(waves)'>phase</a> "
		"in the neighborhood &plusmn;&frac12; centered around zero."
	};

public:// PHASE
	//\note\
	State must be defined without explicit initialization, \
	and before any associated `attribute`s. \
	
	using Y_phi = xtal::process::differential:: phasor_t<sample[2]>;
	using X_phi = xtal::algebra::differential::modular_t<sample[2]>;
	//\
	Y_phi m_phi;
	X_phi m_phi;

	XTAL_OP2_(sample) () ()
	XTAL_0EX
	{
		//\
		return (m_phi()) (0);// `Y_phi`
		return (++m_phi) (0);// `X_phi`
	}
	outlet <> phase_outlet_{this, "(signal) phase", "signal"};

public:// FREQUENCY
	attribute<number> frequency
	{	this
	,	"frequency"
	,	441.0
	,	description{"Frequency in Hertz (Hz)"}
	,	setter{[this] (const atoms &oo, const int i) noexcept {
			assert(0 < oo.size());
			//\
			m_phi <<= xtal::occur::indent_s<X_phi, 1>((sample) oo.front()/samplerate());// `Y_phi`
			m_phi <<= {(sample) oo.front()/samplerate()};// `X_phi`
			return oo;
		}}
	};
	argument<number> frequency_arg
	{	this
	,	frequency.name()
	,	frequency.description_string().c_str()
	,	[this] (const atom &o) noexcept {frequency = o;}
	};
	message<> m_number
	{	this
	,	"number"
	,	frequency.description_string().c_str()
	,	[this] (const atoms &oo, const int i) noexcept {frequency = oo; return atoms{};}
	};
	inlet <> frequency_inlet_{this, "(number) frequency"};
	
};
////////////////////////////////////////////////////////////////////////////////
XTAL_ENV_(pop)


MIN_EXTERNAL(phi_1d1);

///////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
