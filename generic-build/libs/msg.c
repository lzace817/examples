#include "msg.h"

static const char msg[] = "hello from lib";

const char *get_message(void)
{
    return msg;
}
