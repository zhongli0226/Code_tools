#include <iostream>
#include "case_handler.h"
#include <regex>
#include <vector>
#include <iostream>

int main()
{
    setbuf(stdout,NULL);//debug调试 打印数据
    char* case_folder = (char*)"./case";
    case_folder_handler(case_folder);

    
    return 0;
}