#include "HtmlFragment.h"


HtmlFragment::HtmlFragment()
{
}
HtmlFragment::HtmlFragment(int i,HTML::Node hNode,string htmlsource)
{
	_sequence = i;
	_hNode = hNode;
	_htmlSource = htmlsource;
	Init();
}
HtmlFragment::~HtmlFragment()
{
}
 /// Html节点
HTML::Node HtmlFragment::get_hNode()
{
	return  _hNode;
}
 /// 序号
int HtmlFragment::get_sequence()
{
	return _sequence;
}
// Html节点的标签名
string HtmlFragment::get_tagName()
{
	return _tagName;
}
  /// Html内容
string HtmlFragment::get_htmlSource()
{
	return _htmlSource;
}
/// 总字符（包括html字符）
int HtmlFragment::get_htmlCount()
{
	return _htmlCount;
}
/// 总文字数（不包括html字符，但包括a连接内的文字）
int HtmlFragment::get_charsCount()
{
	return _charsCount;
}
/// 总标签数
int HtmlFragment::get_tagCount()
{
	return _tagCount;
}
 /// 总链接数
int HtmlFragment::get_linkCount()
{
	return _linkCount;
}
/// 总链接文字数
int HtmlFragment::get_linkCharsCount()
{
	return _linkCharsCount;
}
  /// 标点符号数,包括如下：， 。、 , .
int HtmlFragment::get_punctuationCount()
{
	return _punctuationCount;
}
/// 标签密度
double HtmlFragment::get_tagDensity()
{
	if (_charsCount == 0)
    {
        return 0;
    }
    return 1000 * _tagCount / _charsCount;
}
/// 非链接文字密度
double HtmlFragment::get_charsDensity()
{
	return _charsDensity;
}
   /// Html文字密度
double HtmlFragment::get_htmlCharsDensity()
{
	return _htmlCharsDensity;
}
/// 标点密度
double HtmlFragment::get_punctuationDensity()
{
	return _punctuationDensity;
}
 /// 文字链接密度
double HtmlFragment::get_linkDensity()
{
	return _linkDensity;
}
/// 链接平均文字数
double HtmlFragment::get_LinkCharsAvg()
{
	if (_linkCount == 0)
    {
		return 0;
    }
	return _linkCharsCount/_linkCount;
}

void HtmlFragment::Init()
{
	_tagName		= _hNode.tagName();
	transform(_tagName.begin(), _tagName.end(), _tagName.begin(), ::tolower); 
//	_htmlSource		= _hNode.text(); /// Html内容
	_htmlCount		= _htmlSource.length(); /// 总字符（包括html字符）
    _charsCount		= RemoveSpecialChars(_htmlSource);/// 总文字数（不包括html字符，但包括a连接内的文字）
    _tagCount		= GettagCount(_htmlSource);/// 总标签数
   // int _linkCount; /// 总链接数
   // int _linkCharsCount; /// 总链接文字数
	GetLinkCountAndLinkCharsCount(_htmlSource,_linkCount,_linkCharsCount);
	_punctuationCount	= GetPunctuationCount(_htmlSource) ; /// 标点符号数,包括如下：， 。、 , .
	//非链接文字密度
     _charsDensity			= _charsCount == 0 ? 0:((((_charsCount - _linkCharsCount) / (double)_charsCount)*100+0.5)/100.0);
     //Html文字密度
     _htmlCharsDensity		= _htmlCount == 0 ? 0:(((_charsCount / (double)_htmlCount)*10000+0.5)/10000.0);
     //标点密度
     _punctuationDensity	= _charsCount == 0?0:(((_punctuationCount / (double)_charsCount)*100+0.5)/100.0);
     //文字链接密度
     _linkDensity			= _charsCount == 0?0:(((_linkCount / (double)_charsCount)*100+0.5)/100.0);
/*	 printf("@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
	 printf("_htmlCount=%d\n",_htmlCount);
	 printf("_charsCount=%d\n",_charsCount); 
	 printf("_tagCount=%d\n",_tagCount); 
	 printf("_linkCount=%d\n",_linkCount);
	 printf("_linkCharsCount=%d\n",_linkCharsCount);
	 printf("_punctuationCount=%d\n",_punctuationCount); 
	 printf("_charsDensity=%f\n",_charsDensity); 
	 printf("_htmlCharsDensity=%f\n",_htmlCharsDensity); 
	 printf("_punctuationDensity=%f\n",_punctuationDensity); 
	 printf("_linkDensity=%f\n",_linkDensity); */
}
int HtmlFragment::RemoveSpecialChars(string htmlSource)
{
	string temp = htmlSource;
	string hanzistr;
	string elsestr;
	//boost::regex reg("((?i)(<a[^>]+>.*?</a>))");
	//temp = boost::regex_replace(temp,reg,"");
	boost::regex reg1("(?i)(<[^>]+>)");
	temp = boost::regex_replace(temp,reg1,"");
	boost::regex reg2("[ \\r\\n\\t]");
	temp = boost::regex_replace(temp,reg2,"");
	hanzistr = temp;
	elsestr  = temp;
	boost::regex reg3("[^\u4e00-\u9fa5]");
	hanzistr = boost::regex_replace(hanzistr,reg3,"");
//	boost::regex reg4("[\u4e00-\u9fa5]");
	//elsestr = boost::regex_replace(elsestr,reg4,"");
	//printf("hanzistr=%s\n",hanzistr.c_str());
	return hanzistr.length()/3;
}

int HtmlFragment::GettagCount(string htmlSource)
{
	
	if(htmlSource.empty())
	{
		return 0;
	}
	string temp = htmlSource;
	boost::regex reg("@");
	temp = boost::regex_replace(temp,reg,"");
	boost::regex reg1("<[^>]+>");
	temp = boost::regex_replace(temp,reg1,"@");
	boost::regex reg2("[^@]");
	temp = boost::regex_replace(temp,reg2,"");
	return temp.length();
}
void HtmlFragment::GetLinkCountAndLinkCharsCount(string htmlSource,int &LinkCount,int &LinkCharsCount)
{
	if(htmlSource.empty())
	{
		LinkCount = 0;
		LinkCharsCount = 0;
		return ;
	}
	string temp = htmlSource;
	boost::regex reg("@");
	temp = boost::regex_replace(temp,reg,"");
//	<a href="http://www.news.cn/" target="_blank" class="logo">新华网</a>
	boost::regex reg1("((?i)<a[^>]+>.*?</a>)");
	temp = boost::regex_replace(temp,reg1,"@");
	boost::regex reg2("[^@]");
	temp = boost::regex_replace(temp,reg2,"");
	LinkCount = temp.length();
	temp = htmlSource;
	string atagstr = htmlSource;
	string atagstring;
	int starIndex = 0;
	string tagstart("<a");
	string tagend("</a>");
	int beginpos = 0;
	int endpos   = 0;
	while( -1 != (beginpos = temp.find(tagstart,starIndex)))
	{
		//int begin = temp.find(tagstart,starIndex);
		//starIndex = begin;
		//printf("777777\n");
		endpos = temp.find(tagend,starIndex);
		if(endpos != -1)
		{
			starIndex = endpos;
			atagstring += temp.substr(beginpos,endpos+4-beginpos);
			starIndex += 4;
		}
		else
		{
			break;
		}
	}
	
	//boost::regex reg3("(?i)(<a\\s[^>]+>)");
	//atagstring = boost::regex_replace(atagstring,reg3,"");
	//printf("atagstring=%s\n",atagstring.c_str());
	//boost::regex reg4("(?i)</a>");
	//atagstring = boost::regex_replace(atagstring,reg,"");
//	printf("atagstring=%s\n",atagstring.c_str());
//	boost::regex reg3("((?i)(<a[^>]+>.*?</a>))");
	//temp = boost::regex_replace(temp,reg3,"");
	boost::regex reg4("(?i)(<[^>]+>)");
	temp = boost::regex_replace(temp,reg4,"");
	boost::regex reg5("[^\u4e00-\u9fa5]");
	atagstring = boost::regex_replace(atagstring,reg5,"");
	boost::regex reg6("[ \\r\\n\\t]");
	atagstring = boost::regex_replace(atagstring,reg6,"");
	//printf("atagstring=%s\n",atagstring.c_str());
	LinkCharsCount = atagstring.length()/3;
	return ;
}

int HtmlFragment::GetPunctuationCount(string htmlSource)
{
	if(htmlSource.empty())
	{
		return 0;
	}
	//boost::regex reg1("[^\u4e00-\u9fa5]");
	//htmlSource = boost::regex_replace(htmlSource,reg1,"");
	string temp = htmlSource;
	boost::regex reg6("(?i)(<[^>]*>)");
	temp = boost::regex_replace(temp,reg6,"");
	boost::regex reg7("[\\r\\n\\t]");
	temp = boost::regex_replace(temp,reg7,"");
	//boost::regex reg8("[a-zA-Z0-9\u4e00-\u9fa5]");
	//temp = boost::regex_replace(temp,reg8,"");
	//printf("temp=%s\n",temp.c_str());
	int Pcount = 0;
	boost::smatch m;
	boost::smatch m1;
	boost::smatch m2;
	boost::smatch m3;
	boost::smatch m4;
	//\xEF\xBC\x8C\xE3\x80\x82\x2C\x2E\xE3\x80\x81
	boost::regex reg("\xEF\xBC\x8C");
	boost::regex reg1("\xE3\x80\x82");
	//boost::regex reg2("\x2C");
	//boost::regex reg3("\x2E");
	boost::regex reg4("\xE3\x80\x81");
	//boost::regex reg("，");
	//boost::regex reg1("。");
	//boost::regex reg2(",");
	//boost::regex reg3(".");
	//boost::regex reg4("、");
	std::string::const_iterator start = htmlSource.begin();
	std::string::const_iterator end = htmlSource.end();
	while(boost::regex_search(start,end,m,reg))
	{
			if (m[0].matched)
			{
				Pcount++;
				start = m[1].second;
			}
	}
	start = htmlSource.begin();
	while(boost::regex_search(start,end,m1,reg1))
	{
			if (m1[0].matched)
			{
				Pcount++;
				start = m1[1].second;
			}	
	}
	/*start = htmlSource.begin();
	while(boost::regex_search(start,end,m2,reg2))
	{
			if (m2[0].matched)
			{
				Pcount++;
				start = m2[1].second;
			}	
	}
	start = htmlSource.begin();
	while(boost::regex_search(start,end,m3,reg3))
	{
			if (m3[0].matched)
			{
				Pcount++;
				start = m3[1].second;
			}		
	}*/
	start = htmlSource.begin();
	while(boost::regex_search(start,end,m4,reg4))
	{
			if (m4[0].matched)
			{
				Pcount++;
				start = m4[1].second;
			}	
	}
	/*string temp = htmlSource;
	boost::regex reg1("(?i)(<[^>]*>)");
	temp = boost::regex_replace(temp,reg1,"");
	boost::regex reg2("[\\r\\n\\t]");
	temp = boost::regex_replace(temp,reg2,"");
	boost::regex reg3("[\\w]");
	temp = boost::regex_replace(temp,reg3,"");
	//boost::regex reg("[^，。,.、]");
	printf("htmlSource=%s\n",htmlSource.c_str());
//	boost::regex reg("[^\xEF\xBC\x8C\xE3\x80\x82\x2C\x2E\xE3\x80\x81]");
	//temp = boost::regex_replace(temp,reg,"");*/
	
	//printf("count = %d\n",Pcount);
	return Pcount;
}

