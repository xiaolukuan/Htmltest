#include "HtmlFragment.h"
class SubjectPart
{
public:
	 //���
	 int _sequence ;
	 //Ȩֵ
     double _power;
	 //�ַ��ܶ�
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