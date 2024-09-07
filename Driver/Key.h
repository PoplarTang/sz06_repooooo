#ifndef __KEY_H__
#define __KEY_H__

#include "Config.h"

void Key_init();

void Key_scan();

extern void Key_on_up();
extern void Key_on_down();

#endif