#include "PublishTimeDeal.h"
PublishTimeDeal::PublishTimeDeal()
{

}
PublishTimeDeal::~PublishTimeDeal()
{

}

void PublishTimeDeal::Process(HtmlFragment HF,string webname)
{
	//printf("aaaaa\n");
	//涓嶅湪澶勭悊鏍囩鍐咃紝杩斿洖
	
	if(HF.get_tagName() != "div" && HF.get_tagName() != "td" && HF.get_tagName() != "span")
	{
		return;
	}
	//娌℃湁鍖呭惈鏃ユ湡瀛楃涓?
	if (!HasDateTime(HF.get_htmlSource()))
    {
        return;
    }
	//printf("bbbbb");
	 //瀹氫箟鍙戣〃鏃堕棿瀵硅薄
	PublishTimePart publishTime(HF.get_sequence(), HF);
	publishTime._power += CharsCountPower(HF);
	publishTime._power += IncludeWordsPower(HF.get_htmlSource());
	publishTime._power += NodeClassPower(HF.get_hNode(),webname);
	//printf("ccccc");
	publishTime._power += DateStylePower(HF.get_htmlSource());
	publisttimelist.push_back(publishTime);

}
double PublishTimeDeal::NodeClassPower(htmlcxx::HTML::Node hnode,string webname)
{
	hnode.parseAttributes();
	string nodeClassstring = hnode.attribute("class").second;
	string nodeIDstring = hnode.attribute("id").second;
	transform(nodeClassstring.begin(), nodeClassstring.end(), nodeClassstring.begin(), ::tolower); 
	transform(nodeIDstring.begin(), nodeIDstring.end(), nodeIDstring.begin(), ::tolower);
	if(0 == webname.compare("forbeschina"))
	{
		if((-1 != nodeClassstring.find("message",0))
		||(-1 != nodeIDstring.find("message",0)))
		{
		//	printf("NodeClassPower return 1\n");
			return 3;
		}	
	}
	if((-1 != nodeClassstring.find("publishtime",0))
		||(-1 != nodeIDstring.find("publishtime",0)))
	{
	//	printf("NodeClassPower return 1\n");
		return 3;
	}
	else
	{
		return 0;
	}
}

bool PublishTimeDeal::HasDateTime(string sourceHtml)
{
	if (sourceHtml.empty())
    {
	//	printf("sourcehtml is null!\n");
       return false;
    }
	//printf("ppppp\n");
	//printf("sourcehtml=%s\n",sourceHtml.c_str());
	boost::regex reg("(?i)(\\d{4}([^\\da-zA-Z]{1,4})\\d{1,2}([^\\da-zA-Z]{1,4})\\d{1,2})");
	//boost::regex reg("<p>");
	boost::smatch m;
	if(boost::regex_search(sourceHtml,m,reg))
	{
		return true;
	}   
	//printf("not Has Date Time\n");
	return false;
}
//日期的格式加权
double PublishTimeDeal::DateStylePower(string sourceHtml)
{
//	printf("1111\n");
	if (sourceHtml.empty())
    {
	//	printf("33333\n");
       return 0;
    }
	boost::regex reg("(?i)(?<dateTime>\\d{4}([^\\da-zA-Z]{1,4})\\d{1,2}([^\\da-zA-Z]{1,4})\\d{1,2}([\\s|\\S]{0,4})\\d{1,2}:\\d{1,2})");
	boost::smatch m;
//	printf("22222\n");
	if(boost::regex_search(sourceHtml,m,reg))
	{
		return 2;
	}  
	return 0;
}
double PublishTimeDeal::CharsCountPower(HtmlFragment HF)
{
	if (HF.get_charsCount() < 50)
    {
        return 1;
    }
    return 0;
}

double PublishTimeDeal::IncludeWordsPower(string sourceHtml)
{
	if (sourceHtml.empty())
    {
       return 0;
    }
	
	//boost::regex reg("(?i)(鏉ユ簮|鍙戣〃|鍙戝竷)");
	boost::regex reg("(?i)(来源|发表|发布)");
	/*if(sourceHtml.find("鏉ユ簮"))
	{
		printf("find 鏉ユ簮\n");
		return 2;
	}*/
	boost::smatch m;
	if(boost::regex_search(sourceHtml,m,reg))
	{
		//printf("sourceHtml=%s\n",sourceHtml.c_str());
	//	printf("IncludeWordsPower=%s\n",m[0].str().c_str());
	//	printf("return 2\n");
		return 2;
	}   

	return 0;
}
PublishTimePart PublishTimeDeal::GetModel()
{
	LISTPUBLISHTIME temp;
	LISTPUBLISHTIME::iterator it;
	PublishTimePart publishtimetemp;
	if (publisttimelist.empty())
	{
		return publishtimetemp;
    }
	it = publisttimelist.begin();
	publishtimetemp = *it;
	it++;
	for(; it != publisttimelist.end(); it++ )
	{
	//	printf("publishtime_sequence=%d\n",it->_sequence);
		//printf("publishtime_power=%f\n",it->_power);
	//	printf("sourcehtml=%s\n",it->_hFragment.get_htmlSource().c_str());
		if(it->_power >= publishtimetemp._power)
		{
			publishtimetemp = *it;
		}
	}
	for(it = publisttimelist.begin(); it != publisttimelist.end(); it++ )
	{
		if(it->_power >= publishtimetemp._power)
		{
			temp.push_back(*it);
		}
	}
	for(it = temp.begin(); it != temp.end(); it++)
	{
		if(it->_sequence < publishtimetemp._sequence)
		{
			publishtimetemp = *it;
		}
	}
	//printf("publishtimetemp_sequence=%d\n",publishtimetemp._sequence);
	//printf("publishtimetemp_power=%f\n",publishtimetemp._power);
	//printf("publishtimetemp_sourcehtml=%s\n",publishtimetemp._hFragment.get_htmlSource().c_str());
	return publishtimetemp;
}

