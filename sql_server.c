
#include <signal.h>
#include "sock.h"
#include "db_interface.h"

int thread_num = 0;

int main()
{
    signal(SIGPIPE,SIG_IGN);
    create_listen_thread();
    
    while(1)
    {
        
    }
}
