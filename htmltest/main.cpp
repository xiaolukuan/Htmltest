#include "main.h"
/// get page from network
bool getPage(const char* url, int method, std::string& response)
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
	if (response.length() < 200)
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
string readfile( string filename)
{
	ifstream file(filename.c_str());
	string strcontent;
	if (!file.is_open()) 
	{
		cerr << "Unable to open file " << endl;
		exit(1);
	}
	while (1)
	{
		char buf[BUFSIZ];
		file.read(buf, BUFSIZ);
		if(file.gcount() == 0) {
			break;
		}
		strcontent.append(buf, file.gcount());
	}
	file.close();
	return strcontent;
}
char lowerCase (char a) {
	if (a >= 'A' && a <= 'Z') {
		return a - 'A'+ 'a' ;
	} else {
		return a;
	}
}

/* test if b starts with a
*/
bool startWith (const char *a,const char *b) {
	int i=0;
	while (a[i] != 0) {
		if (a[i] != b[i]) return false;
		i++;
	}
	return true;
}
string parse_url (const char *charg) 
{
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
}
string _expandlinks(string strurl,string strurlsub)
{
	boost::regex expression;
	boost::smatch what;
	string strsearch="",strsearchhead = "" ,strsearchtail = "",strroot="",strhost="",strurlsubtmp = strurlsub;
	strhost = parse_url(strurl.c_str());
	printf("strhost = %s\n",strhost.c_str());
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
	//	printf("aaaa\n");
		printf("strurlsubtmp=%s\n",strurlsubtmp.c_str());
		expression.assign("/[^/]+/\\.\\./");
		//expression.assign("/[^/\\.]+/\\.\\./");
		strurlsubtmp = boost::regex_replace(strurlsubtmp, expression, "/");
	//	printf("strurlsubtmp=%s\n",strurlsubtmp.c_str());
		if((int)strurlsubtmp.find("http://../") == 0)
			break;
	}
	return strurlsubtmp;
}
string maincontenttagclean(string content)
{
	boost::regex reg4("((?i)(<param[^>]+>))");  
	content = boost::regex_replace(content,reg4,"");
	boost::regex reg9("((?i)(<pre>.*?</pre>))");  
	content = boost::regex_replace(content,reg9,"");
	//printf("aaaa=%s\n",content.c_str());
	boost::regex reg("(?i)</p>");
	content = boost::regex_replace(content,reg,"<pbeikeruishengxlktest>");
	//printf("bbbb=%s\n",content.c_str());
	//boost::regex reg5("((?i)<(?!p|img)[^>]+>)");
	boost::regex reg1("((?i)<(?!p|img|a|/a)[^>]+>)");
	content = boost::regex_replace(content,reg1,"");
//	printf("cccc=%s\n",content.c_str());
	boost::regex reg2("(?i)<pbeikeruishengxlktest>");
	content = boost::regex_replace(content,reg2,"</p>");
//	printf("dddd=%s\n",content.c_str());
	boost::regex reg3("((?i)(<a[^>]+>))");
	content = boost::regex_replace(content,reg3,"<a>");
//	printf("eeee=%s\n",content.c_str());
	boost::regex reg5("((?i)(<p[^>]+>))");
	content = boost::regex_replace(content,reg5,"<p>");	
	boost::regex reg6("((?i)(\\r))");
	content = boost::regex_replace(content,reg6,"");
	boost::regex reg7("((?i)(<a></a>))");
	content = boost::regex_replace(content,reg7,"");
	boost::regex reg8("((?i)(^\\s*$))");
	content = boost::regex_replace(content,reg8,"");
	//boost::regex reg9("((?i)(<pre[^>]+>))");
	//content = boost::regex_replace(content,reg9,"");
	boost::regex reg10("((?i)(&nbsp;))");
	content = boost::regex_replace(content,reg10,"");
	return content;
}
string Introductioncontenttagclean(string content)
{
	boost::regex reg("((?i)(<a[^>]+>.*?</a>))");
	content = boost::regex_replace(content,reg,"");
	boost::regex reg1("(?i)</p>");
	content = boost::regex_replace(content,reg1,"<pbeikeruishengxlktest>");
	boost::regex reg2("((?i)<(?!p|img|meta)[^>]+>)");
	content = boost::regex_replace(content,reg2,"");
	boost::regex reg3("(?i)<pbeikeruishengxlktest>");
	content = boost::regex_replace(content,reg3,"</p>");
	boost::regex reg4("((?i)(<p[^>]+>))"); 
	content = boost::regex_replace(content,reg4,"<p>");
	return content;
}
string mainpagetagclean(string page)
{
	boost::regex reg1("((?i)(<script(.*?)</script>))");
	page = boost::regex_replace(page,reg1,"");
	boost::regex reg2("<!--(.*?)-->");
	page = boost::regex_replace(page,reg2,"");
	boost::regex reg3("((?i)(<iframe[^>]*>.*?</iframe>))");
	page = boost::regex_replace(page,reg3,"");
	boost::regex reg4("((?i)(<style(.*?)</style>))");
	page = boost::regex_replace(page,reg4,"");
	if(!WriteFile("source.html",(const char *)page.c_str(),page.length(),true))
	{
		printf("write source.html failed！\n");
	}
	return page;
}

string publish_time_deal(string publish_time)
{
	printf("publish_time before=%s\n",publish_time.c_str());
	boost::regex reg("((?i)(20\\d{2}([^\\da-zA-Z]{1,4})\\d{1,2}([^\\da-zA-Z]{1,4})\\d{1,2}))");
	boost::smatch m;
	if(boost::regex_search(publish_time,m,reg))
	{
		reg.assign("(?i)([^\\d:\\s])");
		publish_time = boost::regex_replace(publish_time,reg,"-");
		reg.assign("(?i)(-+)");
		publish_time = boost::regex_replace(publish_time,reg,"-");

		string temp;
		time_t now;
		struct tm *tm_now;
		time(&now);
		tm_now = localtime(&now);
		printf("publish_time back=%s\n",publish_time.c_str());
		printf("now date: %d-%d-%d\n",tm_now->tm_year+1990,tm_now->tm_mon+1,tm_now->tm_mday);
		int year = atoi(publish_time.c_str());
		if( year >  tm_now->tm_year + 1900 )
		{
			publish_time.clear();
			return  publish_time;
		}
		else if( year <  tm_now->tm_year + 1900 )
		{
			return  publish_time;
		}
		temp = publish_time.substr(publish_time.find("-")+1);
		int month = atoi(temp.c_str());
		if( month >  tm_now->tm_mon+1 )
		{
			publish_time.clear();
			return  publish_time;
		}
		else if( month < tm_now->tm_mon+1 )
		{
			return  publish_time;
		}
		temp = temp.substr(temp.find("-")+1);
		int day = atoi(temp.c_str());
		if( day >  tm_now->tm_mday )
		{
			publish_time.clear();
			return  publish_time;
		}
		else
		{
			return  publish_time;
		}

	}   
	else
	{
		publish_time.clear();
	}
	
	//printf("publish_time back=%s\n",publish_time.c_str());
	return publish_time;
}
int main(int argc, char* argv[])
{
	QINIU_ACCESS_KEY = "sn7d6X2kmRQKkNyO0_ZY_Hz2utVrXIeEmc8QutVC";
	QINIU_SECRET_KEY = "jeQSWafTp7kczgR4qVa-erKOaHk0_qcvMNacxO8E";
	curl_global_init(CURL_GLOBAL_ALL);
	Qiniu_Global_Init(-1);                  /* 全局初始化函数，整个进程只需要调用一次 */
	if(strcmp(argv[1],"imgtest") == 0 && argc == 4)
	{
		string oldurl,newurl,articleurl;
		articleurl = argv[2];
		oldurl     = argv[3];
		newurl =  _expandlinks(articleurl,oldurl);
		printf("newurl=%s\n",oldurl.c_str());
	}
	if(strcmp(argv[1],"-start") == 0 && argc == 4)
	{
		int ntime = atoi(argv[2]);
		int nprocesstimes = 1;
		ArticleManage m_article;
		theLog.SetLogFilePath(GetFullPath());
		if(!ReadConfigFile((GetFullPath()+"/sqlconfig.conf").c_str(),p_config))
		{
			return 0;
		}
		if(!ReadSpiderRulerConfigFile((GetFullPath()+"/spiderruler.conf").c_str(),p_spiderruler))
		{
			return 0;
		}
		while(1)
		{
			
			m_article.start(atoi(argv[3]));
			theLog.WriteLog(LOG_LEVEL_SYS,"the proc ArticleExtract %d times completed!",nprocesstimes);
			nprocesstimes++;
			sleep(ntime * 60);
		}
	}
	
	if(argc == 3 && strcmp(argv[1],"-start") != 0 )
	{
		if(!ReadSpiderRulerConfigFile((GetFullPath()+"/spiderruler.conf").c_str(),p_spiderruler))
		{
			return 0;
		}
		string page;
		list<string> ImgStrList;
		page.clear();
		const char* url = argv[1];
		string sourcepagename = argv[2];
		//const char* url = "http://admin.wechat.com/mp/appmsg/show?__biz=MjM5MTIwODcxNA==&appmsgid=10001872&itemidx=1&sign=d5997fecd12a3af79f8c8d65600f82a1";
		printf("url=%s\n",url);
		string urlstr = url;
		if(0 == sourcepagename.compare("zatu")) 
		{
			string strtmp = "";
			iconv_string("utf-8","gbk", urlstr.c_str(), urlstr.length(),strtmp,1);
			urlstr = strtmp;
			printf("urlstr=%s\n",urlstr.c_str());
		}
		
		int method = 0;
		if(!getPage(urlstr.c_str(), method,page))
		{
			printf("不能获取URL内容\n");
			return 0;
		}
		string content;
		string Introduction;
		string publishtime;
		string titlestr;
		string contentimg;
		page = mainpagetagclean(page);
		if(page.length() < 2048)
		{
			printf("the page source length too short ! \n");
			return 0;
		}
		printf("page length=%d\n",page.length());
		
		HtmlExtract sorceExtract(page,p_spiderruler[sourcepagename]);
		sorceExtract.Extract();
		titlestr	= sorceExtract.GetTitle();
		boost::regex title_reg("((?i)(&nbsp;))");
		titlestr = boost::regex_replace(titlestr,title_reg,"");
		printf("title=%s\n",titlestr.c_str());
		content			= sorceExtract.ArticleContent;
		publishtime		= sorceExtract.GetDateTime();
		Introduction	= sorceExtract.Introduction;
		strltrim(publishtime);
		publishtime		= publish_time_deal(publishtime);
		if(0 == sourcepagename.compare("geekpark"))  //极客公园特殊处理
		{
			//boost::regex reg14("((?i)(<div\\s{1,4}class=\"yarpp-related\".*?</div>))");
			boost::regex reg14("((?i)(<div\\s{1,4}(id=\"tags\").*?</div>))");
			content = boost::regex_replace(content,reg14,"");
		}
		if(0 == sourcepagename.compare("cuntuba"))  //苹果网特殊处理
		{
			//boost::regex reg14("((?i)(<div\\s{1,4}class=\"yarpp-related\".*?</div>))");
			boost::regex reg14("((?i)(<div\\s{1,4}(?!class=\"cont\").*?</div>))");
			content = boost::regex_replace(content,reg14,"");
		}
		if(0 == sourcepagename.compare("macx"))  //苹果网特殊处理
		{
			//boost::regex reg14("((?i)(<div\\s{1,4}class=\"yarpp-related\".*?</div>))");
			boost::regex reg14("((?i)(<div\\s{1,4}(?!class=\"v2-t_fsz\").*?</div>))");
			content = boost::regex_replace(content,reg14,"");
			reg14.assign("(?i)(<a[^>]*>.*?</a>)");
			content = boost::regex_replace(content,reg14,"");
		}
		if(0 == sourcepagename.compare("leiphone"))  //雷锋网尾部特殊处理
		{
			//boost::regex reg14("((?i)(<div\\s{1,4}class=\"yarpp-related\".*?</div>))");
			boost::regex reg14("((?i)(<div\\s{1,4}(?!class=\"post_content\").*?</div>))");
			content = boost::regex_replace(content,reg14,"");
			reg14.assign("(?i)(<div>.*?</div>)");
			content = boost::regex_replace(content,reg14,"");
		}
		if(0 == sourcepagename.compare("tech163"))  //网易科技尾部特殊处理
		{
			//boost::regex reg14("((?i)(<div\\s{1,4}class=\"yarpp-related\".*?</div>))");
			boost::regex reg14("((?i)(<div\\s{1,4}(?!id=\"endtext\").*?</div>))");
			content = boost::regex_replace(content,reg14,"");
		}
		if(0 == sourcepagename.compare("pingwest"))  //pingwest尾部特殊处理
		{
			//boost::regex reg14("((?i)(<div\\s{1,4}class=\"yarpp-related\".*?</div>))");
			boost::regex reg15("((?i)(<div.*?>[^<]+</div>))");
			content = boost::regex_replace(content,reg15,"");
		}
		if(0 == sourcepagename.compare("zatu"))  //杂图天下尾部特殊处理
		{
			//boost::regex reg14("((?i)(<div\\s{1,4}class=\"yarpp-related\".*?</div>))");
			boost::regex reg14("((?i)(<div\\s{1,4}(?!class=\"format_text entry-content\").*?</div>))");
			content = boost::regex_replace(content,reg14,"");
		}
		if(0 == sourcepagename.compare("jandan"))
		{
			boost::regex reg11("((?i)(<a[^>]*>.*?</a>))");
			content = boost::regex_replace(content,reg11,"");
			boost::regex reg12("((?i)(<span[^>]*>.*?</span>))");
			content = boost::regex_replace(content,reg12,"");
		}
		if(0 == sourcepagename.compare("guaixun"))
		{
			boost::regex reg13("((?i)(<div style=\"position:absolute.*?</div>))");
			content = boost::regex_replace(content,reg13,"");
		}
		
		content = maincontenttagclean(content);	
		strltrim(content);
		boost::smatch m;
		boost::regex reg8;
		if(sourcepagename == "sinablogit")
		{
			reg8.assign("(?i)( src\\s{0,2}=\\s{0,2}\"([^\"]*)\")");
			content = boost::regex_replace(content,reg8,"");
			reg8.assign("(?i)( real_src =)");
			content = boost::regex_replace(content,reg8," src=");
		}
		else if(sourcepagename == "aqee")
		{
			reg8.assign("(?i)( src\\s{0,2}=\\s{0,2}\"([^\"]*)\")");
			content = boost::regex_replace(content,reg8,"");
			reg8.assign("(?i)( data-original=)");
			content = boost::regex_replace(content,reg8," src=");
		}
		else if(sourcepagename == "macx")
		{
			reg8.assign("(?i)( src\\s{0,2}=\\s{0,2}\"([^\"]*)\")");
			content = boost::regex_replace(content,reg8,"");
			reg8.assign("(?i)( zoomfile=)");
			content = boost::regex_replace(content,reg8," src=");
		}
		if(0 == sourcepagename.compare("cuntuba"))  //寸土吧特殊处理
		{
			reg8.assign("(?i)(\')");
			content = boost::regex_replace(content,reg8,"\"");
		}
		reg8.assign("(?i)( src\\s{0,2}=\\s{0,2}\"([^\"]*)\")");
		
		contentimg = content;
		boost::regex reg("((?i)<((?!img)[^>]*>))");
		contentimg = boost::regex_replace(contentimg,reg,"");

		//boost::regex reg8("(?i)(src=\"([^\"]*)\")");
		std::string::const_iterator start = contentimg.begin();
		std::string::const_iterator end = contentimg.end();
		try
		{
			while(boost::regex_search(start,end,m,reg8))
			{
				if (m[0].matched)
				{
					string tempurl(m[0].first,m[0].second);
					string regurl;
					tempurl = tempurl.substr(tempurl.find_first_of('\"',0)+1,tempurl.find_last_of('\"')-tempurl.find_first_of('\"',0)-1);
					//if( 0 == sourcepagename.compare("zatu") )
				//	{
						regurl =  _expandlinks(urlstr,tempurl);
			//		}
					if(0 != tempurl.length())
					{
						boost::regex reg(tempurl);
						content = boost::regex_replace(content,reg,regurl);
						ImgStrList.push_back(regurl);
					}
					start = m[0].second;
				}
			} 
		}
		catch (const boost::bad_expression& e)
		{
			theLog.WriteLog(LOG_LEVEL_ERROR,"cann't create regex with %s!",urlstr.c_str());
		}
		list<string>::iterator it;
		for( it = ImgStrList.begin(); it != ImgStrList.end(); it++)
		{
			printf("%s\n",(*it).c_str());
		}
		if( 0 == Introduction.length() )
		{
			boost::smatch m1;
			boost::regex reg("(?i)(<p>.*?</p>)");
			std::string::const_iterator start = content.begin();
			std::string::const_iterator end = content.end();
			while(boost::regex_search(start,end,m1,reg))
			{
				if (m1[0].matched)
				{
					Introduction = m1[0].str();
				}
				break;
			}
				
			int pos;
			if( 0 == Introduction.length() )
			{
				if(-1 != (pos = content.find_first_of("\x0d\x0a",0)))
				{
					Introduction = content.substr(0,pos);
				}
			}
		}
		Introduction = Introductioncontenttagclean(Introduction);
		strltrim(Introduction);
		if( 0 == p_spiderruler[sourcepagename].summaryisinbody.compare("yes") )
		{
			content = Introduction + content;
		}
		printf("publishtime=%s\n",publishtime.c_str());
		printf("Introduction=%s\n",Introduction.c_str());
		printf("content=%s\n",content.c_str());
	}
	Qiniu_Global_Cleanup();                 /* 全局清理函数，只需要在进程退出时调用一次 */
	return 0;


}
