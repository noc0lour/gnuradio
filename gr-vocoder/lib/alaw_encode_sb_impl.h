/* -*- c++ -*- */
/*
 * Copyright 2011,2013 Free Software Foundation, Inc.
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

#ifndef INCLUDED_VOCODER_ALAW_ENCODER_SB_IMPL_H
#define INCLUDED_VOCODER_ALAW_ENCODER_SB_IMPL_H

#include <gnuradio/vocoder/alaw_encode_sb.h>

namespace gr {
namespace vocoder {

class alaw_encode_sb_impl : public alaw_encode_sb {
    public:
    alaw_encode_sb_impl();
    ~alaw_encode_sb_impl();

    int work(int noutput_items,
             gr_vector_const_void_star& input_items,
             gr_vector_void_star& output_items);
};

} /* namespace vocoder */
} /* namespace gr */

#endif /* INCLUDED_VOCODER_ALAW_ENCODE_SB_IMPL_H */
