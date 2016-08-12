#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"

#include "afe.h"

#include "dsp.h"

#define FS 500

void tasks_ecg_init();