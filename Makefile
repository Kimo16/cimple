###################
#     Cimple      #
#  	 Makefile     #
###################

NAME = cimple
CC = gcc
CFLAGS = -Wall
LDFLAGS= -lreadline
INCLUDES = -I include

include const.mk

.PHONY: all clean view model controller

all : $(NAME)

$(NAME) :  $(OBJS)
	@printf "== LINKING : %s ==\n" $(NAME)
	$(CC) -o $@ $(CFLAGS) $(INCLUDES) $(LDFLAGS) $^
	@printf "=== END LINKING ==\n"

$(BUILD)%.o : $(SRC_FOLDER)%.c
	@mkdir -p $(dir $@)
	@printf "Compile : %s\n" $<
	@$(CC) -c $(CFLAGS) $(INCLUDES) -o $@ $< 

view : $(OBJ_VIEW) 
model : $(OBJ_MOD)
controller : $(OBJ_CONTR)

check_valgrind: 
	@valgrind --error-exitcode=1 			\
		--leak-resolution=low 					\
		--leak-check=full ./cimple


clean :
	@printf "Clean build/ and cimp\n"
	@rm -rf $(BUILD) $(NAME)

