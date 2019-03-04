###################
#     Cimple      #
#    Makefile     #
###################

NAME = cimple
CC = gcc
CFLAGS = -Wall
INCLUDES = -I include
TESTFLAGS = -lcmocka
SDL_CFLAGS=$(shell sdl2-config --cflags --libs) -lSDL2_image

include const.mk

.PHONY: all 
all : $(NAME)

##########
# BUILDS #
##########

$(NAME) :  $(OBJS)
	@printf "\n== LINKING : %s ==\n" $(NAME)
	$(CC) -o $@ $(CFLAGS) $(INCLUDES) $(SDL_CFLAGS) $^
	@printf "=== END LINKING ==\n"

$(BUILD)%.o : $(SRC_FOLDER)%.c
	@mkdir -p $(dir $@)
	@printf "Compile : %s\n" $<
	@$(CC) -c $(CFLAGS) $(INCLUDES) $(SDL_CFLAGS) -o $@ $<

.PHONY: view model controller
view : $(OBJ_VIEW) 
model : $(OBJ_MOD)
controller : $(OBJ_CONTR)

###############
# TESTS_BUILD #
###############

$(BUILD)%.o : $(TESTS_FOLDER)%.c
	@mkdir -p $(dir $@)
	@printf "Compile TEST : %s\n" $<
	@$(CC) -c $(CFLAGS) $(INCLUDES) -o $@ $< $(TESTFLAGS)

$(TESTS)testing: $(OBJ_TESTS) $(TESTS_DEPS)
	@printf "\n== LINKING TESTS ==\n"
	$(CC) -o $@ $(CFLAGS) $(INCLUDES) $^ $(TESTFLAGS) $(SDL_CFLAGS)
	@printf "=== END LINKING ==\n"


#########
# TESTS #
######### 

.PHONY: check
check: $(TESTS)testing
	@printf "\n=== TESTS ===\n"
	$(TESTS)testing
	@printf "=== END TESTS ===\n"

.PHONY: check_valgrind
check_valgrind:
	@printf "\n=== TESTS Valgrind ===\n"
	@valgrind --error-exitcode=1 			\
		--leak-resolution=low 			\
		--leak-check=full $(TESTS)/testing
	@printf "=== END VALGRIND ===\n"

.PHONY: checkall
checkall: check check_valgrind


##########
# CLEANS #
##########

.PHONY: clean_test
clean_test:
	@printf "\n=== CLEAN TESTS ===\n"
	rm -rf $(TESTS)testing $(OBJ_TESTS)
	@printf "=== END CLEAN ===\n"

.PHONY: clean_build
clean_build : 
	@printf "\n=== CLEAN BUILD & CIMP ===\n"
	rm -rf $(BUILD) $(NAME)
	@printf "=== END ===\n"

.PHONY: cleanall
cleanall: clean_build clean_test
