#pragma once
#include <string>
#include <vector>
#include <stdio.h>
#include <algorithm> 
#include <iostream>
#include <fstream>                                                 
//#include <Windows.h>
#include <cstdlib>
#include <math.h>
#include <sstream>
#include "../html/ParserDom.h"
#include "boost/regex.hpp"
#include <boost/filesystem.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/lexical_cast.hpp>
using namespace std;
using namespace htmlcxx;
using namespace boost;
class HtmlFragment
{
private:
	int _sequence ;/// 序号
    HTML::Node _hNode; /// Html节点
    string _tagName;/// Html节点的标签名
    string _htmlSource; /// Html内容
    int _htmlCount; /// 总字符（包括html字符）
    int _charsCount;  /// 总文字数（不包括html字符，但包括a连接内的文字）
    int _tagCount; /// 总标签数
    int _linkCount; /// 总链接数
    int _linkCharsCount; /// 总链接文字数
    int _punctuationCount; /// 标点符号数,包括如下：， 。、 , .
	double _charsDensity ;/// 非链接文字密度
    double _htmlCharsDensity;/// Html文字密度
    double _punctuationDensity;   /// 标点密度
    double _linkDensity;/// //文字链接密度
	void Init();
public:
	HtmlFragment();
	HtmlFragment(int i,HTML::Node hNode,string htmlsource);
	~HtmlFragment();
	HTML::Node get_hNode();
	int get_sequence();
	string get_tagName();
	string get_htmlSource();
	int get_htmlCount();
	int get_charsCount();
	int get_tagCount();
	int get_linkCount();
	int get_linkCharsCount();
	int get_punctuationCount();
	double get_charsDensity();
	double get_htmlCharsDensity();
	double get_punctuationDensity();
	double get_linkDensity();
	double get_tagDensity();
	double get_LinkCharsAvg();
	int RemoveSpecialChars(string htmlSource);
	int GettagCount(string htmlSource);
	void GetLinkCountAndLinkCharsCount(string htmlSource,int &LinkCount,int &LinkCharsCount);
	int GetPunctuationCount(string htmlSource);
};