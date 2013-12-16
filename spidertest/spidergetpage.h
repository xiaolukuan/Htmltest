// FileName : CNetSpider.h
// Author   : luis
// Date     : 2011-11-01
// Desc     : get page

#ifndef __IMSL_ISPIDER_SPIDERGETPAGE_H__
#define __IMSL_ISPIDER_SPIDERGETPAGE_H__

#include <string>
#include <vector>
#include <time.h>
#include <boost/thread/thread.hpp>
#include <boost/bind.hpp>
#include "../threadpool/threadpool.hpp"
#include "../HttpWrap/HttpWrap.h"
#include "../HttpWrap/HTTPClientAuth.h"
#include "../mongo/MongoManager.h"
#include "../html/ParserDom.h"
#include "../DBmanager/DBManager.h"
#include "../common/IMSLCommon.h"
#include "../log/CIMSLLog.h"
using namespace std;
using namespace htmlcxx;
namespace IMSL{
namespace ISpider{
typedef struct urlinfo//urlinfo
{
	string strtitle;
	bool bprocess;

}UrlInfo;
typedef map<string,UrlInfo> UrlInfoMap;
class CNetSpider
{
public:
	CNetSpider();
	~CNetSpider();
public:
	///////////////////////////////////////////////////////////////
	bool OninitDBM();
	void freeDB();
	void test(int pid,int depth);
	bool getPage(const char* url, int method, std::string& response) const;
	string get41articleObj(string startid,string nnum);
	bool senddata(BSONObj obj);
	bool inserturl(CDBManager *pdbm,string strurl,string strtitle,string strsource);
	bool findarticle(CDBManager *pdbm,string urlmd5);
	bool run();
	string getsourceid(string strsource);
	bool getallurl(int depth,UrlInfoMap &urlmap,string keyword,string urlkeyword,string strbuseregex);
	string _expandlinks(string strurl,string strurlsub);
	char lowerCase (char a);
	bool startWith (const char *a,const char *b) ;
	string parse_url (const char *charg);
	bool Readfiltersuffix(const char * filename);
	bool Readfilterarticle(const char * filename);
	bool filtersuffx(string strurl);
	bool filterarticle(string strurl);
	string getrefreshurl(string strpage);
	string reviseurl(string strurl);
public:
	//CDBManager *pdbmspiderdb;
	CDBManager *pdbmarticledb;
	//MongoManager *pmongo;
	map<string,string> m_config;
	vector<string> m_filtersuffix;
	vector<string> m_filterarticle;
	bool btest;

};

#define MAXTHREADNO 32

typedef struct threadparm//传给检查数据库线程的参数
{
	int ires;
	PGresult   *res;
	CNetSpider *pns;

}ThreadParm;

}// end ISpider
}// end IMSL

#endif // end __IMSL_ISPIDER_SPIDERGETPAGE_H__
