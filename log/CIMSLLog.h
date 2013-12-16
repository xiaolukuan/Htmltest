// FileName : CIMSLLog.h
// Creator  : luis yang
// Date     : 2011-07-12
// Comment  : log writer

#ifndef __IMSL_LOG_H__
#define __IMSL_LOG_H__
#include "../common/IMSLCommon.h"


#define   LOG_LEVEL_SYS       0
#define   LOG_LEVEL_ERROR     1
struct SpiderRuler
{
	string hasruler;//�Ƿ���  ��ǩ���Թ���
	string id;//��վID
	string webname;//��վ����
	string textcontenttagname;//���ı�ǩ��������
	string textcontenttag;//�������ݱ�ǩ����
	string summaryisinbody;//ժҪ�Ƿ�����������
	string summarytagname;//ժҪ��ǩ��������
	string summarytag;//ժҪ��ǩ����
	string comment;//���۹���
};

class CIMSLLog 
{
  public:
	  CIMSLLog();
	  ~CIMSLLog();
  public:
	  //������־�ļ�·��
      void SetLogFilePath(string logfilepath);
	  //������־�ļ�·��
	  void SetLogProjName(string strpname);
	  //д��־//modify by luis you must set log level when you write
	  bool WriteLog(int level,const char *fmt,...);
  private:
	  bool CreateLog();
	  //������־����
	  void SetLogLevel(int level);
  private:
	  ofstream fout;
	  pthread_mutex_t log_mutex;//�������
	  string m_logpath;
	  string m_strlogprojname;
	  string m_level;

};
extern CIMSLLog theLog;
extern map<string,string> p_config;
extern pthread_mutex_t config_mutex;
extern map<string,struct SpiderRuler>p_spiderruler;
#endif

