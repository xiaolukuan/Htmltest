// FileName : MongoManager.h
// Creator  : luis yang
// Date     : 2011-08-17
// Comment  : mongoDB manager

#ifndef __IMSL_MONGODB_MANAGER_H__
#define __IMSL_MONGODB_MANAGER_H__
#include "/usr/local/mongodb/mongo/client/dbclient.h"
using namespace mongo;

typedef struct articletype//传给检查数据库线程的参数
{
	string strtitle;
	string strentity;
	string strcontent;
	string strrtcontent;

}ArticleType;

class MongoManager
{
public:
	MongoManager();
	~MongoManager();
public:

	bool conn(const char *hostname);
	bool auth(const string &dbname, const string &username, const string &pwd, bool digestPassword=true);
    ArticleType getsourcearticle(const char *aid,int atype);
	ArticleType getsegarticle(const char *aid);
	//void insertarticle(const char* aid,ArticleType atp);
	void insertarticle(const char* aid,string content);
	vector<string> getatuser(const char *aid);
	void removearticle(const char* aid);
	void settable(string table);
	OID aidtooid(const char* aid);
///////////////////////////////////////////mongo fun///////////////////////////////////////////////////////////////////
	void 	insert (BSONObj obj, int flags=0);
	void 	insert (const vector< BSONObj > &v, int flags=0);
	void 	remove (Query query, bool justOne=0);
	void 	update (Query query, BSONObj obj, bool upsert=0, bool multi=0);
	BSONObj 	findOne (const Query &query, const BSONObj *fieldsToReturn=0, int queryOptions=0);
private:
	DBClientConnection connection;
	string table;

};
#endif // end __IMSL_MONGODB_MANAGER_H__
