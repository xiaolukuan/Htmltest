#include "spidergetpage.h"
#include <pthread.h>
#include "stdlib.h"
#include <iostream>
using namespace std;
using namespace boost::threadpool;
using namespace IMSL::ISpider;

void showhelp();

int main(int argc, char* argv[])
{   
	curl_global_init(CURL_GLOBAL_ALL);
	theLog.SetLogFilePath(GetFullPath());
	CNetSpider m_ssesipder;
	if(!ReadConfigFile((GetFullPath()+"/sqlconfig.conf").c_str(),m_ssesipder.m_config))
		return 0;
	else if (argc == 1)
	{
		showhelp();
		return 0;
	}
	else if(argc == 6 && strcmp(argv[1],"-test") == 0 && strcmp(argv[2],"-uid") == 0 && strcmp(argv[4],"-depth") == 0)//start
	{   
		int uid = atoi(argv[3]);
		int depth = atoi(argv[5]);
		m_ssesipder.test(uid,depth);
	}
	else if(strcmp(argv[1],"-start") == 0 && argc == 2)//start
	{   
		while(1)
		{
			m_ssesipder.run();
			sleep(5*60);
		}
		
// 		pool tp(8);
// 		while(1)
// 		{
// 			pSpiderconf = new CLoadConfig();
// 			if(!pSpiderconf->loadConfig())
// 			{
// 				cout<<"load config error!"<<endl;
// 				return 0;
// 			}
// 			theLog.WriteLog(LOG_LEVEL_SYS,"start!");
// 			vector<CNetSpider*> vectask;
// 			if (!createtask(vectask))
// 			{
// 				theLog.WriteLog(LOG_LEVEL_ERROR,"create task error!");
// 				delete pSpiderconf;
// 				pSpiderconf = NULL;
// 				return 0;
// 			} 
// 			else
// 			{
// 				for(int i = 0; i < (int)vectask.size(); i++)
// 				{
// 					tp.schedule(boost::bind(runChild,vectask[i]));
// 				}
// 				tp.wait();
// 				for(int i = 0; i < (int)vectask.size(); i++)
// 				{
// 					delete vectask[i];
// 					vectask[i] = NULL;
// 				}
// 			}
// 			theLog.WriteLog(LOG_LEVEL_SYS,"complete!");
// 			delete pSpiderconf;
// 			pSpiderconf = NULL;
// 			sleep(60*10);
// 		}
 		
	}
	else
	{
		showhelp();
	}
	return 0;
}
void showhelp()
{
	char *b = "spider v1.0.0.0\n" 
		"\n"
		"-tl -pid no.  test rule list,input no. of planid! \n"
		"-tp -pid no.  test plan list and article,input no. of planid! \n"
		"-ta -pid no. -url www.xxx.com  test rule article,input no. of planid and url! \n"
		"-ts -pid no. -url www.xxx.com  test url subpage,input no. of planid and url! \n"
		"-start start getpage use defult minutes to wait and threads! \n"
		"-starttest start test getpage use defult threads! \n"
		"-start -times no. -threads no. start getpage and input minutes to wait and how many threads! \n"
		"input error ! the process will be exit!\n"
		"\n";
	fprintf(stderr, b, strlen(b));
}
