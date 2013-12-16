#include "MongoManager.h"
#include "../log/CIMSLLog.h"
MongoManager::MongoManager()
{
}

MongoManager::~MongoManager()
{
}

/// connect postgresql 
bool MongoManager::conn(const char *hostname)
{
	string errmsg;
	if (!connection.connect(hostname, errmsg)) 
	{  
		theLog.WriteLog(LOG_LEVEL_ERROR,"Mongodb Error:%s",errmsg.c_str());
		return false;
	}
	else
		return true;
}
bool MongoManager::auth(const string &dbname, const string &username, const string &pwd, bool digestPassword/* =true */)
{
	string errmsg;
	if (!connection.auth(dbname, username,pwd,errmsg)) 
	{  
		theLog.WriteLog(LOG_LEVEL_ERROR,"Mongodb Error:%s",errmsg.c_str());
		return false;
	}
	else
		return true;
}
/*ArticleType MongoManager::getsourcearticle(const char *aid,int atype)
{
	ArticleType m_article;
	BSONObj obj = connection.findOne(table,QUERY("_id"<<aidtooid(aid)));
	if( atype == 1 || atype == 2 || atype == 4)
	{
		string strtitle = obj.getStringField("title");
		string strcontent = obj.getStringField("content");
		boost::regex reg1("<((p)|(/p)|(P)|(/P)){1}>");
		strtitle = boost::regex_replace(strtitle, reg1, "\n");
		strcontent = boost::regex_replace(strcontent, reg1, "\n");
		reg1.assign("<(.*?)>");
		strtitle = boost::regex_replace(strtitle, reg1, "");
		strcontent = boost::regex_replace(strcontent, reg1, "");
		reg1.assign("&\\w{1,8};");
		strtitle = boost::regex_replace(strtitle, reg1, " ");
		strcontent = boost::regex_replace(strcontent, reg1, " ");
		m_article.strtitle = strtitle;
		m_article.strcontent = strcontent;
	}
	else if( atype == 3)
	{
		if(obj.hasField("relatedtweet"))
		{
			m_article.strrtcontent = obj.getObjectField("relatedtweet").getStringField("text");
			m_article.strcontent = obj.getObjectField("content").getStringField("text");
		}
		else
			m_article.strcontent = obj.getObjectField("content").getStringField("text");
		
	}
	return m_article;
}*/
vector<string> MongoManager::getatuser(const char *aid)
{
	BSONObj obj = connection.findOne(table,QUERY("_id"<<aidtooid(aid)));
	vector<string> vec_atUsers;
	vector< BSONElement > vecbe;
	if(obj.hasField("content"))
	{
		BSONObj obj_content = obj.getObjectField("content");
		if(obj_content.hasField("atUsers"))
		{
			BSONObj obj_atUsers = obj_content.getObjectField("atUsers");
			obj_atUsers.elems(vecbe);
			for(int i = 0; i < (int)vecbe.size(); i++)
			{
				string strfieldname = vecbe[i].fieldName();
				string strtmp;
				iconv_string("UTF-8", "GBK", strfieldname.c_str(), strfieldname.length(), strtmp, 1);
				vec_atUsers.push_back(strtmp);
			}
		}
	}
	return vec_atUsers;
}
/*ArticleType MongoManager::getsegarticle(const char *aid)
{
	ArticleType m_article;
	string strtitle="",strentity = "",strcontent = "",strrtcontent = "";
	BSONObj obj = connection.findOne(table,QUERY("_id"<<aidtooid(aid)));
	strtitle = obj.getStringField("title");
	strentity = obj.getStringField("entity");
	strcontent = obj.getStringField("content");
	strrtcontent = obj.getStringField("rtcontent");
	iconv_string("UTF-8","GBK",strtitle.c_str(),strtitle.length(),m_article.strtitle,1);
	iconv_string("UTF-8","GBK",strentity.c_str(),strentity.length(),m_article.strentity,1);
	iconv_string("UTF-8","GBK",strcontent.c_str(),strcontent.length(),m_article.strcontent,1);
	iconv_string("UTF-8","GBK",strrtcontent.c_str(),strrtcontent.length(),m_article.strrtcontent,1);
	return m_article;
}*/
/*void MongoManager::insertarticle(const char* aid,ArticleType atp)
{
	string strtitle="",strentity = "",strcontent = "",strrtcontent = "";
	iconv_string("GBK","UTF-8",atp.strtitle.c_str(),atp.strtitle.length(),strtitle,1);
	iconv_string("GBK","UTF-8",atp.strentity.c_str(),atp.strentity.length(),strentity,1);
	iconv_string("GBK","UTF-8",atp.strcontent.c_str(),atp.strcontent.length(),strcontent,1);
	iconv_string("GBK","UTF-8",atp.strrtcontent.c_str(),atp.strrtcontent.length(),strrtcontent,1);
	BSONObj p = BSON("_id"<<aidtooid(aid)<<"title"<< strtitle.c_str() <<"entity"\
		<<strentity.c_str()<<"content"<< strcontent.c_str()<<"rtcontent"<<strrtcontent.c_str());
	connection.insert(table, p);
}*/
void MongoManager::insertarticle(const char* aid,string content)
{
	//string strcontent;
	//iconv_string("GBK","UTF-8",content.c_str(),content.length(),strcontent,1);
	BSONObj p = BSON("ArticleId"<<aid<<"ArticleContent"<<content.c_str());
	//printf("aid=%s\n",aid);
	connection.insert(table, p,0);
	
}
void MongoManager::removearticle(const char* aid)
{
	BSONObj p = BSON("_id"<<aidtooid(aid));
	connection.remove(table, p);
}
void MongoManager::settable(string table)
{
	this->table = table;
}
OID MongoManager::aidtooid(const char* aid)
{
	string str;
	int i = strlen(aid);
	for(int j = 0 ; j< 24-i ; ++j)
	{
		str += "0";
	}
	return OID(str + aid);
}
void MongoManager::insert (BSONObj obj, int flags)
{
  //  connection.insert(table, obj,flags);
}
void MongoManager::insert (const vector< BSONObj > &v, int flags)
{
//	connection.insert(table, v,flags);
}
void MongoManager::remove (Query query, bool justOne)
{
	connection.remove(table, query,justOne);
}
void MongoManager::update (Query query, BSONObj obj, bool upsert, bool multi)
{
	connection.update(table,query,obj,upsert,multi);
}
BSONObj MongoManager::findOne (const Query &query, const BSONObj *fieldsToReturn, int queryOptions)
{
	return connection.findOne(table,query,fieldsToReturn,queryOptions);
}

