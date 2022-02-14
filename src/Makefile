CC=gcc 
CFLAGS=-Wall -c -g
EXEC= shell
INC=:q
SRCC_TABLE=../librairies/modules_persos/Table_processus/
SRCC_ERREURS=../librairies/modules_persos/Gestion_erreurs/
SRCC_CMD = ../librairies/modules_persos/Execution_commandes/
SRCC_CMD_INT = ../librairies/modules_persos/Execution_commandes/Cmd_internes/
all: $(EXEC)


shell: shell.o redirection.o command.o errors_message.o process_table.o readcmd.o list_process.o cmd_exit.o cmd_stop.o cmd_cd.o cmd_bg.o cmd_fg.o cmd_jobs.o 
	$(CC) -g -Wall redirection.o list_process.o errors_message.o readcmd.o shell.o command.o process_table.o cmd_exit.o cmd_stop.o cmd_cd.o cmd_bg.o cmd_fg.o cmd_jobs.o -o shell

shell.o: shell.c
	$(CC) $(CFLAGS) shell.c

redirection.o: $(SRCC_CMD)redirection.c 
	$(CC) $(CFLAGS) $(SRCC_CMD)redirection.c

list_process.o : $(SRCC_TABLE)list_process.c
	$(CC) $(CFLAGS) $(SRCC_TABLE)list_process.c

command.o: $(SRCC_CMD)command.c 
	$(CC) $(CFLAGS) $(SRCC_CMD)command.c

process_table.o: $(SRCC_TABLE)process_table.c 
	$(CC) $(CFLAGS) $(SRCC_TABLE)process_table.c 

errors_message.o : $(SRCC_ERREURS)errors_message.c
	$(CC) $(CFLAGS) $(SRCC_ERREURS)errors_message.c

readcmd.o : ../librairies/fournitures/readcmd.c
	$(CC) $(CFLAGS) ../librairies/fournitures/readcmd.c

cmd_cd.o : $(SRCC_CMD_INT)cmd_cd.c 
	$(CC) $(CFLAGS) $(SRCC_CMD_INT)cmd_cd.c 

cmd_exit.o : $(SRCC_CMD_INT)cmd_exit.c 
	$(CC) $(CFLAGS) $(SRCC_CMD_INT)cmd_exit.c 
	
cmd_jobs.o : $(SRCC_CMD_INT)cmd_jobs.c 
	$(CC) $(CFLAGS) $(SRCC_CMD_INT)cmd_jobs.c 

cmd_stop.o : $(SRCC_CMD_INT)cmd_stop.c 
	$(CC) $(CFLAGS) $(SRCC_CMD_INT)cmd_stop.c 

cmd_fg.o : $(SRCC_CMD_INT)cmd_fg.c 
	$(CC) $(CFLAGS) $(SRCC_CMD_INT)cmd_fg.c 

cmd_bg.o : $(SRCC_CMD_INT)cmd_bg.c 
	$(CC) $(CFLAGS) $(SRCC_CMD_INT)cmd_bg.c 

clean : 
	rm -f shell *.o
