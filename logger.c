#include "logger.h"

#include "fifoc.h"

/*

    funcion que recibe puntero a string
    ese string tendra appended el tipo de log que es (notif, warn, etc)
    guarda en la cola como puntero a string y en la segunda cola su tamaño


*/

FIFOC_DEF(q_str, const char*, LOGGER_N_ENTRIES);
FIFOC_DEF(q_length, const char*, LOGGER_N_ENTRIES);

static logger_writer callback = NULL;

void log_insert(const char *str, uint16_t length)
{
    fifoc_put(&q_str, &str);   // Store pointer to string
    fifoc_put(&q_length, &str);   // Store string length
}

// True if a new str has been processed. False if log queue was empty
bool log_process()
{
    char* str;
    uint16_t length;
    bool ret_val = false;

    if(fifoc_get(&q_str, &str) 
    && fifoc_get(&q_length, &length) 
    && callback != NULL)
    {
        callback(str, length);
        ret_val = true;
    }
    return ret_val;
}

void log_init(logger_writer handler)
{
    callback = handler;
}