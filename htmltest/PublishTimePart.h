#include "HtmlFragment.h"
class PublishTimePart
{
public:
	PublishTimePart();
	PublishTimePart(int sequence,HtmlFragment hFragment);
	~PublishTimePart();

public:
	int _sequence;
	double _power;
	HtmlFragment _hFragment;
};