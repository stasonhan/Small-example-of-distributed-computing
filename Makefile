LIB = -lpthread -lpq
CCFLAG= -g
all:sql_server
sql_server:db_interface.o sql_server.o thread.o command.o sock.o
	gcc -o  sql_server $(CCFLAG) db_interface.o sql_server.o thread.o command.o sock.o $(LIB)
sql_server.o:sql_server.c 
	cc -c sql_server.c     
command.o:command.c command.h
	cc -c command.c 
db_interface.o: db_interface.c db_interface.h
	cc -c db_interface.c 
sock.o:sock.c sock.h
	cc -c sock.c 
thread.o:thread.c thread.h sock.h
	cc -c thread.c sock.c

clean:
	rm *.o sql_server
    
