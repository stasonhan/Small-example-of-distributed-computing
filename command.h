#ifndef COMMAND_H
#define COMMAND_H
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include "db_interface.h"

#ifndef OUT
#define OUT
#endif

enum operate{ADD,CUT};

typedef struct cmd_s cmd_t;
struct cmd_s
{
    char name[30];
    char bank[128];
    int id;
    char account[128];
    double money;
    char operate[128];   
    char msg[128];
};

typedef struct task_s task_t;
struct task_s
{
    task_t *next;
    cmd_t  command;    
};

typedef struct task_queue_s task_queue_t;
struct task_queue_s
{
    task_t *head;
    task_t *tail;
    int num;
    pthread_cond_t cond;
    pthread_mutex_t mutex;
}; 

int parse_request(char* quest);
void  construct_command_to_sql(OUT char* sql);
/*初始化队列头*/
task_queue_t* init_task(task_queue_t* t);

int insert_queue(task_queue_t* h,task_t *q);
task_t* get_queue(task_queue_t* t);


#endif
