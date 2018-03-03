/* -*- c++ -*- */
/*
 * Copyright 2013 Free Software Foundation, Inc.
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

#ifndef _QA_FAST_ATAN2F_H_
#define _QA_FAST_ATAN2F_H_

#include <cppunit/TestCase.h>
#include <cppunit/extensions/HelperMacros.h>

class qa_fast_atan2f : public CppUnit::TestCase {
    CPPUNIT_TEST_SUITE(qa_fast_atan2f);
    CPPUNIT_TEST(t1);
    CPPUNIT_TEST(t2);
    CPPUNIT_TEST_SUITE_END();

    private:
    void t1();
    void t2();
};

#endif /* _QA_FAST_ATAN2F_H_ */
