#include"HtmlExtract.h"
//#include "HtmlFragment.h"
HtmlExtract::HtmlExtract()
{
}
HtmlExtract::HtmlExtract(string source,struct SpiderRuler rul)
{
	HTML::ParserDom parser;
	dom					 = parser.parseTree(source);
	htmlsource			 = source;
	ruler				 = rul;
	flag				 = 0;
	//ruler.comment		 = rul.comment;
	//ruler.hasruler		 = rul.hasruler;
	//ruler.id			 = rul.id;
	//ruler.summarytag	 = rul.summarytag;
	//ruler.textcontenttag = rul.textcontenttag;
	//ruler.webname		 = rul.webname;
	//SourcePageName	= sourcepagename;
	Sequence		= 0;
	HistoryUrl.clear();
	Introduction.clear();
//	Extract();
}
HtmlExtract::~HtmlExtract()
{
}
void HtmlExtract::Extract()
{
	HTML::ParserDom parser;
	tree<HTML::Node>::iterator it = dom.begin();
	tree<HTML::Node>::iterator end = dom.end();
	for(; it != end; ++it)
	{
		it->parseAttributes();
        string tagname =  it->tagName();
		transform(tagname.begin(), tagname.end(), tagname.begin(), ::tolower); 
		if(it->isTag() && tagname == "title")
		{
			int flag = 0;
			string strhtml = htmlsource.substr(it->offset(),it->length());
			boost::regex regtitle4("<title>");
			strhtml = boost::regex_replace(strhtml,regtitle4,"");
			boost::regex regtitle5("</title>");
			strhtml = boost::regex_replace(strhtml,regtitle5,"");
			string temptitle = strhtml;
			int npos;

			

			if(-1 !=(npos = strhtml.find("|")))
			{
				strhtml = strhtml.substr(0,npos);
				flag = 1;
			}
			if(-1 !=(npos = strhtml.find("-")))
			{
				strhtml = strhtml.substr(0,npos);
				flag = 1;
			}
			if(-1 !=(npos = strhtml.find("_")))
			{
				strhtml = strhtml.substr(0,npos);
				flag = 1;
			}
			if(1 == flag)
			{
				titledeal.left_sourceTitle = strhtml;
			}
			strhtml = temptitle;
			if(-1 !=(npos = strhtml.find_last_of("|")))
			{
				strhtml = strhtml.substr(npos+1);
				flag = 2;
			}
			if(-1 !=(npos = strhtml.find_last_of("-")))
			{
				strhtml = strhtml.substr(npos+1);
				flag = 2;
			}
			if(-1 !=(npos = strhtml.find_last_of("_")))
			{
				strhtml = strhtml.substr(npos+1);
				flag = 2;
			}
			if(2 == flag)
			{
				titledeal.right_sourceTitle = strhtml;
			}
			if(flag == 0 )
			{
				titledeal._sourceTitle = temptitle;
			}
			if(0 == ruler.webname.compare("juyouqu"))
			{
				if(-1 !=(npos = strhtml.find("::")))
				{
					titledeal._sourceTitle = strhtml.substr(0,npos);
				}
			}
			printf("webname=%s\n",ruler.webname.c_str());
			if( (0 == ruler.webname.compare("wshang") 
				|| 0 == ruler.webname.compare("meitui") 
					|| 0 == ruler.webname.compare("eeo") 
						|| 0 == ruler.webname.compare("gemag")
							|| 0 == ruler.webname.compare("techqq")
								|| 0 == ruler.webname.compare("macx")
									|| 0 == ruler.webname.compare("cocoachina")
										|| 0 == ruler.webname.compare("reynold"))	 //标题都在title标签的左边部分
											&& 0!= titledeal.left_sourceTitle.length())//cocoachina   中标题带有多个-字符
			{
				titledeal._sourceTitle = titledeal.left_sourceTitle;
			}
		//	if( 0 == ruler.webname.compare("cocoachina"))//cocoachina   中标题带有多个-字符
			//{
		//		titledeal._sourceTitle = temptitle;
		//	}
			printf("right_sourceTitle=%s\n",titledeal.right_sourceTitle.c_str());
			printf("left_sourceTitle=%s\n",titledeal.left_sourceTitle.c_str());
			printf("_sourceTitle=%s\n",titledeal._sourceTitle.c_str());
		}
		if(it->isTag() && tagname == "meta")
		{
		//	printf("meta=%s\n",htmlsource.substr(it->offset(),it->length()).c_str());
		//	printf("hasruler=%s\n",ruler.hasruler.c_str());
			if( ruler.hasruler.compare("yes") == 0 && 0 == ruler.summaryisinbody.compare("no"))
			{
					
				//it->parseAttributes();
				string nodeClassstring = it->attribute(ruler.summarytagname.c_str()).second; 
			//	printf("summarytagname=%s\n",ruler.summarytagname.c_str());
				transform(nodeClassstring.begin(), nodeClassstring.end(), nodeClassstring.begin(), ::tolower);
				if( 0 == nodeClassstring.compare(ruler.summarytag) && 0 != ruler.summarytag.length())
				{
						Introduction = htmlsource.substr(it->offset(),it->length());
				//		printf("Introductionfirst=%s\n",Introduction.c_str());
				}
			}
		}
		//福布斯中文网站的特殊处理 因为找不到body标签
		if(it->isTag() && 0 == ruler.webname.compare("forbeschina")  && tagname == "div")
		{
			//it->parseAttributes();
			string nodeClassstring = it->attribute("id").second; 
			transform(nodeClassstring.begin(), nodeClassstring.end(), nodeClassstring.begin(), ::tolower);
			if(0 == nodeClassstring.compare("main_content"))
			{
				NodeLoopProcess(it);
			}
		}
		if(it->isTag() && (tagname == "body" || tagname == "tbody" ||tagname == "ibody"))
		{
			NodeLoopProcess(it);
		}
	}
	//所有节点处理结束后
    ProcessEnd();
	//GetTitle();
	
}
void HtmlExtract::ProcessEnd()
{
	ReserveSubject		= subjectDeal.GetModel();
	Reservetitle		= titledeal.GetModel();
	ReservePublishTime	= publishTimeDeal.GetModel();
	if(0 != ruler.hasruler.compare("yes") || 0 == ArticleContent.length())
	{
		ArticleContent		= ReserveSubject.getHtmlFragment().get_htmlSource();
	}
	//publishTimeDeal.GetDateTime(ReservePublishTime._hFragment.get_htmlSource());
	//ReserveSubjectPager	= subjectPagerDeal.GetModel();
}
void HtmlExtract::ExtractContent()
{
}
void HtmlExtract::ExtractTiltle()
{
}

void HtmlExtract::NodeLoopProcess(tree<HTML::Node>::iterator it)
{

//	printf("Sequence = %d\n",Sequence);
	string tagnamestr;
	for (int i =0 ; i < (int)dom.number_of_children(it) ; ++i)
	{
		tree<HTML::Node>::iterator itdlchild = dom.child(it,i);
		tagnamestr = itdlchild->tagName();
		transform(tagnamestr.begin(), tagnamestr.end(), tagnamestr.begin(), ::tolower); 
		if(tagnamestr == "div"  
				|| tagnamestr == "td" 
					|| tagnamestr == "span"
						|| tagnamestr == "p" 
							|| tagnamestr == "h1"
								|| tagnamestr == "h2"
									|| tagnamestr == "h3" 
										|| tagnamestr == "h4" 
											|| tagnamestr == "h5" 
											    || tagnamestr == "h6" 
													|| tagnamestr == "section" )
		{
			string htmltemp = htmlsource.substr(itdlchild->offset(),itdlchild->length());
		//	printf("htmltemp=%s\n",htmltemp.c_str());
			HtmlFragment HF(Sequence++,*itdlchild,htmltemp);
			if( ruler.hasruler.compare("yes") == 0 )
			{
				itdlchild->parseAttributes();
				string contentnodeClassstring = itdlchild->attribute(ruler.textcontenttagname.c_str()).second; 
				//printf("textcontenttagname=%s\n",ruler.textcontenttagname.c_str());
				//printf("contentnodeClassstring=%s\n",contentnodeClassstring.c_str());
				//printf("textcontenttag=%s\n",ruler.textcontenttag.c_str());
				if( contentnodeClassstring.length() != 0 )
				{
					transform(contentnodeClassstring.begin(), contentnodeClassstring.end(), contentnodeClassstring.begin(), ::tolower);
					if( 0 == contentnodeClassstring.compare(ruler.textcontenttag) )
					{
					/*	if(0 == ArticleContent.length() && ( 0 == ruler.webname.compare("jiandan") || ) )//由于煎蛋网站有两个标签属性是一样的
						{
							ArticleContent = htmltemp;
						//	printf("ArticleContent=%s\n",ArticleContent.c_str());
						}
						else
						{
							ArticleContent = htmltemp;
						}*/
						flag++ ;
						if( 0 == ruler.webname.compare("jandan") || 0 == ruler.webname.compare("guao") )//由于煎蛋网站有两个标签属性是一样的
						{
							//printf("flag = %d\n",flag);
							if(1 == flag)
							{
								ArticleContent = htmltemp;
							}
						}
						else
						{
							ArticleContent = htmltemp;
						}
						
					}
				}
				if( 0 == ruler.summaryisinbody.compare("yes") )
				{
					string summarynodeClassstring = itdlchild->attribute(ruler.summarytagname.c_str()).second; 
					if( 0 != summarynodeClassstring.length() )
					{
						transform(summarynodeClassstring.begin(), summarynodeClassstring.end(), summarynodeClassstring.begin(), ::tolower);
						if( 0 == summarynodeClassstring.compare(ruler.summarytag) && 0 != ruler.summarytag.length())
						{
							Introduction = htmltemp;
						//	printf("Introduction=%s\n",Introduction.c_str());
						}
					}
				}
			}
			else
			{
					
				//publishTimeDeal.Process(HF);
				//subjectPagerDeal.Process(HF);
			}
			if(0 != ruler.webname.compare("gemag"))//因为环球企业家   提取不了时间
			{
				publishTimeDeal.Process(HF,ruler.webname);
			}
			subjectDeal.Process(HF);
			titledeal.Process(HF);
		}
		NodeLoopProcess(itdlchild);
	}
}
string HtmlExtract::GetTitle()
{
	 string returnTitle;
	 if(!titledeal._sourceTitle.empty())
	 {
		returnTitle = titledeal._sourceTitle;
		return returnTitle;
	 }
	 if ( 0 != Reservetitle._charsCount)
	 {
		returnTitle = Reservetitle._hFragment.get_htmlSource();
		boost::regex reg("<[^>]+>");
		returnTitle = boost::regex_replace(returnTitle,reg,"");
		boost::regex reg1("[\\n]");
		returnTitle = boost::regex_replace(returnTitle,reg1,"");

		printf("returnTitle=%s\n",returnTitle.c_str());
		if(-1 != returnTitle.find(titledeal.left_sourceTitle) && !titledeal.left_sourceTitle.empty())
		{
			//printf("find the sourcetitle!\n");
			printf("left_sourceTitle return\n");
			return titledeal.left_sourceTitle;
		}
		if(-1 != returnTitle.find(titledeal.right_sourceTitle) && !titledeal.right_sourceTitle.empty())
		{
			//printf("find the sourcetitle!\n");
			printf("right_sourceTitle return\n");
			return titledeal.right_sourceTitle;
		}
		if(!titledeal.right_sourceTitle.empty() || !titledeal.left_sourceTitle.empty())
		{
			if(titledeal.right_sourceTitle.length() > titledeal.left_sourceTitle.length())
			{
				return titledeal.right_sourceTitle;
			}
			return  titledeal.left_sourceTitle;
		}
/*
		if(-1 != returnTitle.find(titledeal._sourceTitle) && !titledeal._sourceTitle.empty())
		{
			//printf("find the sourcetitle!\n");
			returnTitle = titledeal._sourceTitle;
		}*/
		 //returnTitle = ReserveTitle.hFragment.HNode.InnerText.Replace("\r", "").Replace("\n", "").Replace("\t", "").Replace("&nbsp;", " ").TrimStart().TrimEnd();
		 //如果包含源Title，则直接将源Title当做标题
		// if (!string.IsNullOrEmpty(titleDeal.SourceTitle) && returnTitle.Contains(titleDeal.SourceTitle))
		// {
		//	 returnTitle = titleDeal.SourceTitle;
		// }
	 }
	 else
	 {
			if(!titledeal.right_sourceTitle.empty() || !titledeal.left_sourceTitle.empty())
			{
				if(titledeal.right_sourceTitle.length() > titledeal.left_sourceTitle.length())
				{
					return titledeal.right_sourceTitle;
				}
				return  titledeal.left_sourceTitle;
			}
	 }
	  return returnTitle;
}

string HtmlExtract::GetDateTime()
{
	string returntime;
	returntime.clear();
	//if (ReservePublishTime._power == (double) 1.0)
   // {
     //  return NULL;
 //   }
	string sourceHtml = ReservePublishTime._hFragment.get_htmlSource();
	if(sourceHtml.empty())
	{
		return returntime;
	}
	boost::regex reg("(?i)(&nbsp;)");
	sourceHtml = boost::regex_replace(sourceHtml,reg," ");
	boost::regex reg1("(?i)(?<dateTime>\\d{4}([^\\da-zA-Z]{1,4})\\d{1,2}([^\\da-zA-Z]{1,4})\\d{1,2}([\\s|\\S]{0,4})\\d{1,2}:\\d{1,2})");
	//boost::regex reg("<p>");
//	boost::regex reg1("(?i)(?<dateTime>\\d{4}([^\\da-zA-Z]{1,4})\\d{1,2}([^\\da-zA-Z]{1,4})\\d{1,2}([\\s|\\S]))");
	boost::regex reg2("(?i)(?<dateTime>\\d{4}([^\\da-zA-Z]{1,4})\\d{1,2}([^\\da-zA-Z]{1,4})\\d{1,2})");
	boost::smatch m;
	boost::smatch m1;
	if(boost::regex_search(sourceHtml,m,reg1))
	{
		//printf("m[0]=%s\n",m[0].str().c_str());
		return m[0].str();
	}  
	if(boost::regex_search(sourceHtml,m1,reg2))
	{
		//printf("m1[0]=%s\n",m1[0].str().c_str());
		return m1[0].str();
	}  
	return returntime;
}
void HtmlExtract::SubjectPagerExtract(string url)
{
	if(ReserveSubjectPager._power == 0)
	{
		return;
	}
	HistoryUrl.push_back(url);
	string PagerString = ReserveSubjectPager._hFragment.get_htmlSource();
	boost::regex reg1("<[^a^>]+>");
	PagerString = boost::regex_replace(PagerString,reg1,"");
	printf("PagerString=%s\n",PagerString.c_str());
	
}