#ifndef LOGGER_H
#define LOGGER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "utils.h"

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#define LOGGER_N_ENTRIES    32      // Number of strings that can be stored in the logger queue

typedef void (*logger_writer)(char* str, uint16_t length); 


#define LOG_D   "D "
#define LOG_N   "N "
#define LOG_W   "W "
#define LOG_E   "E "
#define LOG_END "\n"
#define log_debug(_str)   log_insert(LOG_D _str LOG_END, (uint16_t)(strlen(LOG_D _str LOG_END)+1))
#define log_notif(_str)   log_insert(LOG_N _str LOG_END, (uint16_t)(strlen(LOG_N _str LOG_END)+1))
#define log_warn(_str)    log_insert(LOG_W _str LOG_END, (uint16_t)(strlen(LOG_W _str LOG_END)+1))
#define log_error(_str)   log_insert(LOG_E _str LOG_END, (uint16_t)(strlen(LOG_E _str LOG_END)+1))

void log_insert(const char *str, uint16_t length);
bool log_process();
void log_init(logger_writer handler);


#ifdef __cplusplus
}
#endif
#endif