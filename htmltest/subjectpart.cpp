#include "subjectpart.h"
SubjectPart::SubjectPart()
{
	//ÐòºÅ
	  _sequence = 0 ;
	 //È¨Öµ
      _power = 0.0 ;
	 //×Ö·ûÃÜ¶È
      _htmlCharsDensity = 0.0;
}
SubjectPart::~SubjectPart()
{

}
SubjectPart::SubjectPart(int sequence, HtmlFragment hFragment)
{
	_power	= 0.0;
	_sequence = sequence;
    _hFragment = hFragment;
	_htmlCharsDensity = hFragment.get_htmlCharsDensity();
}
int SubjectPart::getSequence()
{
	return _sequence;
}
double SubjectPart::getHtmlCharsDensity()
{
	return _htmlCharsDensity;
}
double SubjectPart::getPower()
{
	return _power;
}
HtmlFragment SubjectPart::getHtmlFragment()
{
	return _hFragment;
} 