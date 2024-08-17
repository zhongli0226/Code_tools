#ifndef __FILE_HANDLER_H__
#define __FILE_HANDLER_H__


#include <io.h>
#include <fstream>
#include <string>
#include <vector>
#include <iostream>
using namespace std;

void getAllFiles(string path, vector<string>& files, string fileType);

#endif
