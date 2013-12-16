#pragma once
#include <string>
#include <vector>
#include <stdio.h>
#include <algorithm> 
#include <iostream>
#include <fstream>                                                 
//#include <Windows.h>
#include <cstdlib>
#include <math.h>
#include <sstream>
#include "../html/ParserDom.h"
#include "boost/regex.hpp"
#include <boost/filesystem.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/lexical_cast.hpp>
using namespace std;
using namespace htmlcxx;
using namespace boost;
class HtmlFragment
{
private:
	int _sequence ;/// ���
    HTML::Node _hNode; /// Html�ڵ�
    string _tagName;/// Html�ڵ�ı�ǩ��
    string _htmlSource; /// Html����
    int _htmlCount; /// ���ַ�������html�ַ���
    int _charsCount;  /// ����������������html�ַ���������a�����ڵ����֣�
    int _tagCount; /// �ܱ�ǩ��
    int _linkCount; /// ��������
    int _linkCharsCount; /// ������������
    int _punctuationCount; /// ��������,�������£��� ���� , .
	double _charsDensity ;/// �����������ܶ�
    double _htmlCharsDensity;/// Html�����ܶ�
    double _punctuationDensity;   /// ����ܶ�
    double _linkDensity;/// //���������ܶ�
	void Init();
public:
	HtmlFragment();
	HtmlFragment(int i,HTML::Node hNode,string htmlsource);
	~HtmlFragment();
	HTML::Node get_hNode();
	int get_sequence();
	string get_tagName();
	string get_htmlSource();
	int get_htmlCount();
	int get_charsCount();
	int get_tagCount();
	int get_linkCount();
	int get_linkCharsCount();
	int get_punctuationCount();
	double get_charsDensity();
	double get_htmlCharsDensity();
	double get_punctuationDensity();
	double get_linkDensity();
	double get_tagDensity();
	double get_LinkCharsAvg();
	int RemoveSpecialChars(string htmlSource);
	int GettagCount(string htmlSource);
	void GetLinkCountAndLinkCharsCount(string htmlSource,int &LinkCount,int &LinkCharsCount);
	int GetPunctuationCount(string htmlSource);
};