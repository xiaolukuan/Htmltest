#include"articleextract.h"
typedef struct threadparm//����������ݿ��̵߳Ĳ���
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