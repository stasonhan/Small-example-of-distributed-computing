
#include <signal.h>
#include "sock.h"
#include "db_interface.h"

int thread_num = 0;
task_queue_t task;
int main()
{

    signal(SIGPIPE,SIG_IGN);
    create_listen_thread();
    create_worker_thread(); 
    
    while(1)
    {
        
    }
}
