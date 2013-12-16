#include "HtmlFragment.h"
class TitlePart
{
public:
	int _sequence;
	double _power;
	int _charsCount;
	HtmlFragment _hFragment;
public:
	TitlePart();
	~TitlePart();
	TitlePart(int sequence, HtmlFragment hFragment);


};