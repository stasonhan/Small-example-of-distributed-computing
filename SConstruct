import os
env = Environment()
d = env.ParseFlags("-lpthread -lpq")
env.MergeFlags(d)
env.Program(['sql_server.c','sock.c','command.c','thread.c','db_interface.c'])
