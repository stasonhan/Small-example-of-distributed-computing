#include <stdlib.h>
#include <string.h>
#include <error.h>
#include "db_interface.h"

void PQresultPrint(PGresult *res)
{
    int nFields = PQnfields(res);
    int nTuples = PQntuples(res);
    int i, j;
    for (i=0; i<nTuples; i++)
    {
        for (j=0; j<nFields; j++)
        {
            printf("%s ", PQgetvalue(res, i, j));
        }
        printf("\n");
    }
    printf("\n");
}

char *ExecuteQuery_one(char *query)
{
    PGresult *res = ExecuteQuery("127.0.0.1", 5432, "mydb", query);
    int nFields = PQnfields(res);
    int nTuples = PQntuples(res);
    int j;
    int n = 0;
    char *buf = (char*)malloc(sizeof(char)*1024);
    char *ptr = NULL;
    if(nTuples != 1)
        printf("nTuples not 1");
    for(j = 0;j < nFields;j++)
    {
        ptr = PQgetvalue(res,0,j);
        if(ptr == NULL)
            continue;
        sprintf(buf + n,"%s\r\n",ptr);
        n = n + strlen(ptr) + 2;
    }
    return buf;
}
/*PGconn *PQconnect_to_server_by_params(char* host,int port,char* dbname,char* passwd)
{
    char **keywords ={"host","port","dbname","user","password",NULL};
    char **values = {host,}
}*/
PGconn *PQConnect_to_server(char *host, int port, char *dbname)
{
    int ressafe =  PQisthreadsafe();
    if(ressafe == 1)
    {
        perror("library not thread safe!");
        exit(1);
    }
    PGconn *conn;
    PGresult *res;
    char str[128];
    sprintf(str, "host=%s port=%d dbname=%s password=\"\"", host, port, dbname);
    conn = PQconnectdb(str);
    if(PQstatus(conn) == CONNECTION_BAD)
    {
        fprintf(stderr,"数据库连接失败！ host: %s/n", host);
        fprintf(stderr,"%s",PQerrorMessage(conn));
    }
    return conn;
}
PGresult *ExecuteQuery(char *host, int port, char *dbname, char *query)
{
 
    int ressafe =  PQisthreadsafe();
    if(ressafe == 1)
        printf("the library is safe!\n");
    PGconn *conn;
    PGresult *res;
    char str[128];
    sprintf(str, "host=%s port=%d dbname=%s", host, port, dbname);
    conn = PQconnectdb(str);
    if(PQstatus(conn) == CONNECTION_BAD)
    {
        fprintf(stderr,"数据库连接失败！ host: %s/n", host);
        fprintf(stderr,"%s",PQerrorMessage(conn));
    }

    res = PQexec(conn, query);
    if (PQresultStatus(res) == PGRES_FATAL_ERROR)
    {
        fprintf(stderr, "%s", PQerrorMessage(conn));
    }

    if (PQresultStatus(res) == PGRES_COMMAND_OK)
    {
        printf("%s/n", PQcmdTuples(res));
    }

    if (PQresultStatus(res) == PGRES_TUPLES_OK)
    {

    }
    PQfinish(conn);
    return res;
}

int operate_db(const char* dd)
{
    char *sql = "SELECT name FROM users where id=1;";
    char *user = "insert into users (name,account,money) values(\'hanhuade\',\'6212345678905677\',4567);";
    //if(sql == NULL)
    //    return -1;
    PGresult *res = ExecuteQuery("127.0.0.1", 5432, "sash", sql);
    PQresultPrint(res);
    PQclear(res);
    return 0;
}
/*int main()
{
    char *p = NULL;
    char *sql = "SELECT * FROM users where id=1;";
    p = ExecuteQuery_one(sql);
    if(p == NULL)
        exit(1);
    printf("p=%s\n",p);
}*/
