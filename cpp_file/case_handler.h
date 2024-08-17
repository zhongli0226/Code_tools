#ifndef __PPG_CASE_HANDLER_H__
#define __PPG_CASE_HANDLER_H__
#include <string>
#include <iostream>
#include <vector>

using namespace std;
// 相关文件名结构体 可以认为是文件路径
typedef struct case_tag
{
	string file_1_name;
	string file_2_name;
    string result_file_name;
}case_struct;

using namespace std;

void case_folder_handler(string case_folder);


#endif