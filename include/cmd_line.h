#ifndef CMD_LINE_H
#define CMD_LINE_H

#include <stdio.h>
#include <stdlib.h>

#include "cmd_view.h"
#include "parse.h"
#include "m_color.h"
#include "m_image.h"
#include "m_transform.h"
#include "m_frame.h"
#include "in.h"
#include "out.h"
#include "window.h"
#include "event.h"


void filter_handler(char * option/*,pixels matrice struct*/);
void cimple_handler();

#endif
