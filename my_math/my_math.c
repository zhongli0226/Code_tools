/*
 * @Author: tangwc
 * @Date: 2023-03-20 17:10:53
 * @LastEditors: tangwc tangwc@chipsea.com
 * @LastEditTime: 2023-04-12 17:37:08
 * @Description:
 * @FilePath: \math_cbb\fast_math.c
 *
 *  Copyright (c) 2022 by tangwc, All Rights Reserved.
 */
#include <stdio.h>
#include <math.h>
#include "my_math.h"


 /**
  * @description: 快速2^x快速运算
  * @param {float} x: 指数
  * @return {float} 2^x的结果
  */
#define _BIT(n) (1<<(n))
float fast_exp2(float x)
{
    union { uint32_t i; float f; } v;
    float offset = (x < 0) ? 1.0f : 0.0f;
    float clipp = (x < -126) ? -126.0f : x;
    int w = clipp;
    float z = clipp - w + offset;
    v.i = (uint32_t)(_BIT(23) * (clipp + 121.2740575f + 27.7280233f / (4.84252568f - z) - 1.49012907f * z));
    return v.f;
}

/**
 * @description: 快速平方运算
 * @param {float} base: 底数
 * @param {float} exponent: 指数
 * @return {float} x^y的结果
 */
float fast_powf(float base, float exponent)
{
    return fast_exp2(fast_log2(base)*exponent);
}

/**
 * @description: exp指数快速运算
 * @param {double} y:指数
 * @return {double} e^y运行结果
 */
double FastExp(double y)
{
    double result;
    *((int*)(&result) + 0) = 0;
    *((int*)(&result) + 1) = 1512775 * y + 1072632447;
    return result;
}

/**
 * @description: 快速log2(x)运算
 * @param {float} x:真数
 * @return {float} log2(x)的结果
 */
float fast_log2(float x)
{
    union { float f; uint32_t i; } vx;
    union { uint32_t i; float f; } mx;
    vx.f = x;
    mx.i = (vx.i & 0x007FFFFF) | 0x3F000000;
    float y = vx.i;
    y *= 1.1920928955078125e-7f;
    return y - 124.22551499f - 1.498030302f * mx.f - 1.72587999f / (0.3520887068f + mx.f);
}

/**
 * @description: 快速ln(x)运算
 * @param {double} x:真数
 * @return {double} log2(x)的结果
 */
double FastLog(double x)
{
    double m;
    int k = 1, op = 2;
    double tmp = x / op;
    if (x <= 1) return 0;
    while (tmp >= 2)
    {
        op <<= 1;
        tmp = x / op;
        k++;
    }
    k--;
    m = tmp - 1;
    if (m <= 0.25) return 1.3125 * m + k;
    else if (m <= 0.5) return 1.078125 * m + 0.00015625 + k;
    else if (m <= 0.75) return 1.015625 * m + 0.0625 + k;
    else if (m <= 1) return 0.75 * m + 0.25 + k;
    else return 0;
}

/**
 * @description: 快速平方根
 * @param {float} number: 被开方数
 * @return {float} 开方结果
 */
float Q_sqrt( float number )
{
	int i;
	float x2, y;
	const float threehalfs = 1.5F;

	x2 = number * 0.5F;
	y  = number;
	i  = * ( int * ) &y;     
	i  = 0x5f375a86 - ( i >> 1 ); 
	y  = * ( float * ) &i;
	y  = y * ( threehalfs - ( x2 * y * y ) ); 
	y  = y * ( threehalfs - ( x2 * y * y ) );  	
	y  = y * ( threehalfs - ( x2 * y * y ) ); 
	return number*y;
}

/**
 * @description: 快速平方根求导
 * @param {float} number: 被开方数
 * @return {float} 平方根求导结果
 */
float Q_rsqrt( float number )
{
	long i;
	float x2, y;
	const float threehalfs = 1.5F;
 
	x2 = number * 0.5F;
	y  = number;
	i  = * ( long * ) &y;						// evil floating point bit level hacking
	i  = 0x5f3759df - ( i >> 1 );               // what the fuck?
	y  = * ( float * ) &i;
	y  = y * ( threehalfs - ( x2 * y * y ) );   // 1st iteration
//	y  = y * ( threehalfs - ( x2 * y * y ) );   // 2nd iteration, this can be removed
 
#ifndef Q3_VM
#ifdef __linux__
	assert( !isnan(y) ); // bk010122 - FPE?
#endif
#endif
	return y;
}


/**
 * @description: 快速sin/cos运算
 * @return {float} sin/cos结果
 */
#define M_TAU (6.28318530717958647692) //2*3.14.....
float fast_cosc(float x)
{
    x -= 0.25f + floorf(x + 0.25f);
    x *= 16.0f * (fabs(x) - 0.5f);
    x += 0.225f * x * (fabs(x) - 1.0f);
    return x;
}

float fast_sinf(float x)
{
    return fast_cosc(x/M_TAU - 0.25f);
}

float fast_cosf(float x)
{
    return fast_cosc(x/M_TAU);
}