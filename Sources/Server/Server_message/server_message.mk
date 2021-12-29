CSRCS+= ./Server_message/server_handle_message.c
CSRCS+= ./Server_message/state_0_login.c
CSRCS+= ./Server_message/state_1_createroom.c
CSRCS+= ./Server_message/state_1_joinroom.c


HDRS+= $(wildcard ./Server_message/*.h)