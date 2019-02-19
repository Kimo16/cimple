BUILD = build/

SDL_CFLAGS=$(shell sdl2-config --cflags --libs)

SRC_FOLDER = src/
VIEW_FOLDER = src/view/
MOD_FOLDER = src/model/
CONTR_FOLDER = src/controller/

SRC_MAIN = main.c
SRC_VIEW = cmd_view.c \
					 window.c  \
					 event.c
SRC_MOD = in.c		  		\
					out.c 	  		\
					m_color.c 		\
					m_frame.c 		\
					m_transform.c \
					parse.c
SRC_CONTR = cmd_line.c

SRCP_MAIN = $(addprefix $(SRC_FOLDER), $(SRC_MAIN))
SRCP_VIEW = $(addprefix $(VIEW_FOLDER), $(SRC_VIEW))
SRCP_MOD = $(addprefix $(MOD_FOLDER), $(SRC_MOD))
SRCP_CONTR = $(addprefix $(CONTR_FOLDER), $(SRC_CONTR))

OBJ_MAIN = $(SRCP_MAIN:$(SRC_FOLDER)%.c=$(BUILD)%.o)
OBJ_VIEW = $(SRCP_VIEW:$(SRC_FOLDER)%.c=$(BUILD)%.o)
OBJ_MOD = $(SRCP_MOD:$(SRC_FOLDER)%.c=$(BUILD)%.o)
OBJ_CONTR = $(SRCP_CONTR:$(SRC_FOLDER)%.c=$(BUILD)%.o)

OBJS = $(OBJ_MAIN) $(OBJ_VIEW) $(OBJ_MOD) $(OBJ_CONTR)
