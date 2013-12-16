// FileName : CNetSpider.h
// Author   : luis
// Date     : 2011-11-01
// Desc     : get page
#include "spidergetpage.h"

namespace IMSL{
namespace ISpider{
boost::threadpool::pool tp(10);
static void threadfun(ThreadParm tp);
CNetSpider::CNetSpider() /*_hEvent(NULL),*/
{
	btest = false;
	pdbmarticledb = NULL;
	
}
CNetSpider::~CNetSpider()
{

}
bool CNetSpider::Readfiltersuffix(const char * filename)
{
	std::istream * is;
	std::ifstream fin;
	if(!filename) 
	{
		is = & std::cin;
	} 
	else 
	{
		fin.open(filename);
		if(!fin.is_open())
		{
			theLog.WriteLog(LOG_LEVEL_ERROR,"cann't open the configfile: %s !",filename);
			return false;
		}
		else
			is = & fin;
	}
	while(!is->eof()) 
	{
		std::string line;
		getline(*is, line);
		if ((int)line.find("\r") > 0)
		{
			line = line.substr(0,line.find("\r"));
		}
		if ((int)line.find("#") == 0)
		{
			continue;
		} 
		else
		{
			int npos = 0;
			while((npos=(int)line.find(" ")) > 0)
			{
				m_filtersuffix.push_back(line.substr(0,npos));
				line = line.substr(npos+1);
			}
			if((int)line.length() > 0) m_filtersuffix.push_back(line);
		}
	}
	if(filename)
	{
		fin.close();
	}
	return (int)m_filtersuffix.size()>0?true:false;
}
bool CNetSpider::Readfilterarticle(const char * filename)
{
	std::istream * is;
	std::ifstream fin;
	if(!filename) 
	{
		is = & std::cin;
	} 
	else 
	{
		fin.open(filename);
		if(!fin.is_open())
		{
			theLog.WriteLog(LOG_LEVEL_ERROR,"cann't open the configfile: %s !",filename);
			return false;
		}
		else
			is = & fin;
	}
	while(!is->eof()) 
	{
		std::string line;
		getline(*is, line);
		if ((int)line.find("\r") > 0)
		{
			line = line.substr(0,line.find("\r"));
		}
		if ((int)line.find("#") == 0)
		{
			continue;
		} 
		else
		{
			m_filterarticle.push_back(line);
		}
	}
	if(filename)
	{
		fin.close();
	}
	return (int)m_filterarticle.size()>0?true:false;
}
void CNetSpider::freeDB()
{
// 	if (pdbmspiderdb)
// 	{
// 		delete pdbmspiderdb;
// 		pdbmspiderdb = NULL;
// 	}
	if (pdbmarticledb)
	{
		delete pdbmarticledb;
		pdbmarticledb = NULL;
	}
// 	if (pmongo)
// 	{
// 		delete pmongo;
// 		pmongo = NULL;
// 	}
}
bool CNetSpider::OninitDBM()
{
	Readfiltersuffix("filter.conf");
	Readfilterarticle("filtearticle.txt");
	pdbmarticledb = new CDBManager();
	//pmongo = new MongoManager();
	assert(pdbmarticledb != NULL);

	if(!pdbmarticledb->ConnectDB(m_config["articledbconninfo"].c_str()))
	{
		if(pdbmarticledb)
		{
			delete pdbmarticledb;
		}
		return false;
	}
	else if(!pdbmarticledb->SetClientEncoding("utf-8"))
	{
		if(pdbmarticledb)
		{
			delete pdbmarticledb;
		}
		return false;
	}
// 	else if(!pmongo->conn(m_config["mongohost"].c_str()))
// 		return false;
// 	else if (!pmongo->auth(m_config["mongodbname"].c_str(),\
// 		m_config["mongouser"].c_str(),m_config["mongopwd"].c_str()))
// 		return false;
// 	else 
// 	{
// 		pmongo->settable(m_config["mongosourceat"]);
// 		return true;
// 	}
	return true;
}
void CNetSpider::test(int pid,int depth)
{
	btest = true;
	if(!OninitDBM())
	{
		freeDB();
		return;
	}
	PGresult   *res = NULL;
	std::string strsql= "select * from sourceurltable where Uid = " + my_itoa(pid);
	res = pdbmarticledb->QueryDB(strsql.c_str());
	int row2 = pdbmarticledb->Getntuples(res);
	if (row2>0)
	{
		for (int i = 0 ; i < row2 ; ++i)
		{
			string struid = pdbmarticledb->Getvalue(res,i,0);
			string strname = pdbmarticledb->Getvalue(res,i,1);
			string strurl = pdbmarticledb->Getvalue(res,i,2);
			strurl = reviseurl(strurl);
			string strkeyword = pdbmarticledb->Getvalue(res,i,3);
			string strurlkeyword = pdbmarticledb->Getvalue(res,i,4);
			string strbuseregex = pdbmarticledb->Getvalue(res,i,5);
			//printf("struid=%s\n",struid.c_str());
			//printf("strname=%s\n",strname.c_str());
			//printf("strurl=%s\n",strurl.c_str());
			//printf("strkeyword=%s\n",strkeyword.c_str());
			//printf("strurlkeyword=%s\n",strurlkeyword.c_str());
			//printf("strbuseregex=%s\n",strbuseregex.c_str());
			UrlInfoMap m_url ;
			UrlInfo m_ui;
			m_ui.bprocess = false;
			m_ui.strtitle = pdbmarticledb->Getvalue(res,i,1);
			m_url.insert(pair<string,UrlInfo>(strurl,m_ui));
 			getallurl(depth,m_url,strkeyword,strurlkeyword,strbuseregex);
 			UrlInfoMap::iterator ite;
			cout<<strname<<"<:::::::::::::::::>"<<strurl<<endl;
 			for (ite=m_url.begin();ite!=m_url.end();++ite)
 			{
				if(ite->second.strtitle == "" || filterarticle(ite->second.strtitle))
					continue;
				if(strbuseregex == "t")
				{
					boost::regex regkeyword(strkeyword);
					boost::cmatch what;
					if(boost::regex_match(ite->first.c_str(), what, regkeyword))
					cout<<ite->first<<"<==artilce==>"<<ite->second.strtitle<<endl;
				}
				else
				{
					if((int)ite->first.find(strkeyword) > 0)
						cout<<ite->first<<"<==artilce==>"<<ite->second.strtitle<<endl;
				}
				
 			}
		}
	}
	pdbmarticledb->Clearres(res);
	freeDB();
}
string CNetSpider::reviseurl(string strurl)
{
	char * chtime = GetLocalTime();
	string strtime = chtime;
	delete chtime;
	string stryear = "", strmonth = "", strdate = "" ,strurltmp = strurl;
	boost::regex regkeyword("(\\d+)-(\\d+)-(\\d+) (.*?)");
	boost::cmatch what;
	if(boost::regex_match(strtime.c_str(), what, regkeyword))
	{
		stryear = what[1].str();
		strmonth = what[2].str();
		strdate = what[3].str();
	}
	regkeyword.assign("<year>");
	strurltmp = boost::regex_replace(strurltmp, regkeyword, stryear);
	regkeyword.assign("<month>");
	strurltmp = boost::regex_replace(strurltmp, regkeyword, strmonth);
	regkeyword.assign("<date>");
	strurltmp = boost::regex_replace(strurltmp, regkeyword, strdate);
	return strurltmp;
}
bool CNetSpider::inserturl(CDBManager *pdbm,string strurl,string strtitle,string strsource)
{
	if(findarticle(pdbm,strurl))
		return false;
	string strsql = "insert into newarticleurltable(newarticleurl,articletitle,create_time,sourcepagename) values('"+\
		strurl + "','"  + strtitle + "',LOCALTIMESTAMP(0),'" + strsource +"');";
	if(!pdbm->CommandDB(strsql.c_str()))
	{
		theLog.WriteLog(LOG_LEVEL_ERROR,"inserturl to  postgrel  failed! insertinfo: %s...",strsql.c_str());
		return false;
	}
	return true;
}
void threadfun(ThreadParm tp)
{
	ThreadParm pparm = (ThreadParm)tp;
	CDBManager *pdbmarticledb = new CDBManager();
	struct timeval tvpre, tvafter;
	gettimeofday(&tvpre, NULL);
	if(!pdbmarticledb->ConnectDB(pparm.pns->m_config["articledbconninfo"].c_str()))
	{
		if(NULL != pdbmarticledb)
		{
			delete pdbmarticledb;
			pdbmarticledb =NULL;
		}
		return;
	}
	else if(!pdbmarticledb->SetClientEncoding("utf-8"))
	{
		if(NULL != pdbmarticledb)
		{
			delete pdbmarticledb;
			pdbmarticledb =NULL;
		}
		return;
	}
	string struid				= pdbmarticledb->Getvalue(pparm.res,pparm.ires,0);
	string strname				= pdbmarticledb->Getvalue(pparm.res,pparm.ires,1);
	string strurl				= pdbmarticledb->Getvalue(pparm.res,pparm.ires,2);
	string strkeyword			= pdbmarticledb->Getvalue(pparm.res,pparm.ires,3);
	string strurlkeyword		= pdbmarticledb->Getvalue(pparm.res,pparm.ires,4);
	string strbuseregex			= pdbmarticledb->Getvalue(pparm.res,pparm.ires,5);
	string strrootname			= strname;
	//string strrootname = pdbmarticledb->Getvalue(pparm->res,pparm->ires,9);
	UrlInfoMap m_url ;
	UrlInfo m_ui;
	m_ui.bprocess = false;
	m_ui.strtitle = strname;
	m_url.insert(pair<string,UrlInfo>(strurl,m_ui));
	pparm.pns->getallurl(2,m_url,strkeyword,strurlkeyword,strbuseregex);
	UrlInfoMap::iterator ite;
	int narticlecount = 0 ;
	for (ite=m_url.begin();ite!=m_url.end();++ite)
	{
		if(ite->second.strtitle == "")
			continue;
		else if(pparm.pns->filterarticle(ite->second.strtitle))
		{
			narticlecount++;
			narticlecount++;
			continue;
		}
		if(strbuseregex == "t")
		{
			boost::regex regkeyword(strkeyword);
			boost::cmatch what;
			if(boost::regex_match(ite->first.c_str(), what, regkeyword))
			{
				if(ite->second.strtitle.length() >= 255 && strname != "pingwest" )
					continue;
				//cout<<ite->first<<":"<<ite->second.strtitle<<endl;
				if(!pparm.pns->inserturl(pdbmarticledb,ite->first,ite->second.strtitle,strrootname))
					pparm.pns->inserturl(pdbmarticledb,ite->first,"maybe title error!",strrootname);
				narticlecount++;
			}
		}
		else
		{
			if((int)ite->first.find(strkeyword) > 0)
			{
				if(ite->second.strtitle.length() >= 255 && strname != "pingwest" )
					continue;
				//cout<<ite->first<<":"<<ite->second.strtitle<<endl;
				if(!pparm.pns->inserturl(pdbmarticledb,ite->first,ite->second.strtitle,strrootname))
					pparm.pns->inserturl(pdbmarticledb,ite->first,"maybe title error!",strrootname);
				narticlecount++;
			}
		}
	}
	if(narticlecount > 0 )
	{
		string strsql = "update sourceurltable set state = 1 where uid = " + struid ;
		pdbmarticledb->CommandDB(strsql.c_str());
	}
	else
	{
		string strsql = "update sourceurltable set state = -1 where uid = " + struid ;
		pdbmarticledb->CommandDB(strsql.c_str());
	}
	delete pdbmarticledb;
	pdbmarticledb = NULL;
	gettimeofday(&tvafter, NULL);
	int nusetime = (int)((tvafter.tv_sec-tvpre.tv_sec)*1000+(tvafter.tv_usec-tvpre.tv_usec)/1000);
	theLog.WriteLog(LOG_LEVEL_SYS,"the  newssite uid:%s name:%s was fetch complete!  used time %d ms!!!",struid.c_str(),strname.c_str(),nusetime);
}
bool CNetSpider::run()
{
	if(!OninitDBM())
	{
		freeDB();
		return false;
	}
	PGresult   *res = NULL;
	vector<ThreadParm> vectask;
	res = pdbmarticledb->QueryDB(m_config["plansql"].c_str());
	if(NULL == res)
	{
		freeDB();
		return false;
	}
	int row2 = pdbmarticledb->Getntuples(res);
	theLog.WriteLog(LOG_LEVEL_SYS,"the %d newssite was be selected !",row2);
	for(int i = 0;i < row2;++i)
	{
		//ThreadParm* ptp = new ThreadParm;
		ThreadParm ptp;
	//	if(NULL == ptp)
	//	{
	//		theLog.WriteLog(LOG_LEVEL_ERROR,"new ThreadParm failed !rows=%d",row2);
		//	break;
	//	}
		ptp.ires = i;
		ptp.res = res;
		ptp.pns = this;
		vectask.push_back(ptp);
	}
	for(int i = 0; i < (int)vectask.size(); i++)
	{
		tp.schedule(boost::bind(threadfun,vectask[i]));
	}
	tp.wait();
	tp.empty();
	/*for(int i = 0; i < (int)vectask.size(); i++)
	{
		if(!vectask[i])
		{
			delete vectask[i];
			vectask[i] = NULL;
		}
	}*/
	if(!res)
	{
		pdbmarticledb->Clearres(res);
	}
	theLog.WriteLog(LOG_LEVEL_SYS,"the  fetch complete!",row2);
	freeDB();
	return true;
}
/// get page from network
bool CNetSpider::getPage(const char* url, int method, std::string& response) const
{
	// Todo : add proxy and authentication support
	response.clear();
	string strurl = url;
	boost::regex regspace(" ");
	strurl = boost::regex_replace(strurl, regspace, "%20");
	HttpWrap *phttpwrap = new HttpWrap();
	response += phttpwrap->Get(strurl.c_str());
	delete phttpwrap;
	phttpwrap = NULL;
	if (response.length() == 0)
	{
		return false;
	} 
	else
	{
		boost::regex regRn("\\r\\n");
		response = boost::regex_replace(response, regRn, "\\n");
		boost::regex reg1("<meta(.*?)charset=(.*?)>",boost::regex::icase); 
		boost::smatch what;
		std::string::const_iterator start = response.begin();
		std::string::const_iterator end1 = response.end();
		if( boost::regex_search(start, end1, what, reg1) )
		{
			string strcharset(what[2].first,what[2].second);
			if((int)strcharset.length() == 0)
			{
				if(!IsUTF8(response.c_str(),response.length()))
				{
					string strtmp = "";
					iconv_string("gbk","utf-8", response.c_str(), response.length(),strtmp,1);
					response = strtmp;

				}
			}
			else 
			{
				if ((int)strcharset.find("UTF") >= 0 || (int)strcharset.find("utf") >= 0)
				{
					;
				} 
				else
				{
					string strtmp = "";
					iconv_string("gbk","utf-8", response.c_str(), response.length(),strtmp,1);
					response = strtmp;
				}
			}
		}
		else if(!IsUTF8(response.c_str(),response.length())/*pSpiderconf->m_config.ruleCollections[pSpiderconf->m_config.plans[m_plan_id].collectionRuleId].charset != "utf-8"*/)
		{
			string strtmp = "";
			iconv_string("gbk","utf-8", response.c_str(), response.length(),strtmp,1);
			response = strtmp;

		}
	}
	return true;
}
/*bool CNetSpider::senddata(BSONObj obj)
{

     string straid = obj.getStringField("id");
	 char *title = urldecode(const_cast<char*>(obj.getStringField("title")));
	 string strtitle = title;
	 free(title);
	 char *url = urldecode(const_cast<char*>(obj.getStringField("url")));
	 string strurl = url;
	 free(url);
	 string strurlsum = obj.getStringField("urlsum");
	 char *source = urldecode(const_cast<char*>(obj.getStringField("source")));
	 string strsource = source;
	 free(source);
	 string strpubtime = obj.getStringField("pubtime");
	 char *content = urldecode(const_cast<char*>(obj.getStringField("content")));
	 string strcontent = content;
	 free(content);
 	 if(straid == "" || strtitle == "" || strurl == "" || strurlsum == "" || strsource == "" || strpubtime == "" || strcontent == "" )
 		 return false;
// 	 else if(findarticle(strurlsum))
// 		 return false;
 	 else
 	{
		boost::regex regspace("\\r\\n");
		strcontent = boost::regex_replace(strcontent, regspace, "\\n");
// 		BSONObj p = BSON("_id"<<222<<"title"<< strtitle.c_str()<<"url"<<strurl.c_str()<<"content"<< strcontent.c_str());
// 		cout<<p.jsonString()<<endl<<endl;
		string strsql = "insert into article(url_md5,source_id,plan_id,create_time,fetch_time,article_type) values('"+\
			strurlsum + "',"  + getsourceid(strsource) + "," + my_itoa(999999) +",timestamp'" + strpubtime + "',LOCALTIMESTAMP(0)," \
			+ my_itoa(1) + ");insert into articleidfor41(aid) values(" + straid + ");";
		pdbmarticledb->CommandDB(strsql.c_str());
		strsql = "select article_id from article where url_md5 = '" + strurlsum +"'";
		PGresult* res = pdbmarticledb->QueryDB(strsql.c_str());
		if (res)
		{
			if (pdbmarticledb->Getntuples(res) == 0)
			{
				pdbmarticledb->Clearres(res);
				return false;
			}
			else
			{
				string strtmp = pdbmarticledb->Getvalue(res,0,0);
				pdbmarticledb->Clearres(res);
			//	BSONObj p = BSON("_id"<<pmongo->aidtooid(strtmp.c_str())<<"title"<< strtitle.c_str()<<"url"<<strurl.c_str()<<"content"<< strcontent.c_str());
			//	pmongo->insert(p);
				return true;

			}
		} 
		else
			return false;
	 }

	 return true;
}*/
bool CNetSpider::findarticle(CDBManager *pdbm,string urlmd5)
{
	string strsql = "select newarticleurl from newarticleurltable where newarticleurl = '" + urlmd5 +"';";
	PGresult* res = pdbm->QueryDB(strsql.c_str());
	int ntuples = pdbm->Getntuples(res);
	pdbm->Clearres(res);
	if (ntuples == 0)	 
		return false;
	else
		return true;
}
/*string CNetSpider::get41articleObj(string startid,string nnum)
{

 	string strurl = "http://218.17.161.34:10085/cspider/query.php?num=" + nnum + "&s_id=" + startid; 
 	string page;
	if (!getPage(strurl.c_str(), VEBGET, page))
	{
		return false;
	}
 	if((int)page.find("{") != 0 || page.substr(page.length()-1) != "}")
 	{
 		theLog.WriteLog(LOG_LEVEL_ERROR,"stock code: %s can't get data from see !",strurl.c_str());
 		return "";
 	}
 	else
 	{
 		BSONObj obj;
 		try
 		{
 			obj = fromjson(page);
 			if(obj.hasField("articles") && obj.hasField("endid"))
 			{
 				mongo::BSONObj eles = obj["articles"].Obj();
 				vector<mongo::BSONObj> specs;  
 				eles.Vals(specs);
 				for(int i = 0; i < (int)specs.size(); ++i)
 				{
					//cout<<specs[i].jsonString()<<endl<<endl;
 					senddata(specs[i]);
 				}
				return obj.getStringField("endid");
 			}
 			else
 				return"";
 		}
 		catch (...)
 		{
 			theLog.WriteLog(LOG_LEVEL_ERROR,"url: %s can't to json !",strurl.c_str());
 			return"";
 		}
 	}
	return "";
		
}*/
bool CNetSpider::getallurl(int depth,UrlInfoMap &urlmap,string keyword,string urlkeyword,string strbuseregex)
{
	UrlInfoMap urlmaptmp = urlmap;
	for (int i = 0 ; i < depth; ++i)
	{
		urlmap = urlmaptmp;
		UrlInfoMap::iterator iter;
		for (iter = urlmap.begin();iter!=urlmap.end();++iter)
		{
			if(iter->second.bprocess == true || (int)iter->first.find("https://") >= 0)
				continue;
			string strpage;
			string strurltmp = iter->first;
			if (!getPage(strurltmp.c_str(), VEBGET, strpage))
			{
				continue;
			}
			string strrefreshurl = "";
			bool bgetpage = true;
			while ( ( strrefreshurl = getrefreshurl(strpage)) != "")
			{
				string strintactrefreshurl = _expandlinks(strurltmp,strrefreshurl);
				strurltmp = strintactrefreshurl;
				if (!getPage(strurltmp.c_str(), VEBGET, strpage))
				{
					bgetpage = false;
					break;
				}
			}
			if(!bgetpage)
				continue;
			if(btest)
				cout<<"refreshurl<:::::::::::::::::>"<<strurltmp<<endl;
			string host = parse_url(strurltmp.c_str());
			//printf("host=%s\n",host.c_str());
			HTML::ParserDom parser;

			tree<HTML::Node> dom = parser.parseTree(strpage);

			tree<HTML::Node>::iterator it = dom.begin();

			tree<HTML::Node>::iterator end = dom.end();
			for(; it != end; ++it)
			{
				if (it->isTag() && it->tagName() == "a" || it->isTag() && it->tagName() == "A")
				{
					it->parseAttributes();
					string substrurl = it->attribute("href").second;
					if((int)substrurl.find("www.") == 0)
						substrurl = "http://" + substrurl;
					if(substrurl != "" && substrurl != host && substrurl != ("http://" + host) && (int)substrurl.find("#") < 0 && \
						(int)substrurl.find("https://") < 0 && (int)substrurl.find("javascript:") < 0 && !filtersuffx(substrurl))
					{
						UrlInfo uitmp;
						string strhtml = strpage.substr(it->offset(),it->length());
						boost::regex expression;
						expression.assign("<.*?>");
						strhtml = boost::regex_replace(strhtml, expression, "");
						expression.assign("&\\w{1,8};");
						strhtml = boost::regex_replace(strhtml, expression, " ");
						utfstrltrim(strhtml);
						utfstrrtrim(strhtml);
						if(btest)
							cout<<strurltmp<<":"<<substrurl<<endl;
						string strtmp = _expandlinks(strurltmp,substrurl);
						if((int)strtmp.find(urlkeyword) < 0 )
							continue;
						if(strhtml == strtmp)
							strhtml = "";
						uitmp.strtitle = strhtml;
						uitmp.bprocess = false;
						if (strbuseregex == "t")
						{
							try
							{
								//cout<<"strtmp="<<strtmp<<",keyword="<<keyword<<endl;
								expression.assign(keyword);
								boost::cmatch what;
								if(boost::regex_match(strtmp.c_str(), what, expression))
								{
									//cout<<"!!!!!!!!!!!!!!!!!!!!!!strtmp="<<strtmp<<",keyword="<<keyword<<endl;
									uitmp.bprocess=true;
								}
								}
							catch (const boost::bad_expression& e)
							{
								theLog.WriteLog(LOG_LEVEL_ERROR,"regex_match keyword:%s , strtmp:%s !",keyword.c_str(),strtmp.c_str());
							}
						} 
						else
						{
							if((int)strtmp.find(keyword) > 0 )
								uitmp.bprocess=true;
						}
						if(btest)
						{
							if(uitmp.bprocess == true)
								cout<<strtmp<<"<==article==>"<<strhtml<<endl;
							else
								cout<<strtmp<<"<==notarticle==>"<<strhtml<<endl;
						}
						//if(urlmaptmp.find(strtmp) == urlmaptmp.end() && (int)strtmp.find(host) > 0)
						if(urlmaptmp.find(strtmp) == urlmaptmp.end())
							urlmaptmp.insert(pair<string,UrlInfo>(strtmp,uitmp));
						else if (urlmaptmp.find(strtmp) != urlmaptmp.end())
						{
							if(urlmaptmp[strtmp].strtitle == "")
								urlmaptmp[strtmp].strtitle = strhtml;
						}
					}
				}
			}
			urlmaptmp[iter->first].bprocess = true;
		}
	}
	urlmap = urlmaptmp;
	return true;
}
string CNetSpider::getrefreshurl(string strpage)
{
	boost::regex expression;
	boost::smatch what;
	string strrefreshurl = "";
	std::string::const_iterator start1 = strpage.begin();
	std::string::const_iterator end11 = strpage.end();
	expression.assign("<META HTTP-EQUIV=\"REFRESH\" CONTENT=\"0;\\s?URL=(.*?)\"",boost::regex::icase);
	while( boost::regex_search(start1, end11, what, expression) )
	{
		std::string msg(what[1].first, what[1].second);
		strrefreshurl = msg;
		printf("strrefreshurl=%s\n",strrefreshurl.c_str());
		start1 = what[1].second;
	}
	if(strrefreshurl == "" && (int)strpage.length() < 250)
	{
		expression.assign("<script(.*?)\\.location(.*?)=['|\"]+(.*?)['|\"]+(.*?)</script>",boost::regex::icase);
		while( boost::regex_search(start1, end11, what, expression) )
		{
			std::string msg(what[3].first, what[3].second);
			strrefreshurl = msg;
			printf("strrefreshurl=%s\n",strrefreshurl.c_str());
			start1 = what[3].second;
		}
	}
	return strrefreshurl;
}
bool CNetSpider::filtersuffx(string strurl)
{
	int npos = 0;
	string strurltmp = strurl;
	transform(strurltmp.begin(),strurltmp.end(),strurltmp.begin(), ::tolower);//תСд
	string strsuffix;
	if((npos=(int)strurltmp.rfind(".")) > 0)
		strsuffix = strurltmp.substr(npos);
	if(find(m_filtersuffix.begin(),m_filtersuffix.end(),strsuffix)!=m_filtersuffix.end())
		return true;
	else
		return false;
}
bool CNetSpider::filterarticle(string strtitle)
{
	if(find(m_filterarticle.begin(),m_filterarticle.end(),strtitle)!=m_filterarticle.end())
		return true;
	else
		return false;
}
char CNetSpider::lowerCase (char a) {
	if (a >= 'A' && a <= 'Z') {
		return a - 'A'+ 'a' ;
	} else {
		return a;
	}
}

/* test if b starts with a
*/
bool CNetSpider::startWith (const char *a,const char *b) {
	int i=0;
	while (a[i] != 0) {
		if (a[i] != b[i]) return false;
		i++;
	}
	return true;
}
string CNetSpider::parse_url (const char *charg) {
 	if(!charg)
 		return "";
 	int deb = 0, fin = deb;
 	char *arg = new char[strlen(charg)+1];
	char *argtmp = arg;
 	strcpy(arg,charg);
 	if(startWith("http://",arg))
 		arg += 7;
 	while (arg[fin] != '/' && arg[fin] != ':' && arg[fin] != 0) {
 		fin++;
 	}
 	if (fin == 0) 
 		return "";
 
 	// get host name
 	char *host = new char[fin+1];
 	for (int  i=0; i<fin; i++) {
 		host[i] = lowerCase(arg[i]);
 	}
 	host[fin] = 0;
 	string strhost = host;
    delete [] argtmp;
	arg = NULL;
	argtmp = NULL;
 	delete [] host;
 	host = NULL;
 	return strhost;
	/*// get port number
	if (arg[fin] == ':') {
	int port = 0;
	fin++;
	while (arg[fin] >= '0' && arg[fin] <= '9') {
	port = port*10 + arg[fin]-'0';
	fin++;
	}
	}

	// get file name
	char *file = new char[strlen(arg)-fin+1];
	if (arg[fin] != '/') {
	// www.inria.fr => add the final /
	//file = string("/");
	strcpy(file,"/");
	} else {
	strcpy(file,arg+fin);
	}*/
}
string CNetSpider::_expandlinks(string strurl,string strurlsub)
{
	boost::regex expression;
	boost::smatch what;
	string strsearch="",strsearchhead = "" ,strsearchtail = "",strroot="",strhost="",strurlsubtmp = strurlsub;
	strhost = parse_url(strurl.c_str());
	std::string::const_iterator start1 = strurl.begin();
	std::string::const_iterator end11 = strurl.end();
	expression.assign("^([^\\?]+)");
	while( boost::regex_search(start1, end11, what, expression) )
	{
		std::string msg(what[1].first, what[1].second);
		strsearch = msg;
		start1 = what[1].second;
	}
	strsearchhead = strsearch.substr(0,strsearch.find(strhost)+strhost.length());
	strsearchtail = strsearch.substr(strsearch.find(strhost)+strhost.length());
	expression.assign("/[^/]+\\.[^/\\.]+$");
	strsearchtail = boost::regex_replace(strsearchtail, expression, "");
	expression.assign("/$");
	strsearchtail = boost::regex_replace(strsearchtail, expression, "");
	strsearch = strsearchhead + strsearchtail;
	strroot = "http://" + strhost;
	expression.assign("http://"+strhost);
	strurlsubtmp = boost::regex_replace(strurlsubtmp, expression, "");
	expression.assign("^/");
	strurlsubtmp = boost::regex_replace(strurlsubtmp, expression, strroot+"/");
	expression.assign("^(?!mailto:)(?!http://)");
	strurlsubtmp = boost::regex_replace(strurlsubtmp, expression, strsearch+"/");
	expression.assign("/\\./");
	strurlsubtmp = boost::regex_replace(strurlsubtmp, expression, "/");
	while((int)strurlsubtmp.find("/../") > 0)
	{
		expression.assign("/[^/]+/\\.\\./");
		strurlsubtmp = boost::regex_replace(strurlsubtmp, expression, "/");
		if((int)strurlsubtmp.find("http://../") == 0)
			break;
	}
	return strurlsubtmp;
}
string CNetSpider::getsourceid(string strsource)
 {
 	 string strsql = "select source_id from source where source_name = '" + strsource + "'";
 	 string strsqlinsert = "insert into source(source_name) values('" + strsource +"')";
 	 PGresult* res = pdbmarticledb->QueryDB(strsql.c_str());
 	 if(res)
 	 {
 		 if (pdbmarticledb->Getntuples(res) == 0) 
 		 {
 			 pdbmarticledb->Clearres(res);
 			 PGresult* res1 = NULL;
 			 pdbmarticledb->CommandDB(strsqlinsert.c_str());
 			 res1 = pdbmarticledb->QueryDB(strsql.c_str());
 			 if(pdbmarticledb->Getntuples(res1) == 0)
 			 {
 				 pdbmarticledb->Clearres(res1);
 				 return "0";
 
 			 }
 			 else 
 			 {
 				 string strtmp = pdbmarticledb->Getvalue(res1,0,0);
 				 pdbmarticledb->Clearres(res1);
 				 return strtmp;
 			 }
 
 		 }
 		 else
 		 {
 			 string strtmp = pdbmarticledb->Getvalue(res,0,0);
 			 pdbmarticledb->Clearres(res);
 			 return strtmp;
 		 }
 	 }
 	 else return "0";
 }
} // end ISpider
}
/*if(!OninitDBM())
	{
		freeDB();
		return false;
	}
	PGresult   *res = NULL;
	ThreadParm m_tp[MAXTHREADNO];
	thread_group threads;
	int nthreads = 8;
	//std::string strsql= "select * from seed_url where bselect = true order by uid limit 50;";
	res = pdbmarticledb->QueryDB(m_config["plansql"].c_str());
	int row2 = pdbmarticledb->Getntuples(res);
	theLog.WriteLog(LOG_LEVEL_SYS,"the %d newssite was be selected !",row2);
	for(int i=1;i<=8;++i)
	{
		m_tp[i-1].nthread = i;
		m_tp[i-1].ifront = (i-1)*row2/nthreads;
		m_tp[i-1].inext = i*row2/nthreads;
		m_tp[i-1].res = res;
		m_tp[i-1].pns = this;
		threads.create_thread(bind(&threadfun,&m_tp[i-1]));
	}
	threads.join_all();
	pdbmarticledb->Clearres(res);
	theLog.WriteLog(LOG_LEVEL_SYS,"the  fetch complete!",row2);
	freeDB();*/
/*	if(!OninitDBM())
	{
		freeDB();
		return false;
	}
	PGresult   *res = NULL;
	//std::string strsql= "select * from seed_url where bselect = true order by uid limit 50;";
	res = pdbmarticledb->QueryDB(m_config["plansql"].c_str());
	int row2 = pdbmarticledb->Getntuples(res);
	theLog.WriteLog(LOG_LEVEL_SYS,"the %d newspaper was be selected !",row2);
	if (row2>0)
	{
		for (int i = 0 ; i < row2 ; ++i)
		{
			string struid = pdbmarticledb->Getvalue(res,i,0);
			string strname = pdbmarticledb->Getvalue(res,i,1);
			string strurl = pdbmarticledb->Getvalue(res,i,2);
			string strkeyword = pdbmarticledb->Getvalue(res,i,3);
			string strurlkeyword = pdbmarticledb->Getvalue(res,i,6);
			string strbuseregex = pdbmarticledb->Getvalue(res,i,7);
			UrlInfoMap m_url ;
			UrlInfo m_ui;
			m_ui.bprocess = false;
			m_ui.strtitle = strname;
			m_url.insert(pair<string,UrlInfo>(strurl,m_ui));
			getallurl(2,m_url,strkeyword,strurlkeyword,strbuseregex);
			UrlInfoMap::iterator ite;
			int narticlecount = 0 ;
			for (ite=m_url.begin();ite!=m_url.end();++ite)
			{
				if(ite->second.strtitle == "")
					continue;
				else if(filterarticle(ite->second.strtitle))
				{
					narticlecount++;
					continue;
				}
				if(strbuseregex == "t")
				{
					boost::regex regkeyword(strkeyword);
					boost::cmatch what;
					if(boost::regex_match(ite->first.c_str(), what, regkeyword))
					{
						inserturl(ite->first,ite->second.strtitle,strname);
						narticlecount++;
					}
				}
				else
				{
					if((int)ite->first.find(strkeyword) > 0)
					{
						inserturl(ite->first,ite->second.strtitle,strname);
						narticlecount++;
					}
				}
				
			}
			if(narticlecount > 0 )
			{
				string strsql = "update seed_url set state = 1 where uid = " + struid ;
				pdbmarticledb->CommandDB(strsql.c_str());
			}
			else
			{
				string strsql = "update seed_url set state = -1 where uid = " + struid ;
				pdbmarticledb->CommandDB(strsql.c_str());
			}
			theLog.WriteLog(LOG_LEVEL_SYS,"the  newspaper uid:%s name:%s was fetch complete!",struid.c_str(),strname.c_str());
		}
	}
	pdbmarticledb->Clearres(res);
	theLog.WriteLog(LOG_LEVEL_SYS,"the  fetch complete!",row2);
	freeDB();*/
// 	if(!OninitDBM())
// 	{
// 		freeDB();
// 		return false;
// 	}
// 	theLog.WriteLog(LOG_LEVEL_ERROR,"run start!!!");
// 	PGresult   *res = NULL;
// 	std::string strsql= "select max(aid) from articleidfor41;";
// 	res = pdbmarticledb->QueryDB(strsql.c_str());
// 	int row2 = pdbmarticledb->Getntuples(res);
// 	if (row2>0)
// 	{
// 		string nmaxid = pdbmarticledb->Getvalue(res,0,0),max41aid;
// 		pdbmarticledb->Clearres(res);
// 		string strurl = "http://218.17.161.34:10085/cspider/query.php?max_num=getmaxnum"; 
// 		string page;
// 		int error_id = getPage(const_cast<char*>(strurl.c_str()), VerbGet, NULL, 0,20, page);
// 		if(error_id)
// 		{
// 			theLog.WriteLog(LOG_LEVEL_ERROR,"get nmaxpage = NULL!!!");
// 			freeDB();
// 			return false;
// 		}
// 		regex reg("^\\{\"maxid\":\"(.*?)\"\\}$"); 
// 		cmatch what;
// 		if(regex_match(page.c_str(), what, reg))
// 			max41aid = what[1];
// 		else
// 		{
// 			theLog.WriteLog(LOG_LEVEL_ERROR,"get max41aid = NULL!!!");
// 			freeDB();
// 			return false;
// 		}
//  		while(1)
//  		{
//  			nmaxid = get41articleObj(nmaxid,my_itoa(500));
// 			if(nmaxid == "")
// 			{
// 				theLog.WriteLog(LOG_LEVEL_ERROR,"get nmaxid = NULL!!!");
// 				break;
// 			}
//  			else if(atoi(nmaxid.c_str())>=atoi(max41aid.c_str()))
// 			{
// 				theLog.WriteLog(LOG_LEVEL_ERROR,"the nmaxid = %d!!!",atoi(nmaxid.c_str()));
//  				break;
// 			}
//  		}
// 		theLog.WriteLog(LOG_LEVEL_ERROR,"break!!!!!");
// 		
// 	}
// 	else 
// 	{
// 		pdbmarticledb->Clearres(res);
// 	}
// 	freeDB();
