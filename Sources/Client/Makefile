 .PHONY: all, install, clean  
   
 TARGET:=client
   
 -include ./Client_message/client_message.mk  
 CSRCS+= client.c  
   
 OBJSDIR=./build
 OBJS+= $(patsubst %.c, $(OBJSDIR)/%.o, $(CSRCS))

 INCDIR+= -I./Client_message
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