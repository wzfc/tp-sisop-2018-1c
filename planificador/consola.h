#ifndef PLANIFICADOR_CONSOLA_H_
#define PLANIFICADOR_CONSOLA_H_

#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include "../libs/comando.h"
#include "planificador.h"

extern int scheduler_paused_flag;
extern int block_esi_by_console_flag;
extern int unlock_esi_by_console_flag;
extern int killed_esi_flag;
extern char* last_unlocked_key_by_console;
extern t_list* g_new_killed_esis;
extern t_list* g_esis_sexpecting_keys;
extern t_list* g_new_blocked_by_console_esis;

/**
 * Recibe una linea de comando y lo ejecuta.
 * El valor de retorno indica si fue ejecutado exitosamente u ocurrio un error:
 *   -  (1) Exito.
 *   - (-1) Comando no existente.
 *   - (-2) Cantidad incorrecta de argumentos.
 */
int execute_console_command(char *command_line);

void *init_console();

/*
 * Muestra los procesos bloqueados por una determinada clave
 */

void show_blocked_process(char* resource);

/*
 * Se encarga de detectar todos los deadlocks que hay en el momento y los muestra
 */

void detect_and_show_all_deadlocks();

#endif /* PLANIFICADOR_CONSOLA_H_ */
