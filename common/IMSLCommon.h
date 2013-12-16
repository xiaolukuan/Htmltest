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

//�ַ�����ת��
int iconv_string(const char *from, const char *to,
				 const char *src, size_t len,
				 ::std::string& result,
				 int c = 0, size_t buf_size = 512);

//�Ƿ�Ϊutf-8����
bool IsUTF8(const void* pBuffer, long size);

//ȥ�������ո�
void rtrim( char *str );
void strltrim(string &s);
void utfstrltrim(string &s);
void utfstrrtrim(string &s);
void utfstrtrim(string &s);
//////////////////////////////////////////////////////////////////////////

//url����
char *urldecode(char *in);
string urlencode( string sIn );
unsigned char toHex(const unsigned char &x);
//��ȡ�ı��ļ�
string ReadFile(const char* filename);

//д�ļ�
bool WriteFile(const char *filename,const char *content,int length,bool bNewfile = true);

//ȡ���ļ���С
int  GetFilenSize(const char* filename);

//�����ļ���
bool Makedir(const char* rootdir,const char* tempdir);

//��ȡ��ǰʱ�䣬ʹ������Ҫdelete����ָ��
 char *GetLocalTime();
//��ȡ�����ļ�
bool ReadConfigFile(const char * filename,map<string,string> &m_config);

//��ȡ������������ļ�
bool ReadSpiderRulerConfigFile(const char * filename,map<string,struct SpiderRuler> &m_config);
//��ȡ��ǰ·��
string GetFullPath();
//���ֵ��ַ���
string my_itoa(int i);
//΢�뼶���߳���ͣ
int usSleep(unsigned int nusecs);
int my_system(const char *cmdstring);
int mysystem(char* cmdstring, char* buf, int len);
int my_pscmd(char *proname,int *pid);
int my_killcmd(int pid);
int my_startcmd(char *path);
int my_killallcmd(char *proname);
#endif

