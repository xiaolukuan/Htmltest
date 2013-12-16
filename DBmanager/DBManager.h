// FileName : DBManager.h
// Creator  : luis yang
// Date     : 2011-07-12
// Comment  : DB manager

#ifndef __IMSL_DB_MANAGER_H__
#define __IMSL_DB_MANAGER_H__

#include "/usr/include/postgresql/libpq-fe.h"
#include "../common/IMSLCommon.h"

class CDBManager
{
public:
	CDBManager();

	~CDBManager();

public:
	/// connect postgresql 
	bool ConnectDB(const char* connectInfo);

	/// disconnect DB
	void DisconnectDB();

    /// query table from database
	PGresult* QueryDB(const char* querySql);
	
    ///获取记录数
    int Getntuples(PGresult* res);
	
	///返回一个PGresult 里面一行的单独的一个字段的值
	char *Getvalue(PGresult* res,int row_number,int column_number);

	///清理res只在QueryDB获取数据，或者不需要res的时候使用
	void Clearres(PGresult* res);

	///command table from database
	bool CommandDB(const char* commandSql);

    ///逃逸字符
	string EscapeString(string strsource);

	///设置客户端编码
	bool SetClientEncoding(char *encoding);

    ///获取客户端编码
	string GetClientEncoding();
private:
	PGconn* m_pdbConnection; // postgresql connection object
    //PGresult* m_pres;
};

#endif // end __IMSL_DB_MANAGER_H__
