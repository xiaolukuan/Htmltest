#include "IMSLCommon.h"
#include "../log/CIMSLLog.h"
int iconv_string(const char *from, const char *to,
						   const char *src, size_t len,
						   ::std::string& result,
						   int c , size_t buf_size )
{
	iconv_t cd;

	char *pinbuf = const_cast< char* >(src);
	size_t inbytesleft = len;
	char *poutbuf = NULL;
	size_t outbytesleft = buf_size;

	char *dst = NULL;
	size_t retbytes = 0;
	int done = 0;
	int errno_save = 0;

	if ((iconv_t)-1 == (cd = iconv_open(to, from))) 
	{
		return -1;
	}

	dst = new char[buf_size];

	while(inbytesleft > 0 && !done) 
	{
		poutbuf = dst;
		outbytesleft = buf_size;
		retbytes = iconv(cd, &pinbuf, &inbytesleft, &poutbuf, &outbytesleft);
		errno_save = errno;

		if (dst != poutbuf) 
		{// we have something to write
			result.append(dst, poutbuf-dst);
		}

		if (retbytes != (size_t)-1) 
		{
			poutbuf = dst;
			outbytesleft = buf_size;
			(void)iconv(cd, NULL, NULL, &poutbuf, &outbytesleft);

			if (dst != poutbuf) 
			{// we have something to write
				result.append(dst, poutbuf-dst);
			}

			errno_save = 0;
			break;
		}


		switch(errno_save) 
		{
		case E2BIG:
			//  printf("E2BIG\n");
			break;
		case EILSEQ:
			// printf("EILSEQ\n");
			if (c) 
			{
				errno_save = 0;
				inbytesleft = len-(pinbuf-src); // forward one illegal byte
				inbytesleft--;
				pinbuf++;
				break;
			}

			done = 1;
			break;
		case EINVAL:
			// printf("EINVAL\n");
			done = 1;
			break;
		default:
			done = 1;
			break;
		}

	}

	delete[] dst;
	iconv_close(cd);

	errno = errno_save;
	return (errno_save) ? -1 : 0;
}
bool IsUTF8(const void* pBuffer, long size)  
{  
	bool IsUTF8 = true;  
	unsigned char* start = (unsigned char*)pBuffer;  
	unsigned char* end = (unsigned char*)pBuffer + size;  
	while (start < end)  
	{  
		if (*start < 0x80) // (10000000): 值小于0x80的为ASCII字符  
		{  
			start++;  
		}  
		else if (*start < (0xC0)) // (11000000): 值介于0x80与0xC0之间的为无效UTF-8字符  
		{  
			IsUTF8 = false;  
			break;  
		} 
		else if (*start < (0xE0)) // (11100000): 此范围内为2字节UTF-8字符  
		{  
			if (start >= end - 1)   
				break;  
			if ((start[1] & (0xC0)) != 0x80)  
			{  
				IsUTF8 = false;  
				break;  
			}  
			start += 2;  
		}   
		else if (*start < (0xF0)) // (11110000): 此范围内为3字节UTF-8字符  
		{ 
			if (start >= end - 2)   
				break;  
			if ((start[1] & (0xC0)) != 0x80 || (start[2] & (0xC0)) != 0x80)  
			{  
				IsUTF8 = false;  
				break;  
			}  
			start += 3;  
		}   
		else  
		{  
			IsUTF8 = false;  
			break;  
		}  
	}  
	return IsUTF8;  
}
void strltrim(string &s)
{
	if(s == "") return;
	const char* start = s.data();
	const char* end = start + s.length();
	string strtmp = "";
	while (start < end)  
	{  
		if ((unsigned char)*start < 0x80) // (10000000): 值小于0x80的为ASCII字符  
		{  
			if(*start == ' '|| *start == '\f' || *start == '\n' || *start == '\r'|| *start == '\v'|| *start == '\t')
			{
				strtmp = start+1;
			}
			else
			{
				strtmp = start;
				break;
			}
			start++;
		}
		else 
		{
			string str(start,start+2);
			if (str == "　")
			{
				strtmp = start+2;
			} 
			else
			{
				strtmp = start;
				break;
			}
			start += 2;
		}
	}  
	s = strtmp;  
}
void rtrim( char *str )
{
	char *copied, *tail = NULL;

	if ( str == NULL )
		return;

	for( copied = str; *str; str++ )
	{
		if ( *str != ' ' && *str != '\t' )
		{
			*copied++ = *str;
			tail = copied;
		}
		else
		{
			if ( tail )
				*copied++ = *str;
		}
	}

	if ( tail )
		*tail = 0;
	else
		*copied = 0;

	return;
}
unsigned char toHex(const unsigned char &x)
{
	return x > 9 ? x + 55: x + 48;
}
char *urldecode(char *in)
{
	char  *out;
	char   temp[32];
	char  *p, *q;
	int             value;

	out = (char *) malloc(strlen(in) + 1);

	p = in, q = out;
	while (*p != 0) 
	{
		if (*p == '+') 
		{
			*q++ = ' ';
		} else if (*p == '%')
		{
			temp[0] = *(p + 1);
			temp[1] = *(p + 2);
			temp[2] = 0;
			sscanf(temp, "%x", &value);

			*q++ = 0xff & value;

			p += 2;
		} else {
			*q++ = *p;
		}

		p++;
	}
	*q = '\0';

	return (char *) out;
}
string urlencode( string sIn )
{
	string sOut;
	for( int ix = 0; ix < (int)sIn.size(); ix++ )
	{
		unsigned char buf[4];
		memset( buf, 0, 4 );
		if( isalnum( (unsigned char)sIn[ix] ) )
		{
			buf[0] = sIn[ix];
		}
		else if ( isspace( (unsigned char)sIn[ix] ) )
		{
			buf[0] = '+';
		}
		else
		{
			buf[0] = '%';
			buf[1] = toHex( (unsigned char)sIn[ix] >> 4 );
			buf[2] = toHex( (unsigned char)sIn[ix] % 16);
		}
		sOut += (char *)buf;
	}
	return sOut;
}
void utfstrltrim(string &s)
{
	if(s == "") return;
	const char* start = s.data();
	const char* end = start + s.length();
	string strtmp = "";
	while (start < end)  
	{  
		if ((unsigned char)*start < 0x80) // (10000000): 值小于0x80的为ASCII字符  
		{  
			if(*start == ' '|| *start == '\f' || *start == '\n' || *start == '\r'|| *start == '\v'|| *start == '\t')
			{
				start++;
			}
			else
			{
				strtmp = start;
				break;
			}
		}
		else 
		{
			string str(start,start+3);
			if (str == "\xe3\x80\x80")
			{
				start += 3;
			} 
			else
			{
				strtmp = start;
				break;
			}
		}
	}  
	s = strtmp;  
}
void utfstrtrim(string &s)
{
	if(s == "") return;
	const char* start = s.data();
	const char* end = start + s.length();
	string strtmp = "";
	while (start < end)  
	{  
		if ((unsigned char)*start < 0x80) // (10000000): 值小于0x80的为ASCII字符  
		{  
			if(*start == ' '|| *start == '\f' || *start == '\n' || *start == '\r'|| *start == '\v'|| *start == '\t')
			{
				start++;
			}
			else
			{
				strtmp += *start;
				start++;
			}
		}
		else 
		{
			string str(start,start+3);
			if (str == "\xe3\x80\x80")
			{
				start += 3;
			} 
			else
			{
				strtmp += str;
				start += 3;
			}
		}
	}  
	s = strtmp;  
}
void utfstrrtrim(string &s)
{
	if(s == "") return;
	const char* start = s.data();
	const char* end = start + s.length()-1;
	string strtmp = "";
	while (start != end)  
	{  
		if ((unsigned char)*end < 0x80) // (10000000): 值小于0x80的为ASCII字符  
		{  
			if(*end == ' '|| *end == '\f' || *end == '\n' || *end == '\r'|| *end == '\v'|| *end == '\t')
			{
				end--;
			}
			else
			{
				strtmp = string(start,end+1);
				break;
			}
		}
		else 
		{
			string str(end-2,end+1);
			if (str == "\xe3\x80\x80")
			{
				end -= 3;
			} 
			else
			{
				strtmp = string(start,end+1);
				break;
			}
		}
	}  
	s = strtmp;  
}
string ReadFile(const char* filename)
{
	std::istream * is;
	std::ifstream fin;
	if(!filename) 
	{
		is = & std::cin;
	} else 
	{
		fin.open(filename);
		if(!fin.is_open())
		{
			theLog.WriteLog(LOG_LEVEL_ERROR,"cann't open the configfile: %s !",filename);
			return "";
		}
		else
			is = & fin;
	}
	std::string text = "";
	while(!is->eof()) 
	{
		std::string line;
		getline(*is, line);
		text += line + "\n";
	}
	if(filename)
	{
		fin.close();
	}
	return text;
}
bool WriteFile(const char *filename,const char *content,int length,bool bNewfile /* = true*/)
{
	if (bNewfile)
	{
		std::ofstream fout;
		fout.open(filename);
		fout.seekp(std::ios::beg);
		fout.write(content,length);
		fout.close();
	} 
	else
	{
		std::ofstream fout;
		fout.open(filename,ios_base::app|ios_base::out);
		fout.seekp(std::ios::beg);
		fout.write(content,length);
		fout.close();
	}
	return true;
}
int GetFilenSize(const char* filename)
{
	std::ifstream InFile;
	InFile.open(filename,std::ios::in);
	if(!InFile)
	{
		std::cerr<<"cannot open file"<<filename<<std::endl;
		return 0;
	}
	std::streampos EndPos;
	InFile.seekg(0, std::ios::end);
	EndPos = InFile.tellg();
	int nflen=(int)EndPos;
	InFile.close();
	return nflen;
}
bool Makedir(const char* prootdir,const char* tempdir)
{
	char tempbuff[256];
    char rootdir[512];
	if ((int)strlen(prootdir) == 0)
	{
		strcpy(rootdir,"./");
		strcpy(tempbuff,"./");
		strcat(tempbuff,tempdir);
	} 
	else
	{
		if(access(prootdir,F_OK) != 0)
		{
			//cout<<"root dir: "<<prootdir<<"can not find!"<<endl;
			theLog.WriteLog(LOG_LEVEL_ERROR,"root dir: %s can not find !",prootdir);
			return false;
		}
		strcpy(rootdir,prootdir);
		strcpy(tempbuff,prootdir);
		strcat(tempbuff,tempdir);
	}
	if(access(tempbuff,F_OK) == 0)
		return true;
	char* strtemp = (char*)strstr(tempdir,"/");
	if(strtemp)
	{
		strncat(rootdir,tempdir,strtemp-tempdir);
		if (access(rootdir,F_OK) == 0)
		{
			strncat(rootdir,"/",1);
			Makedir(rootdir,strtemp+1);

		}
		else
		{
			if(mkdir(rootdir, 0755)!=0) 
				return false;
			strncat(rootdir,"/",1);
			Makedir(rootdir,strtemp+1);
		}
	}
	else
	{
		strncat(rootdir,tempdir,strlen(tempdir));
		if (access(rootdir,F_OK) == 0)
		{
			//printf("dir is exist\n");
		}
		else
		{
			//printf("mkdir %s\n",rootdir);
			if(mkdir(rootdir, 0755)!=0) 
				return false;
		}
	}

	return true;
}
char *GetLocalTime()
{
	struct tm  *timenow;
	time_t now;
	char *strtemptime = new char[64];
	time(&now);
	timenow = localtime(&now);
	strftime(strtemptime,64,"%Y-%m-%d %H:%M:%S",timenow);
	return strtemptime;
}
bool ReadConfigFile(const char * filename,map<string,string> &m_config)
{
	std::istream * is;
	std::ifstream fin;
	if(!filename) 
	{
		is = & std::cin;
	} 
	else 
	{
		fin.open(filename);
		if(!fin.is_open())
		{
			theLog.WriteLog(LOG_LEVEL_ERROR,"cann't open the configfile: %s !",filename);
			return false;
		}
		else
			is = & fin;
	}
	while(!is->eof()) 
	{
		std::string line;
		getline(*is, line);
		if ((int)line.find("#") == 0)
		{
			continue;
		} 
		else
		{
			boost::regex reg1("^\\[(.*?)\\]\\s*=\\s*(.*?)\\s*$"); 
			boost::cmatch what;
			if(boost::regex_match(line.c_str(), what, reg1))
			{
				//for(int i=1;i<(int)what.size();i++)
				//	std::cout<<"str :"<<what[i].str()<<std::endl;
				m_config.insert(pair<string,string>(what[1].str(),what[2].str()));
			}
		}
	}
	if(filename)
	{  
		fin.close();
	}
	return (int)m_config.size()>0?true:false;
}


bool ReadSpiderRulerConfigFile(const char * filename,map<string,struct SpiderRuler> &m_config)
{
	std::istream * is;
	std::ifstream fin;
	if(!filename) 
	{
		is = & std::cin;
	} 
	else 
	{
		fin.open(filename);
		if(!fin.is_open())
		{
			theLog.WriteLog(LOG_LEVEL_ERROR,"cann't open the configfile: %s !",filename);
			return false;
		}
		else
			is = & fin;
	}
	while(!is->eof()) 
	{
		std::string line;
		getline(*is, line);
		if ((int)line.find("#") == 0)
		{
			continue;
		} 
		else
		{
			//[webname:csdn][id:764][textcontenttag:con news_content][summarytag:summary][comment:]
			boost::regex reg("^\\[webname:(.*?)\\]\\s*\\[hasruler:(.*?)\\]\\s*\\[id:(.*?)\\]\\s*\\[textcontenttagname:(.*?)\\]\\s*\\[textcontenttag:(.*?)\\]\\s*\\[summaryisinbody:(.*?)\\]\\s*\\[summarytagname:(.*?)\\]\\s*\\[summarytag:(.*?)\\]\\s*\\[comment:(.*?)\\]\\s*$"); 
			//boost::regex reg1("^\\[(.*?)\\]\\s*=\\s*(.*?)\\s*$"); 
			boost::cmatch what;
			//printf("line=%s\n",line.c_str());
			if(boost::regex_match(line.c_str(), what, reg))
			{
				struct SpiderRuler temp;
				temp.webname			= what[1].str();
				temp.hasruler			= what[2].str();
				temp.id					= what[3].str();
				temp.textcontenttagname = what[4].str();
				temp.textcontenttag		= what[5].str();
				temp.summaryisinbody	= what[6].str();
				temp.summarytagname		= what[7].str();
				temp.summarytag			= what[8].str();
				temp.comment			= what[9].str();
				/*std::cout<<"webname:"<<temp.webname<<"  "
					<<"hasruler:"<<temp.hasruler<<"  "
						<<"id:"<<temp.id<<"  "
							<<"textcontenttagname:"<<temp.textcontenttagname<<"  "
								<<"textcontenttag:"<<temp.textcontenttag<<"  "
									<<"summaryisinbody:"<<temp.summaryisinbody<<"  "
										<<"summarytagname:"<<temp.summarytagname<<"  "
											<<"summarytag:"<<temp.summarytag<<"  "
												<<"comment:"<<temp.comment<<std::endl;
			*/	//for(int i=1;i<(int)what.size();i++)
				//	std::cout<<"str :"<<what[i].str()<<std::endl;
				m_config.insert(pair<string,struct SpiderRuler>(what[1].str(),temp));
			}
		}
	}
	if(filename)
	{  
		fin.close();
	}
	return (int)m_config.size()>0?true:false;
}

string GetFullPath()
{
	char buf[ 1024 ];
	int  count;
	string str;
	count = readlink( "/proc/self/exe", buf, 1024 );
	if( count < 0 || count >= 1024 )
	{
		theLog.WriteLog(LOG_LEVEL_ERROR,"can't get the proc fullpath !");
		return "";
	}
	buf[ count ] = '\0';
	str = buf;
	str = str.substr(0,str.rfind("/"));
	return str;
}
string my_itoa(int i)
{
	char ch[24];
	sprintf(ch,"%d",i);
	return ch;
}
int usSleep(unsigned int nusecs)
{
	struct timeval tval;
	int ret = 0;
	tval.tv_sec = nusecs / 1000000;
	tval.tv_usec = nusecs % 1000000;
	ret = select(0, NULL, NULL, NULL, &tval);
	return ret;
}

int my_system(const char *cmdstring)
{
	int status;
	status  = system(cmdstring);
	if (-1 == status)	
	{  
		return -1;
	}  
	else  
	{  	
		if (WIFEXITED(status))  
		{  
			if (0 == WEXITSTATUS(status))  
			{  
				return 0;
			}  
			else  
			{  
				return -2;
			}  
		}  
		else  
		{  
			return 0;
			printf("exit status = [%d]\n", WEXITSTATUS(status));  
		}  
	}  
}
 /**   
  * 增强的system函数，能够返回system调用的输出   
  *   
  * @param[in] cmdstring 调用外部程序或脚本的命令串   
  * @param[out] buf 返回外部命令的结果的缓冲区   
  * @param[in] len 缓冲区buf的长度   
  *   
  * @return 0: 成功; -1: 失败    
  */  
int mysystem(char* cmdstring, char* buf, int len)  
  {
        int   fd[2];      
        pid_t pid;      
        int   n, count;   
            
        memset(buf, 0, len);      
        if (pipe(fd) < 0)    
		{      
			printf("pipe\n");
        	return -1;      
		}
        if ((pid = fork()) < 0)     
		{     
			printf("fork\n");
        	return -1;
		}
        else if (pid > 0)     /* parent process */      
        {          
        	close(fd[1]);     /* close write end */          
        	count = 0;          
        	while ((n = read(fd[0], buf + count, len)) > 0 && count > len)              
        		count += n;          
			close(fd[0]);          
        	if (waitpid(pid, NULL, 0) > 0)    
			{
	//			printf("waitpid\n");
    //    		return -1;      
			}
		}      
        else                  /* child process */      
        {
        	close(fd[0]);     /* close read end */          
        	if (fd[1] != STDOUT_FILENO)          
        	{              
        		if (dup2(fd[1], STDOUT_FILENO) != STDOUT_FILENO)              
        		{       
					printf("dup2\n");
        			return -1;              
        		}
        		close(fd[1]);
			}
		
			if (execl("/bin/sh", "sh", "-c", cmdstring, (char*)0) == -1)
			{
				printf("execl\n");
				return -1;
			}
		}
	//printf("11111\n");
	return 0;
  }  
  /**   
  *主要是执行ps命令看程序是否在运行
  *0 表示正常运行，-1表示不再运行，-2表示执行脚本失败
  */  
 int my_pscmd(char *proname,int *pid)
 {
 	string cmdstr;
	char buf[2000];
 	cmdstr.clear();
	cmdstr += "ps -ef |grep ";
	cmdstr += proname;
	cmdstr += " |grep -v \"grep\"";
	//printf("noheartbeat: ps -ef : %s\n",cmdstr.c_str());
	memset(buf,0,2000);
	if(0 == mysystem((char*)cmdstr.c_str(),buf,2000))
	{
	//	printf("noheartbeat: pidname:%s success!! buf=%s.\n",proname,buf);
		if(0 == strlen(buf))
		{
			printf("noheartbeat:the pidname:%s is not exist!!\n",proname);
			return -1;
		}
		else
		{
			char *temp = strchr(buf,' ');
			while(' ' == *temp)
			{
				temp++;
			}
			*pid = atoi(temp);
			//printf("pid = %d\n",*pid);
			printf("noheartbeat: the pidname:%s is exist!!\n",proname);
			return 0;
		}
						
		}
		else
		{
			printf("noheartbeat:ps -ef  pidname:%s failed!!\n",proname);
			return -2;
		}
 }

  int my_killcmd(int pid)
  {
  		if(pid <= 0)
		{
			return -1;
		}
	  	string cmdstr;
		int status = -1;
		cmdstr.clear();
		cmdstr += "kill -9 ";
		cmdstr  +=  my_itoa(pid);
		status  = my_system(cmdstr.c_str());
		if (-1 == status)	
		{  
			printf("system error!\n");
			return -1;
		}  
		else if( -2 == status )
		{  	
			printf("run shell script fail!\n");
			return -2;
		}  
		else  
		{  
			printf("kill pid =%d success !\n",pid);
			return 0;
		} 
  }
  int my_startcmd(char *path)
  {
  	string cmdstr;
	int status = -1;
  	cmdstr	+=  "sh  ";
	cmdstr 	+= 	path;
	cmdstr	+=	"start.sh";
	printf("cmdstr =%s\n",cmdstr.c_str());
	status  = my_system(cmdstr.c_str());
	if (-2 == status)	
	{  
		printf("system error!\n");
		return -2;
	}  
	else if( -1 == status )
	{  		
		printf("run shell script fail!\n");
		return -1;
	}  
	else  
	{  
		printf("reboot proc name:%s success !\n",path);
		return 0;
	}  
  }
  int my_killallcmd(char *proname)
  {
  	string cmdstr;
	int status = -1;
  	cmdstr += "killall ";
	cmdstr += proname;
	printf("killall cmdstr =%s\n",cmdstr.c_str());
	status  = my_system(cmdstr.c_str());
	if (-2 == status)	
	{  
		printf("killall procname:%s system error!\n",proname);
		return -2;
	}  
	else if( -1 == status )
	{  		
		printf("killall procname:%s fail!\n",proname);
		return -1;
	}  
	else  
	{  
		printf("killall proc name:%s success !\n",proname);
		return 0;
	}  
  }

