
#include "command.h"

task_queue_t task;
/*
*function: parse the quest from the server into cmd_s
@para:
* quest:
* name@acout@cut/add@money
*/
int parse_request(char* quest)
{
    if(quest != NULL)
        printf("quest:%s\n",quest);
    task_t *new_task = (task_t*)malloc(sizeof(task_t));
    if(new_task == NULL)
    {
        perror("malloc");
        return -1;
    }
    char* pos = strstr(quest,"@");
    size_t len = pos - quest;
    strncpy(new_task->command.name,quest,len);
    printf("command.name = %s\n",new_task->command.name);
    char * scd_pos = strstr(pos+1,"@");
    len = scd_pos - pos - 1;
    strncpy(new_task->command.account,pos + 1,len);
    printf("command.account = %s\n",new_task->command.account);
    pos = strstr(scd_pos + 1,"a");
    len = pos - scd_pos - 1;
    if(strncmp(scd_pos + 1,"cut",3) == 0){
        new_task->command.op = CUT;
    }
    else
    {
        new_task->command.op = ADD;
    }
    printf("operate = %d\n",new_task->command.op);
    new_task->command.money = atoi(pos + 1);
    printf("command.money = %f\n",new_task->command.money);
    return 0;
}

task_queue_t* init_task(task_queue_t* t)
{
    t->head = t->tail = NULL;
    t->num   = 0;
    pthread_mutex_init(&t->mutex,NULL);
    pthread_cond_init(&t->cond,NULL);

}

int insert_queue(task_queue_t* t,task_t* q)
{
    if(pthread_mutex_lock(&t->mutex))
    {
        perror("pthread_mutex_lock in init_queue");
        return -1;
    }
    if(t == NULL || q == NULL)
    {
        perror("");
        return -1;
        pthread_mutex_unlock(&t->mutex);
    }
    if(t->tail == NULL)
    {
        q->next = NULL;
        t->tail = q;
        t->head = q;
    }
    else{
        q->next = NULL;
        t->tail->next = q;
        t->tail = q;
    }
    t->num++;
    if(pthread_mutex_unlock(&t->mutex))
    {
        perror("pthread_mutex_lock in init_queue");
        return -1;
    }
    if(pthread_cond_signal(&t->cond))
    {
        perror("pthread_cond_signal");
        return -1;
    }

}
task_t* get_queue(task_queue_t* t)
{
    if(t == NULL)
    {    
        perror("t null");
        return NULL;
    }
    if(pthread_mutex_lock(&t->mutex))
    {
        perror("pthread_mutex_lock in init_queue");
        return NULL;
    }
    if(t->head == NULL)
    {
       if(pthread_cond_wait(&t->cond,&t->mutex))
       {
           pthread_mutex_unlock(&t->mutex);
           perror("cond wait");
           return NULL;
       }
    }
    task_t *ptr = t->head;
    t->head = t->head->next;
    if(pthread_mutex_unlock(&t->mutex))
    {
        perror("unlock");
        return NULL;
    }
    
    return ptr;

}
