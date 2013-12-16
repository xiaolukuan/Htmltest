#include "SubjectPagerPart.h"
typedef list<SubjectPagerPart> LISTPAGER; 
class SubjectPagerDeal
{
public:
	SubjectPagerDeal();
	~SubjectPagerDeal();
	void Process(HtmlFragment htmlFragment);
	double NodeClassPower(htmlcxx::HTML::Node hnode);
	double LinkDensityPower(HtmlFragment HF);
	double LinkCharsAvgPower(HtmlFragment HF);
	double LinkFeature(string sourcehtml);
	double TagDensityPower(HtmlFragment HF);
	double IncludeWordsPower(string sourceHtml);
	double ExcludeWordsPower(string sourceHtml);
	SubjectPagerPart GetModel();
public:
	LISTPAGER SubjectPagers;
};	