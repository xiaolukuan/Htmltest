// FileName : IMSLCommon.h
// Creator  : luis yang
// Date     : 2011-07-12
// Comment  : global function

#ifndef __IMSL_COMMON_H__
#define __IMSL_COMMON_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/queue.h>
#include <err.h>
#include <errno.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <cstdlib>
#include <iconv.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdarg.h>
#include <boost/regex.hpp>
using namespace std;

//字符编码转换
int iconv_string(const char *from, const char *to,
				 const char *src, size_t len,
				 ::std::string& result,
				 int c = 0, size_t buf_size = 512);

//是否为utf-8编码
bool IsUTF8(const void* pBuffer, long size);

//去除常见空格
void rtrim( char *str );
void strltrim(string &s);
void utfstrltrim(string &s);
void utfstrrtrim(string &s);
void utfstrtrim(string &s);
//////////////////////////////////////////////////////////////////////////

//url解码
char *urldecode(char *in);
string urlencode( string sIn );
unsigned char toHex(const unsigned char &x);
//读取文本文件
string ReadFile(const char* filename);

//写文件
bool WriteFile(const char *filename,const char *content,int length,bool bNewfile = true);

//取得文件大小
int  GetFilenSize(const char* filename);

//创建文件夹
bool Makedir(const char* rootdir,const char* tempdir);

//获取当前时间，使用完需要delete返回指针
 char *GetLocalTime();
//读取配置文件
bool ReadConfigFile(const char * filename,map<string,string> &m_config);

//读取爬虫规则配置文件
bool ReadSpiderRulerConfigFile(const char * filename,map<string,struct SpiderRuler> &m_config);
//获取当前路径
string GetFullPath();
//数字到字符串
string my_itoa(int i);
//微秒级的线程暂停
int usSleep(unsigned int nusecs);
int my_system(const char *cmdstring);
int mysystem(char* cmdstring, char* buf, int len);
int my_pscmd(char *proname,int *pid);
int my_killcmd(int pid);
int my_startcmd(char *path);
int my_killallcmd(char *proname);
#endif

