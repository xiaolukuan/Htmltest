#include "PublishTimePart.h"
PublishTimePart::PublishTimePart()
{
	_sequence = 0;
	_power   = 1.0;
}
PublishTimePart::~PublishTimePart()
{

}
PublishTimePart::PublishTimePart(int sequence,HtmlFragment hFragment)
{
	_power   = 1.0;
	_sequence = sequence;
	_hFragment = hFragment;
}