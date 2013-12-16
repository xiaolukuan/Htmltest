#include "DBManager.h"
#include "../log/CIMSLLog.h"
CDBManager::CDBManager() : m_pdbConnection(NULL)
{
}

CDBManager::~CDBManager()
{
	DisconnectDB();
}

/// connect postgresql 
bool CDBManager::ConnectDB(const char* connectInfo)
{
	m_pdbConnection = PQconnectdb(connectInfo);
	string errorInfo = "Connection to database failed:";
	if (PQstatus(m_pdbConnection) != CONNECTION_OK)
	{
		errorInfo += PQerrorMessage(m_pdbConnection);
		//cout<<errorInfo<<endl;
		theLog.WriteLog(LOG_LEVEL_ERROR,errorInfo.c_str());
		DisconnectDB();
		return false;
	}
	else
	return true;
	
}

/// disconnect DB
void CDBManager::DisconnectDB()
{
	if (m_pdbConnection != NULL)
	{
		PQfinish(m_pdbConnection);
		m_pdbConnection = NULL;
	}
}


/// query table from database
PGresult* CDBManager::QueryDB(const char* querySql)
{
	PGresult* res = PQexec(m_pdbConnection, querySql);
	string errorInfo = "QueryBD failed:";
	if (PQresultStatus(res) != PGRES_TUPLES_OK)
	{
		errorInfo += PQerrorMessage(m_pdbConnection);
		//cout<<errorInfo<<endl;
		theLog.WriteLog(LOG_LEVEL_ERROR,errorInfo.c_str());
		theLog.WriteLog(LOG_LEVEL_ERROR,querySql);
		PQclear(res);
		res = NULL;
		return NULL;
	}
	else
	{
		return res;
	}
	
}

///获取记录数
int CDBManager::Getntuples(PGresult* res)
{
	return PQntuples(res);
}

///返回一个PGresult 里面一行的单独的一个字段的值
char * CDBManager::Getvalue(PGresult* res,int row_number,int column_number)
{
	return PQgetvalue(res, row_number, column_number);
}

///清理res
void CDBManager::Clearres(PGresult* res)
{
   PQclear(res);
}
///command table from database
bool CDBManager::CommandDB(const char* commandSql)
{
	PGresult *res = PQexec(m_pdbConnection, commandSql);
	string errorInfo = "ComandBD failed:";
	if (PQresultStatus(res) != PGRES_COMMAND_OK)
	{
		errorInfo += PQerrorMessage(m_pdbConnection);
		//cout<<errorInfo<<endl;
		theLog.WriteLog(LOG_LEVEL_ERROR,errorInfo.c_str());
		theLog.WriteLog(LOG_LEVEL_ERROR,commandSql);
		PQclear(res);
		res = NULL;
		return false;
	}
	else
	{
		PQclear(res);
		res = NULL;
		return true;
	}
}
string CDBManager::EscapeString(string strsource)
{
	char *ch = new char[strsource.length() + 50];
    PQescapeString (ch,strsource.c_str(),strsource.length());
	string strtmp = ch;
	delete [] ch;
    return strtmp;
}
bool CDBManager::SetClientEncoding(char *encoding)
{
	string errorInfo = "setClientEncoding failed:";
	if (-1 == PQsetClientEncoding(m_pdbConnection, encoding))
	{
		
		errorInfo +=PQerrorMessage(m_pdbConnection);
		//cout<<errorInfo<<endl;
		theLog.WriteLog(LOG_LEVEL_ERROR,errorInfo.c_str());
		return false;
	}
	else
	return true;
}
string CDBManager::GetClientEncoding()
{
    int nEncoding =  PQclientEncoding(m_pdbConnection);
	string strtmp = pg_encoding_to_char(nEncoding);
	return strtmp;
}
