/* -*- c++ -*- */
/*
 * Copyright 2006,2010-2012 Free Software Foundation, Inc.
 *
 * This file is part of GNU Radio
 *
 * GNU Radio is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * GNU Radio is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with GNU Radio; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "costas_loop_cc_impl.h"
#include <gnuradio/expj.h>
#include <gnuradio/io_signature.h>
#include <gnuradio/math.h>
#include <gnuradio/sincos.h>
#include <boost/format.hpp>

namespace gr {
namespace digital {

costas_loop_cc::sptr costas_loop_cc::make(float loop_bw, unsigned int order, bool use_snr)
{
    return gnuradio::get_initial_sptr(new costas_loop_cc_impl(loop_bw, order, use_snr));
}

static int ios[] = { sizeof(gr_complex), sizeof(float), sizeof(float), sizeof(float) };
static std::vector<int> iosig(ios, ios + sizeof(ios) / sizeof(int));

costas_loop_cc_impl::costas_loop_cc_impl(float loop_bw, unsigned int order, bool use_snr)
    : sync_block("costas_loop_cc",
                 io_signature::make(1, 1, sizeof(gr_complex)),
                 io_signature::makev(1, 4, iosig)),
      blocks::control_loop(loop_bw, 1.0, -1.0),
      d_order(order),
      d_error(0),
      d_noise(1.0),
      d_use_snr(use_snr)
{
    message_port_register_in(pmt::mp("noise"));
    set_msg_handler(pmt::mp("noise"),
                    boost::bind(&costas_loop_cc_impl::handle_set_noise, this, _1));
}

costas_loop_cc_impl::~costas_loop_cc_impl() {}

void costas_loop_cc_impl::handle_set_noise(pmt::pmt_t msg)
{
    if (pmt::is_real(msg)) {
        d_noise = pmt::to_double(msg);
        d_noise = powf(10.0f, d_noise / 10.0f);
    }
}

int costas_loop_cc_impl::work(int noutput_items,
                              gr_vector_const_void_star& input_items,
                              gr_vector_void_star& output_items)
{
    const gr_complex* iptr = (gr_complex*)input_items[0];
    gr_complex* optr = (gr_complex*)output_items[0];
    float* freq_optr = output_items.size() >= 2 ? (float*)output_items[1] : NULL;
    float* phase_optr = output_items.size() >= 3 ? (float*)output_items[2] : NULL;
    float* error_optr = output_items.size() >= 4 ? (float*)output_items[3] : NULL;

    std::vector<tag_t> tags;
    get_tags_in_range(tags,
                      0,
                      nitems_read(0),
                      nitems_read(0) + noutput_items,
                      pmt::intern("phase_est"));

    // Get this out of the for loop if not used:
    bool hasAdditionalOutputs = false;
    if (freq_optr)
        hasAdditionalOutputs = true;
    else if (phase_optr)
        hasAdditionalOutputs = true;
    else if (error_optr)
        hasAdditionalOutputs = true;

    float i_r, i_i, n_r, n_i, o_r, o_i;

    for (int i = 0; i < noutput_items; i++) {
        if (!tags.empty()) {
            if (tags[0].offset - nitems_read(0) == (size_t)i) {
                d_phase = (float)pmt::to_double(tags[0].value);
                tags.erase(tags.begin());
            }
        }

        // EXPENSIVE LINES.  Below was 50% faster than gr_expj (30%) and gr_complex (20%)
        // multiplication nco_out = gr_expj(-d_phase);
        // optr[i] = iptr[i] * nco_out;
        n_i = sinf(-d_phase);
        n_r = cosf(-d_phase);

        // optr[i] = iptr[i] * nco_out;
        i_r = iptr[i].real();
        i_i = iptr[i].imag();
        o_r = (i_r * n_r) - (i_i * n_i);
        o_i = (i_r * n_i) + (i_i * n_r);

        optr[i].real(o_r);
        optr[i].imag(o_i);

        // EXPENSIVE LINE, switch was 20% faster in testing.
        // d_error = phase_detector_2(optr[i]);
        switch (d_order) {
        case 2:
            if (d_use_snr)
                d_error = phase_detector_snr_2(optr[i]);
            else
                d_error = phase_detector_2(optr[i]);
            break;
        case 4:
            if (d_use_snr)
                d_error = phase_detector_snr_4(optr[i]);
            else
                d_error = phase_detector_4(optr[i]);
            break;
        case 8:
            if (d_use_snr)
                d_error = phase_detector_snr_8(optr[i]);
            else
                d_error = phase_detector_8(optr[i]);
            break;
        }
        d_error = gr::branchless_clip(d_error, 1.0);

        advance_loop(d_error);
        phase_wrap();
        frequency_limit();

        if (hasAdditionalOutputs) {
            if (freq_optr)
                freq_optr[i] = d_freq;
            if (phase_optr)
                phase_optr[i] = d_phase;
            if (error_optr)
                error_optr[i] = d_error;
        }
    }

    return noutput_items;
}

void costas_loop_cc_impl::setup_rpc()
{
#ifdef GR_CTRLPORT
    // Getters
    add_rpc_variable(rpcbasic_sptr(
        new rpcbasic_register_get<costas_loop_cc, float>(alias(),
                                                         "error",
                                                         &costas_loop_cc::error,
                                                         pmt::mp(-2.0f),
                                                         pmt::mp(2.0f),
                                                         pmt::mp(0.0f),
                                                         "",
                                                         "Error signal of loop",
                                                         RPC_PRIVLVL_MIN,
                                                         DISPTIME | DISPOPTSTRIP)));

    add_rpc_variable(rpcbasic_sptr(
        new rpcbasic_register_get<control_loop, float>(alias(),
                                                       "frequency",
                                                       &control_loop::get_frequency,
                                                       pmt::mp(0.0f),
                                                       pmt::mp(2.0f),
                                                       pmt::mp(0.0f),
                                                       "",
                                                       "Frequency Est.",
                                                       RPC_PRIVLVL_MIN,
                                                       DISPTIME | DISPOPTSTRIP)));

    add_rpc_variable(rpcbasic_sptr(
        new rpcbasic_register_get<control_loop, float>(alias(),
                                                       "phase",
                                                       &control_loop::get_phase,
                                                       pmt::mp(0.0f),
                                                       pmt::mp(2.0f),
                                                       pmt::mp(0.0f),
                                                       "",
                                                       "Phase Est.",
                                                       RPC_PRIVLVL_MIN,
                                                       DISPTIME | DISPOPTSTRIP)));

    add_rpc_variable(rpcbasic_sptr(
        new rpcbasic_register_get<control_loop, float>(alias(),
                                                       "loop_bw",
                                                       &control_loop::get_loop_bandwidth,
                                                       pmt::mp(0.0f),
                                                       pmt::mp(2.0f),
                                                       pmt::mp(0.0f),
                                                       "",
                                                       "Loop bandwidth",
                                                       RPC_PRIVLVL_MIN,
                                                       DISPTIME | DISPOPTSTRIP)));

    // Setters
    add_rpc_variable(rpcbasic_sptr(
        new rpcbasic_register_set<control_loop, float>(alias(),
                                                       "loop_bw",
                                                       &control_loop::set_loop_bandwidth,
                                                       pmt::mp(0.0f),
                                                       pmt::mp(1.0f),
                                                       pmt::mp(0.0f),
                                                       "",
                                                       "Loop bandwidth",
                                                       RPC_PRIVLVL_MIN,
                                                       DISPNULL)));
#endif /* GR_CTRLPORT */
}

} /* namespace digital */
} /* namespace gr */
