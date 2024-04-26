/// @file
/// @ingroup 	minexamples
/// @copyright	Copyright 2024 @goomtrex. All rights reserved.
/// @license    Use of this code is governed by the MIT License found in the `License.md` file.

//////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////

#include "c74_min.h"
using namespace c74::min;

#include <xtal/all.hh>
namespace  x_ = xtal;
namespace ax_ = xtal::algebra;
namespace bx_ = xtal::bond;
namespace ox_ = xtal::occur;
namespace px_ = xtal::process;
namespace qx_ = xtal::processor;
namespace rx_ = xtal::resource;
namespace sx_ = xtal::schedule;


XTAL_ENV_(push)
////////////////////////////////////////////////////////////////////////////////

class phi_2d1 : public object<phi_2d1>, public vector_operator<>//public mc_operator<>
{
	using re = xtal::bond::realize<number>;
	using T_alpha = typename re::alpha_t;
	using T_sigma = typename re::sigma_t;
	using T_delta = typename re::delta_t;

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
	
	using W_phi = sample[2];
	using X_phi = ax_::differential::modular_t<W_phi>;
	using Y_phi = px_::confined_t<px_::mop<x_::valve_f>, px_::differential::phasor<W_phi>>;
	using Z_phi = qx_::monomer_t<Y_phi, rx_::restore<x_::constant_t<0x1000>>>;
	Z_phi::bind_t<> z_phi{};
	ox_::sample_t<> z_sample{2*2*3*3*5*5*7*7};
	ox_::render_t<> z_render{1};
	ox_::resize_t<> z_resize{1};

//	XTAL_TYP_(bx_::pact_bind_f<2>(XTAL_ANY_(samples **), XTAL_ANY_(size_t))) foo;

	message<> dspsetup {this, "dspsetup", 
		[this] (const atoms &oo, const int i) noexcept -> atoms {
			size_t const sample_rate = oo[0];
			size_t const vector_size = oo[1];

			if (not z_resize.heading(vector_size)) z_phi <<= z_resize = ox_::resize_t<>{vector_size};
			if (not z_render.heading(vector_size))           z_render = ox_::render_t<>{vector_size};
			if (not z_sample.heading(sample_rate))           z_sample = ox_::sample_t<>{sample_rate};

			return {};
		}
	};
	void operator() (audio_bundle inner, audio_bundle outer) {
		if (2 == outer.channel_count()) {
			auto z_out = bx_::pact_bind_f<2>(outer.samples(), outer.frame_count());
			z_phi >>= z_render++ >> ox_::revise_f(z_out);
		}

	}
	outlet <> y0_{this, "(signal) phase", "signal"};
	outlet <> y1_{this, "(signal) phase", "signal"};


public:// FREQUENCY
	attribute<number> frequency
	{	this
	,	"frequency"
	,	441.0
	,	description{"Frequency in Hertz (Hz)"}
	,	setter{[this] (const atoms &oo, const int i) noexcept -> atoms {
			z_phi <<= ox_::indent_s<X_phi, 1>((sample) oo.front()/samplerate());
			return oo;
		}}
	};
	argument<number> frequency_arg
	{	this
	,	frequency.name()
	,	frequency.description_string().c_str()
	,	[this] (const atom &o) noexcept {
			frequency = o;
		}
	};
	message<> m_number
	{	this
	,	"number"
	,	frequency.description_string().c_str()
	,	[this] (const atoms &oo, const int i) noexcept -> atoms {
			frequency = oo;
			return {};
		}
	};
	inlet <> x0_{this, "(number) frequency"};
	
};
////////////////////////////////////////////////////////////////////////////////
XTAL_ENV_(pop)

MIN_EXTERNAL(phi_2d1);


///////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
