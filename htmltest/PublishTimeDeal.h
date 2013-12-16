#include "PublishTimePart.h"
typedef list<PublishTimePart> LISTPUBLISHTIME; 
class PublishTimeDeal
{
public:
	PublishTimeDeal();
	~PublishTimeDeal();
	void Process(HtmlFragment htmlFragment,string webname);
	bool HasDateTime(string sourceHtml);
	double CharsCountPower(HtmlFragment HF);
	double IncludeWordsPower(string sourceHtml);
	double NodeClassPower(HTML::Node hnode,string webname);
	double DateStylePower(string sourceHtml);
	PublishTimePart GetModel();
	LISTPUBLISHTIME publisttimelist;
};