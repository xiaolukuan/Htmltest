#ifndef CIMSLSEGH_XXX
#define CIMSLSEGH_XXX
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
#include <sstream>
#include <cstdlib>
#include <iconv.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <boost/regex.hpp>
#include <boost/filesystem.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/lexical_cast.hpp>
#include "HtmlFragment.h"
#include "HtmlExtract.h"
#include<openssl/md5.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <mysql/mysql.h>
//#include "Configuration.h"
#include "../HttpWrap/HttpWrap.h"
#include "../common/IMSLCommon.h"
#include "../log/CIMSLLog.h"
#include "../DBmanager/DBManager.h"
#include "../mongo/MongoManager.h"
#include "../qiniu/io.h"
#include "../qiniu/rs.h"
//#include "../b64/b64.h"
//#include "../b64/urlsafe_b64.h"
#include "../qiniu/base.h"
#include "../qiniu/resumable_io.h"
using namespace std;
class articleextract
{
public:
	articleextract();
	~articleextract();
public:
	bool OnInitDB();
	bool OnExitDB();
	void extract(string arctileid,string newarticleurl,string sourcepagename);
	bool getPage(const char* url, int method, std::string& response);
	char lowerCase (char a);
	bool startWith (const char *a,const char *b);
	string parse_url (const char *charg); 
	string _expandlinks(string strurl,string strurlsub);
	string pagetagclean(string page);
	string contenttagclean(string content);
	string Introductioncontenttagclean(string content);
	string publish_time_deal(string publish_time);
	bool insertmysqldbres(string origin_title,string sourceurl,string with_image,string discription,string id,string publishtime);
	bool insertmongodbres(string articleid,string articlecontent);
	bool updatepostgrlurlstatus(string arctileid);
public:
	MongoManager *pmongom ;
	CDBManager *m_dbmanager;
	MYSQL *MySqlConn;
	string article_id;
	Qiniu_Client client;
};
#endif
