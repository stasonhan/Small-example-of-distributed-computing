#include <stdio.h>
#include "postgresql/libpq-fe.h"


void PQresultPrint(PGresult *res);

PGresult *ExecuteQuery(char *host, int port, char *dbname, char *query);

char *ExecuteQuery_one(char *query);

PGconn *PQConnect_to_server(char *host, int port, char *dbname);

int operate_db(const char* dd);
