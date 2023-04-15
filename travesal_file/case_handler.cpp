/*
 * @Description: 
 * @Version: 
 * @Autor: tangwc
 * @Date: 2023-04-13 15:33:20
 * @LastEditors: tangwc
 * @LastEditTime: 2023-04-15 19:30:32
 * @FilePath: \travesal_file\case_handler.cpp
 * 
 *  Copyright (c) 2023 by tangwc, All Rights Reserved. 
 */
#include <stdio.h>
#include <io.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <regex>
#include <map>
#include <direct.h>
#include <stdint.h>

#include "filehandler.h"
#include "case_handler.h"




#define CSV_1_READ_LEN 1  //文件1读取的长度
#define CSV_2_READ_LEN 1  //文件2读取的长度

//第一种文件每列数据名称
typedef struct csv_1_line
{
    int32_t data_1;
    int32_t data_2;
    int32_t data_3;
    int32_t data_4;
    int32_t data_5;
    int32_t data_6;
}csv_1_line;

//第二种文件每列数据名称
typedef struct csv_2_line
{
    int32_t data_1;
    int32_t data_2;
    int32_t data_3;
    int32_t data_4;
    int32_t data_5;
}csv_2_line;


//读取一次文件内容
int csv_1_case_get_data(FILE* fIn, int32_t* data_1, int32_t* data_2, int32_t* data_3, int32_t* data_4, int32_t* data_5, int32_t* data_6)
{
    csv_1_line data;
    for (int index = 0; index < CSV_1_READ_LEN; index++)
    {
        int ret = fscanf(fIn, "%d,%d,%d,%d,%d,%d\r\n", &data.data_1, &data.data_2, &data.data_3, &data.data_4, &data.data_5, &data.data_6);

        if (ret == EOF)
            return 1;

        data_1[index] = data.data_1;
        data_2[index] = data.data_2;
        data_3[index] = data.data_3;
        data_4[index] = data.data_4;
        data_5[index] = data.data_5;
        data_6[index] = data.data_6;
    }
    return 0;
}

int csv_2_case_get_data(FILE* fIn, int32_t* data_1, int32_t* data_2, int32_t* data_3, int32_t* data_4, int32_t* data_5)
{
    csv_2_line data;
    for (int index = 0; index < CSV_2_READ_LEN; index++)
    {
        int ret = fscanf(fIn, "%d,%d,%d,%d,%d\r\n", &data.data_1, &data.data_2, &data.data_3, &data.data_4, &data.data_5);

        if (ret == EOF)
            return 1;

        data_1[index] = data.data_1;
        data_2[index] = data.data_2;
        data_3[index] = data.data_3;
        data_4[index] = data.data_4;
        data_5[index] = data.data_5;
    }
    return 0;
}


//读取文件内所以东西，并处理生成结果保存在另一个文件下
int csv_case_handler(case_struct* file_case)
{
    printf("%s\r\n", file_case->file_1_name.c_str());
    int32_t csv1_data_1[CSV_1_READ_LEN], csv1_data_2[CSV_1_READ_LEN], \
        csv1_data_3[CSV_1_READ_LEN], csv1_data_4[CSV_1_READ_LEN], \
        csv1_data_5[CSV_1_READ_LEN], csv1_data_6[CSV_1_READ_LEN];

    int32_t csv2_data_1[CSV_2_READ_LEN], csv2_data_2[CSV_2_READ_LEN], \
        csv2_data_3[CSV_2_READ_LEN], csv2_data_4[CSV_2_READ_LEN], \
        csv2_data_5[CSV_2_READ_LEN];

    FILE* fIn_csv1 = fopen(file_case->file_1_name.c_str(), "r");
    FILE* fIn_csv2 = fopen(file_case->file_2_name.c_str(), "r");
    FILE* fOut_csv = fopen(file_case->result_file_name.c_str(), "w+");
    char c[1000];
    if (fgets(c, sizeof(c), fIn_csv1) == NULL)   //先读文件表头
    {
        fclose(fIn_csv1);
        return 1;
    }
    if (fgets(c, sizeof(c), fIn_csv2) == NULL)   //先读文件表头
    {
        fclose(fIn_csv2);
        return 1;
    }

    fputs("data_1,data_2,data_3\n", fOut_csv);
    uint32_t data[3] = { 0 };
    while ((csv_1_case_get_data(fIn_csv1, csv1_data_1, csv1_data_2, csv1_data_3, csv1_data_4, csv1_data_5, csv1_data_6) == 0) && csv_2_case_get_data(fIn_csv2, csv2_data_1, csv2_data_2, csv2_data_3, csv2_data_4, csv2_data_5))
    {

#if 0
        //result[0] = 0;result[1] = 0;
        for (int index = 0; index < CSV_1_READ_LEN; index++)
        {
            cout << csv1_data_1[index] << ", " << csv1_data_2[index] << ", " << csv1_data_3[index] << ", " << csv1_data_4[index] << ", " << csv1_data_5[index] << ", " << csv1_data_6[index] << endl;
        }
        for (int index = 0; index < CSV_2_READ_LEN; index++)
        {
            cout << csv1_data_1[index] << ", " << csv1_data_2[index] << ", " << csv1_data_3[index] << ", " << csv1_data_4[index] << ", " << csv1_data_5[index] << endl;
        }
#endif

        fprintf(fOut_csv, "%d,%d,%d\n", data[0], data[1], data[2]);
    }

    fclose(fIn_csv1);
    fclose(fIn_csv2);
    fclose(fOut_csv);
    return 0;
}


//将文件夹case_folder中的文件转化为用例 
void csv_file_case_classify(string case_folder, vector<case_struct>& csv_cases)
{
	//首先筛选所有的.csv文件
	vector<string> csv_files;
	string fileType = ".csv";
	getAllFiles(case_folder, csv_files, fileType);

	//c++的正则表达式好想不支持正向预查相关功能
	//筛选所有的csv1和csv2文件,并建立map
	map<string, string> csv1_map,csv2_map;
	string csv1_pattern = "travesal_csv1_(\\d)+";
	regex csv1_case_filter(csv1_pattern);

	string csv2_pattern = "travesal_csv2_(\\d)+";
	regex csv2_case_filter(csv2_pattern);

	//将csv用例以 “编号：文件名”加入map
	for (auto i = csv_files.begin(); i < csv_files.end(); i++)
	{
		smatch results;
		string case_num;
		regex r("_"); //以-为分隔符
		if (regex_search(*i, results, csv1_case_filter))  //获取到csv1用例
		{
			sregex_token_iterator beg((*i).begin(), (*i).end(), r, -1); //传入-1，代表对匹配到的分隔符之间的内容感兴趣
			sregex_token_iterator end; //结束标志
			for (; beg != end; beg++)
				case_num = beg->str();   //获取csv用例的编号（带.csv）  比如1.csv
			//去掉.csv
			case_num = case_num.substr(0, case_num.rfind("."));
			csv1_map.emplace(case_num, *i);
		}
        else if(regex_search(*i, results, csv2_case_filter))//获取到csv2用例
        {
			sregex_token_iterator beg((*i).begin(), (*i).end(), r, -1); //传入-1，代表对匹配到的分隔符之间的内容感兴趣
			sregex_token_iterator end; //结束标志
			for (; beg != end; beg++)
				case_num = beg->str();   //获取csv用例的编号（带.csv）  比如1.csv
			//去掉.csv
			case_num = case_num.substr(0, case_num.rfind("."));
			csv2_map.emplace(case_num, *i); 
        }
	}
	//找到匹配的用例
	for (auto csv1_iter = csv1_map.begin(); csv1_iter != csv1_map.end(); ++csv1_iter)
	{
		case_struct csv_case;
        auto csv2_iter = csv2_map.find(csv1_iter->first);//找到两个文件相同编号的csv文件
		if (csv2_iter != csv2_map.end())
		{
			csv_case.file_1_name = csv1_iter->second;
            csv_case.file_2_name = csv2_iter->second;

			string result_file = case_folder + "\\csv1_result\\csv1_result_" + csv1_iter->first + ".csv";
			csv_case.result_file_name = result_file;

			csv_cases.push_back(csv_case);
		}
	}
#if 0//可以打印出来看下找到的文件是否编号一一对应
	for (auto i = csv_cases.begin(); i < csv_cases.end(); i++)
	{
		cout << i->file_1_name << " --- " << i->file_2_name << endl;
	}
#endif
}


void case_folder_handler(string case_folder)
{
	vector<case_struct> csv_cases;
	csv_file_case_classify(case_folder, csv_cases);

	//创建result文件夹
	string result_folder = case_folder + "\\result";
	_mkdir(result_folder.c_str());

	for (auto iter = csv_cases.begin(); iter != csv_cases.end(); ++iter)
	{
        //循环遍历每一个文件，每次循环相当于对一个文件的处理
		csv_case_handler(&(*iter));
	}
}
