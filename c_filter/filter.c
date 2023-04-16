/*
 * @Description: 
 * @Version: 
 * @Autor: tangwc
 * @Date: 2023-04-15 19:48:20
 * @LastEditors: tangwc
 * @LastEditTime: 2023-04-16 16:25:08
 * @FilePath: \c_filter\filter.c
 * 
 *  Copyright (c) 2023 by tangwc, All Rights Reserved. 
 */
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>



typedef double single_type; //信号数据类型
typedef double filter_type; //滤波器数据类型

//滤波器滤波阶数选择
#define FILTER_ORDER_1 13         
#define FILTER_ORDER_2 13
#define FILTER_ORDER_3 13
#define FILTER_ORDER_4 13
#define FILTER_ORDER_5 13

//滤波器系数设定
const filter_type a_1[FILTER_ORDER_1] = { 1, -8.16373220837048, 30.8331081306628, -71.5088925157920, 113.775507892350, -131.100753923188,
                                        112.287728681655,-72.0412431122408, 34.3554222557604, -11.8735784088283, 2.82202575235606, -0.413933808568325,
                                        0.0283414576932032 };
const filter_type b_1[FILTER_ORDER_1] = { 0.00188432874045137, 0, -0.0113059724427082, 0, 0.0282649311067706, 0, -0.0376865748090275, 0,
                                          0.0282649311067706,0, -0.0113059724427082, 0, 0.00188432874045137 };

const filter_type a_2[FILTER_ORDER_2] = { 0 };
const filter_type b_2[FILTER_ORDER_2] = { 0 };

const filter_type a_3[FILTER_ORDER_3] = { 0 };
const filter_type b_3[FILTER_ORDER_3] = { 0 };

const filter_type a_4[FILTER_ORDER_4] = { 0 };
const filter_type b_4[FILTER_ORDER_4] = { 0 };

const filter_type a_5[FILTER_ORDER_5] = { 0 };
const filter_type b_5[FILTER_ORDER_5] = { 0 };


typedef struct
{
        uint32_t init_flag;   //init_flag = 1表示初始化后未进行初始值（用于码值差分）设定
        single_type init_adc;   //记录第一笔码值，用于差分计算
        filter_type* input_save;   //滤波中间参数
        filter_type* output_save;  //滤波中间参数
        uint32_t filter_length;  //滤波器长度
        const filter_type* a;  //滤波器参数
        const filter_type* b;  //滤波器参数
}filter_info;

//滤波器种类选择
typedef enum
{
        FILTER_TYPE_1 = 1,
        FILTER_TYPE_2,
        FILTER_TYPE_3,
        FILTER_TYPE_4,
        FILTER_TYPE_5
}filter_species_t;

/**
 * @description: 初始化滤波器信息
 * @param {filter_type} data_type:
 * @return {*}
 */
filter_info* filter_init(filter_species_t data_type)
{
        //filter_info* _filter = (filter_info*)malloc(sizeof(filter_info));
        //分配内存空间后，对于分配空间初始化
        filter_info* _filter = (filter_info*)calloc(1, sizeof(filter_info));
        _filter->init_flag = 1;
        switch (data_type)
        {
        case FILTER_TYPE_1:
                _filter->filter_length = FILTER_ORDER_1;
                _filter->a = a_1;
                _filter->b = b_1;
                break;
        case FILTER_TYPE_2:
                _filter->filter_length = FILTER_ORDER_2;
                _filter->a = a_2;
                _filter->b = b_2;
                break;
        case FILTER_TYPE_3:
                _filter->filter_length = FILTER_ORDER_3;
                _filter->a = a_3;
                _filter->b = b_3;
                break;
        case FILTER_TYPE_4:
                _filter->filter_length = FILTER_ORDER_4;
                _filter->a = a_4;
                _filter->b = b_4;
                break;
        case FILTER_TYPE_5:
                _filter->filter_length = FILTER_ORDER_5;
                _filter->a = a_5;
                _filter->b = b_5;
                break;
        default:
                return NULL;
                break;
        }
        // _filter->input_save = (filter_type*)malloc((_filter->filter_length) * sizeof(filter_type));
        // _filter->output_save = (filter_type*)malloc((_filter->filter_length) * sizeof(filter_type));
        //分配内存空间后，对于分配空间初始化
        _filter->input_save = (filter_type*)calloc((_filter->filter_length), sizeof(filter_type));
        _filter->output_save = (filter_type*)calloc((_filter->filter_length), sizeof(filter_type));
        return _filter;
}

/**
* @description: 返回滤波后的数据并更新input_save, output_save
* @param {filter_info*} filter_info_para
* @param {int16_t} data_in
* @return {*}返回滤波后的数据
*/
filter_type signal_bandpass_filter_single(filter_info* filter_info_para, single_type single_in)
{
        filter_type filter_result = 0;
        int32_t index;

        if (filter_info_para->init_flag)//第一笔数据赋值
        {
                filter_info_para->init_adc = single_in;
                filter_info_para->init_flag = 0;
        }
        //输入差分
        single_type single_differ = (single_type)(single_in - filter_info_para->init_adc);  //所有数据和第一笔数据进行差分

        //更新input_save
        for (index = filter_info_para->filter_length - 1; index > 0; index--)
                filter_info_para->input_save[index] = filter_info_para->input_save[index - 1];
        filter_info_para->input_save[0] = single_differ;

        //滤波运算
        for (index = 0; index < filter_info_para->filter_length; index++)
                filter_result += filter_info_para->b[index] * filter_info_para->input_save[index];

        for (index = 1; index < filter_info_para->filter_length; index++)
                filter_result -= filter_info_para->a[index] * filter_info_para->output_save[index - 1];

        //更新output_save
        for (index = filter_info_para->filter_length - 1; index > 0; index--)
                filter_info_para->output_save[index] = filter_info_para->output_save[index - 1];
        filter_info_para->output_save[0] = filter_result;

        return filter_result;
}
/**
 * @description: 释放滤波器信息，释放内存
 * @param {filter_info} *_filter:
 * @return {*}
 */
uint32_t filter_free(filter_info* _filter)
{
        free(_filter->input_save);
        free(_filter->output_save);
        free(_filter);
        return 0;
}

#define single_len 320
int main(void)
{
        single_type single_data[single_len] = { 0 };
        filter_type filter_result[single_len] = { 0 };
        filter_info* filter = filter_init(FILTER_TYPE_1);//选择滤波器种类 
        for (int i = 0;i < single_len;i++)//滤波器点对点输入输出
        {
                filter_result[i] = signal_bandpass_filter_single(filter, single_data[i]);
        }
        filter_free(filter);//回收滤波器内存
        return 0;
}