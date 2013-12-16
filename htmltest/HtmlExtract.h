#pragma once
#include <string>
#include <vector>
#include <stdio.h>
#include <iostream>
#include <fstream>
//#include <Windows.h>
#include <sstream>
#include "../html/ParserDom.h"
#include "boost/regex.hpp"
#include <boost/filesystem.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/lexical_cast.hpp>
#include "SubjectDeal.h"
#include "titleDeal.h"
#include "PublishTimeDeal.h"
#include "SubjectPagerDeal.h"
#include "../log/CIMSLLog.h"
using namespace std;
using namespace htmlcxx;
using namespace boost;
class HtmlExtract
{
public:
	HtmlExtract();
	~HtmlExtract();
	HtmlExtract(string source,struct SpiderRuler rul);
	void ExtractTiltle();
	void ExtractContent();
	void Extract();
	void NodeLoopProcess(tree<HTML::Node>::iterator it);
	void ProcessEnd();
	void SubjectPagerExtract(string url);
	string GetTitle();
	string GetDateTime();
public:
	int flag;
	tree<HTML::Node> dom;
	SubjectDeal subjectDeal;
	SubjectPart ReserveSubject;
	titleDeal	titledeal;
	TitlePart	Reservetitle;
	PublishTimeDeal publishTimeDeal;
	PublishTimePart ReservePublishTime;
	SubjectPagerDeal subjectPagerDeal;
    SubjectPagerPart ReserveSubjectPager;
	string Introduction;
	string ArticleContent;
	//string SourcePageName;
	list<string> HistoryUrl;
	int  Sequence;
	struct SpiderRuler ruler;
	string htmlsource;
};
