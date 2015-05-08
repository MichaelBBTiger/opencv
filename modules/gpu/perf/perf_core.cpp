/*M///////////////////////////////////////////////////////////////////////////////////////
//
//  IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.
//
//  By downloading, copying, installing or using the software you agree to this license.
//  If you do not agree to this license, do not download, install,
//  copy or use the software.
//
//
//                           License Agreement
//                For Open Source Computer Vision Library
//
// Copyright (C) 2000-2008, Intel Corporation, all rights reserved.
// Copyright (C) 2009, Willow Garage Inc., all rights reserved.
// Third party copyrights are property of their respective owners.
//
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
//   * Redistribution's of source code must retain the above copyright notice,
//     this list of conditions and the following disclaimer.
//
//   * Redistribution's in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation
//     and/or other materials provided with the distribution.
//
//   * The name of the copyright holders may not be used to endorse or promote products
//     derived from this software without specific prior written permission.
//
// This software is provided by the copyright holders and contributors "as is" and
// any express or implied warranties, including, but not limited to, the implied
// warranties of merchantability and fitness for a particular purpose are disclaimed.
// In no event shall the Intel Corporation or contributors be liable for any direct,
// indirect, incidental, special, exemplary, or consequential damages
// (including, but not limited to, procurement of substitute goods or services;
// loss of use, data, or profits; or business interruption) however caused
// and on any theory of liability, whether in contract, strict liability,
// or tort (including negligence or otherwise) arising in any way out of
// the use of this software, even if advised of the possibility of such damage.
//
//M*/

#include "perf_precomp.hpp"

using namespace std;
using namespace testing;
using namespace perf;

#define ARITHM_MAT_DEPTH Values(CV_8U, CV_16U, CV_32F, CV_64F)

//////////////////////////////////////////////////////////////////////
// Merge

PERF_TEST_P(Sz_Depth_Cn, Core_Merge,
            Combine(GPU_TYPICAL_MAT_SIZES,
                    ARITHM_MAT_DEPTH,
                    Values(2, 3, 4)))
{
    const cv::Size size = GET_PARAM(0);
    const int depth = GET_PARAM(1);
    const int channels = GET_PARAM(2);

    std::vector<cv::Mat> src(channels);
    for (int i = 0; i < channels; ++i)
    {
        src[i].create(size, depth);
        declare.in(src[i], WARMUP_RNG);
    }

    if (PERF_RUN_GPU())
    {
        std::vector<cv::gpu::GpuMat> d_src(channels);
        for (int i = 0; i < channels; ++i)
            d_src[i].upload(src[i]);

        cv::gpu::GpuMat dst;

        TEST_CYCLE() cv::gpu::merge(d_src, dst);

        GPU_SANITY_CHECK(dst, 1e-10);
    }
    else
    {
        cv::Mat dst;

        TEST_CYCLE() cv::merge(src, dst);

        SANITY_CHECK_NOTHING();
    }
}

//////////////////////////////////////////////////////////////////////
// Split

PERF_TEST_P(Sz_Depth_Cn, Core_Split,
            Combine(GPU_TYPICAL_MAT_SIZES,
                    ARITHM_MAT_DEPTH,
                    Values(2, 3, 4)))
{
    const cv::Size size = GET_PARAM(0);
    const int depth = GET_PARAM(1);
    const int channels = GET_PARAM(2);

    cv::Mat src(size, CV_MAKE_TYPE(depth, channels));
    declare.in(src, WARMUP_RNG);

    if (PERF_RUN_GPU())
    {
        const cv::gpu::GpuMat d_src(src);
        std::vector<cv::gpu::GpuMat> dst;

        TEST_CYCLE() cv::gpu::split(d_src, dst);

        const cv::gpu::GpuMat& dst0 = dst[0];
        const cv::gpu::GpuMat& dst1 = dst[1];

        GPU_SANITY_CHECK(dst0, 1e-10);
        GPU_SANITY_CHECK(dst1, 1e-10);
    }
    else
    {
        std::vector<cv::Mat> dst;

        TEST_CYCLE() cv::split(src, dst);

        SANITY_CHECK_NOTHING();
    }
}

//////////////////////////////////////////////////////////////////////
// AddMat

PERF_TEST_P(Sz_Depth, Core_AddMat,
            Combine(GPU_TYPICAL_MAT_SIZES,
                    ARITHM_MAT_DEPTH))
{
    const cv::Size size = GET_PARAM(0);
    const int depth = GET_PARAM(1);

    cv::Mat src1(size, depth);
    declare.in(src1, WARMUP_RNG);

    cv::Mat src2(size, depth);
    declare.in(src2, WARMUP_RNG);

    if (PERF_RUN_GPU())
    {
        const cv::gpu::GpuMat d_src1(src1);
        const cv::gpu::GpuMat d_src2(src2);
        cv::gpu::GpuMat dst;

        TEST_CYCLE() cv::gpu::add(d_src1, d_src2, dst);

        GPU_SANITY_CHECK(dst, 1e-10);
    }
    else
    {
        cv::Mat dst;

        TEST_CYCLE() cv::add(src1, src2, dst);

        SANITY_CHECK_NOTHING();
    }
}

//////////////////////////////////////////////////////////////////////
// AddScalar

PERF_TEST_P(Sz_Depth, Core_AddScalar,
            Combine(GPU_TYPICAL_MAT_SIZES,
                    ARITHM_MAT_DEPTH))
{
    const cv::Size size = GET_PARAM(0);
    const int depth = GET_PARAM(1);

    cv::Mat src(size, depth);
    declare.in(src, WARMUP_RNG);

    cv::Scalar s;
    declare.in(s, WARMUP_RNG);

    if (PERF_RUN_GPU())
    {
        const cv::gpu::GpuMat d_src(src);
        cv::gpu::GpuMat dst;

        TEST_CYCLE() cv::gpu::add(d_src, s, dst);

        GPU_SANITY_CHECK(dst, 1e-10);
    }
    else
    {
        cv::Mat dst;

        TEST_CYCLE() cv::add(src, s, dst);

        SANITY_CHECK_NOTHING();
    }
}

//////////////////////////////////////////////////////////////////////
// SubtractMat

PERF_TEST_P(Sz_Depth, Core_SubtractMat,
            Combine(GPU_TYPICAL_MAT_SIZES,
                    ARITHM_MAT_DEPTH))
{
    const cv::Size size = GET_PARAM(0);
    const int depth = GET_PARAM(1);

    cv::Mat src1(size, depth);
    declare.in(src1, WARMUP_RNG);

    cv::Mat src2(size, depth);
    declare.in(src2, WARMUP_RNG);

    if (PERF_RUN_GPU())
    {
        const cv::gpu::GpuMat d_src1(src1);
        const cv::gpu::GpuMat d_src2(src2);
        cv::gpu::GpuMat dst;

        TEST_CYCLE() cv::gpu::subtract(d_src1, d_src2, dst);

        GPU_SANITY_CHECK(dst, 1e-10);
    }
    else
    {
        cv::Mat dst;

        TEST_CYCLE() cv::subtract(src1, src2, dst);

        SANITY_CHECK_NOTHING();
    }
}

//////////////////////////////////////////////////////////////////////
// SubtractScalar

PERF_TEST_P(Sz_Depth, Core_SubtractScalar,
            Combine(GPU_TYPICAL_MAT_SIZES,
                    ARITHM_MAT_DEPTH))
{
    const cv::Size size = GET_PARAM(0);
    const int depth = GET_PARAM(1);

    cv::Mat src(size, depth);
    declare.in(src, WARMUP_RNG);

    cv::Scalar s;
    declare.in(s, WARMUP_RNG);

    if (PERF_RUN_GPU())
    {
        const cv::gpu::GpuMat d_src(src);
        cv::gpu::GpuMat dst;

        TEST_CYCLE() cv::gpu::subtract(d_src, s, dst);

        GPU_SANITY_CHECK(dst, 1e-10);
    }
    else
    {
        cv::Mat dst;

        TEST_CYCLE() cv::subtract(src, s, dst);

        SANITY_CHECK_NOTHING();
    }
}

//////////////////////////////////////////////////////////////////////
// MultiplyMat

PERF_TEST_P(Sz_Depth, Core_MultiplyMat,
            Combine(GPU_TYPICAL_MAT_SIZES,
                    ARITHM_MAT_DEPTH))
{
    const cv::Size size = GET_PARAM(0);
    const int depth = GET_PARAM(1);

    cv::Mat src1(size, depth);
    declare.in(src1, WARMUP_RNG);

    cv::Mat src2(size, depth);
    declare.in(src2, WARMUP_RNG);

    if (PERF_RUN_GPU())
    {
        const cv::gpu::GpuMat d_src1(src1);
        const cv::gpu::GpuMat d_src2(src2);
        cv::gpu::GpuMat dst;

        TEST_CYCLE() cv::gpu::multiply(d_src1, d_src2, dst);

        GPU_SANITY_CHECK(dst, 1e-6);
    }
    else
    {
        cv::Mat dst;

        TEST_CYCLE() cv::multiply(src1, src2, dst);

        SANITY_CHECK_NOTHING();
    }
}

//////////////////////////////////////////////////////////////////////
// MultiplyScalar

PERF_TEST_P(Sz_Depth, Core_MultiplyScalar,
            Combine(GPU_TYPICAL_MAT_SIZES,
                    ARITHM_MAT_DEPTH))
{
    const cv::Size size = GET_PARAM(0);
    const int depth = GET_PARAM(1);

    cv::Mat src(size, depth);
    declare.in(src, WARMUP_RNG);

    cv::Scalar s;
    declare.in(s, WARMUP_RNG);

    if (PERF_RUN_GPU())
    {
        const cv::gpu::GpuMat d_src(src);
        cv::gpu::GpuMat dst;

        TEST_CYCLE() cv::gpu::multiply(d_src, s, dst);

        GPU_SANITY_CHECK(dst, 1e-6);
    }
    else
    {
        cv::Mat dst;

        TEST_CYCLE() cv::multiply(src, s, dst);

        SANITY_CHECK_NOTHING();
    }
}

//////////////////////////////////////////////////////////////////////
// DivideMat

PERF_TEST_P(Sz_Depth, Core_DivideMat,
            Combine(GPU_TYPICAL_MAT_SIZES,
                    ARITHM_MAT_DEPTH))
{
    const cv::Size size = GET_PARAM(0);
    const int depth = GET_PARAM(1);

    cv::Mat src1(size, depth);
    declare.in(src1, WARMUP_RNG);

    cv::Mat src2(size, depth);
    declare.in(src2, WARMUP_RNG);

    if (PERF_RUN_GPU())
    {
        const cv::gpu::GpuMat d_src1(src1);
        const cv::gpu::GpuMat d_src2(src2);
        cv::gpu::GpuMat dst;

        TEST_CYCLE() cv::gpu::divide(d_src1, d_src2, dst);

        GPU_SANITY_CHECK(dst, 1e-6);
    }
    else
    {
        cv::Mat dst;

        TEST_CYCLE() cv::divide(src1, src2, dst);

        SANITY_CHECK_NOTHING();
    }
}

//////////////////////////////////////////////////////////////////////
// DivideScalar

PERF_TEST_P(Sz_Depth, Core_DivideScalar,
            Combine(GPU_TYPICAL_MAT_SIZES,
                    ARITHM_MAT_DEPTH))
{
    const cv::Size size = GET_PARAM(0);
    const int depth = GET_PARAM(1);

    cv::Mat src(size, depth);
    declare.in(src, WARMUP_RNG);

    cv::Scalar s;
    declare.in(s, WARMUP_RNG);

    if (PERF_RUN_GPU())
    {
        const cv::gpu::GpuMat d_src(src);
        cv::gpu::GpuMat dst;

        TEST_CYCLE() cv::gpu::divide(d_src, s, dst);

        GPU_SANITY_CHECK(dst, 1e-6);
    }
    else
    {
        cv::Mat dst;

        TEST_CYCLE() cv::divide(src, s, dst);

        SANITY_CHECK_NOTHING();
    }
}

//////////////////////////////////////////////////////////////////////
// DivideScalarInv

PERF_TEST_P(Sz_Depth, Core_DivideScalarInv,
            Combine(GPU_TYPICAL_MAT_SIZES,
                    ARITHM_MAT_DEPTH))
{
    const cv::Size size = GET_PARAM(0);
    const int depth = GET_PARAM(1);

    cv::Mat src(size, depth);
    declare.in(src, WARMUP_RNG);

    cv::Scalar s;
    declare.in(s, WARMUP_RNG);

    if (PERF_RUN_GPU())
    {
        const cv::gpu::GpuMat d_src(src);
        cv::gpu::GpuMat dst;

        TEST_CYCLE() cv::gpu::divide(s[0], d_src, dst);

        GPU_SANITY_CHECK(dst, 1e-6);
    }
    else
    {
        cv::Mat dst;

        TEST_CYCLE() cv::divide(s, src, dst);

        SANITY_CHECK_NOTHING();
    }
}

//////////////////////////////////////////////////////////////////////
// AbsDiffMat

PERF_TEST_P(Sz_Depth, Core_AbsDiffMat,
            Combine(GPU_TYPICAL_MAT_SIZES,
                    ARITHM_MAT_DEPTH))
{
    const cv::Size size = GET_PARAM(0);
    const int depth = GET_PARAM(1);

    cv::Mat src1(size, depth);
    declare.in(src1, WARMUP_RNG);

    cv::Mat src2(size, depth);
    declare.in(src2, WARMUP_RNG);

    if (PERF_RUN_GPU())
    {
        const cv::gpu::GpuMat d_src1(src1);
        const cv::gpu::GpuMat d_src2(src2);
        cv::gpu::GpuMat dst;

        TEST_CYCLE() cv::gpu::absdiff(d_src1, d_src2, dst);

        GPU_SANITY_CHECK(dst, 1e-10);
    }
    else
    {
        cv::Mat dst;

        TEST_CYCLE() cv::absdiff(src1, src2, dst);

        SANITY_CHECK_NOTHING();
    }
}

//////////////////////////////////////////////////////////////////////
// AbsDiffScalar

PERF_TEST_P(Sz_Depth, Core_AbsDiffScalar,
            Combine(GPU_TYPICAL_MAT_SIZES,
                    ARITHM_MAT_DEPTH))
{
    const cv::Size size = GET_PARAM(0);
    const int depth = GET_PARAM(1);

    cv::Mat src(size, depth);
    declare.in(src, WARMUP_RNG);

    cv::Scalar s;
    declare.in(s, WARMUP_RNG);

    if (PERF_RUN_GPU())
    {
        const cv::gpu::GpuMat d_src(src);
        cv::gpu::GpuMat dst;

        TEST_CYCLE() cv::gpu::absdiff(d_src, s, dst);

        GPU_SANITY_CHECK(dst, 1e-10);
    }
    else
    {
        cv::Mat dst;

        TEST_CYCLE() cv::absdiff(src, s, dst);

        SANITY_CHECK_NOTHING();
    }
}

//////////////////////////////////////////////////////////////////////
// Abs

PERF_TEST_P(Sz_Depth, Core_Abs,
            Combine(GPU_TYPICAL_MAT_SIZES,
                    Values(CV_16S, CV_32F)))
{
    const cv::Size size = GET_PARAM(0);
    const int depth = GET_PARAM(1);

    cv::Mat src(size, depth);
    declare.in(src, WARMUP_RNG);

    if (PERF_RUN_GPU())
    {
        const cv::gpu::GpuMat d_src(src);
        cv::gpu::GpuMat dst;

        TEST_CYCLE() cv::gpu::abs(d_src, dst);

        GPU_SANITY_CHECK(dst);
    }
    else
    {
        FAIL_NO_CPU();
    }
}

//////////////////////////////////////////////////////////////////////
// Sqr

PERF_TEST_P(Sz_Depth, Core_Sqr,
            Combine(GPU_TYPICAL_MAT_SIZES,
                    Values(CV_8U, CV_16S, CV_32F)))
{
    const cv::Size size = GET_PARAM(0);
    const int depth = GET_PARAM(1);

    cv::Mat src(size, depth);
    declare.in(src, WARMUP_RNG);

    if (PERF_RUN_GPU())
    {
        const cv::gpu::GpuMat d_src(src);
        cv::gpu::GpuMat dst;

        TEST_CYCLE() cv::gpu::sqr(d_src, dst);

        GPU_SANITY_CHECK(dst);
    }
    else
    {
        FAIL_NO_CPU();
    }
}

//////////////////////////////////////////////////////////////////////
// Sqrt

PERF_TEST_P(Sz_Depth, Core_Sqrt,
            Combine(GPU_TYPICAL_MAT_SIZES,
                    Values(CV_8U, CV_16S, CV_32F)))
{
    const cv::Size size = GET_PARAM(0);
    const int depth = GET_PARAM(1);

    cv::Mat src(size, depth);
    cv::randu(src, 0, 100000);

    if (PERF_RUN_GPU())
    {
        const cv::gpu::GpuMat d_src(src);
        cv::gpu::GpuMat dst;

        TEST_CYCLE() cv::gpu::sqrt(d_src, dst);

        GPU_SANITY_CHECK(dst);
    }
    else
    {
        cv::Mat dst;

        TEST_CYCLE() cv::sqrt(src, dst);

        SANITY_CHECK_NOTHING();
    }
}

//////////////////////////////////////////////////////////////////////
// Log

PERF_TEST_P(Sz_Depth, Core_Log,
            Combine(GPU_TYPICAL_MAT_SIZES,
                    Values(CV_8U, CV_16S, CV_32F)))
{
    const cv::Size size = GET_PARAM(0);
    const int depth = GET_PARAM(1);

    cv::Mat src(size, depth);
    cv::randu(src, 0, 100000);

    if (PERF_RUN_GPU())
    {
        const cv::gpu::GpuMat d_src(src);
        cv::gpu::GpuMat dst;

        TEST_CYCLE() cv::gpu::log(d_src, dst);

        GPU_SANITY_CHECK(dst);
    }
    else
    {
        cv::Mat dst;

        TEST_CYCLE() cv::log(src, dst);

        SANITY_CHECK_NOTHING();
    }
}

//////////////////////////////////////////////////////////////////////
// Exp

PERF_TEST_P(Sz_Depth, Core_Exp,
            Combine(GPU_TYPICAL_MAT_SIZES,
                    Values(CV_8U, CV_16S, CV_32F)))
{
    const cv::Size size = GET_PARAM(0);
    const int depth = GET_PARAM(1);

    cv::Mat src(size, depth);
    cv::randu(src, 0, 10);

    if (PERF_RUN_GPU())
    {
        const cv::gpu::GpuMat d_src(src);
        cv::gpu::GpuMat dst;

        TEST_CYCLE() cv::gpu::exp(d_src, dst);

        GPU_SANITY_CHECK(dst);
    }
    else
    {
        cv::Mat dst;

        TEST_CYCLE() cv::exp(src, dst);

        SANITY_CHECK_NOTHING();
    }
}

//////////////////////////////////////////////////////////////////////
// Pow

DEF_PARAM_TEST(Sz_Depth_Power, cv::Size, MatDepth, double);

PERF_TEST_P(Sz_Depth_Power, Core_Pow,
            Combine(GPU_TYPICAL_MAT_SIZES,
                    Values(CV_8U, CV_16S, CV_32F),
                    Values(0.3, 2.0, 2.4)))
{
    const cv::Size size = GET_PARAM(0);
    const int depth = GET_PARAM(1);
    const double power = GET_PARAM(2);

    cv::Mat src(size, depth);
    declare.in(src, WARMUP_RNG);

    if (PERF_RUN_GPU())
    {
        const cv::gpu::GpuMat d_src(src);
        cv::gpu::GpuMat dst;

        TEST_CYCLE() cv::gpu::pow(d_src, power, dst);

        GPU_SANITY_CHECK(dst);
    }
    else
    {
        cv::Mat dst;

        TEST_CYCLE() cv::pow(src, power, dst);

        SANITY_CHECK_NOTHING();
    }
}

//////////////////////////////////////////////////////////////////////
// CompareMat

CV_ENUM(CmpCode, CMP_EQ, CMP_GT, CMP_GE, CMP_LT, CMP_LE, CMP_NE)

DEF_PARAM_TEST(Sz_Depth_Code, cv::Size, MatDepth, CmpCode);

PERF_TEST_P(Sz_Depth_Code, Core_CompareMat,
            Combine(GPU_TYPICAL_MAT_SIZES,
                    ARITHM_MAT_DEPTH,
                    CmpCode::all()))
{
    const cv::Size size = GET_PARAM(0);
    const int depth = GET_PARAM(1);
    const int cmp_code = GET_PARAM(2);

    cv::Mat src1(size, depth);
    declare.in(src1, WARMUP_RNG);

    cv::Mat src2(size, depth);
    declare.in(src2, WARMUP_RNG);

    if (PERF_RUN_GPU())
    {
        const cv::gpu::GpuMat d_src1(src1);
        const cv::gpu::GpuMat d_src2(src2);
        cv::gpu::GpuMat dst;

        TEST_CYCLE() cv::gpu::compare(d_src1, d_src2, dst, cmp_code);

        GPU_SANITY_CHECK(dst);
    }
    else
    {
        cv::Mat dst;

        TEST_CYCLE() cv::compare(src1, src2, dst, cmp_code);

        SANITY_CHECK_NOTHING();
    }
}

//////////////////////////////////////////////////////////////////////
// CompareScalar

PERF_TEST_P(Sz_Depth_Code, Core_CompareScalar,
            Combine(GPU_TYPICAL_MAT_SIZES,
                    ARITHM_MAT_DEPTH,
                    CmpCode::all()))
{
    const cv::Size size = GET_PARAM(0);
    const int depth = GET_PARAM(1);
    const int cmp_code = GET_PARAM(2);

    cv::Mat src(size, depth);
    declare.in(src, WARMUP_RNG);

    cv::Scalar s;
    declare.in(s, WARMUP_RNG);

    if (PERF_RUN_GPU())
    {
        const cv::gpu::GpuMat d_src(src);
        cv::gpu::GpuMat dst;

        TEST_CYCLE() cv::gpu::compare(d_src, s, dst, cmp_code);

        GPU_SANITY_CHECK(dst);
    }
    else
    {
        cv::Mat dst;

        TEST_CYCLE() cv::compare(src, s, dst, cmp_code);

        SANITY_CHECK_NOTHING();
    }
}

//////////////////////////////////////////////////////////////////////
// BitwiseNot

PERF_TEST_P(Sz_Depth, Core_BitwiseNot,
            Combine(GPU_TYPICAL_MAT_SIZES,
                    Values(CV_8U, CV_16U, CV_32S)))
{
    const cv::Size size = GET_PARAM(0);
    const int depth = GET_PARAM(1);

    cv::Mat src(size, depth);
    declare.in(src, WARMUP_RNG);

    if (PERF_RUN_GPU())
    {
        const cv::gpu::GpuMat d_src(src);
        cv::gpu::GpuMat dst;

        TEST_CYCLE() cv::gpu::bitwise_not(d_src, dst);

        GPU_SANITY_CHECK(dst);
    }
    else
    {
        cv::Mat dst;

        TEST_CYCLE() cv::bitwise_not(src, dst);

        SANITY_CHECK_NOTHING();
    }
}

//////////////////////////////////////////////////////////////////////
// BitwiseAndMat

PERF_TEST_P(Sz_Depth, Core_BitwiseAndMat,
            Combine(GPU_TYPICAL_MAT_SIZES,
                    Values(CV_8U, CV_16U, CV_32S)))
{
    const cv::Size size = GET_PARAM(0);
    const int depth = GET_PARAM(1);

    cv::Mat src1(size, depth);
    declare.in(src1, WARMUP_RNG);

    cv::Mat src2(size, depth);
    declare.in(src2, WARMUP_RNG);

    if (PERF_RUN_GPU())
    {
        const cv::gpu::GpuMat d_src1(src1);
        const cv::gpu::GpuMat d_src2(src2);
        cv::gpu::GpuMat dst;

        TEST_CYCLE() cv::gpu::bitwise_and(d_src1, d_src2, dst);

        GPU_SANITY_CHECK(dst);
    }
    else
    {
        cv::Mat dst;

        TEST_CYCLE() cv::bitwise_and(src1, src2, dst);

        SANITY_CHECK_NOTHING();
    }
}

//////////////////////////////////////////////////////////////////////
// BitwiseAndScalar

PERF_TEST_P(Sz_Depth_Cn, Core_BitwiseAndScalar,
            Combine(GPU_TYPICAL_MAT_SIZES,
                    Values(CV_8U, CV_16U, CV_32S),
                    GPU_CHANNELS_1_3_4))
{
    const cv::Size size = GET_PARAM(0);
    const int depth = GET_PARAM(1);
    const int channels = GET_PARAM(2);

    const int type = CV_MAKE_TYPE(depth, channels);

    cv::Mat src(size, type);
    declare.in(src, WARMUP_RNG);

    cv::Scalar s;
    declare.in(s, WARMUP_RNG);
    cv::Scalar_<int> is = s;

    if (PERF_RUN_GPU())
    {
        const cv::gpu::GpuMat d_src(src);
        cv::gpu::GpuMat dst;

        TEST_CYCLE() cv::gpu::bitwise_and(d_src, is, dst);

        GPU_SANITY_CHECK(dst);
    }
    else
    {
        cv::Mat dst;

        TEST_CYCLE() cv::bitwise_and(src, is, dst);

        SANITY_CHECK_NOTHING();
    }
}

//////////////////////////////////////////////////////////////////////
// BitwiseOrMat

PERF_TEST_P(Sz_Depth, Core_BitwiseOrMat,
            Combine(GPU_TYPICAL_MAT_SIZES,
                    Values(CV_8U, CV_16U, CV_32S)))
{
    const cv::Size size = GET_PARAM(0);
    const int depth = GET_PARAM(1);

    cv::Mat src1(size, depth);
    declare.in(src1, WARMUP_RNG);

    cv::Mat src2(size, depth);
    declare.in(src2, WARMUP_RNG);

    if (PERF_RUN_GPU())
    {
        const cv::gpu::GpuMat d_src1(src1);
        const cv::gpu::GpuMat d_src2(src2);
        cv::gpu::GpuMat dst;

        TEST_CYCLE() cv::gpu::bitwise_or(d_src1, d_src2, dst);

        GPU_SANITY_CHECK(dst);
    }
    else
    {
        cv::Mat dst;

        TEST_CYCLE() cv::bitwise_or(src1, src2, dst);

        SANITY_CHECK_NOTHING();
    }
}

//////////////////////////////////////////////////////////////////////
// BitwiseOrScalar

PERF_TEST_P(Sz_Depth_Cn, Core_BitwiseOrScalar,
            Combine(GPU_TYPICAL_MAT_SIZES,
                    Values(CV_8U, CV_16U, CV_32S),
                    GPU_CHANNELS_1_3_4))
{
    const cv::Size size = GET_PARAM(0);
    const int depth = GET_PARAM(1);
    const int channels = GET_PARAM(2);

    const int type = CV_MAKE_TYPE(depth, channels);

    cv::Mat src(size, type);
    declare.in(src, WARMUP_RNG);

    cv::Scalar s;
    declare.in(s, WARMUP_RNG);
    cv::Scalar_<int> is = s;

    if (PERF_RUN_GPU())
    {
        const cv::gpu::GpuMat d_src(src);
        cv::gpu::GpuMat dst;

        TEST_CYCLE() cv::gpu::bitwise_or(d_src, is, dst);

        GPU_SANITY_CHECK(dst);
    }
    else
    {
        cv::Mat dst;

        TEST_CYCLE() cv::bitwise_or(src, is, dst);

        SANITY_CHECK_NOTHING();
    }
}

//////////////////////////////////////////////////////////////////////
// BitwiseXorMat

PERF_TEST_P(Sz_Depth, Core_BitwiseXorMat,
            Combine(GPU_TYPICAL_MAT_SIZES,
                    Values(CV_8U, CV_16U, CV_32S)))
{
    const cv::Size size = GET_PARAM(0);
    const int depth = GET_PARAM(1);

    cv::Mat src1(size, depth);
    declare.in(src1, WARMUP_RNG);

    cv::Mat src2(size, depth);
    declare.in(src2, WARMUP_RNG);

    if (PERF_RUN_GPU())
    {
        const cv::gpu::GpuMat d_src1(src1);
        const cv::gpu::GpuMat d_src2(src2);
        cv::gpu::GpuMat dst;

        TEST_CYCLE() cv::gpu::bitwise_xor(d_src1, d_src2, dst);

        GPU_SANITY_CHECK(dst);
    }
    else
    {
        cv::Mat dst;

        TEST_CYCLE() cv::bitwise_xor(src1, src2, dst);

        SANITY_CHECK_NOTHING();
    }
}

//////////////////////////////////////////////////////////////////////
// BitwiseXorScalar

PERF_TEST_P(Sz_Depth_Cn, Core_BitwiseXorScalar,
            Combine(GPU_TYPICAL_MAT_SIZES,
                    Values(CV_8U, CV_16U, CV_32S),
                    GPU_CHANNELS_1_3_4))
{
    const cv::Size size = GET_PARAM(0);
    const int depth = GET_PARAM(1);
    const int channels = GET_PARAM(2);

    const int type = CV_MAKE_TYPE(depth, channels);

    cv::Mat src(size, type);
    declare.in(src, WARMUP_RNG);

    cv::Scalar s;
    declare.in(s, WARMUP_RNG);
    cv::Scalar_<int> is = s;

    if (PERF_RUN_GPU())
    {
        const cv::gpu::GpuMat d_src(src);
        cv::gpu::GpuMat dst;

        TEST_CYCLE() cv::gpu::bitwise_xor(d_src, is, dst);

        GPU_SANITY_CHECK(dst);
    }
    else
    {
        cv::Mat dst;

        TEST_CYCLE() cv::bitwise_xor(src, is, dst);

        SANITY_CHECK_NOTHING();
    }
}

//////////////////////////////////////////////////////////////////////
// RShift

PERF_TEST_P(Sz_Depth_Cn, Core_RShift,
            Combine(GPU_TYPICAL_MAT_SIZES,
                    Values(CV_8U, CV_16U, CV_32S),
                    GPU_CHANNELS_1_3_4))
{
    const cv::Size size = GET_PARAM(0);
    const int depth = GET_PARAM(1);
    const int channels = GET_PARAM(2);

    const int type = CV_MAKE_TYPE(depth, channels);

    cv::Mat src(size, type);
    declare.in(src, WARMUP_RNG);

    const cv::Scalar_<int> val = cv::Scalar_<int>::all(4);

    if (PERF_RUN_GPU())
    {
        const cv::gpu::GpuMat d_src(src);
        cv::gpu::GpuMat dst;

        TEST_CYCLE() cv::gpu::rshift(d_src, val, dst);

        GPU_SANITY_CHECK(dst);
    }
    else
    {
        FAIL_NO_CPU();
    }
}

//////////////////////////////////////////////////////////////////////
// LShift

PERF_TEST_P(Sz_Depth_Cn, Core_LShift,
            Combine(GPU_TYPICAL_MAT_SIZES,
                    Values(CV_8U, CV_16U, CV_32S),
                    GPU_CHANNELS_1_3_4))
{
    const cv::Size size = GET_PARAM(0);
    const int depth = GET_PARAM(1);
    const int channels = GET_PARAM(2);

    const int type = CV_MAKE_TYPE(depth, channels);

    cv::Mat src(size, type);
    declare.in(src, WARMUP_RNG);

    const cv::Scalar_<int> val = cv::Scalar_<int>::all(4);

    if (PERF_RUN_GPU())
    {
        const cv::gpu::GpuMat d_src(src);
        cv::gpu::GpuMat dst;

        TEST_CYCLE() cv::gpu::lshift(d_src, val, dst);

        GPU_SANITY_CHECK(dst);
    }
    else
    {
        FAIL_NO_CPU();
    }
}

//////////////////////////////////////////////////////////////////////
// MinMat

PERF_TEST_P(Sz_Depth, Core_MinMat,
            Combine(GPU_TYPICAL_MAT_SIZES,
                    Values(CV_8U, CV_16U, CV_32F)))
{
    const cv::Size size = GET_PARAM(0);
    const int depth = GET_PARAM(1);

    cv::Mat src1(size, depth);
    declare.in(src1, WARMUP_RNG);

    cv::Mat src2(size, depth);
    declare.in(src2, WARMUP_RNG);

    if (PERF_RUN_GPU())
    {
        const cv::gpu::GpuMat d_src1(src1);
        const cv::gpu::GpuMat d_src2(src2);
        cv::gpu::GpuMat dst;

        TEST_CYCLE() cv::gpu::min(d_src1, d_src2, dst);

        GPU_SANITY_CHECK(dst);
    }
    else
    {
        cv::Mat dst;

        TEST_CYCLE() cv::min(src1, src2, dst);

        SANITY_CHECK_NOTHING();
    }
}

//////////////////////////////////////////////////////////////////////
// MinScalar

PERF_TEST_P(Sz_Depth, Core_MinScalar,
            Combine(GPU_TYPICAL_MAT_SIZES,
                    Values(CV_8U, CV_16U, CV_32F)))
{
    const cv::Size size = GET_PARAM(0);
    const int depth = GET_PARAM(1);

    cv::Mat src(size, depth);
    declare.in(src, WARMUP_RNG);

    cv::Scalar val;
    declare.in(val, WARMUP_RNG);

    if (PERF_RUN_GPU())
    {
        const cv::gpu::GpuMat d_src(src);
        cv::gpu::GpuMat dst;

        TEST_CYCLE() cv::gpu::min(d_src, val[0], dst);

        GPU_SANITY_CHECK(dst);
    }
    else
    {
        cv::Mat dst;

        TEST_CYCLE() cv::min(src, val[0], dst);

        SANITY_CHECK_NOTHING();
    }
}

//////////////////////////////////////////////////////////////////////
// MaxMat

PERF_TEST_P(Sz_Depth, Core_MaxMat,
            Combine(GPU_TYPICAL_MAT_SIZES,
                    Values(CV_8U, CV_16U, CV_32F)))
{
    const cv::Size size = GET_PARAM(0);
    const int depth = GET_PARAM(1);

    cv::Mat src1(size, depth);
    declare.in(src1, WARMUP_RNG);

    cv::Mat src2(size, depth);
    declare.in(src2, WARMUP_RNG);

    if (PERF_RUN_GPU())
    {
        const cv::gpu::GpuMat d_src1(src1);
        const cv::gpu::GpuMat d_src2(src2);
        cv::gpu::GpuMat dst;

        TEST_CYCLE() cv::gpu::max(d_src1, d_src2, dst);

        GPU_SANITY_CHECK(dst);
    }
    else
    {
        cv::Mat dst;

        TEST_CYCLE() cv::max(src1, src2, dst);

        SANITY_CHECK_NOTHING();
    }
}

//////////////////////////////////////////////////////////////////////
// MaxScalar

PERF_TEST_P(Sz_Depth, Core_MaxScalar,
            Combine(GPU_TYPICAL_MAT_SIZES,
                    Values(CV_8U, CV_16U, CV_32F)))
{
    const cv::Size size = GET_PARAM(0);
    const int depth = GET_PARAM(1);

    cv::Mat src(size, depth);
    declare.in(src, WARMUP_RNG);

    cv::Scalar val;
    declare.in(val, WARMUP_RNG);

    if (PERF_RUN_GPU())
    {
        const cv::gpu::GpuMat d_src(src);
        cv::gpu::GpuMat dst;

        TEST_CYCLE() cv::gpu::max(d_src, val[0], dst);

        GPU_SANITY_CHECK(dst);
    }
    else
    {
        cv::Mat dst;

        TEST_CYCLE() cv::max(src, val[0], dst);

        SANITY_CHECK_NOTHING();
    }
}

//////////////////////////////////////////////////////////////////////
// AddWeighted

DEF_PARAM_TEST(Sz_3Depth, cv::Size, MatDepth, MatDepth, MatDepth);

PERF_TEST_P(Sz_3Depth, Core_AddWeighted,
            Combine(GPU_TYPICAL_MAT_SIZES,
                    Values(CV_8U, CV_16U, CV_32F, CV_64F),
                    Values(CV_8U, CV_16U, CV_32F, CV_64F),
                    Values(CV_8U, CV_16U, CV_32F, CV_64F)))
{
    const cv::Size size = GET_PARAM(0);
    const int depth1 = GET_PARAM(1);
    const int depth2 = GET_PARAM(2);
    const int dst_depth = GET_PARAM(3);

    cv::Mat src1(size, depth1);
    declare.in(src1, WARMUP_RNG);

    cv::Mat src2(size, depth2);
    declare.in(src2, WARMUP_RNG);

    if (PERF_RUN_GPU())
    {
        const cv::gpu::GpuMat d_src1(src1);
        const cv::gpu::GpuMat d_src2(src2);
        cv::gpu::GpuMat dst;

        TEST_CYCLE() cv::gpu::addWeighted(d_src1, 0.5, d_src2, 0.5, 10.0, dst, dst_depth);

        GPU_SANITY_CHECK(dst, 1e-10);
    }
    else
    {
        cv::Mat dst;

        TEST_CYCLE() cv::addWeighted(src1, 0.5, src2, 0.5, 10.0, dst, dst_depth);

        SANITY_CHECK_NOTHING();
    }
}

//////////////////////////////////////////////////////////////////////
// GEMM

#ifdef HAVE_CUBLAS

CV_FLAGS(GemmFlags, 0, GEMM_1_T, GEMM_2_T, GEMM_3_T)
#define ALL_GEMM_FLAGS Values(0, CV_GEMM_A_T, CV_GEMM_B_T, CV_GEMM_C_T, CV_GEMM_A_T | CV_GEMM_B_T, CV_GEMM_A_T | CV_GEMM_C_T, CV_GEMM_A_T | CV_GEMM_B_T | CV_GEMM_C_T)

DEF_PARAM_TEST(Sz_Type_Flags, cv::Size, MatType, GemmFlags);

PERF_TEST_P(Sz_Type_Flags, Core_GEMM,
            Combine(Values(cv::Size(512, 512), cv::Size(1024, 1024)),
                    Values(CV_32FC1, CV_32FC2, CV_64FC1),
                    ALL_GEMM_FLAGS))
{
    const cv::Size size = GET_PARAM(0);
    const int type = GET_PARAM(1);
    const int flags = GET_PARAM(2);

    cv::Mat src1(size, type);
    declare.in(src1, WARMUP_RNG);

    cv::Mat src2(size, type);
    declare.in(src2, WARMUP_RNG);

    cv::Mat src3(size, type);
    declare.in(src3, WARMUP_RNG);

    if (PERF_RUN_GPU())
    {
        declare.time(5.0);

        const cv::gpu::GpuMat d_src1(src1);
        const cv::gpu::GpuMat d_src2(src2);
        const cv::gpu::GpuMat d_src3(src3);
        cv::gpu::GpuMat dst;

        TEST_CYCLE() cv::gpu::gemm(d_src1, d_src2, 1.0, d_src3, 1.0, dst, flags);

        GPU_SANITY_CHECK(dst, 1e-6, ERROR_RELATIVE);
    }
    else
    {
        declare.time(50.0);

        cv::Mat dst;

        TEST_CYCLE() cv::gemm(src1, src2, 1.0, src3, 1.0, dst, flags);

        SANITY_CHECK_NOTHING();
    }
}

#endif

//////////////////////////////////////////////////////////////////////
// Transpose

PERF_TEST_P(Sz_Type, Core_Transpose,
            Combine(GPU_TYPICAL_MAT_SIZES,
                    Values(CV_8UC1, CV_8UC4, CV_16UC2, CV_16SC2, CV_32SC1, CV_32SC2, CV_64FC1)))
{
    const cv::Size size = GET_PARAM(0);
    const int type = GET_PARAM(1);

    cv::Mat src(size, type);
    declare.in(src, WARMUP_RNG);

    if (PERF_RUN_GPU())
    {
        const cv::gpu::GpuMat d_src(src);
        cv::gpu::GpuMat dst;

        TEST_CYCLE() cv::gpu::transpose(d_src, dst);

        GPU_SANITY_CHECK(dst, 1e-10);
    }
    else
    {
        cv::Mat dst;

        TEST_CYCLE() cv::transpose(src, dst);

        SANITY_CHECK_NOTHING();
    }
}

//////////////////////////////////////////////////////////////////////
// Flip

enum {FLIP_BOTH = 0, FLIP_X = 1, FLIP_Y = -1};
CV_ENUM(FlipCode, FLIP_BOTH, FLIP_X, FLIP_Y)

DEF_PARAM_TEST(Sz_Depth_Cn_Code, cv::Size, MatDepth, MatCn, FlipCode);

PERF_TEST_P(Sz_Depth_Cn_Code, Core_Flip,
            Combine(GPU_TYPICAL_MAT_SIZES,
                    Values(CV_8U, CV_16U, CV_32F),
                    GPU_CHANNELS_1_3_4,
                    FlipCode::all()))
{
    const cv::Size size = GET_PARAM(0);
    const int depth = GET_PARAM(1);
    const int channels = GET_PARAM(2);
    const int flipCode = GET_PARAM(3);

    const int type = CV_MAKE_TYPE(depth, channels);

    cv::Mat src(size, type);
    declare.in(src, WARMUP_RNG);

    if (PERF_RUN_GPU())
    {
        const cv::gpu::GpuMat d_src(src);
        cv::gpu::GpuMat dst;

        TEST_CYCLE() cv::gpu::flip(d_src, dst, flipCode);

        GPU_SANITY_CHECK(dst);
    }
    else
    {
        cv::Mat dst;

        TEST_CYCLE() cv::flip(src, dst, flipCode);

        SANITY_CHECK_NOTHING();
    }
}

//////////////////////////////////////////////////////////////////////
// LutOneChannel

PERF_TEST_P(Sz_Type, Core_LutOneChannel,
            Combine(GPU_TYPICAL_MAT_SIZES,
                    Values(CV_8UC1, CV_8UC3)))
{
    const cv::Size size = GET_PARAM(0);
    const int type = GET_PARAM(1);

    cv::Mat src(size, type);
    declare.in(src, WARMUP_RNG);

    cv::Mat lut(1, 256, CV_8UC1);
    declare.in(lut, WARMUP_RNG);

    if (PERF_RUN_GPU())
    {
        const cv::gpu::GpuMat d_src(src);
        cv::gpu::GpuMat dst;

        TEST_CYCLE() cv::gpu::LUT(d_src, lut, dst);

        GPU_SANITY_CHECK(dst);
    }
    else
    {
        cv::Mat dst;

        TEST_CYCLE() cv::LUT(src, lut, dst);

        SANITY_CHECK_NOTHING();
    }
}

//////////////////////////////////////////////////////////////////////
// LutMultiChannel

PERF_TEST_P(Sz_Type, Core_LutMultiChannel,
            Combine(GPU_TYPICAL_MAT_SIZES,
                    Values<MatType>(CV_8UC3)))
{
    const cv::Size size = GET_PARAM(0);
    const int type = GET_PARAM(1);

    cv::Mat src(size, type);
    declare.in(src, WARMUP_RNG);

    cv::Mat lut(1, 256, CV_MAKE_TYPE(CV_8U, src.channels()));
    declare.in(lut, WARMUP_RNG);

    if (PERF_RUN_GPU())
    {
        const cv::gpu::GpuMat d_src(src);
        cv::gpu::GpuMat dst;

        TEST_CYCLE() cv::gpu::LUT(d_src, lut, dst);

        GPU_SANITY_CHECK(dst);
    }
    else
    {
        cv::Mat dst;

        TEST_CYCLE() cv::LUT(src, lut, dst);

        SANITY_CHECK_NOTHING();
    }
}

//////////////////////////////////////////////////////////////////////
// MagnitudeComplex

PERF_TEST_P(Sz, Core_MagnitudeComplex,
            GPU_TYPICAL_MAT_SIZES)
{
    const cv::Size size = GetParam();

    cv::Mat src(size, CV_32FC2);
    declare.in(src, WARMUP_RNG);

    if (PERF_RUN_GPU())
    {
        const cv::gpu::GpuMat d_src(src);
        cv::gpu::GpuMat dst;

        TEST_CYCLE() cv::gpu::magnitude(d_src, dst);

        GPU_SANITY_CHECK(dst);
    }
    else
    {
        cv::Mat xy[2];
        cv::split(src, xy);

        cv::Mat dst;

        TEST_CYCLE() cv::magnitude(xy[0], xy[1], dst);

        SANITY_CHECK_NOTHING();
    }
}

//////////////////////////////////////////////////////////////////////
// MagnitudeSqrComplex

PERF_TEST_P(Sz, Core_MagnitudeSqrComplex,
            GPU_TYPICAL_MAT_SIZES)
{
    const cv::Size size = GetParam();

    cv::Mat src(size, CV_32FC2);
    declare.in(src, WARMUP_RNG);

    if (PERF_RUN_GPU())
    {
        const cv::gpu::GpuMat d_src(src);
        cv::gpu::GpuMat dst;

        TEST_CYCLE() cv::gpu::magnitudeSqr(d_src, dst);

        GPU_SANITY_CHECK(dst);
    }
    else
    {
        FAIL_NO_CPU();
    }
}

//////////////////////////////////////////////////////////////////////
// Magnitude

PERF_TEST_P(Sz, Core_Magnitude,
            GPU_TYPICAL_MAT_SIZES)
{
    const cv::Size size = GetParam();

    cv::Mat src1(size, CV_32FC1);
    declare.in(src1, WARMUP_RNG);

    cv::Mat src2(size, CV_32FC1);
    declare.in(src2, WARMUP_RNG);

    if (PERF_RUN_GPU())
    {
        const cv::gpu::GpuMat d_src1(src1);
        const cv::gpu::GpuMat d_src2(src2);
        cv::gpu::GpuMat dst;

        TEST_CYCLE() cv::gpu::magnitude(d_src1, d_src2, dst);

        GPU_SANITY_CHECK(dst);
    }
    else
    {
        cv::Mat dst;

        TEST_CYCLE() cv::magnitude(src1, src2, dst);

        SANITY_CHECK_NOTHING();
    }
}

//////////////////////////////////////////////////////////////////////
// MagnitudeSqr

PERF_TEST_P(Sz, Core_MagnitudeSqr,
            GPU_TYPICAL_MAT_SIZES)
{
    const cv::Size size = GetParam();

    cv::Mat src1(size, CV_32FC1);
    declare.in(src1, WARMUP_RNG);

    cv::Mat src2(size, CV_32FC1);
    declare.in(src2, WARMUP_RNG);

    if (PERF_RUN_GPU())
    {
        const cv::gpu::GpuMat d_src1(src1);
        const cv::gpu::GpuMat d_src2(src2);
        cv::gpu::GpuMat dst;

        TEST_CYCLE() cv::gpu::magnitudeSqr(d_src1, d_src2, dst);

        GPU_SANITY_CHECK(dst);
    }
    else
    {
        FAIL_NO_CPU();
    }
}

//////////////////////////////////////////////////////////////////////
// Phase

DEF_PARAM_TEST(Sz_AngleInDegrees, cv::Size, bool);

PERF_TEST_P(Sz_AngleInDegrees, Core_Phase,
            Combine(GPU_TYPICAL_MAT_SIZES,
                    Bool()))
{
    const cv::Size size = GET_PARAM(0);
    const bool angleInDegrees = GET_PARAM(1);

    cv::Mat src1(size, CV_32FC1);
    declare.in(src1, WARMUP_RNG);

    cv::Mat src2(size, CV_32FC1);
    declare.in(src2, WARMUP_RNG);

    if (PERF_RUN_GPU())
    {
        const cv::gpu::GpuMat d_src1(src1);
        const cv::gpu::GpuMat d_src2(src2);
        cv::gpu::GpuMat dst;

        TEST_CYCLE() cv::gpu::phase(d_src1, d_src2, dst, angleInDegrees);

        GPU_SANITY_CHECK(dst, 1e-6, ERROR_RELATIVE);
    }
    else
    {
        cv::Mat dst;

        TEST_CYCLE() cv::phase(src1, src2, dst, angleInDegrees);

        SANITY_CHECK_NOTHING();
    }
}

//////////////////////////////////////////////////////////////////////
// CartToPolar

PERF_TEST_P(Sz_AngleInDegrees, Core_CartToPolar,
            Combine(GPU_TYPICAL_MAT_SIZES,
                    Bool()))
{
    const cv::Size size = GET_PARAM(0);
    const bool angleInDegrees = GET_PARAM(1);

    cv::Mat src1(size, CV_32FC1);
    declare.in(src1, WARMUP_RNG);

    cv::Mat src2(size, CV_32FC1);
    declare.in(src2, WARMUP_RNG);

    if (PERF_RUN_GPU())
    {
        const cv::gpu::GpuMat d_src1(src1);
        const cv::gpu::GpuMat d_src2(src2);
        cv::gpu::GpuMat magnitude;
        cv::gpu::GpuMat angle;

        TEST_CYCLE() cv::gpu::cartToPolar(d_src1, d_src2, magnitude, angle, angleInDegrees);

        GPU_SANITY_CHECK(magnitude);
        GPU_SANITY_CHECK(angle, 1e-6, ERROR_RELATIVE);
    }
    else
    {
        cv::Mat magnitude;
        cv::Mat angle;

        TEST_CYCLE() cv::cartToPolar(src1, src2, magnitude, angle, angleInDegrees);

        SANITY_CHECK_NOTHING();
    }
}

//////////////////////////////////////////////////////////////////////
// PolarToCart

PERF_TEST_P(Sz_AngleInDegrees, Core_PolarToCart,
            Combine(GPU_TYPICAL_MAT_SIZES,
                    Bool()))
{
    const cv::Size size = GET_PARAM(0);
    const bool angleInDegrees = GET_PARAM(1);

    cv::Mat magnitude(size, CV_32FC1);
    declare.in(magnitude, WARMUP_RNG);

    cv::Mat angle(size, CV_32FC1);
    declare.in(angle, WARMUP_RNG);

    if (PERF_RUN_GPU())
    {
        const cv::gpu::GpuMat d_magnitude(magnitude);
        const cv::gpu::GpuMat d_angle(angle);
        cv::gpu::GpuMat x;
        cv::gpu::GpuMat y;

        TEST_CYCLE() cv::gpu::polarToCart(d_magnitude, d_angle, x, y, angleInDegrees);

        GPU_SANITY_CHECK(x);
        GPU_SANITY_CHECK(y);
    }
    else
    {
        cv::Mat x;
        cv::Mat y;

        TEST_CYCLE() cv::polarToCart(magnitude, angle, x, y, angleInDegrees);

        SANITY_CHECK_NOTHING();
    }
}

//////////////////////////////////////////////////////////////////////
// MeanStdDev

PERF_TEST_P(Sz, Core_MeanStdDev,
            GPU_TYPICAL_MAT_SIZES)
{
    const cv::Size size = GetParam();

    cv::Mat src(size, CV_8UC1);
    declare.in(src, WARMUP_RNG);


    if (PERF_RUN_GPU())
    {
        const cv::gpu::GpuMat d_src(src);
        cv::gpu::GpuMat d_buf;
        cv::Scalar gpu_mean;
        cv::Scalar gpu_stddev;

        TEST_CYCLE() cv::gpu::meanStdDev(d_src, gpu_mean, gpu_stddev, d_buf);

        SANITY_CHECK(gpu_mean);
        SANITY_CHECK(gpu_stddev);
    }
    else
    {
        cv::Scalar cpu_mean;
        cv::Scalar cpu_stddev;

        TEST_CYCLE() cv::meanStdDev(src, cpu_mean, cpu_stddev);

        SANITY_CHECK_NOTHING();
    }
}

//////////////////////////////////////////////////////////////////////
// Norm

DEF_PARAM_TEST(Sz_Depth_Norm, cv::Size, MatDepth, NormType);

PERF_TEST_P(Sz_Depth_Norm, Core_Norm,
            Combine(GPU_TYPICAL_MAT_SIZES,
                    Values(CV_8U, CV_16U, CV_32S, CV_32F),
                    Values(NormType(cv::NORM_INF), NormType(cv::NORM_L1), NormType(cv::NORM_L2))))
{
    const cv::Size size = GET_PARAM(0);
    const int depth = GET_PARAM(1);
    const int normType = GET_PARAM(2);

    cv::Mat src(size, depth);
    if (depth == CV_8U)
        cv::randu(src, 0, 254);
    else
        declare.in(src, WARMUP_RNG);

    if (PERF_RUN_GPU())
    {
        const cv::gpu::GpuMat d_src(src);
        cv::gpu::GpuMat d_buf;
        double gpu_dst;

        TEST_CYCLE() gpu_dst = cv::gpu::norm(d_src, normType, d_buf);

        SANITY_CHECK(gpu_dst, 1e-6, ERROR_RELATIVE);
    }
    else
    {
        TEST_CYCLE() cv::norm(src, normType);

        SANITY_CHECK_NOTHING();
    }
}

//////////////////////////////////////////////////////////////////////
// NormDiff

DEF_PARAM_TEST(Sz_Norm, cv::Size, NormType);

PERF_TEST_P(Sz_Norm, Core_NormDiff,
            Combine(GPU_TYPICAL_MAT_SIZES,
                    Values(NormType(cv::NORM_INF), NormType(cv::NORM_L1), NormType(cv::NORM_L2))))
{
    const cv::Size size = GET_PARAM(0);
    const int normType = GET_PARAM(1);

    cv::Mat src1(size, CV_8UC1);
    declare.in(src1, WARMUP_RNG);

    cv::Mat src2(size, CV_8UC1);
    declare.in(src2, WARMUP_RNG);

    if (PERF_RUN_GPU())
    {
        const cv::gpu::GpuMat d_src1(src1);
        const cv::gpu::GpuMat d_src2(src2);
        double gpu_dst;

        TEST_CYCLE() gpu_dst = cv::gpu::norm(d_src1, d_src2, normType);

        SANITY_CHECK(gpu_dst);

    }
    else
    {
        TEST_CYCLE() cv::norm(src1, src2, normType);

        SANITY_CHECK_NOTHING();
    }
}

//////////////////////////////////////////////////////////////////////
// Sum

PERF_TEST_P(Sz_Depth_Cn, Core_Sum,
            Combine(GPU_TYPICAL_MAT_SIZES,
                    Values(CV_8U, CV_16U, CV_32F),
                    GPU_CHANNELS_1_3_4))
{
    const cv::Size size = GET_PARAM(0);
    const int depth = GET_PARAM(1);
    const int channels = GET_PARAM(2);

    const int type = CV_MAKE_TYPE(depth, channels);

    cv::Mat src(size, type);
    declare.in(src, WARMUP_RNG);

    if (PERF_RUN_GPU())
    {
        const cv::gpu::GpuMat d_src(src);
        cv::gpu::GpuMat d_buf;
        cv::Scalar gpu_dst;

        TEST_CYCLE() gpu_dst = cv::gpu::sum(d_src, d_buf);

        SANITY_CHECK(gpu_dst, 1e-5, ERROR_RELATIVE);
    }
    else
    {
        cv::Scalar cpu_dst;

        TEST_CYCLE() cpu_dst = cv::sum(src);

        SANITY_CHECK_NOTHING();
    }
}

//////////////////////////////////////////////////////////////////////
// SumAbs

PERF_TEST_P(Sz_Depth_Cn, Core_SumAbs,
            Combine(GPU_TYPICAL_MAT_SIZES,
                    Values(CV_8U, CV_16U, CV_32F),
                    GPU_CHANNELS_1_3_4))
{
    const cv::Size size = GET_PARAM(0);
    const int depth = GET_PARAM(1);
    const int channels = GET_PARAM(2);

    const int type = CV_MAKE_TYPE(depth, channels);

    cv::Mat src(size, type);
    declare.in(src, WARMUP_RNG);

    if (PERF_RUN_GPU())
    {
        const cv::gpu::GpuMat d_src(src);
        cv::gpu::GpuMat d_buf;
        cv::Scalar gpu_dst;

        TEST_CYCLE() gpu_dst = cv::gpu::absSum(d_src, d_buf);

        SANITY_CHECK(gpu_dst, 1e-6, ERROR_RELATIVE);
    }
    else
    {
        FAIL_NO_CPU();
    }
}

//////////////////////////////////////////////////////////////////////
// SumSqr

PERF_TEST_P(Sz_Depth_Cn, Core_SumSqr,
            Combine(GPU_TYPICAL_MAT_SIZES,
                    Values<MatDepth>(CV_8U, CV_16U, CV_32F),
                    GPU_CHANNELS_1_3_4))
{
    const cv::Size size = GET_PARAM(0);
    const int depth = GET_PARAM(1);
    const int channels = GET_PARAM(2);

    const int type = CV_MAKE_TYPE(depth, channels);

    cv::Mat src(size, type);
    declare.in(src, WARMUP_RNG);

    if (PERF_RUN_GPU())
    {
        const cv::gpu::GpuMat d_src(src);
        cv::gpu::GpuMat d_buf;
        cv::Scalar gpu_dst;

        TEST_CYCLE() gpu_dst = cv::gpu::sqrSum(d_src, d_buf);

        SANITY_CHECK(gpu_dst, 1e-6, ERROR_RELATIVE);
    }
    else
    {
        FAIL_NO_CPU();
    }
}

//////////////////////////////////////////////////////////////////////
// MinMax

PERF_TEST_P(Sz_Depth, Core_MinMax,
            Combine(GPU_TYPICAL_MAT_SIZES,
                    Values(CV_8U, CV_16U, CV_32F, CV_64F)))
{
    const cv::Size size = GET_PARAM(0);
    const int depth = GET_PARAM(1);

    cv::Mat src(size, depth);
    if (depth == CV_8U)
        cv::randu(src, 0, 254);
    else
        declare.in(src, WARMUP_RNG);

    if (PERF_RUN_GPU())
    {
        const cv::gpu::GpuMat d_src(src);
        cv::gpu::GpuMat d_buf;
        double gpu_minVal, gpu_maxVal;

        TEST_CYCLE() cv::gpu::minMax(d_src, &gpu_minVal, &gpu_maxVal, cv::gpu::GpuMat(), d_buf);

        SANITY_CHECK(gpu_minVal, 1e-10);
        SANITY_CHECK(gpu_maxVal, 1e-10);
    }
    else
    {
        double cpu_minVal, cpu_maxVal;

        TEST_CYCLE() cv::minMaxLoc(src, &cpu_minVal, &cpu_maxVal);

        SANITY_CHECK_NOTHING();
    }
}

//////////////////////////////////////////////////////////////////////
// MinMaxLoc

PERF_TEST_P(Sz_Depth, Core_MinMaxLoc,
            Combine(GPU_TYPICAL_MAT_SIZES,
                    Values(CV_8U, CV_16U, CV_32F, CV_64F)))
{
    const cv::Size size = GET_PARAM(0);
    const int depth = GET_PARAM(1);

    cv::Mat src(size, depth);
    if (depth == CV_8U)
        cv::randu(src, 0, 254);
    else
        declare.in(src, WARMUP_RNG);

    if (PERF_RUN_GPU())
    {
        const cv::gpu::GpuMat d_src(src);
        cv::gpu::GpuMat d_valbuf, d_locbuf;
        double gpu_minVal, gpu_maxVal;
        cv::Point gpu_minLoc, gpu_maxLoc;

        TEST_CYCLE() cv::gpu::minMaxLoc(d_src, &gpu_minVal, &gpu_maxVal, &gpu_minLoc, &gpu_maxLoc, cv::gpu::GpuMat(), d_valbuf, d_locbuf);

        SANITY_CHECK(gpu_minVal, 1e-10);
        SANITY_CHECK(gpu_maxVal, 1e-10);
    }
    else
    {
        double cpu_minVal, cpu_maxVal;
        cv::Point cpu_minLoc, cpu_maxLoc;

        TEST_CYCLE() cv::minMaxLoc(src, &cpu_minVal, &cpu_maxVal, &cpu_minLoc, &cpu_maxLoc);

        SANITY_CHECK_NOTHING();
    }
}

//////////////////////////////////////////////////////////////////////
// CountNonZero

PERF_TEST_P(Sz_Depth, Core_CountNonZero,
            Combine(GPU_TYPICAL_MAT_SIZES,
                    Values(CV_8U, CV_16U, CV_32F, CV_64F)))
{
    const cv::Size size = GET_PARAM(0);
    const int depth = GET_PARAM(1);

    cv::Mat src(size, depth);
    declare.in(src, WARMUP_RNG);

    if (PERF_RUN_GPU())
    {
        const cv::gpu::GpuMat d_src(src);
        cv::gpu::GpuMat d_buf;
        int gpu_dst = 0;

        TEST_CYCLE() gpu_dst = cv::gpu::countNonZero(d_src, d_buf);

        SANITY_CHECK(gpu_dst);
    }
    else
    {
        TEST_CYCLE() cv::countNonZero(src);

        SANITY_CHECK_NOTHING();
    }
}

//////////////////////////////////////////////////////////////////////
// Reduce

enum {Rows = 0, Cols = 1};
CV_ENUM(ReduceCode, CV_REDUCE_SUM, CV_REDUCE_AVG, CV_REDUCE_MAX, CV_REDUCE_MIN)
CV_ENUM(ReduceDim, Rows, Cols)

DEF_PARAM_TEST(Sz_Depth_Cn_Code_Dim, cv::Size, MatDepth, MatCn, ReduceCode, ReduceDim);

PERF_TEST_P(Sz_Depth_Cn_Code_Dim, Core_Reduce,
            Combine(GPU_TYPICAL_MAT_SIZES,
                    Values(CV_8U, CV_16U, CV_16S, CV_32F),
                    Values(1, 2, 3, 4),
                    ReduceCode::all(),
                    ReduceDim::all()))
{
    const cv::Size size = GET_PARAM(0);
    const int depth = GET_PARAM(1);
    const int channels = GET_PARAM(2);
    const int reduceOp = GET_PARAM(3);
    const int dim = GET_PARAM(4);

    const int type = CV_MAKE_TYPE(depth, channels);

    cv::Mat src(size, type);
    declare.in(src, WARMUP_RNG);

    if (PERF_RUN_GPU())
    {
        const cv::gpu::GpuMat d_src(src);
        cv::gpu::GpuMat dst;

        TEST_CYCLE() cv::gpu::reduce(d_src, dst, dim, reduceOp);

        GPU_SANITY_CHECK(dst);
    }
    else
    {
        cv::Mat dst;

        TEST_CYCLE() cv::reduce(src, dst, dim, reduceOp);

        SANITY_CHECK_NOTHING();
    }
}
//////////////////////////////////////////////////////////////////////
// Normalize

DEF_PARAM_TEST(Sz_Depth_NormType, cv::Size, MatDepth, NormType);

PERF_TEST_P(Sz_Depth_NormType, Core_Normalize,
            Combine(GPU_TYPICAL_MAT_SIZES,
                    Values(CV_8U, CV_16U, CV_32F, CV_64F),
                    Values(NormType(cv::NORM_INF),
                           NormType(cv::NORM_L1),
                           NormType(cv::NORM_L2),
                           NormType(cv::NORM_MINMAX))))
{
    const cv::Size size = GET_PARAM(0);
    const int type = GET_PARAM(1);
    const int norm_type = GET_PARAM(2);

    const double alpha = 1;
    const double beta = 0;

    cv::Mat src(size, type);
    declare.in(src, WARMUP_RNG);

    if (PERF_RUN_GPU())
    {
        const cv::gpu::GpuMat d_src(src);
        cv::gpu::GpuMat dst;
        cv::gpu::GpuMat d_norm_buf, d_cvt_buf;

        TEST_CYCLE() cv::gpu::normalize(d_src, dst, alpha, beta, norm_type, type, cv::gpu::GpuMat(), d_norm_buf, d_cvt_buf);

        GPU_SANITY_CHECK(dst, 1e-6);
    }
    else
    {
        cv::Mat dst;

        TEST_CYCLE() cv::normalize(src, dst, alpha, beta, norm_type, type);

        SANITY_CHECK_NOTHING();
    }
}
