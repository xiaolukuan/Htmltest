#pragma once
#include <string>
#include <vector>
#include <stdio.h>
#include <iostream>
#include <fstream>
//#include <Windows.h>
#include <sstream>
#include "../html/ParserDom.h"
#include "boost/regex.hpp"
#include <boost/filesystem.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/lexical_cast.hpp>
#include "HtmlFragment.h"
#include "HtmlExtract.h"
#include"articleextract.h"
#include "ArticleManage.h"
#include "../common/IMSLCommon.h"
#include "../HttpWrap/HttpWrap.h"
using namespace boost;
using namespace std;
using namespace htmlcxx;