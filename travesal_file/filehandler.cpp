#include "filehandler.h"

/*
path: 指定目录
files: 保存结果
fileType: 指定的文件格式，如 .jpg
*/
void getAllFiles(string path, vector<string>& files, string fileType)
{
	// 文件句柄
	long long hFile = 0;
	// 文件信息
	struct _finddata_t fileinfo;

	string p;

	if ((hFile = _findfirst(p.assign(path).append("\\*" + fileType).c_str(), &fileinfo)) != -1) {
		do {
			// 保存文件的全路径
			files.push_back(p.assign(path).append("\\").append(fileinfo.name));

		} while (_findnext(hFile, &fileinfo) == 0);  //寻找下一个，成功返回0，否则-1

		_findclose(hFile);
	}
}