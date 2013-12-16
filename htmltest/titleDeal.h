#pragma once
#include <string>
#include "TitlePart.h"
#include <list>
typedef list<TitlePart> LISTTILEPART; 
class titleDeal
{
public:
	titleDeal();
	~titleDeal();
	void Process(HtmlFragment HF);
	// ��ǩȨ��
	double TagPower(string sourceHtml);
	// �ڵ���ʽȨ��
	double NodeClassPower(HTML::Node hnode);
	double CharsCountPower(int charscount);
	double SourceTitlePower(string sourcetile,string sourceHtml);
	TitlePart GetModel();
public:
	string left_sourceTitle;
	string right_sourceTitle;
	string _sourceTitle;
	LISTTILEPART titlepartlist;
};