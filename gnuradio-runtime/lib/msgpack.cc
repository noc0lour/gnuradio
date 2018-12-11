/* -*- c++ -*- */
/*
 * Copyright 2018 Free Software Foundation, Inc.
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

#include "gnuradio/msgpack.h"

#include <msgpack.hpp>
#include <new>
#include <type_traits>

namespace gr {

  static int nifty_counter;
  static typename std::aligned_storage<sizeof(msgpack::zone), alignof(msgpack::zone)>::type msgpack_zone_buf;

  msgpack::zone& msgpack_zone = reinterpret_cast<msgpack::zone&>(msgpack_zone_buf);

  gr_msgpack_zone_initializer::gr_msgpack_zone_initializer(){
    if (nifty_counter++ == 0) new (&msgpack_zone) msgpack::zone();
  }

  gr_msgpack_zone_initializer::~gr_msgpack_zone_initializer(){
    if (nifty_counter-- == 0) (&msgpack_zone)->~zone();
  }


}
