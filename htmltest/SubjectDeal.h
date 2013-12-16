#include "subjectpart.h"
#include <list>
typedef list<SubjectPart> LISTSUBJECTPART; 
class SubjectDeal
{
private:
	LISTSUBJECTPART subjectpartlist;
	
public:
	SubjectDeal();
	~SubjectDeal();
	void Process(HtmlFragment HF);
	// ��ǩȨ��
	double TagPower(string sourceHtml);
	// �ڵ���ʽȨ��
	double NodeClassPower(HTML::Node hnode);
	// �ڵ�IDȨ��
	double NodeIdPower(HTML::Node hnode);
	// ��������������Ȩ��
	double NoLinkCharsCountPower(HtmlFragment HF);
	// ��������Ȩ��
	double PunctuationCountPower(HtmlFragment HF);
	// ����ܶ�Ȩ��
	double PunctuationDensityPower(HtmlFragment HF);
	// ��ǩ�ܶ�Ȩ��
	double TagDensityPower(HtmlFragment HF);
	// �����������ܶ�Ȩ��
	double CharsDensityPower(HtmlFragment HF);
	// Html�����ܶ�Ȩ��
    // HF.HtmlCharsDensity*10 Ȩֵȡ����������(��������)�����Ϊ3��
	double HtmlCharsDensityPower(HtmlFragment HF);
	// ���������ܶ�Ȩ��
	double LinkDensityPower(HtmlFragment HF);
	// �ų��ؼ���Ȩ��
	double ExcludeWordsPower(string sourceHtml);
	// ��ȡ����
	 SubjectPart GetModel();
};
