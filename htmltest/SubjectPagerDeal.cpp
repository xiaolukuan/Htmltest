#include "SubjectPagerDeal.h"
SubjectPagerDeal::SubjectPagerDeal()
{

}
SubjectPagerDeal::~SubjectPagerDeal()
{

}
void SubjectPagerDeal::Process(HtmlFragment HF)
{
	if(HF.get_tagName() != "div" 
		&& HF.get_tagName() != "td"
		 && HF.get_tagName() != "p" 
		  && HF.get_tagName() != "span")
	{
		return ;
	}
	if(HF.get_charsCount() > 100 ||
		HF.get_linkCount() > 30 ||
		HF.get_LinkCharsAvg() > 5)
	{
		return ;
	}
	boost::regex reg("(?i)(页|\\d+)");
	//boost::regex reg("<p>");
	boost::smatch m;
	if(!boost::regex_search(HF.get_htmlSource(),m,reg))
	{
		return ;
	}
	SubjectPagerPart pager(HF.get_sequence(),HF);
	pager._power += NodeClassPower(HF.get_hNode());
    pager._power += LinkDensityPower(HF);
	pager._power += LinkCharsAvgPower(HF);
	pager._power += LinkFeature(HF.get_htmlSource());
	pager._power += TagDensityPower(HF);
	pager._power += IncludeWordsPower(HF.get_htmlSource());
	pager._power += ExcludeWordsPower(HF.get_htmlSource());
	if(pager._power > 3)
	{
		SubjectPagers.push_back(pager);
	}
}
double SubjectPagerDeal::NodeClassPower(htmlcxx::HTML::Node hnode)
{
	double power = 0;
	hnode.parseAttributes();
	string nodeClassstring = hnode.attribute("id").second;
	transform(nodeClassstring.begin(), nodeClassstring.end(), nodeClassstring.begin(), ::tolower); 
	if(-1 != nodeClassstring.find("page",0))
	{
		power += 1;
	}
	nodeClassstring = hnode.attribute("class").second;
	transform(nodeClassstring.begin(), nodeClassstring.end(), nodeClassstring.begin(), ::tolower);
	if(-1 != nodeClassstring.find("page",0))
	{
		power += 1;
	}
	return power;
}
double SubjectPagerDeal::LinkDensityPower(HtmlFragment HF)
{
	if (HF.get_linkDensity() > 0.2)
	{
		return 1;
	}
	return 0;
}
double SubjectPagerDeal::LinkCharsAvgPower(HtmlFragment HF)
{
	if (HF.get_linkCount() > 3 && HF.get_LinkCharsAvg() > 3)
	{
		return -2;
	}
	return 0;
}
double SubjectPagerDeal::LinkFeature(string sourcehtml)
{
	double power = 0.0;
	boost::regex reg("(?i)(<a[^>]*?>(<.*>)?([[])?(?<name>[\\d]+?)([]])?(<.*>)?</a>)");
	boost::regex reg1("(?i)(<a[^>]*?>(<.*>)?(?<name>[上一页]+?)(<.*>)?</a>)");
	boost::regex reg2("(?i)(<a[^>]*?>(<.*>)?(?<name>[下一页|尾页]+?)(<.*>)?</a>)");
	boost::regex reg3("(?i)共(.*) \\d+(.*)[条|页]");
	//boost::regex reg("<p>");
	boost::smatch m;
	if(boost::regex_search(sourcehtml,m,reg))
	{
		power += 2 ;
	}
	if(boost::regex_search(sourcehtml,m,reg1))
	{
		power += 1 ;
	}
	if(boost::regex_search(sourcehtml,m,reg2))
	{
		power += 1 ;
	}
	if(boost::regex_search(sourcehtml,m,reg3))
	{
		power += 1 ;
	}
	return power;
}
double SubjectPagerDeal::TagDensityPower(HtmlFragment HF)
{
	if (HF.get_linkDensity() > 500 && HF.get_tagDensity() < 3000)
	{
		return 1;
	}
	return 0;
}
double SubjectPagerDeal::IncludeWordsPower(string sourceHtml)
{
	if (sourceHtml.empty())
	{
		return 0;
	}
    double power = 0;
    boost::regex reg("(?i)(Total|上一页|上页)");
	boost::regex reg1("(?i)(尾页|下一页|下页)");
	//boost::regex reg("<p>");
	boost::smatch m;
	if(boost::regex_search(sourceHtml,m,reg))
	{
		power += 1;
	}
	if(boost::regex_search(sourceHtml,m,reg1))
	{
		power += 1;
	}
	return power;
}
double SubjectPagerDeal::ExcludeWordsPower(string sourceHtml)
 {
    if (sourceHtml.empty())
	{
		return 0;
	}
    double power = 0;
	boost::regex reg("(?i)(首页|联系|版权|Copyright|关注)");
	boost::regex reg1("(?i)(ICP备\\d+号)");
	boost::smatch m;
	if(boost::regex_search(sourceHtml,m,reg))
	{
		power += -5;
	}
	if(boost::regex_search(sourceHtml,m,reg1))
	{
		power += -5;
	}
	return power;
}
SubjectPagerPart SubjectPagerDeal::GetModel()
{
	SubjectPagerPart temp;
	if(SubjectPagers.empty())
	{
		return temp;
	}
	LISTPAGER::iterator it;
	for(it = SubjectPagers.begin();it != SubjectPagers.end(); it++)
	{
		printf("SubjectPagerspower = %f\n",it->_power);
		printf("SubjectPagerslinkCharsCount = %d\n",it->_hFragment.get_linkCharsCount());
		printf("SubjectPagerssource=%s\n",it->_hFragment.get_htmlSource().c_str());
		if(it->_power > temp._power )
		{
			temp = *it;
		}
	}
	for(it = SubjectPagers.begin();it != SubjectPagers.end(); it++)
	{
		if(it->_power == temp._power 
			&& it->_hFragment.get_linkCharsCount() > temp._hFragment.get_linkCharsCount() )
		{
			temp = *it;
		}
	}
	//printf("SubjectPagerspower = %f\n",temp._power);
	//printf("SubjectPagerslinkCharsCount = %d\n",temp._hFragment.get_linkCharsCount());
	//printf("SubjectPagerssource=%s\n",temp._hFragment.get_htmlSource().c_str());
	return temp;
}
