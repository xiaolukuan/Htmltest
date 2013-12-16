#include "ArticleManage.h"
#include <boost/thread/thread.hpp>
#include"articleextract.h"
#include <boost/bind.hpp>
static void run(ThreadParm* tp);
ArticleManage::ArticleManage()
{
}
ArticleManage::~ArticleManage()
{
}
bool ArticleManage::start(int nthreads)
{
	CDBManager m_dbmanager;
	PGresult   *res;
	ThreadParm m_tp[32];
	thread_group threads;
	if (!m_dbmanager.ConnectDB(p_config["articledbconninfo"].c_str()))
	{
		theLog.WriteLog(LOG_LEVEL_SYS,"Connect postgrl faild :%s ...",p_config["articledbconninfo"].c_str());
		return false;
	}
	else 
		m_dbmanager.SetClientEncoding("GB18030");
	//theconfig.OnUpdateCompanyAndManager(&m_dbmanager);
 	if ((res = m_dbmanager.QueryDB(p_config["querysql"].c_str())) == NULL)
 	{
		theLog.WriteLog(LOG_LEVEL_SYS,"QueryDB postgrl faild :%s ...",p_config["querysql"].c_str());
 		return false;
 	}
 	int nrecrods = m_dbmanager.Getntuples(res);
	theLog.WriteLog(LOG_LEVEL_SYS,"executing sql retrun %d records will be get!",nrecrods);
	if(0 == nrecrods)
	{
		return true;
	}
 	for(int i=1;i<=nthreads;++i)
 	{
 		m_tp[i-1].nthread = i;
 		m_tp[i-1].ifront = (i-1)*nrecrods/nthreads;
 		m_tp[i-1].inext = i*nrecrods/nthreads;
 		m_tp[i-1].res = res;
 		threads.create_thread(bind(&run,&m_tp[i-1]));
 	}
 	threads.join_all();
	m_dbmanager.Clearres(res);
	m_dbmanager.DisconnectDB();
	return true;
}
void run(ThreadParm* tp)
{
	ThreadParm *pparm = (ThreadParm*)tp;
	articleextract* aticle = new articleextract();
	if( !aticle->OnInitDB() )
	{
		aticle->OnExitDB();
		if(aticle)
		{
			delete aticle;
			aticle = NULL;
		}
		theLog.WriteLog(LOG_LEVEL_ERROR,"the thread %d exited! sql init failed!",pparm->nthread);
		return;
	}
	for (int j = pparm->ifront; j < pparm->inext; j++)
	{
//		printf("321321\n");
		string aid			  = PQgetvalue(pparm->res,j,0);
		string newarticleurl  = PQgetvalue(pparm->res,j,1);
		string sourcepagename = PQgetvalue(pparm->res,j,2);
		aticle->extract(aid,newarticleurl,sourcepagename);
//		printf("5555\n");
	}
	aticle->OnExitDB();
	if(aticle)
	{
		delete aticle;
		aticle = NULL;
	
	}
	theLog.WriteLog(LOG_LEVEL_SYS,"the thread %d exited!",pparm->nthread);
	return;	
}
