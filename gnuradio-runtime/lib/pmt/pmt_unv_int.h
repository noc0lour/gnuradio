/* -*- c++ -*- */
/*
 * Copyright 2006,2009,2018 Free Software Foundation, Inc.
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

#ifndef INCLUDED_PMT_UNV_INT_H
#define INCLUDED_PMT_UNV_INT_H

#include "pmt_int.h"
#include <complex>
#include <cstdint>
#include <string>
#include <vector>

namespace pmt {
template <class T>
class pmt_vector_t : public pmt_uniform_vector {
    std::vector<T> d_v;

    public:
    pmt_vector_t(size_t k, T fill);
    pmt_vector_t(size_t k, const T* data);
    ~pmt_vector_t();

    size_t length() const { return d_v.size(); }
    size_t itemsize() const { return sizeof(T); }
    T ref(size_t k) const;
    void set(size_t k, T x);
    const T* elements(size_t& len);
    T* writable_elements(size_t& len);
    const void* uniform_elements(size_t& len);
    void* uniform_writable_elements(size_t& len);
    virtual const std::string string_ref(size_t k) const;
};

template <>
bool pmt_vector_t<std::uint8_t>::is_u8vector() const {
    return true;
}

template <>
bool pmt_vector_t<std::int8_t>::is_s8vector() const {
    return true;
}

template <>
bool pmt_vector_t<std::uint16_t>::is_u16vector() const {
    return true;
}

template <>
bool pmt_vector_t<std::int16_t>::is_s16vector() const {
    return true;
}

template <>
bool pmt_vector_t<std::uint32_t>::is_u32vector() const {
    return true;
}

template <>
bool pmt_vector_t<std::int32_t>::is_s32vector() const {
    return true;
}
template <>
bool pmt_vector_t<std::uint64_t>::is_u64vector() const {
    return true;
}

template <>
bool pmt_vector_t<std::int64_t>::is_s64vector() const {
    return true;
}
template <>
bool pmt_vector_t<float>::is_f32vector() const {
    return true;
}

template <>
bool pmt_vector_t<double>::is_f64vector() const {
    return true;
}

template <>
bool pmt_vector_t<std::complex<float>>::is_c32vector() const {
    return true;
}

template <>
bool pmt_vector_t<std::complex<double>>::is_c64vector() const {
    return true;
}

  typedef pmt_vector_t<std::uint8_t> pmt_u8vector;
  typedef pmt_vector_t<std::int8_t> pmt_s8vector;
  typedef pmt_vector_t<std::uint16_t> pmt_u16vector;
  typedef pmt_vector_t<std::int16_t> pmt_s16vector;
  typedef pmt_vector_t<std::uint32_t> pmt_u32vector;
  typedef pmt_vector_t<std::int32_t> pmt_s32vector;
  typedef pmt_vector_t<std::uint64_t> pmt_u64vector;
  typedef pmt_vector_t<std::int64_t> pmt_s64vector;
  typedef pmt_vector_t<float> pmt_f32vector;
  typedef pmt_vector_t<double> pmt_f64vector;
  typedef pmt_vector_t< std::complex<float> > pmt_c32vector;
  typedef pmt_vector_t< std::complex<double> > pmt_c64vector;
} // namespace pmt
#endif
