#include "HtmlFragment.h"
class SubjectPagerPart
{
public:
	SubjectPagerPart();
	~SubjectPagerPart();
	SubjectPagerPart(int sequence,HtmlFragment hFragment);
	
public:
	int _sequence;
	double _power;
	HtmlFragment _hFragment;
};