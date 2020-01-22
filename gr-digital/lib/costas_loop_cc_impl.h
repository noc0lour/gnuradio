/* -*- c++ -*- */
/*
 * Copyright 2006,2011,2012,2014 Free Software Foundation, Inc.
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


#ifndef INCLUDED_DIGITAL_COSTAS_LOOP_CC_IMPL_H
#define INCLUDED_DIGITAL_COSTAS_LOOP_CC_IMPL_H

#include <gnuradio/digital/costas_loop_cc.h>

namespace gr {
namespace digital {

class costas_loop_cc_impl : public costas_loop_cc
{
private:
    const unsigned int d_order;
    float d_error;
    float d_noise;
    bool d_use_snr;

    void fast_cc_multiply(gr_complex& out, const gr_complex cc1, const gr_complex cc2)
    {
        // The built-in complex.h multiply has significant NaN/INF checking that
        // considerably slows down performance.  While on some compilers the
        // -fcx-limit-range flag can be used, this fast function makes the math consistent
        // in terms of performance for the Costas loop.
        float o_r, o_i;

        o_r = (cc1.real() * cc2.real()) - (cc1.imag() * cc2.imag());
        o_i = (cc1.real() * cc2.imag()) + (cc1.imag() * cc2.real());

        out.real(o_r);
        out.imag(o_i);
    }

    /*! \brief the phase detector circuit for 8th-order PSK loops.
     *
     *  \param sample complex sample
     *  \return the phase error
     */
    float phase_detector_8(gr_complex sample) const // for 8PSK
    {
        /* This technique splits the 8PSK constellation into 2 squashed
   QPSK constellations, one when I is larger than Q and one
   where Q is larger than I. The error is then calculated
   proportionally to these squashed constellations by the const
   K = sqrt(2)-1.

   The signal magnitude must be > 1 or K will incorrectly bias
   the error value.

   Ref: Z. Huang, Z. Yi, M. Zhang, K. Wang, "8PSK demodulation for
   new generation DVB-S2", IEEE Proc. Int. Conf. Communications,
   Circuits and Systems, Vol. 2, pp. 1447 - 1450, 2004.
         */

        const float K = (sqrtf(2.0) - 1);
        if (fabsf(sample.real()) >= fabsf(sample.imag())) {
            return ((sample.real() > 0 ? 1.0 : -1.0) * sample.imag() -
                    (sample.imag() > 0 ? 1.0 : -1.0) * sample.real() * K);
        } else {
            return ((sample.real() > 0 ? 1.0 : -1.0) * sample.imag() * K -
                    (sample.imag() > 0 ? 1.0 : -1.0) * sample.real());
        }
    };

    /*! \brief the phase detector circuit for fourth-order loops.
     *
     *  \param sample complex sample
     *  \return the phase error
     */
    float phase_detector_4(gr_complex sample) const // for QPSK
    {
        return ((sample.real() > 0 ? 1.0 : -1.0) * sample.imag() -
                (sample.imag() > 0 ? 1.0 : -1.0) * sample.real());
    };

    /*! \brief the phase detector circuit for second-order loops.
     *
     *  \param sample a complex sample
     *  \return the phase error
     */
    float phase_detector_2(gr_complex sample) const // for BPSK
    {
        return (sample.real() * sample.imag());
    }

    /*! \brief the phase detector circuit for 8th-order PSK
     *  loops. Uses tanh instead of slicing and the noise estimate
     *  from the message port to estimated SNR of the samples.
     *
     *  \param sample complex sample
     *  \return the phase error
     */
    float phase_detector_snr_8(gr_complex sample) const // for 8PSK
    {
        const float K = (sqrtf(2.0) - 1);
        const float snr = (sample * std::conj(sample)).real() / d_noise;
        if (fabsf(sample.real()) >= fabsf(sample.imag())) {
            return ((blocks::tanhf_lut(snr * sample.real()) * sample.imag()) -
                    (blocks::tanhf_lut(snr * sample.imag()) * sample.real() * K));
        } else {
            return ((blocks::tanhf_lut(snr * sample.real()) * sample.imag() * K) -
                    (blocks::tanhf_lut(snr * sample.imag()) * sample.real()));
        }
    };

    /*! \brief the phase detector circuit for fourth-order
     *  loops. Uses tanh instead of slicing and the noise estimate
     *  from the message port to estimated SNR of the samples.
     *
     *  \param sample complex sample
     *  \return the phase error
     */
    float phase_detector_snr_4(gr_complex sample) const // for QPSK
    {
        const float snr = (sample * std::conj(sample)).real() / d_noise;
        return ((blocks::tanhf_lut(snr * sample.real()) * sample.imag()) -
                (blocks::tanhf_lut(snr * sample.imag()) * sample.real()));
    };

    /*! \brief the phase detector circuit for second-order
     *  loops. Uses tanh instead of slicing and the noise estimate
     *  from the message port to estimated SNR of the samples.
     *
     *  \param sample a complex sample
     *  \return the phase error
     */
    float phase_detector_snr_2(gr_complex sample) const // for BPSK
    {
        const float snr = (sample * std::conj(sample)).real() / d_noise;
        return blocks::tanhf_lut(snr * sample.real()) * sample.imag();
    };

public:
    costas_loop_cc_impl(float loop_bw, unsigned int order, bool use_snr = false);
    ~costas_loop_cc_impl();

    float error() const { return d_error; };

    void handle_set_noise(pmt::pmt_t msg);

    void setup_rpc();

    int work(int noutput_items,
             gr_vector_const_void_star& input_items,
             gr_vector_void_star& output_items);
};

} /* namespace digital */
} /* namespace gr */

#endif /* INCLUDED_DIGITAL_COSTAS_LOOP_CC_IMPL_H */
