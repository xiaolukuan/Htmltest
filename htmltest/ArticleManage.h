#include"articleextract.h"
typedef struct threadparm//传给检查数据库线程的参数
{
	int ifront;
	int inext;
	int nthread;
	PGresult   *res;

}ThreadParm;

class ArticleManage
{
public:
	ArticleManage();
	~ArticleManage();
	bool start(int nthreads);
public:
};