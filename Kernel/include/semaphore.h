#pragma once

#include "queue.h"
#include "semaphores.h"
#define MAX_SEMAPHORES 128
#include <stdbool.h>



SID semInit(const char *name, semValue value);

bool semClose(SID sem);

bool semWait(SID sem);

bool semPost(SID sem);

SID semOpen(const char *name);

void semPrintList();

