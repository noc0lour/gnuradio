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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "pmt_int.h"
#include <pmt/pmt.h>
#include <boost/lexical_cast.hpp>
#include <vector>


namespace pmt {

  // Static function for each type because why not
template <class T>
static pmt_vector_t<T> *
_pmt_vector(pmt_t x)
{
  return dynamic_cast<T*>(x.get());
}

  template <class T>
  pmt_vector_t<T>::pmt_vector_t(size_t k, T fill)
  : d_v(k)
{
  for (size_t i = 0; i < k; i++)
    d_v[i] = fill;
}

  template <class T>
  pmt_vector_t<T>::pmt_vector_t(size_t k, const T *data)
  : d_v(k)
{
  if(k)
    memcpy( &d_v[0], data, k * sizeof(T) );
}

template <class T>
T
pmt_vector_t<T>::ref(size_t k) const
{
  if (k >= length())
    throw out_of_range(__FUNCTION__, from_long(k));
  return d_v[k];
}

  template <class T>
void
pmt_vector_t<T>::set(size_t k, T x)
{
  if (k >= length())
    throw out_of_range(__FUNCTION__, from_long(k));
  d_v[k] = x;
}

  template <class T>
const T *
  pmt_vector_t<T>::elements(size_t &len)
{
  len = length();
  return len ?  &d_v[0] : nullptr;
}

T *
pmt_vector_t<T>::writable_elements(size_t &len)
{
  len = length();
  return len ? &d_v[0] : nullptr;
}

  template <class T>
const void*
pmt_vector_t<T>::uniform_elements(size_t &len)
{
  len = length() * sizeof(T);
  return len ? &d_v[0] : nullptr;
}

  template <class T>
void*
  pmt_vector_t<T>::uniform_writable_elements(size_t &len)
{
  len = length() * sizeof(T);
  return len ? (&d_v[0]) : nullptr;
}

  template <class T>
bool
is_pmt_vector_t<T>(pmt_t obj)
{
  return obj->is_@TAG@vector();
}

  template <class T>
pmt_t
make_pmt_vector_t(size_t k, T fill)
{
  return pmt_t(new pmt_vector_t<T>(k, fill));
}

pmt_t
init_@TAG@vector(size_t k, const @TYPE@ *data)
{
  return pmt_t(new pmt_@TAG@vector(k, data));
}

pmt_t
init_@TAG@vector(size_t k, const std::vector< @TYPE@ > &data)
{
  if(k) {
    return pmt_t(new pmt_@TAG@vector(k, &data[0]));
  }
  return pmt_t(new pmt_@TAG@vector(k, static_cast< @TYPE@ >(0))); // fills an empty vector with 0
}

@TYPE@
@TAG@vector_ref(pmt_t vector, size_t k)
{
  if (!vector->is_@TAG@vector())
    throw wrong_type("pmt_@TAG@vector_ref", vector);
  return _@TAG@vector(vector)->ref(k);
}

void
@TAG@vector_set(pmt_t vector, size_t k, @TYPE@ obj)
{
  if (!vector->is_@TAG@vector())
    throw wrong_type("pmt_@TAG@vector_set", vector);
  _@TAG@vector(vector)->set(k, obj);
}

const @TYPE@ *
@TAG@vector_elements(pmt_t vector, size_t &len)
{
  if (!vector->is_@TAG@vector())
    throw wrong_type("pmt_@TAG@vector_elements", vector);
  return _@TAG@vector(vector)->elements(len);
}

const std::vector< @TYPE@ >
@TAG@vector_elements(pmt_t vector)
{
  if (!vector->is_@TAG@vector())
    throw wrong_type("pmt_@TAG@vector_elements", vector);
  size_t len;
  const @TYPE@ *array = _@TAG@vector(vector)->elements(len);
  const std::vector< @TYPE@ > vec(array, array+len);
  return vec;
}


@TYPE@ *
@TAG@vector_writable_elements(pmt_t vector, size_t &len)
{
  if (!vector->is_@TAG@vector())
    throw wrong_type("pmt_@TAG@vector_writable_elements", vector);
  return _@TAG@vector(vector)->writable_elements(len);
}

const std::string
pmt_@TAG@vector::string_ref(size_t k) const
{
  return boost::lexical_cast< std::string, @TYPE@ > (ref(k));
}

} /* namespace pmt */
