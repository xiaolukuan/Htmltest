
GCC=g++
GCC_OPT=-Wall -Wunused -g -DOS_LINUX
INC_PATH=-I/usr/local/boost1461/include 
LIB_PATH= -L/usr/local/lib -L/usr/local/boost1461/lib -L/usr/lib64 -L.	
ADDITIONAL_LIBRARIES =  -lpq -lcurl -lcrypt -lpthread -lboost_system -lboost_regex  -lboost_thread -lboost_filesystem -lboost_program_options 

PROGRAM_NAMES = IMSLnewsspider
LOGDIR = ../log
HTTPSRCDIR = ../HttpWrap
DBMDIR = ../DBmanager
COMDIR = ../common
#MONGODIR = ../mongo
HTML = ../html
UTIL = ./util
all:$(PROGRAM_NAMES)		


PROGRAM_SOURCES = $(wildcard $(LOGDIR)/*.cpp $(UTIL)/*.cpp $(DBMDIR)/*.cpp $(COMDIR)/*.cpp $(HTTPSRCDIR)/*.cpp  \
										$(HTML)/*cc $(HTML)/*tcc *.cxx *.cpp)
PROGRAM_NOSOURCES=$(notdir $(PROGRAM_SOURCES))
PROGRAM_OBJ = $(patsubst %.cxx,%.o,$(patsubst %.cpp,%.o,$(patsubst %.cc,%.o,$(patsubst %.tcc,%.o,$(PROGRAM_NOSOURCES)))))
IMSLnewsspider:	$(PROGRAM_OBJ)
	$(GCC) $(GCC_OPT) -o $@ $^ $(INC_PATH) $(LIB_PATH) $(ADDITIONAL_LIBRARIES)
#$(PROGRAM_OBJ):	$(PROGRAM_SOURCES)
#	$(GCC) $^ $(INC_PATH) -c
#	@echo $(PROGRAM_SOURCES)
#	@echo $(PROGRAM_OBJ)
  
%.o:$(LOGDIR)/%.cpp
	$(GCC) $(GCC_OPT) $^ $(INC_PATH) -c -o $@
	
%.o:$(DBMDIR)/%.cpp
	$(GCC) $(GCC_OPT) $^ $(INC_PATH) -c -o $@
	
%.o:$(COMDIR)/%.cpp
	$(GCC) $(GCC_OPT) $^ $(INC_PATH) -c -o $@

%.o:$(HTTPSRCDIR)/%.cpp
	$(GCC) $(GCC_OPT) $^ $(INC_PATH) -c -o $@
	
#%.o:$(MONGODIR)/%.cpp
#	$(GCC) $(GCC_OPT) $^ $(INC_PATH) -c -o $@
	
%.o:$(HTML)/%.cc	
	$(GCC) $(GCC_OPT) $^ -c -o $@

%.o:	%.cpp	
	$(GCC) $(GCC_OPT) $^ $(INC_PATH) -c -o $@

%.o:	%.cxx
	$(GCC) $(GCC_OPT) $^ $(INC_PATH) -c -o $@

clean :
		rm -rf *.o	
