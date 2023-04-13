/*
 * @Author: tangwc
 * @Date: 2023-03-20 17:10:53
 * @LastEditors: tangwc tangwc@chipsea.com
 * @LastEditTime: 2023-03-20 17:14:11
 * @Description: 
 * @FilePath: \math_cbb\fast_math.h
 *  
 *  Copyright (c) 2022 by tangwc, All Rights Reserved.
 */
#ifndef __FAST_MATH_H__
#define __FAST_MATH_H__

#include <stdint.h>

 /**
  * @description: 快速2^x快速运算
  * @param {float} x: 指数
  * @return {float} 2^x的结果
  */
float fast_exp2(float x);

/**
 * @description: 快速平方运算
 * @param {float} base: 底数
 * @param {float} exponent: 指数
 * @return {float} x^y的结果
 */
float fast_powf(float base, float exponent);

/**
 * @description: exp指数快速运算
 * @param {double} y:指数
 * @return {double} e^y运行结果
 */
double FastExp(double y);

/**
 * @description: 快速log2(x)运算
 * @param {float} x:真数
 * @return {float} log2(x)的结果
 */
float fast_log2(float x);

/**
 * @description: 快速ln(x)运算
 * @param {double} x:真数
 * @return {double} log2(x)的结果
 */
double FastLog(double x);

/**
 * @description: 快速平方根
 * @param {float} number: 被开方数
 * @return {float} 开方结果
 */
float Q_sqrt( float number );

/**
 * @description: 快速平方根求导
 * @param {float} number: 被开方数
 * @return {float} 平方根求导结果
 */
float Q_rsqrt( float number );

/**
 * @description: 快速sin运算
 * @param {float} x:角度
 * @return {float} sin 结果
 */
float fast_sinf(float x);

/**
 * @description: 快速cos运算
 * @param {float} x:角度
 * @return {float} cos 结果
 */
float fast_cosf(float x);

#endif

