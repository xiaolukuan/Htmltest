#include "HttpWrap.h"
#include <cstring>
#include <cstdlib>
#include "HTTPClientAuth.h"
#include <iostream>
//文件写入流
int CurlDebug(CURL *pcurl, curl_infotype itype, char * pData, size_t size, void *)
{
    if(itype == CURLINFO_TEXT)
    {
        std::cout<<"[TEXT]:" << pData;
    }
    else if(itype == CURLINFO_HEADER_IN)
    {
        std::cout << "[HEADER_IN]:" << pData;
    }
    else if(itype == CURLINFO_HEADER_OUT)
    {
        std::cout << "[HEADER_OUT]:" << pData;
    }
    else if(itype == CURLINFO_DATA_IN)
    {
        std::cout << "[DATA_IN]:" << pData;
    }
    else if(itype == CURLINFO_DATA_OUT)
    {
        std::cout << "[DATA_OUT]:" << pData;
    }
    return 0;
}

size_t my_fwrite(void *buffer, size_t size, size_t nmemb, void *user_p)
{
//	printf("buffer=%s\n",(char*)buffer);
	FILE *fp = (FILE *)user_p;
	size_t return_size = fwrite(buffer,size,nmemb,fp);
	return return_size;
}
void set_share_handle(CURL* curl_handle) 
{ 
	static CURLSH* share_handle = NULL; 
	if (!share_handle) 
	{ 
		share_handle = curl_share_init(); 
		curl_share_setopt(share_handle, CURLSHOPT_SHARE, CURL_LOCK_DATA_DNS); 
	} 
	curl_easy_setopt(curl_handle, CURLOPT_SHARE, share_handle); 
	curl_easy_setopt(curl_handle, CURLOPT_DNS_CACHE_TIMEOUT, 60 * 5); 
}
HttpWrap::HttpWrap()
{
	m_curl=curl_easy_init();	
}
HttpWrap::~HttpWrap()
{
	if(m_curl)curl_easy_cleanup(m_curl);
}
static size_t recvdata(void* pVoid, size_t size, size_t nmemb, void* stream)
{
	std::string* pStr=(std::string*)stream;
	int num=size*nmemb;
	pStr->append((char*)pVoid, num);
	//printf("recv result:%d nbytes\n",num);
	return num;
}
void HttpWrap::SetHeader(const char* pHeader)
{
	if(pHeader)m_HeaderVect.push_back(pHeader);
	else{
		m_HeaderVect.clear();
	}
}
/**pCode means use url encode**/
std::string HttpWrap::Get(const char* pHttpUrl)
{
	if(!m_curl||!pHttpUrl)return "";
	CURLcode res;
	m_recvdata.clear();
	set_share_handle(m_curl);
	res=curl_easy_setopt(m_curl, CURLOPT_URL, pHttpUrl);
	res=curl_easy_setopt(m_curl, CURLOPT_FOLLOWLOCATION, 1);
	res=curl_easy_setopt(m_curl, CURLOPT_FORBID_REUSE, 1);
	res=curl_easy_setopt(m_curl, CURLOPT_USERAGENT, "Mozilla/4.0 (compatible; MSIE 5.5; Windows NT 5.0)");
//	curl_easy_setopt(m_curl, CURLOPT_USERAGENT, "FireFox");
	res=curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, &m_recvdata);
	res=curl_easy_setopt(m_curl, CURLOPT_WRITEFUNCTION, recvdata);
	res=curl_easy_setopt(m_curl,CURLOPT_NOSIGNAL,1);
	//res=curl_easy_setopt(m_curl, CURLOPT_CONNECTTIMEOUT, nSeconds);
	res=curl_easy_setopt(m_curl, CURLOPT_TIMEOUT, 10l);
	res=curl_easy_perform(m_curl);
	if(res!=CURLE_OK){
		return "";
	}
	return m_recvdata;
}
bool HttpWrap::GetImg(const char *Imgurl,const char *ImgSavepath,const char *ImgDir)
{
	if(!m_curl || !Imgurl || !ImgDir)
	{
		return false;
	}
	CURLcode res;
	FILE *fp = fopen(ImgSavepath, "wb+");
	if(NULL == fp)
	{
		printf("open %s failed\n",ImgSavepath);
		return false;
	}
  //      printf("Imgurl=%s\n",Imgurl);
//	printf("ImgSavepath=%s\n",ImgSavepath);
//	printf("ImgDir=%s\n",ImgDir);
	set_share_handle(m_curl);
	//设置远端地址
	res = curl_easy_setopt(m_curl,CURLOPT_URL,Imgurl);
	res=curl_easy_setopt(m_curl, CURLOPT_FOLLOWLOCATION, 1);
	res=curl_easy_setopt(m_curl, CURLOPT_FORBID_REUSE, 1);
	res=curl_easy_setopt(m_curl, CURLOPT_USERAGENT, "Mozilla/4.0 (compatible; MSIE 5.5; Windows NT 5.0)");
//	curl_easy_setopt(m_curl, CURLOPT_USERAGENT, "FireFox"); 
//	curl_easy_setopt(m_curl, CURLOPT_DEBUGFUNCTION, CurlDebug);
//	curl_easy_setopt(m_curl, CURLOPT_VERBOSE, 1);
	res=curl_easy_setopt(m_curl,CURLOPT_NOSIGNAL,1);
	res = curl_easy_setopt(m_curl,CURLOPT_TIMEOUT,101);
	//res = curl_easy_setopt(m_curl,CURLOPT_CONNECTIONTIMEOUT,0);
	//res = curl_easy_setopt(m_curl,CURLOPT_HEADER, true); 
	//执行写入文件流操作
	res = curl_easy_setopt(m_curl,CURLOPT_WRITEFUNCTION,my_fwrite);
	res = curl_easy_setopt(m_curl,CURLOPT_WRITEDATA,fp);
//	res=curl_easy_setopt(m_curl,CURLOPT_NOSIGNAL,1);
//	res = curl_easy_setopt(m_curl, CURLOPT_TIMEOUT, 10l);
//	if(res == CURLE_OK)
//	{
//		printf("curl_easy_setopt is CURLE_OK\n");
//	}
	res = curl_easy_perform(m_curl);
//	printf("getimg  end\n");
	if (fp)
	{
		//关闭文件流
		fclose(fp);
	}
	if(res != CURLE_OK){
		return false;
	}
	return true;
}
const char* HttpWrap::Post(const char* pHttpUrl, const char* pData, size_t len)
{
	if(!m_curl)return false;
	CURLcode res;
	m_recvdata.clear();
	res=curl_easy_setopt(m_curl, CURLOPT_URL, pHttpUrl);
	curl_slist* slist=NULL;
	for(int i=0; i<(int)m_HeaderVect.size(); ++i){
		slist=curl_slist_append(slist,m_HeaderVect[i].c_str());
	}
	if(m_HeaderVect.size()>0){
		res=curl_easy_setopt(m_curl,CURLOPT_HTTPHEADER,slist);
	}
	res=curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, &m_recvdata);
	res=curl_easy_setopt(m_curl, CURLOPT_WRITEFUNCTION, recvdata);
	std::string postdata;
	if(pData){
		if((int)len==-1){
			res=curl_easy_setopt(m_curl, CURLOPT_POSTFIELDSIZE, strlen(pData));
		}
		else{
			res=curl_easy_setopt(m_curl, CURLOPT_POSTFIELDSIZE, len);
		}
		res=curl_easy_setopt(m_curl, CURLOPT_POSTFIELDS, pData);
	}
	res=curl_easy_perform(m_curl);
	if((int)m_HeaderVect.size()>0){
		curl_slist_free_all(slist);
	}
	if(res!=CURLE_OK)return NULL;
	return m_recvdata.c_str();
}

