
GCC=g++
GCC_OPT=-Wall -Wunused -g -DOS_LINUX
INC_PATH=-I/usr/local/mongodb -I/usr/local/boost1461/include 
LIB_PATH= -L/usr/local/lib  -L/usr/local/boost1461/lib -L/usr/local/mongodb/lib64 -L/usr/lib64  -L.	
#ADDITIONAL_LIBRARIES = -lpq -levent -lcrypt  -lpthread -lboost_regex  -lboost_thread -lboost_filesystem -lboost_program_options		
ADDITIONAL_LIBRARIES = -lpq -lcurl -lcrypt   -lpthread  -lboost_regex  -lmongoclient -lboost_thread -lmysqlclient -lboost_filesystem -lboost_program_options -lm -lboost_system
PROGRAM_NAMES = Htmltest
LOGDIR = ../log
DBMDIR = ../DBmanager
MONGODIR = ../mongo
HTMLDIR = ../html
HTTPDIR = ../HttpWrap
COMDIR = ../common
QINIUDIR = ../qiniu
B64DIR = ../b64
CJSONDIR = ../cJSON
all:$(PROGRAM_NAMES)		


#PROGRAM_SOURCES = $(wildcard $(LOGDIR)/*.cpp $(DBMDIR)/*.cpp   $(MONGODIR)/*.cpp\
#										 *.cxx *.cpp)

PROGRAM_SOURCES = $(wildcard $(HTMLDIR)/*.cc $(COMDIR)/*.cpp $(QINIUDIR)/*.c $(CJSONDIR)/*.c $(B64DIR)/*.c $(LOGDIR)/*.cpp $(DBMDIR)/*.cpp $(HTTPDIR)/*.cpp $(MONGODIR)/*.cpp  *.cxx *.cpp)

PROGRAM_NOSOURCES=$(notdir $(PROGRAM_SOURCES))
PROGRAM_OBJ = $(patsubst %.cpp,%.o,$(patsubst %.cc,%.o,$(patsubst %.c,%.o,$(PROGRAM_NOSOURCES))))
#PROGRAM_OBJ = $(patsubst %.cc,%.o,$(PROGRAM_NOSOURCES))
Htmltest:	$(PROGRAM_OBJ) 
	$(GCC) $(GCC_OPT) -o $@ $^ $(INC_PATH) $(LIB_PATH) $(ADDITIONAL_LIBRARIES)
#$(PROGRAM_OBJ):	$(PROGRAM_SOURCES)
#	$(GCC) $^ $(INC_PATH) -c
#	@echo $(PROGRAM_SOURCES)
#	@echo $(PROGRAM_OBJ)
%.o:$(HTMLDIR)/%.cc
	$(GCC) $(GCC_OPT) $^ $(INC_PATH) -c -o $@
%.o:	%.cpp	
	$(GCC) $(GCC_OPT) $^ $(INC_PATH) -c -o $@
%.o:	%.cc
	$(GCC) $(GCC_OPT) $^ $(INC_PATH) -c -o $@
%.o:$(HTTPDIR)/%.cpp
	$(GCC) $(GCC_OPT) $^ $(INC_PATH) -c -o $@	
%.o:$(COMDIR)/%.cpp	
	$(GCC) $(GCC_OPT) $^ $(INC_PATH) -c -o $@
%.o:$(B64DIR)/%.c	
	gcc $(GCC_OPT)  $^ $(INC_PATH) -c -O $@
%.o:$(CJSONDIR)/%.c	
	gcc $(GCC_OPT)  $^ $(INC_PATH) -c -O $@
%.o:$(QINIUDIR)/%.c	
	gcc $(GCC_OPT)  $^ $(INC_PATH) -c -O $@
%.o:$(LOGDIR)/%.cpp	
	$(GCC) $(GCC_OPT) $^ $(INC_PATH) -c -o $@
%.o:$(MONGODIR)/%.cpp
	$(GCC) $(GCC_OPT) $^ $(INC_PATH) -c -o $@
%.o:$(DBMDIR)/%.cpp
	$(GCC) $(GCC_OPT) $^ $(INC_PATH) -c -o $@
clean :
		rm -rf Htmltest
		rm -rf *.o	
