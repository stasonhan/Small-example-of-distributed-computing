
#include "command.h"
#include "cJSON.h"
extern task_queue_t task;
/*
*function: parse the quest from the server into cmd_s
@para:
* quest:
*"{name:"123456",bank:"china",operate:"cut/add",money:234}"
*/


cJSON* create_msg_obj(const char* text)
{
    cJSON *obj = cJSON_Parse(text);
    char* out;
    if(!obj)
    {
        printf("create cJSON failed!\n");
    }
    else
    {
        return obj;
        //out=cJSON_Print(obj);
        //cJSON_Delete(obj);
        //printf("%s\n",out);
        //free(out); 

    }
}

/*
Parse the quest This function is called by ...
*/
int parse_request(char* quest)
{
    printf("quest = %s\n",quest);
    //create_msg_obj(quest);
    char *items[] ={"name","bank","operate","money"};
    cJSON *json = cJSON_Parse(quest);
    cJSON *item;
    task_t *t;
    char *string;
    t = (task_t *)malloc(sizeof(task_t));
    if(!t)
    {
        perror("malloc task_t");
        return -1;
    }
    
    string = cJSON_Print(json);
    printf("string=%s\n",string);

    item = cJSON_GetObjectItem(json,"name");
    string = cJSON_Print(item);
    strncpy(t->command.name,string,strlen(string) + 1);
    printf("t->command.name :%s\n",t->command.name);
    //cJSON_Delete(item);
    free(string);
    item = cJSON_GetObjectItem(json,"bank");
    string = cJSON_Print(item);
    printf("test string = %s\n",string);
    strncpy(t->command.bank,string,strlen(string) + 1);
    printf("t->command.bank :%s\n",t->command.bank);
    //cJSON_Delete(item);
    free(string);

    item = cJSON_GetObjectItem(json,"operate");
    string = cJSON_Print(item);
    strncpy(t->command.operate,string,strlen(string) + 1);
    printf("t->command.operate :%s\n",t->command.operate);
    //cJSON_Delete(item);
    free(string);

    item = cJSON_GetObjectItem(json,"money");
    string = cJSON_Print(item);
    int num = atoi(string);
    t->command.money = num;
    free(string);
    cJSON_Delete(json);

    insert_queue(&task,t);

}

void  construct_command_to_sql(OUT char* sql)
{
    task_t *ptr=NULL;
    printf("construct_command_to_sql\n");
    while(1)
    {
        ptr = get_queue(&task);
        if(ptr == NULL)
            continue;
        printf("ptr->command.bank:%s\n",ptr->command.bank);
    }
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
    if(/*t->tail == NULL || */t->head == NULL)
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
    printf("In insert_queue t->num=%d\n",t->num);
    if(pthread_mutex_unlock(&t->mutex))
    {
        perror("pthread_mutex_lock in insert_queue");
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
    task_t *ptr;
    if(t == NULL )
    {    
        //perror("t null");
        return NULL;
    }
    if(pthread_mutex_lock(&t->mutex))
    {
        perror("pthread_mutex_lock in init_queue");
        return NULL;
    }
    if(t->head == NULL || t->num == 0)
    {
       if(pthread_cond_wait(&t->cond,&t->mutex))
       {
           pthread_mutex_unlock(&t->mutex);
           perror("cond wait");
           return NULL;
       }
    }
    
    ptr = t->head;
    t->head = t->head->next;
    t->num--;
    printf("In get_queue t->num = %d\n",t->num);
    
    if(pthread_mutex_unlock(&t->mutex))
    {
        perror("unlock");
        return NULL;
    }
    
    return ptr;

}
