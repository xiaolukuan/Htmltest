#ifndef HTTP_WRAP_H
#define HTTP_WRAP_H
#include <curl/curl.h>
#include <curl/types.h>
#include <curl/easy.h>
#include <string>
#include <vector>
#include <stdio.h>
#define VEBGET 1
#define VEBPOST 2
class HttpWrap
{
public:
	HttpWrap();
	~HttpWrap();
	void SetHeader(const char* pHeader);
	std::string Get(const char* pHttpUrl);
	bool GetImg(const char *Imgurl,const char *ImgSavepath,const char *ImgDir);
	const char* Post(const char* pHttpUrl, const char* pData, size_t len=-1);
private:
	CURL* m_curl;
	std::string m_recvdata;
	std::vector<std::string>m_HeaderVect;
};
#endif

