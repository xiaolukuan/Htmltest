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
	string hasruler;//是否有  标签属性规则
	string id;//网站ID
	string webname;//网站名称
	string textcontenttagname;//正文标签属性名称
	string textcontenttag;//正文内容标签属性
	string summaryisinbody;//摘要是否在正文里面
	string summarytagname;//摘要标签属性名称
	string summarytag;//摘要标签属性
	string comment;//评论规则
};

class CIMSLLog 
{
  public:
	  CIMSLLog();
	  ~CIMSLLog();
  public:
	  //设置日志文件路径
      void SetLogFilePath(string logfilepath);
	  //设置日志文件路径
	  void SetLogProjName(string strpname);
	  //写日志//modify by luis you must set log level when you write
	  bool WriteLog(int level,const char *fmt,...);
  private:
	  bool CreateLog();
	  //设置日志级别
	  void SetLogLevel(int level);
  private:
	  ofstream fout;
	  pthread_mutex_t log_mutex;//互斥对象
	  string m_logpath;
	  string m_strlogprojname;
	  string m_level;

};
extern CIMSLLog theLog;
extern map<string,string> p_config;
extern pthread_mutex_t config_mutex;
extern map<string,struct SpiderRuler>p_spiderruler;
#endif

