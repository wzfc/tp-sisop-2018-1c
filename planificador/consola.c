#include "consola.h"

static void pause_scheduler(char **_);
static void resume_scheduler(char **_);
static void lock_process(char **args);
static void unlock_process(char **args);
static void list_locked_process(char **args);
static void kill_process(char **args);
static void key_status(char **args);
static void check_deadlock(char **_);

#define COMMANDS_SIZE (sizeof(commands) / sizeof(*commands))
#define eprintf(args...) fprintf (stderr, args)

static struct command_t commands[] = {
	DEF_COMMAND("pausar",      0, pause_scheduler),
	DEF_COMMAND("continuar",   0, resume_scheduler),
	DEF_COMMAND("bloquear",    2, lock_process),
	DEF_COMMAND("desbloquear", 1, unlock_process),
	DEF_COMMAND("listar",      1, list_locked_process),
	DEF_COMMAND("kill",        1, kill_process),
	DEF_COMMAND("status",      1, key_status),
	DEF_COMMAND("deadlock",    0, check_deadlock)
};

int execute_console_command(char *command_line)
{
	return execute_command_line(commands, COMMANDS_SIZE, command_line);
}

void *init_console(void* _)
{

	for (;;) {
		char *line = readline("> ");
		int execute_result = execute_console_command(line);
		switch (execute_result) {
		case NO_COMMAND_ERROR:
			eprintf("Comando invalido!\n");
			break;
		case ARGUMENT_COUNT_ERROR:
			eprintf("Numero de argumentos incorrecto\n");
			break;
		default:
			printf("Comando ejecutado correctamente\n");
			break;
		}
		free(line);
	}

	return NULL;
}

static void pause_scheduler(char **_)
{
	printf("Pausar planificacion\n");

	scheduler_paused_flag = 1;
}

static void resume_scheduler(char **_)
{
	printf("Continuar planificacion\n");

	scheduler_paused_flag = 0;
}

static void lock_process(char **args)
{
	void _lock_process(char *key, char *pid) {

		printf("Bloquear proceso ESI (clave = %s, id = %s)\n", key, pid);
		block_esi_by_console_flag = 1;
		list_add(g_new_blocked_by_console_esis, (void*)create_esi_sexpecting_key(atoi(pid), key));

	}
	_lock_process(args[0], args[1]);
}

static void unlock_process(char **args)
{
	void _unlock_process(char* key) {
		printf("Desbloquear proceso ESI (clave = %s)\n", key);
		unlock_esi_by_console_flag = 1;
		last_unlocked_key_by_console = strdup(key);
	}

	_unlock_process(args[0]);
}

static void list_locked_process(char **args)
{
	void _list_locked_process(char* resource) {

		printf("Listar procesos bloqueados por recurso %s\n", resource);
		show_blocked_process(resource);
	}
	_list_locked_process(args[0]);
}

static void kill_process(char **args)
{
	void _kill_process(char *pid) {
		printf("Finalizar proceso %s\n", pid);
		killed_esi_flag = 1;
		int esi_id = atoi(pid);
		list_add(g_new_killed_esis, (void*)&esi_id);
	}
	_kill_process(args[0]);
}

static void key_status(char **args)
{
	void _key_status(char *key) {
		printf("Informar estado de la clave %s\n", key);

		/* Probablemente se necesite un mutex para sincronizar la comunicación con el coordinador y que no explote TODO horrendamente */

		/* Hay que comunicarse con el coordinador para obtener los datos de la clave respecto de las instancias */

		show_blocked_process(key);
	}
	_key_status(args[0]);
}

static void check_deadlock(char **_)
{
	printf("Detectar deadlock\n");
}

void show_blocked_process(char* resource) {

	void show_esi_from_resource(void* resource2){

		if (strcmp(((esi_sexpecting_key*)resource2)->key, resource) == 0) {

			esi_information* esi_inf = obtain_esi_information_by_id(((esi_sexpecting_key*)resource2)->esi_fd);
			printf("El ESI %i se encuentra bloqueado por la clave %s\n", esi_inf->esi_numeric_name, resource);
		}
	}

	list_iterate(g_esis_sexpecting_keys, show_esi_from_resource);
}

