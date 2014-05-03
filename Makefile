LIB = -lpthread -lpq -lm
CCFLAG= -g
all:sql_server
sql_server:db_interface.o sql_server.o thread.o command.o sock.o cJSON.o
	gcc -o  sql_server $(CCFLAG) db_interface.o sql_server.o thread.o command.o cJSON.o sock.o $(LIB)

sql_server.o:sql_server.c 
	cc -c $(CCFLAG) sql_server.c     
command.o:command.c command.h
	cc -c $(CCFLAG) command.c 
db_interface.o: db_interface.c db_interface.h
	cc -c $(CCFLAG) db_interface.c 
sock.o:sock.c sock.h
	cc -c $(CCFLAG) sock.c 
thread.o:thread.c thread.h sock.h
	cc -c $(CCFLAG) thread.c sock.c
cJSON.o:cJSON.c cJSON.h
	cc -c $(CCFLAG) cJSON.c

clean:
	rm *.o sql_server test
    
