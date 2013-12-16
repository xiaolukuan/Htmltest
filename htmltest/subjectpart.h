#include "HtmlFragment.h"
class SubjectPart
{
public:
	 //ÐòºÅ
	 int _sequence ;
	 //È¨Öµ
     double _power;
	 //×Ö·ûÃÜ¶È
     double _htmlCharsDensity;
	 HtmlFragment _hFragment;
public:
	SubjectPart();
	~SubjectPart();
	SubjectPart(int sequence, HtmlFragment hFragment);
	int getSequence();
	double getPower();
	double getHtmlCharsDensity();
	HtmlFragment getHtmlFragment();
};