#include"articleextract.h"

/* @gist debug */
void qiniudebug(Qiniu_Client* client, Qiniu_Error err)
{
	theLog.WriteLog(LOG_LEVEL_ERROR,"error code: %d, message: %s",err.code,err.message);
	theLog.WriteLog(LOG_LEVEL_ERROR,"respose header:\n%s",Qiniu_Buffer_CStr(&client->respHeader));
	theLog.WriteLog(LOG_LEVEL_ERROR,"respose body:\n%s",Qiniu_Buffer_CStr(&client->b));
	//printf("error code: %d, message: %s\n", err.code, err.message);
	//printf("respose header:\n%s", Qiniu_Buffer_CStr(&client->respHeader));
	//printf("respose body:\n%s", Qiniu_Buffer_CStr(&client->b));
}
/* @endgist */
char* uptoken(Qiniu_Client* client, const char* bucket)
{
    Qiniu_RS_PutPolicy putPolicy;
    Qiniu_Zero(putPolicy);
    putPolicy.scope = bucket;
    return Qiniu_RS_PutPolicy_Token(&putPolicy, NULL);
}
char* upload(Qiniu_Client* client, char* uptoken, const char* key, const char* localFile)
{
    Qiniu_Error err;
    Qiniu_Io_PutRet putRet;
    err = Qiniu_Io_PutFile(client, &putRet, uptoken, key, localFile, NULL);
    if (err.code != 200) {
        qiniudebug(client, err);
        return NULL;
    }
    return strdup(putRet.hash); /* 注意需要后续使用的变量要复制出来 */
}
bool articleextract::getPage(const char* url, int method, std::string& response)
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
char articleextract::lowerCase (char a) {
	if (a >= 'A' && a <= 'Z') {
		return a - 'A'+ 'a' ;
	} else {
		return a;
	}
}

/* test if b starts with a
*/
bool articleextract::startWith (const char *a,const char *b) {
	int i=0;
	while (a[i] != 0) {
		if (a[i] != b[i]) return false;
		i++;
	}
	return true;
}
string articleextract::parse_url (const char *charg) 
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
string articleextract::contenttagclean(string content)
{
	//printf("aaaa=%s\n",content.c_str());
	boost::regex reg4("((?i)(<param[^>]+>))");
	content = boost::regex_replace(content,reg4,"");
	boost::regex reg9("((?i)(<pre>.*?</pre>))");  
	content = boost::regex_replace(content,reg9,"");
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
	boost::regex reg10("((?i)(&nbsp;))");
	content = boost::regex_replace(content,reg10,"");
	return content;
}
string articleextract::pagetagclean(string page)
{
	boost::regex reg1("((?i)(<script(.*?)</script>))");
	page = boost::regex_replace(page,reg1,"");
	boost::regex reg2("<!--(.*?)-->");
	page = boost::regex_replace(page,reg2,"");
	//boost::regex reg3("((?i)(<iframe(.*?)</iframe>))");
	boost::regex reg3("((?i)(<iframe[^>]*>.*?</iframe>))");
	page = boost::regex_replace(page,reg3,"");
	boost::regex reg4("((?i)(<style(.*?)</style>))");
	page = boost::regex_replace(page,reg4,"");
	return page;
}
string articleextract::Introductioncontenttagclean(string content)
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
	boost::regex reg5("((?i)(\\r))");
	content = boost::regex_replace(content,reg5,"");
	return content;  
}
string articleextract::publish_time_deal(string publish_time)
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
	
	printf("publish_time back=%s\n",publish_time.c_str());
	return publish_time;
}
string articleextract::_expandlinks(string strurl,string strurlsub)
{
	//printf("strurl=%s\n",strurl.c_str());	
	//printf("strurlsub=%s\n",strurlsub.c_str());
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
	//	printf("aaaa\n");
		expression.assign("/[^/]+/\\.\\./");
		strurlsubtmp = boost::regex_replace(strurlsubtmp, expression, "/");
		printf("strurlsubtmp=%s\n",strurlsubtmp.c_str());
		if((int)strurlsubtmp.find("http://../") == 0)
			break;
	}
	return strurlsubtmp;
}
articleextract::articleextract():pmongom(NULL)
{
}
articleextract::~articleextract()
{
}
bool articleextract::OnExitDB()
{
	Qiniu_Client_Cleanup(&client);          /* 每个HTTP客户端使用完后释放 */
	m_dbmanager->DisconnectDB();
	if (pmongom != NULL)
	{
		delete pmongom;
		pmongom = NULL;
	}
	if(MySqlConn != NULL)
	{
		mysql_close(MySqlConn);
	}
	return true;
}
bool articleextract::OnInitDB()
{
	pthread_mutex_lock(&config_mutex);
	Qiniu_Client_InitNoAuth(&client, 1024); /* HTTP客户端初始化。HTTP客户端是线程不安全的，不要在多个线程间共用 */
	//postgrel初始化连接
	m_dbmanager = new CDBManager();
	if (!m_dbmanager->ConnectDB(p_config["articledbconninfo"].c_str()))
	{
		theLog.WriteLog(LOG_LEVEL_ERROR,"connect postgrel  failed! connectinfo: %s...",p_config["articledbconninfo"].c_str());
		pthread_mutex_unlock(&config_mutex);
		return false;
	}
	else 
		m_dbmanager->SetClientEncoding("GB18030");

	//mongodb初始化连接，验证
	pmongom = new MongoManager();
	if (!pmongom->conn(p_config["mongohost"].c_str()))
	{
		theLog.WriteLog(LOG_LEVEL_ERROR,"connect mongodb  failed! connectinfo: %s...",p_config["mongohost"].c_str());
		pthread_mutex_unlock(&config_mutex);
		return false;
	}
	if(!pmongom->auth(p_config["mongodbname"].c_str(),\
		p_config["mongouser"].c_str(),p_config["mongopwd"].c_str()))
	{
		theLog.WriteLog(LOG_LEVEL_ERROR,"mongodb auth  failed! authinfo: mongodbname:%s，mongouser:%s,mongopwd:%s  ",
			p_config["mongodbname"].c_str(),p_config["mongouser"].c_str(),p_config["mongopwd"].c_str());
		pthread_mutex_unlock(&config_mutex);
		return false;
	}
	pmongom->settable(p_config["mongosourceat"].c_str());
	//mysql的初始化连接
	MySqlConn = mysql_init(NULL);
	if( NULL == MySqlConn )
	{
		theLog.WriteLog(LOG_LEVEL_ERROR,"mysql_init failed!");
		pthread_mutex_unlock(&config_mutex);
		return false;
	}
	if (!mysql_real_connect(MySqlConn, p_config["mysqlhost"].c_str(),
			p_config["mysqluser"].c_str(), p_config["mysqlpassword"].c_str(),
				p_config["mysqldbname"].c_str(), 0, NULL, 0)) 
	{
		theLog.WriteLog(LOG_LEVEL_ERROR,"mysql_real_connect auth  failed! authinfo: mysqlhost:%s，mysqluser:%s,mysqlpassword:%s，mysqldbname:%s  ",
			p_config["mysqlhost"].c_str(),p_config["mysqluser"].c_str(),p_config["mysqlpassword"].c_str(),p_config["mysqldbname"].c_str());
		pthread_mutex_unlock(&config_mutex);
		return false;
	}
	if(mysql_set_character_set(MySqlConn,"utf8"))
	{
		theLog.WriteLog(LOG_LEVEL_ERROR,mysql_error(MySqlConn));
		pthread_mutex_unlock(&config_mutex);
		return false;
	}
	pthread_mutex_unlock(&config_mutex);
	return true;
}
void articleextract::extract(std::string arctileid, std::string newarticleurl, std::string sourcepagename)
{
	string page;
	string with_imageurl;//配图URL
	list<string> ImgStrList;
	page.clear();
	int method = 0;
	/*if(0 == sourcepagename.compare("zatu")) 
	{
		//string strtmp = "";
		//iconv_string("utf-8","u", newarticleurl.c_str(), newarticleurl.length(),strtmp,1);
		//newarticleurl = strtmp;
		newarticleurl = urldecode((char *)newarticleurl.c_str());
	}*/
    printf("url=%s\n",newarticleurl.c_str());
	if(!getPage(newarticleurl.c_str(), method,page))
	{
		theLog.WriteLog(LOG_LEVEL_ERROR,"getpage failed url=%s ...",newarticleurl.c_str());
		updatepostgrlurlstatus(arctileid);
		return ;
	}
	
	string content;
	string titlestr;
	string Introduction;
	string contentimg;
	string publishtime;
	page = pagetagclean(page);
	if(page.length() < 2048)
	{
		theLog.WriteLog(LOG_LEVEL_ERROR,"the page source length too short ! url=%s",newarticleurl.c_str());
		updatepostgrlurlstatus(arctileid);
		return;
	}
	HtmlExtract sorceExtract(page,p_spiderruler[sourcepagename.c_str()]);
	sorceExtract.Extract();
	titlestr	 = sorceExtract.GetTitle();
	printf("lasttitle=%s\n",titlestr.c_str());
	boost::regex title_reg("((?i)(&nbsp;))");
	titlestr		= boost::regex_replace(titlestr,title_reg,"");
	
	Introduction	= sorceExtract.Introduction;
	content			= sorceExtract.ArticleContent;
	if( 0 == sourcepagename.compare("paidai") )  //派代网的时间提取不出来
	{
		publishtime.clear();
	}
	else
	{
		publishtime		= sorceExtract.GetDateTime();
		strltrim(publishtime);
		publishtime		= publish_time_deal(publishtime);
	}
	printf("lastpublishtime = %s\n",publishtime.c_str());
	if(0 == sourcepagename.compare("geekpark"))  //极客公园特殊处理
	{
		boost::regex reg14("((?i)(<div\\s{1,4}(id=\"tags\").*?</div>))");
		content = boost::regex_replace(content,reg14,"");
	}
	if(0 == sourcepagename.compare("cuntuba"))  //寸土吧特殊处理
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
		boost::regex reg14("((?i)(<div\\s{1,4}(?!id=\"endtext\").*?</div>))");
		content = boost::regex_replace(content,reg14,"");
	}
	if(0 == sourcepagename.compare("pingwest"))  //pingwest尾部特殊处理
	{
		boost::regex reg15("((?i)(<div.*?>[^<]+</div>))");
		content = boost::regex_replace(content,reg15,"");
	}
	if(0 == sourcepagename.compare("zatu"))  //杂图天下尾部特殊处理
	{
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
	if(0 == sourcepagename.compare("guaixun")) //怪讯网的上一页下一页图片标签要去掉
	{
		boost::regex reg13("((?i)(<div style=\"position:absolute.*?</div>))");
		content = boost::regex_replace(content,reg13,"");
	}
	content = contenttagclean(content);
	strltrim(content);
	//utfstrrtrim(content);
	
	try
	{
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
		contentimg = content;
		boost::regex reg("((?i)<((?!img)[^>]*>))");
		contentimg = boost::regex_replace(contentimg,reg,"");
		//printf("contentimg=%s\n",contentimg.c_str());
		reg8.assign("(?i)( src\\s{0,2}=\\s{0,2}\"([^\"]*)\")");
		boost::smatch m;
		std::string::const_iterator start	= contentimg.begin();
		std::string::const_iterator end		= contentimg.end();
		while(boost::regex_search(start,end,m,reg8))
		{
			//printf("bbbbb\n");
			if (m[0].matched)
			{
				//printf("aaaaa\n");
				string tempurl(m[0].first,m[0].second);
				string regurl;
				tempurl = tempurl.substr(tempurl.find_first_of('\"',0)+1,tempurl.find_last_of('\"')-tempurl.find_first_of('\"',0)-1);
				regurl =  _expandlinks(newarticleurl,tempurl);
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
	catch(const boost::bad_expression& e)
	{
		theLog.WriteLog(LOG_LEVEL_ERROR,"cann't create regex with %s!",newarticleurl.c_str());
	}
	list<string>::iterator it;
	MD5_CTX ctx;
    unsigned char md[16];
	string ispictureurl;
    char buf[100]={'\0'};
    char tmp[3]={'\0'};
	string ImgDir;
    int i;  
	HttpWrap *phttpwrap = new HttpWrap();
	for( it = ImgStrList.begin(); it != ImgStrList.end(); it++)
	{
		memset(buf,0,100);
		memset(md,0,16);
		MD5_Init(&ctx);    
		MD5_Update(&ctx,(*it).c_str() ,(*it).length());   
		MD5_Final(md,&ctx); 
		for( i=0; i<16; i++ )
		{
			sprintf(tmp,"%02X",md[i]);
			if(i == 0)
			{
				strcat(buf,"/mnt/diaobao/diaobaoimg/");
			}
			strcat(buf,tmp);
			if(i == 1 || i == 0 )
			{
				strcat(buf,"/");
				if(-1 == access(buf,0))
				{
					if(-1 == mkdir(buf,0777))
					{
						printf("mkdir %s failed!\n ",buf);
						break;
					}
				}
			}
		}
		ImgDir = buf;
		strcat(buf,".jpg");
		
		if(it == ImgStrList.begin())
		{
			with_imageurl = buf+13;
		}
		//printf("imgsourceurl=%s\n",(*it).c_str());
		//printf("pathdir=%s\n",buf);
		//printf("ImgDir=%s\n",ImgDir.c_str());	
		phttpwrap->GetImg((*it).c_str(),buf,ImgDir.c_str());
		string upload_token = std::string(uptoken(&client,"diaobao"));
		if(NULL == upload(&client,(char *)upload_token.c_str(),buf+13,buf))
		{
			//return;
		}
		boost::regex reg((*it).c_str());
		content = boost::regex_replace(content,reg,buf+13);
	}
	delete phttpwrap;
	if(0 != content.length())
	{

		if(0 == Introduction.length())
		{
			boost::smatch m1;
			boost::regex reg("(?i)(<p>.*?</p>)");
			std::string::const_iterator start = content.begin();
			std::string::const_iterator end = content.end();
			while(boost::regex_search(start,end,m1,reg))
			{
				if (m1[0].matched)
				{
					printf("Introduction m1[0] =%s\n",Introduction.c_str());
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
		if(false == insertmysqldbres(titlestr,newarticleurl,with_imageurl,Introduction,p_spiderruler[sourcepagename.c_str()].id,publishtime))
		{
			return;
		}
		insertmongodbres(article_id,content);
	}
	updatepostgrlurlstatus(arctileid);
	phttpwrap = NULL;
	
	return;
}
bool articleextract::updatepostgrlurlstatus(string arctileid)
{
	string updatestr = "update newarticleurltable set isget = true where(articleid = "+  arctileid + ");";
	return m_dbmanager->CommandDB(updatestr.c_str());
}
bool articleextract::insertmongodbres(string articleid, std::string articlecontent)
{
	pmongom->insertarticle(articleid.c_str(),articlecontent);
}
bool articleextract::insertmysqldbres(std::string origin_title, std::string sourceurl, std::string with_image, std::string discription,string id,std::string publishtime)
{

	if(0 != mysql_ping(MySqlConn))
	{
		pthread_mutex_lock(&config_mutex);
		mysql_close(MySqlConn);
		if (!mysql_real_connect(MySqlConn, p_config["mysqlhost"].c_str(),
			p_config["mysqluser"].c_str(), p_config["mysqlpassword"].c_str(),
				p_config["mysqldbname"].c_str(), 0, NULL, 0)) 
		{
			theLog.WriteLog(LOG_LEVEL_ERROR,"mysql_ping mysql_real_connect auth  failed! authinfo: mysqlhost:%s，mysqluser:%s,mysqlpassword:%s，mysqldbname:%s  ",
					p_config["mysqlhost"].c_str(),p_config["mysqluser"].c_str(),p_config["mysqlpassword"].c_str(),p_config["mysqldbname"].c_str());
			pthread_mutex_unlock(&config_mutex);
			return false;
		}
		if(mysql_set_character_set(MySqlConn,"utf8"))
		{
			theLog.WriteLog(LOG_LEVEL_ERROR,mysql_error(MySqlConn));
			theLog.WriteLog(LOG_LEVEL_ERROR,"mysql_ping mysql_set_character_set urf8 error!");
			pthread_mutex_unlock(&config_mutex);
			return false;
		}
		pthread_mutex_unlock(&config_mutex);
	}
	MYSQL_RES *MySqlRes;
	MYSQL_ROW MySqlRow;
	string sqlstr;
	char *discriptiontemp = new char[discription.length()*2];
	if(NULL == discriptiontemp)
	{
		theLog.WriteLog(LOG_LEVEL_ERROR,"discriptiontemp mysql_escape_string failed!");
		return false;
	}
	mysql_escape_string(discriptiontemp,discription.c_str(),discription.length());
	
	char *urltemp = new char[sourceurl.length()*2];
	if(NULL == urltemp)
	{
		theLog.WriteLog(LOG_LEVEL_ERROR,"sourceurl new mysql_escape_string failed!");
		return false;
	}
	mysql_escape_string(urltemp,sourceurl.c_str(),sourceurl.length());

	char *tiltletemp = new char[origin_title.length()*2];
	if(NULL == tiltletemp)
	{
		theLog.WriteLog(LOG_LEVEL_ERROR,"origin_title new mysql_escape_string failed!");
		return false;
	}
	mysql_escape_string(tiltletemp,origin_title.c_str(),origin_title.length());
	sqlstr = "insert into article_info (wemediaid,origin_title,url,with_image,discription,status,publish_time,read_times,comment_times,collect_times,share_times,add_time) values(" +
		id + ",'" + tiltletemp + "','" + urltemp + "','" + with_image + "','" + discriptiontemp + "',1,UNIX_TIMESTAMP(),0,0,0,0,UNIX_TIMESTAMP(";
	if( 0 == publishtime.length())
	{
		sqlstr += "));";
	}
	else
	{
		sqlstr +=	"'" + publishtime + "'));";
	}
	printf("sqlstr=%s\n",sqlstr.c_str());
	return false;
	
	if(mysql_query(MySqlConn,sqlstr.c_str()))
	{
		theLog.WriteLog(LOG_LEVEL_ERROR,mysql_error(MySqlConn));
		theLog.WriteLog(LOG_LEVEL_ERROR,"insert article_info to mysql error ,sqlstr: %s  !",sqlstr.c_str());
		if(discriptiontemp)
		{
			delete discriptiontemp;
			discriptiontemp = NULL;
		}
		if(urltemp)
		{
			delete urltemp;
			urltemp = NULL;  
		}
		if(tiltletemp)
		{
			delete tiltletemp;
			tiltletemp = NULL;  
		}
		return false;
	}
	//string selectstr = "select last_insert_id()";
	string selectstr = "select id from article_info where url = '" + sourceurl + "';";
	if(mysql_query(MySqlConn,selectstr.c_str()))
	{
		theLog.WriteLog(LOG_LEVEL_ERROR,mysql_error(MySqlConn));
		if(discriptiontemp)
		{
			delete discriptiontemp;
			discriptiontemp = NULL;
		}
		if(urltemp)
		{
			delete urltemp;
			urltemp = NULL;  
		}
		if(tiltletemp)
		{
			delete tiltletemp;
			tiltletemp = NULL;  
		}
		return false;
	}
	MySqlRes = mysql_store_result(MySqlConn);    
	MySqlRow = mysql_fetch_row(MySqlRes); 
	article_id = MySqlRow[0];
	mysql_free_result(MySqlRes);   
	if(discriptiontemp)
	{
		delete discriptiontemp;
		discriptiontemp = NULL;
	}
	if(urltemp)
	{
		delete urltemp;
		urltemp = NULL;  
	}
	if(tiltletemp)
	{
		delete tiltletemp;
		tiltletemp = NULL;  
	}
	return true;
	//string sqlstring ="insert into article_info (origin_title,url,add_time,with_image,discription,status) values('%s','%s',%d,'%s','%s',%d)";

}
