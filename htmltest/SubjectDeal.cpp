#pragma once
#include <string>
#include "SubjectDeal.h"
#define LOGINNAME "\xE7\x99\xBB\xE5\xBD\x95\xE5\x9\\x8D"   //登录名
#define NIMING   "\xe5\x8c\xbf\xe5\x9\\x8d\xe8\xaf\x84\xe8\xae\xba"//匿名评论
#define BANQUANSUOYOU "\xE7\x89\x88\xE6\x9D\x83\xE6\x89\x80\xE6\x9C\x89"//版权所有
#define BANQUANSHENGMING "\xE7\x89\x88\xE6\x9D\x83\xE5\xA3\xB0\xE6\x98\x8E"//版权声明

#define MIANZESHENGMING "\xE5\x85\x8D\xE8\xB4\xA3\xE5\xA3\xB0\xE6\x98\x8E" //免责声明
SubjectDeal::SubjectDeal()
{

}
SubjectDeal::~SubjectDeal()
{

}
void SubjectDeal::Process(HtmlFragment HF)
{
	string tempname = HF.get_tagName();
	//不在处理标签内，返回
	if(HF.get_tagName() != "div" && HF.get_tagName() != "td")
	{
		return;
	}
	//字符数等亿的，返回
	if (HF.get_charsCount() == 0)
    {
        return;
    }
	//定义主题实体
    SubjectPart subject(HF.get_sequence(),HF);
	subject._power += TagPower(HF.get_htmlSource());
	subject._power += NodeClassPower(HF.get_hNode());
    subject._power += NodeIdPower(HF.get_hNode());
    subject._power += NoLinkCharsCountPower(HF);
    subject._power += PunctuationCountPower(HF);
    subject._power += PunctuationDensityPower(HF);
    subject._power += TagDensityPower(HF);
    subject._power += CharsDensityPower(HF);
    subject._power += HtmlCharsDensityPower(HF);
    subject._power += LinkDensityPower(HF);
    subject._power += ExcludeWordsPower(HF.get_htmlSource());
	//权值必须大亿，才进入候选队刿
	if (subject._power > (double) 0.0)
    {
	//	printf("power= %d\n",(int)subject._power);
	//	printf("HtmlCharsDensity = %f\n",subject.getHtmlCharsDensity());
	//	printf("sourcehtml=%s\n",HF.get_htmlSource().c_str());
		subjectpartlist.push_back(subject);
     }
	
} 

double SubjectDeal::TagPower(string sourceHtml)
{
	if (sourceHtml.empty())
    {
       return 0;
    }
	boost::regex reg("(?i)(<p>|<br(\\s?)(/?)>)");
	//boost::regex reg("<p>");
	boost::smatch m;
	if(boost::regex_search(sourceHtml,m,reg))
	{
		return 1;
	}   
	return 0;
}
double SubjectDeal::NodeClassPower(htmlcxx::HTML::Node hnode)
{
	hnode.parseAttributes();
	string nodeClassstring = hnode.attribute("class").second;
	transform(nodeClassstring.begin(), nodeClassstring.end(), nodeClassstring.begin(), ::tolower); 
	if((-1 != nodeClassstring.find("articalcontent",0)) 
		|| (-1 != nodeClassstring.find("article_content",0)) 
			|| (-1 != nodeClassstring.find("article",0))
				||(-1 != nodeClassstring.find("textmain",0)))
	{
		return 5;
	}
	else
	{
		return 0;
	}
/*	else if((-1 != nodeClassstring.find("body",0)) || (-1 != nodeClassstring.find("content",0)))
	{
		if(0 == sourcepagename.compare("cnBeta") && 0 == nodeClassstring.compare("content"))
		{
//			printf("aaaaa\n");
			return 4;
		}
		else
		{
			return 1;
		}
	}
	else if(-1 != nodeClassstring.find("introduction",0) && 0 == sourcepagename.compare("cnBeta"))
	{
		return -4;
	}*/
	
}
double SubjectDeal::NodeIdPower(htmlcxx::HTML::Node hnode)
{
	hnode.parseAttributes();
	string nodeIdstring = hnode.attribute("id").second;
	transform(nodeIdstring.begin(), nodeIdstring.end(), nodeIdstring.begin(), ::tolower); 
	if((-1 != nodeIdstring.find("post",0)) || (-1 != nodeIdstring.find("content",0)))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
/// 非链接文字总数权限
/// (HF.CharsCount-HF.LinkCharsCount)/200 权值取其整数部分，最大为5
double SubjectDeal::NoLinkCharsCountPower(HtmlFragment HF)
{
	int multiple = (HF.get_charsCount() - HF.get_linkCharsCount()) / 200;
    if (multiple > 0)
    {
         multiple = multiple + 1;
         return multiple >= 5 ? 5 : multiple;
    }
	if ((HF.get_charsCount() - HF.get_linkCharsCount()) > 100)
    {
         return 1;
    }
	return 0;
}

/// 标点符号数权陿
double SubjectDeal::PunctuationCountPower(HtmlFragment HF)
{
	if (HF.get_punctuationCount() > 5)
    {
        return 1;
    }
    return 0;
}
/// 标点密度权限
double SubjectDeal::PunctuationDensityPower(HtmlFragment HF)
{
	if (HF.get_punctuationCount() < 5)
    {
		return 0;
	}
	if (HF.get_punctuationDensity() > 0.03)
	{
		return 1;
	}
	return 0;
}
 /// 标签密度权限
double SubjectDeal::TagDensityPower(HtmlFragment HF)
{
	//printf("tagDensity=%f\n",HF.get_tagDensity());
	if (HF.get_charsCount() < 100)
	{
		return 0;
	}
	/*if (HF.get_tagDensity() > 5 && HF.get_tagDensity() <= 30)
	{
		return 3;
	}
	else if (HF.get_tagDensity() > 30 && HF.get_tagDensity() <= 70)
	{
		return 2;
	}
	else if (HF.get_tagDensity() > 70 && HF.get_tagDensity() <= 140)
	{
		return 1;
	}
	else if (HF.get_tagDensity() > 220)
	{
		return -1;
	}*/
	if (HF.get_tagDensity() > 10 && HF.get_tagDensity() <= 70)
	{
		return 3;
	}
	else if (HF.get_tagDensity() > 70 && HF.get_tagDensity() <= 140)
	{
		return 2;
	}
	else if (HF.get_tagDensity() > 140 && HF.get_tagDensity() <= 200)
	{
		return 1;
	}
	else if (HF.get_tagDensity() > 300)
	{
		return -1;
	}
	return 0;
}
  /// 非链接文字密度权陿
double SubjectDeal::CharsDensityPower(HtmlFragment HF)
{
	if (HF.get_charsDensity() > 0.7)
    {
       return 1;
    }
    return 0;
}
/// Html文字密度权限
/// HF.HtmlCharsDensity*10 权值取其整数部刿四舍五入)，最大为3〿
double SubjectDeal::HtmlCharsDensityPower(HtmlFragment HF)
{
	if (HF.get_charsCount() < 100)
     {
		 return 0;
	 }
	 int temp = (int)(HF.get_htmlCharsDensity()+0.5);
	 if (temp > 0)
	 {
		 return temp >= 3 ? 3 : temp;
	 }
	 return 0;
}
double SubjectDeal::LinkDensityPower(HtmlFragment HF)
{
	if (HF.get_linkDensity() < 0.08)
	{
		return 1;
	}
	return 0;
}
double SubjectDeal::ExcludeWordsPower(std::string sourceHtml)
{
	if (sourceHtml.empty())
    {
		return 0;
	}
	double power = 0;
	
	boost::regex reg(LOGINNAME);//登录名
	boost::regex reg1(NIMING); //匿名评论
	//boost::regex reg1("(?i)(BANQUANSUOYOU|BANQUANSHENGMING)");
	//boost::regex reg1("(?i)(\xE7\x89\x88\xE6\x9D\x83\xE6\x89\x80\xE6\x9C\x89|\xE7\x89\x88\xE6\x9D\x83\xE5\xA3\xB0\xE6\x98\x8E|Copyright)");
	boost::regex reg2("(?i)(ICP备\\d+号)");
//	ICP\xE5\xA4\x87\\d\xE5\x8F\xB7
	//boost::regex reg2("(?i)(ICP\xE5\xA4\x87\\d\xE5\x8F\xB7))");
	//boost::regex reg2("(?i)(\x49\x43\x50\xE5\xA4\x87\x5C\x5C\x64\x2B\xE5\x8F\xB7))");
	//boost::regex reg3("(?i)(免责声明)");
	//boost::regex reg3(MIANZESHENGMING);
	//boost::regex reg4("(?i)(分享到(新浪微博|腾讯微博|人人网|网易微博|QQ空间))");
	boost::regex reg5(BANQUANSUOYOU);
	boost::regex reg6(BANQUANSHENGMING);
	boost::regex reg7("(?i)(Copyright)");
	//分享到新浪微博
	boost::regex reg8("\xE6\x96\xB0\xE6\xB5\xAA\xE5\xBE\xAE\xE5\x8D\x9A");
	//分享到腾讯文博
	boost::regex reg9("\xE8\x85\xBE\xE8\xAE\xAF\xE5\xBE\xAE\xE5\x8D\x9A");
	//分享到人人网
	boost::regex reg10("\xE4\xBA\xBA\xE4\xBA\xBA\xE7\xBD\x91");
	//分享到网易微博
	boost::regex reg11("\xE7\xBD\x91\xE6\x98\x93\xE5\xBE\xAE\xE5\x8D\x9A");
	//分享QQ空间
	boost::regex reg12("(?i)(\x51\x51\xE7\xA9\xBA\xE9\x97\xB4)");
	//分享到
	boost::regex reg13("\xE5\x88\x86\xE4\xBA\xAB\xE5\x88\xB0");
	boost::smatch m;
	if(boost::regex_search(sourceHtml,m,reg)||boost::regex_search(sourceHtml,m,reg1) )
	{
	//	printf("reg0 or 1 search\n");
        power += -2;
	}
	if (boost::regex_search(sourceHtml,m,reg2))
	{
	//	printf("reg2 ICP search\n");
		power += -2;
	}
	//if (boost::regex_search(sourceHtml,m,reg3))
//	{
		//printf("reg2 search\n");
	//	power += -2;
	//}
/*	if (boost::regex_search(sourceHtml,m,reg4))
	{
		power += -2;
	}*/
	if (boost::regex_search(sourceHtml,m,reg5) 
			|| boost::regex_search(sourceHtml,m,reg6)
				|| boost::regex_search(sourceHtml,m,reg7))
	{
	//	printf("reg5 or 6 or 7 search %s\n",m[0].str().c_str());
		power += -2;
	}
/*	if ((boost::regex_search(sourceHtml,m,reg8) 
			|| boost::regex_search(sourceHtml,m,reg9)
				|| boost::regex_search(sourceHtml,m,reg10)
					|| boost::regex_search(sourceHtml,m,reg11)
						|| boost::regex_search(sourceHtml,m,reg12))
						&&boost::regex_search(sourceHtml,m,reg13))
	{
	//	printf("reg8 or 9 or 10 or 11 or 12 search\n");
		power += -2;
	}*/
	return power;
}
SubjectPart SubjectDeal::GetModel()
{
	LISTSUBJECTPART temp;
	LISTSUBJECTPART::iterator it;
	SubjectPart subjecttemp;
	for(it = subjectpartlist.begin() ;it != subjectpartlist.end(); it++ )
	{
		//printf("44444\n");
		if( it->getPower() > subjecttemp.getPower() )
		{
			//temp.push_back(*it);
			subjecttemp = *it;
		}
	}
	
	for(it = subjectpartlist.begin(); it != subjectpartlist.end(); it++)
	{
		if( it->getPower() >= subjecttemp.getPower() )
		{
			temp.push_back(*it);
		}
	//	printf("55555\n");
		//if(it->getHtmlCharsDensity() > subjecttemp.getHtmlCharsDensity())
	//	{
		//	subjecttemp = *it;
		//}
	}
	for(it = temp.begin(); it != temp.end(); it++)
	{
		if(it->getHtmlCharsDensity() > subjecttemp.getHtmlCharsDensity())
		{
			subjecttemp = *it;
		}
	}
	if(subjecttemp.getPower() == (double)0.0)
	{
		for(it = subjectpartlist.begin(); it != subjectpartlist.end(); it++)
		{	
			if(it->getHtmlCharsDensity() > subjecttemp.getHtmlCharsDensity())
			{
				subjecttemp = *it;
			}
		}
	}
	/*for(it = temp.begin(); it != temp.end(); it++)
	{
	//	printf("55555\n");
		if(it->getHtmlFragment().get_charsDensity() > subjecttemp.getHtmlFragment().get_charsDensity())
		{
			subjecttemp = *it;
		}
	}
	if(subjecttemp.getPower() == (double)0.0)
	{
		for(it = subjectpartlist.begin(); it != subjectpartlist.end(); it++)
		{	
	//	printf("55555\n");
			if(it->getHtmlFragment().get_charsDensity() > subjecttemp.getHtmlFragment().get_charsDensity())
			{
				subjecttemp = *it;
			}
		}
	}*/
	/*printf("***********************************************************\n");
	printf("_htmlCount=%d\n",subjecttemp.getHtmlFragment().get_htmlCount());
	printf("_charsCount=%d\n",subjecttemp.getHtmlFragment().get_charsCount()); 
	printf("_tagCount=%d\n",subjecttemp.getHtmlFragment().get_tagCount()); 
	printf("_linkCount=%d\n",subjecttemp.getHtmlFragment().get_linkCount());
	printf("_linkCharsCount=%d\n",subjecttemp.getHtmlFragment().get_linkCharsCount());
	printf("_punctuationCount=%d\n",subjecttemp.getHtmlFragment().get_punctuationCount()); 
	printf("_charsDensity=%f\n",subjecttemp.getHtmlFragment().get_charsDensity()); 
	printf("_htmlCharsDensity=%f\n",subjecttemp.getHtmlFragment().get_htmlCharsDensity()); 
	printf("_punctuationDensity=%f\n",subjecttemp.getHtmlFragment().get_punctuationDensity()); 
	printf("_linkDensity=%f\n",subjecttemp.getHtmlFragment().get_linkDensity()); 
	printf("_tagDensity=%f\n",subjecttemp.getHtmlFragment().get_tagDensity()); 
	printf("the last power = %d\n",(int)subjecttemp.getPower());
	printf("the _htmlCharsDensity =%f\n",subjecttemp.getHtmlFragment().get_htmlCharsDensity());
	printf("source html=%s\n",subjecttemp.getHtmlFragment().get_htmlSource().c_str());
*/
	return subjecttemp;
}
