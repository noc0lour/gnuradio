/* -*- c++ -*- */
/*
 * Copyright 2002,2012 Free Software Foundation, Inc.
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

#include <cppunit/TestAssert.h>
#include <gnuradio/filter/firdes.h>
#include <gnuradio/gr_complex.h>
#include <iomanip>
#include <iostream>
#include <qa_firdes.h>
#include <stdio.h>
#include <string.h>

namespace gr {
namespace filter {

#define NELEM(x) (sizeof(x) / sizeof(x[0]))

using std::vector;

void print_taps(std::ostream& s, vector<float>& v) {
    std::streamsize tmp = s.precision(9);
    for (unsigned int i = 0; i < v.size(); i++) {
        s << v[i] << ", ";
    }
    s << std::endl;
    s.precision(tmp);
}

static void check_symmetry(vector<float>& v) {
    int n = v.size();
    int m = n / 2;

    for (int i = 0; i < m; i++)
        CPPUNIT_ASSERT_DOUBLES_EQUAL(v[i], v[n - i - 1], 1e-9);
}

const static float t1_exp[39] = {
    0.00111410965,  -0.000583702058, -0.00192639488, 2.30933896e-18, 0.00368289859,  0.00198723329,
    -0.0058701504,  -0.00666110823,  0.0068643163,   0.0147596458,   -0.00398709066, -0.0259727165,
    -0.0064281947,  0.0387893915,    0.0301109217,   -0.0507995859,  -0.0833103433,  0.0593735874,
    0.310160041,    0.437394291,     0.310160041,    0.0593735874,   -0.0833103433,  -0.0507995859,
    0.0301109217,   0.0387893915,    -0.0064281947,  -0.0259727165,  -0.00398709066, 0.0147596458,
    0.0068643163,   -0.00666110823,  -0.0058701504,  0.00198723329,  0.00368289859,  2.30933896e-18,
    -0.00192639488, -0.000583702058, 0.00111410965};

const static float t2_exp[39] = {
    -0.00111255341, 0.000582886743, 0.00192370394,  -2.30611317e-18, -0.0036777542, -0.00198445749,
    0.00586195057,  0.00665180339,  -0.00685472786, -0.0147390282,   0.00398152089, 0.0259364359,
    0.00641921535,  -0.0387352072,  -0.0300688613,  0.0507286265,    0.0831939653,  -0.0592906512,
    -0.309726775,   0.561578512,    -0.309726775,   -0.0592906512,   0.0831939653,  0.0507286265,
    -0.0300688613,  -0.0387352072,  0.00641921535,  0.0259364359,    0.00398152089, -0.0147390282,
    -0.00685472786, 0.00665180339,  0.00586195057,  -0.00198445749,  -0.0036777542, -2.30611317e-18,
    0.00192370394,  0.000582886743, -0.00111255341};

const static float t3_exp[77] = {
    0.000119983582, 0.000607753696, 0.000897691818, -0.0010834164,  2.31763315e-05, -0.00179765455,
    0.000822491478, 0.0014836716,   0.000661226455, 0.00204213755,  -0.00466352375, -0.000186616904,
    -0.00289339852, 0.00297895772,  0.00664081471,  -0.00184599939, 0.00318629085,  -0.0135707017,
    3.90737225e-17, 0.000884963025, 0.00652826019,  0.0175401419,   -0.013614703,   0.00140484457,
    -0.0266032815,  0.00285289111,  0.0201372877,   0.00739659835,  0.0347237773,   -0.0490655154,
    -0.00766157778, -0.0385900773,  0.0151521852,   0.0955788717,   -0.0159830209,  0.0826964602,
    -0.281061709,   -0.109556615,   0.528591633,    -0.109556615,   -0.281061709,   0.0826964602,
    -0.0159830209,  0.0955788717,   0.0151521852,   -0.0385900773,  -0.00766157778, -0.0490655154,
    0.0347237773,   0.00739659835,  0.0201372877,   0.00285289111,  -0.0266032815,  0.00140484457,
    -0.013614703,   0.0175401419,   0.00652826019,  0.000884963025, 3.90737225e-17, -0.0135707017,
    0.00318629085,  -0.00184599939, 0.00664081471,  0.00297895772,  -0.00289339852, -0.000186616904,
    -0.00466352375, 0.00204213755,  0.000661226455, 0.0014836716,   0.000822491478, -0.00179765455,
    2.31763315e-05, -0.0010834164,  0.000897691818, 0.000607753696, 0.000119983582};

const static float t4_exp[49] = { // low pass
    0.00105995836,  0.000226392993, -0.00127760693, -0.000967577624, 0.00159226439,  0.00243603508,
    -0.00145168288, -0.00476933597, 5.28154159e-18, 0.0075675128,    0.00365885533,  -0.00976149458,
    -0.010118301,   0.00963691529,  0.0193619132,   -0.0049355682,   -0.0306062996,  -0.00726737641,
    0.0423667729,   0.0319742262,   -0.0527484827,  -0.0850463286,   0.0598905981,   0.31065014,
    0.437056929,    0.31065014,     0.0598905981,   -0.0850463286,   -0.0527484827,  0.0319742262,
    0.0423667729,   -0.00726737641, -0.0306062996,  -0.0049355682,   0.0193619132,   0.00963691529,
    -0.010118301,   -0.00976149458, 0.00365885533,  0.0075675128,    5.28154159e-18, -0.00476933597,
    -0.00145168288, 0.00243603508,  0.00159226439,  -0.000967577624, -0.00127760693, 0.000226392993,
    0.00105995836};

const static float t5_exp[49] = { // high pass
    -0.00106212357, -0.000226855438, 0.00128021673,   0.000969554123, -0.00159551692,
    -0.00244101114, 0.00145464821,   0.00477907853,   -5.2923301e-18, -0.00758297089,
    -0.00366632943, 0.0097814342,    0.0101389699,    -0.00965660065, -0.0194014646,
    0.00494565023,  0.0306688193,    0.00728222169,   -0.0424533151,  -0.0320395418,
    0.052856233,    0.0852200612,    -0.0600129403,   -0.311284721,   0.563078225,
    -0.311284721,   -0.0600129403,   0.0852200612,    0.052856233,    -0.0320395418,
    -0.0424533151,  0.00728222169,   0.0306688193,    0.00494565023,  -0.0194014646,
    -0.00965660065, 0.0101389699,    0.0097814342,    -0.00366632943, -0.00758297089,
    -5.2923301e-18, 0.00477907853,   0.00145464821,   -0.00244101114, -0.00159551692,
    0.000969554123, 0.00128021673,   -0.000226855438, -0.00106212357};

const static float t6_exp[] = { // bandpass
    0.000280927314, -0.00104732765, 7.93654181e-05, -0.000427086081, 0.000759583549, 0.000896608108,
    -0.0004236323,  0.000242393609, -0.00221229903, 0.000480753428,  0.000262036163, 0.00144372846,
    0.002229932,    -0.00272060721, 5.73114157e-05, -0.00429763459,  0.0018788334,   0.00321715139,
    0.00135705515,  0.00396509003,  -0.0085761901,  -0.000325722882, -0.00480572786, 0.00472192047,
    0.0100754956,   -0.00268871989, 0.00446796743,  -0.0183707699,   5.11965838e-17, 0.00112507516,
    0.00807165075,  0.0211376436,   -0.0160245355,  0.00161809532,   -0.0300405379,  0.00316381129,
    0.0219683405,   0.00795029569,  0.036828734,    -0.0514246747,   -0.00794606097, -0.0396579579,
    0.0154495509,   0.0968139917,   -0.0161030479,  0.0829729438,    -0.281171471,   -0.109406292,
    0.527556598,    -0.109406292,   -0.281171471,   0.0829729438,    -0.0161030479,  0.0968139917,
    0.0154495509,   -0.0396579579,  -0.00794606097, -0.0514246747,   0.036828734,    0.00795029569,
    0.0219683405,   0.00316381129,  -0.0300405379,  0.00161809532,   -0.0160245355,  0.0211376436,
    0.00807165075,  0.00112507516,  5.11965838e-17, -0.0183707699,   0.00446796743,  -0.00268871989,
    0.0100754956,   0.00472192047,  -0.00480572786, -0.000325722882, -0.0085761901,  0.00396509003,
    0.00135705515,  0.00321715139,  0.0018788334,   -0.00429763459,  5.73114157e-05, -0.00272060721,
    0.002229932,    0.00144372846,  0.000262036163, 0.000480753428,  -0.00221229903, 0.000242393609,
    -0.0004236323,  0.000896608108, 0.000759583549, -0.000427086081, 7.93654181e-05, -0.00104732765,
    0.000280927314};

void qa_firdes::t1() {
    vector<float> taps = firdes::low_pass(1.0, 8000, 1750, 500, firdes::WIN_HAMMING);

    // std::cout << "ntaps: " << taps.size() << std::endl;
    // print_taps(std::cout, taps);

    CPPUNIT_ASSERT_EQUAL(NELEM(t1_exp), taps.size());
    for (unsigned int i = 0; i < taps.size(); i++)
        CPPUNIT_ASSERT_DOUBLES_EQUAL(t1_exp[i], taps[i], 1e-9);

    check_symmetry(taps);
}

void qa_firdes::t2() {
    vector<float> taps = firdes::high_pass(1.0, 8000, 1750, 500, firdes::WIN_HAMMING);

    // std::cout << "ntaps: " << taps.size() << std::endl;
    // print_taps(std::cout, taps);

    CPPUNIT_ASSERT_EQUAL(NELEM(t2_exp), taps.size());

    for (unsigned int i = 0; i < taps.size(); i++)
        CPPUNIT_ASSERT_DOUBLES_EQUAL(t2_exp[i], taps[i], 1e-9);

    check_symmetry(taps);
}

void qa_firdes::t3() {
    vector<float> taps = firdes::band_pass(
        1.0, 20e6, 5.75e6 - (5.28e6 / 2), 5.75e6 + (5.28e6 / 2), 0.62e6, firdes::WIN_HAMMING);

    // std::cout << "ntaps: " << taps.size() << std::endl;
    // print_taps(std::cout, taps);

    CPPUNIT_ASSERT_EQUAL(NELEM(t3_exp), taps.size());

    for (unsigned int i = 0; i < taps.size(); i++)
        CPPUNIT_ASSERT_DOUBLES_EQUAL(t3_exp[i], taps[i], 1e-7);

    check_symmetry(taps);
}

void qa_firdes::t4() {
    vector<float> taps = firdes::low_pass_2(1.0, 8000, 1750, 500, 66, firdes::WIN_HAMMING);

    // std::cout << "ntaps: " << taps.size() << std::endl;
    // print_taps(std::cout, taps);

    CPPUNIT_ASSERT_EQUAL(NELEM(t4_exp), taps.size());
    for (unsigned int i = 0; i < taps.size(); i++)
        CPPUNIT_ASSERT_DOUBLES_EQUAL(t4_exp[i], taps[i], 1e-9);

    check_symmetry(taps);
}

void qa_firdes::t5() {
    vector<float> taps = firdes::high_pass_2(1.0, 8000, 1750, 500, 66, firdes::WIN_HAMMING);

    // std::cout << "ntaps: " << taps.size() << std::endl;
    // print_taps(std::cout, taps);

    CPPUNIT_ASSERT_EQUAL(NELEM(t5_exp), taps.size());

    for (unsigned int i = 0; i < taps.size(); i++)
        CPPUNIT_ASSERT_DOUBLES_EQUAL(t5_exp[i], taps[i], 1e-9);

    check_symmetry(taps);
}

void qa_firdes::t6() {
    vector<float> taps = firdes::band_pass_2(
        1.0, 20e6, 5.75e6 - (5.28e6 / 2), 5.75e6 + (5.28e6 / 2), 0.62e6, 66, firdes::WIN_HAMMING);

    // std::cout << "ntaps: " << taps.size() << std::endl;
    // print_taps(std::cout, taps);

    CPPUNIT_ASSERT_EQUAL(NELEM(t6_exp), taps.size());

    for (unsigned int i = 0; i < taps.size(); i++)
        CPPUNIT_ASSERT_DOUBLES_EQUAL(t6_exp[i], taps[i], 1e-7);

    check_symmetry(taps);
}

void qa_firdes::t7() {}

} /* namespace filter */
} /* namespace gr */
