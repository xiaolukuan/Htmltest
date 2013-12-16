#include "TitlePart.h"

TitlePart::TitlePart()
{
	_sequence = 0;
	_charsCount = 0;
	_power = 0.0;
}
TitlePart::~TitlePart()
{

}
TitlePart::TitlePart(int sequence, HtmlFragment hFragment)
{
	_sequence	= sequence;
	_charsCount = hFragment.get_charsCount();
	_hFragment	= hFragment;
	_power		= 0.0;
}