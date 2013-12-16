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
	// 标签权限
	double TagPower(string sourceHtml);
	// 节点样式权限
	double NodeClassPower(HTML::Node hnode);
	// 节点ID权限
	double NodeIdPower(HTML::Node hnode);
	// 非链接文字总数权限
	double NoLinkCharsCountPower(HtmlFragment HF);
	// 标点符号数权限
	double PunctuationCountPower(HtmlFragment HF);
	// 标点密度权限
	double PunctuationDensityPower(HtmlFragment HF);
	// 标签密度权限
	double TagDensityPower(HtmlFragment HF);
	// 非链接文字密度权限
	double CharsDensityPower(HtmlFragment HF);
	// Html文字密度权限
    // HF.HtmlCharsDensity*10 权值取其整数部分(四舍五入)，最大为3。
	double HtmlCharsDensityPower(HtmlFragment HF);
	// 文字链接密度权限
	double LinkDensityPower(HtmlFragment HF);
	// 排除关键字权限
	double ExcludeWordsPower(string sourceHtml);
	// 获取主题
	 SubjectPart GetModel();
};
