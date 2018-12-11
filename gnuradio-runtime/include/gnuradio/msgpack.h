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

#ifndef GR_MSGPACK_H
#define GR_MSGPACK_H

#include <gnuradio/api.h>
#include <msgpack.hpp>

namespace gr {
  extern GR_RUNTIME_API msgpack::zone& msgpack_zone;

  template <class T>
  GR_RUNTIME_API  msgpack::object msgpack_object(const T& object){return msgpack::object(object, msgpack_zone);};

  static struct GR_RUNTIME_API gr_msgpack_zone_initializer{
    gr_msgpack_zone_initializer();
    ~gr_msgpack_zone_initializer();
  } zone_initializer;

}
#endif
