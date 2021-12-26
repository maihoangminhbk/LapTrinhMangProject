 .PHONY: all, install, clean  
   
 TARGET:=server
   
 -include ./Server_message/server_message.mk  
 -include ./Global_variable/global_variable.mk  

 CSRCS+= server.c
   
 OBJSDIR=./build
 OBJS+= $(patsubst %.c, $(OBJSDIR)/%.o, $(CSRCS))

 INCDIR+= -I./Server_message
 INCDIR+= -I./Global_variable

 CFLAGS += -DDEBUG -Wall -g
 LDFLAGS += -L./lib -lm

 CC:= gcc

 all: ${TARGET}

 ${TARGET} : $(OBJS)  
	@echo " [LINK] $@"
	@mkdir -p $(shell dirname $@)  
	@$(CC) $(OBJS) -o $@ $(LDFLAGS)  
        
 $(OBJSDIR)/%.o: %.c $(HDRS)  
	@echo " [CC]  $@"  
	@mkdir -p $(shell dirname $@)  
	@$(CC) -c $< -o $@ $(CFLAGS) ${INCDIR}
 clean:  
	rm -rf ${OBJSDIR}/*.o  
	rm -rf ${TARGET}