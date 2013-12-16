#include "titleDeal.h"
titleDeal::titleDeal()
{
}
titleDeal::~titleDeal()
{
}
void titleDeal::Process(HtmlFragment HF)
{
	if(HF.get_tagName() != "div" 
		&& HF.get_tagName() != "td"
		 && HF.get_tagName() != "h1" 
		  && HF.get_tagName() != "h2"
		   && HF.get_tagName() != "h3")
	{
		return ;
	}
	 //字符数等于0或者大于100的，返回
	if (HF.get_charsCount() == 0 || HF.get_charsCount() > 100)
    {
		return;
    }
	TitlePart title(HF.get_sequence(),HF);
	//节点是h标签的，加权
    if (HF.get_tagName() == "h1" || HF.get_tagName() == "h2" || HF.get_tagName() == "h3")
    {
		title._power += 2;
    }
	title._power += TagPower(HF.get_htmlSource());
	title._power += NodeClassPower(HF.get_hNode());
	title._power += CharsCountPower(HF.get_charsCount());
	title._power += SourceTitlePower(_sourceTitle,HF.get_htmlSource());
	if(title._power > 2 )
	{
		titlepartlist.push_back(title);
	}

}
double titleDeal::TagPower(std::string sourceHtml)
{
	if(sourceHtml.empty())
	{
		return 0;
	}
	boost::regex reg("(?i)(<h1.*?>|<h2.*?>|<h3.*?>|<h4.*?>)");
	boost::regex reg1("(?i)(<b>|<strong>)");
	boost::smatch m;
	if(boost::regex_search(sourceHtml,m,reg))
	{
	//	printf("TagPower return 2\n");
		return 2;
	}   
	if(boost::regex_search(sourceHtml,m,reg1))
	{
		return 1;
	}  
	return 0;
}
double titleDeal::NodeClassPower(htmlcxx::HTML::Node hnode)
{
	hnode.parseAttributes();
	string nodeClassstring = hnode.attribute("class").second;
	string nodeIDstring = hnode.attribute("id").second;
	transform(nodeClassstring.begin(), nodeClassstring.end(), nodeClassstring.begin(), ::tolower); 
	transform(nodeIDstring.begin(), nodeIDstring.end(), nodeIDstring.begin(), ::tolower);
	if((-1 != nodeClassstring.find("title",0)) || (-1 != nodeClassstring.find("head",0))
		||(-1 != nodeIDstring.find("title",0)))
	{
	//	printf("NodeClassPower return 1\n");
		return 1;
	}
	else
	{
		return 0;
	}
}
double titleDeal::CharsCountPower(int charscount)
{
	 if (charscount >= 13 && charscount <= 100)
     {
         return 1;
     }
	 return 0;
}
double titleDeal::SourceTitlePower(std::string sourcetile, std::string sourceHtml)
{
	if(sourcetile.empty() || sourceHtml.empty())
	{
		return 0;
	}
	//printf("sourceHtml=%s\n",sourceHtml.c_str());
	boost::regex reg4("[^\u4e00-\u9fa5]");
	boost::regex reg5("　");
	boost::regex reg6("[ \\r\\n\\t]");
	sourcetile = boost::regex_replace(sourcetile,reg4,"");
	sourcetile = boost::regex_replace(sourcetile,reg5,"");	
	sourcetile = boost::regex_replace(sourcetile,reg6,"");
	sourceHtml = boost::regex_replace(sourceHtml,reg4,"");
	sourceHtml = boost::regex_replace(sourceHtml,reg5,"");	
	sourceHtml = boost::regex_replace(sourceHtml,reg6,"");
	
	if(sourcetile == sourceHtml)
	{
	//	printf("SourceTitlePower return 3\n");
		return 3;
	}
	if(-1 != sourcetile.find(sourceHtml))
	{
	//	printf("SourceTitlePower return 2\n");
		return 2;
	}
	if(-1 != sourceHtml.find(sourcetile))
	{
	//	printf("SourceTitlePower return 1\n");
		return 1;
	}
	return 0;
}

TitlePart titleDeal::GetModel()
{
	LISTTILEPART temp;
	LISTTILEPART::iterator it;
	TitlePart titletemp;
	for(it  = titlepartlist.begin(); it !=  titlepartlist.end(); it ++ )
	{
		//printf("titlepower = %f\n",it->_power);
	//	printf("titlecharscount = %d\n",it->_charsCount);
	//	printf("titlesource=%s\n",it->_hFragment.get_htmlSource().c_str());
		if((it->_power > 3 )&&(it->_power > titletemp._power))
		{
			titletemp = *it;
		}	
	}
	for(it = titlepartlist.begin(); it != titlepartlist.end(); it++)
	{
		if(it->_charsCount < titletemp._charsCount && it->_power == titletemp._power  )
		{
			titletemp = *it;
		}
	}
	//printf("titlepower = %f\n",titletemp._power);
	//printf("titlecharscount = %d\n",titletemp._charsCount);
	//printf("titlesource=%s\n",titletemp._hFragment.get_htmlSource().c_str());
	return titletemp;
}
