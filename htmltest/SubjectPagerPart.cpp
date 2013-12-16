#include "SubjectPagerPart.h"

SubjectPagerPart::SubjectPagerPart()
{
	_sequence	= 0;
	_power		= 0.0;
}
SubjectPagerPart::~SubjectPagerPart()
{

}
SubjectPagerPart::SubjectPagerPart(int sequence, HtmlFragment hFragment)
{
	_sequence	= sequence;
	_hFragment	= hFragment;
}