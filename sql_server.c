
#include <signal.h>
#include "sock.h"
#include "db_interface.h"
#include "command.h"
int thread_num = 0;
task_queue_t task;
void signal_quit()
{
    destroy_task(&task);
    exit(2);
}
int main()
{

    init_task(&task);
    signal(SIGPIPE,SIG_IGN);
    signal(SIGINT,signal_quit);
    create_listen_thread();
    create_worker_thread(); 
    
    while(1)
    {
        
    }
}
