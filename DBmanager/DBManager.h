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
	
    ///��ȡ��¼��
    int Getntuples(PGresult* res);
	
	///����һ��PGresult ����һ�еĵ�����һ���ֶε�ֵ
	char *Getvalue(PGresult* res,int row_number,int column_number);

	///����resֻ��QueryDB��ȡ���ݣ����߲���Ҫres��ʱ��ʹ��
	void Clearres(PGresult* res);

	///command table from database
	bool CommandDB(const char* commandSql);

    ///�����ַ�
	string EscapeString(string strsource);

	///���ÿͻ��˱���
	bool SetClientEncoding(char *encoding);

    ///��ȡ�ͻ��˱���
	string GetClientEncoding();
private:
	PGconn* m_pdbConnection; // postgresql connection object
    //PGresult* m_pres;
};

#endif // end __IMSL_DB_MANAGER_H__
