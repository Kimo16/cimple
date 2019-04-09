###################
#     Cimple      #
#    Makefile     #
###################

NAME = cimple
CC = gcc
INCLUDES = -I include
STD_CFLAGS = -Wall $(shell pkg-config sdl2 SDL2_image libjpeg libpng --cflags)
STD_CLIBS = $(shell pkg-config sdl2 SDL2_image libjpeg libpng --libs)
TEST_CFLAGS = $(shell pkg-config cmocka --cflags) $(STD_CFLAGS)
TEST_CLIBS = $(shell pkg-config cmocka --libs) $(STD_CLIBS)

include const.mk

.PHONY: all 
all : $(NAME)

##########
# BUILDS #
##########

$(NAME) :  $(OBJS)
	@printf "\n== LINKING : %s ==\n" $(NAME)
	$(CC) $(STD_CFLAGS) $(INCLUDES) -o $@ $^ $(STD_CLIBS) 
	@printf "=== END LINKING ==\n"

$(BUILD)%.o : $(SRC_FOLDER)%.c
	@mkdir -p $(dir $@)
	@printf "Compile : %s\n" $<
	@$(CC) $(STD_CFLAGS) $(INCLUDES) -c -o $@ $< $(STD_CLIBS) 

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
	@$(CC) $(INCLUDES) $(TEST_CFLAGS) -c -o $@ $< $(TEST_CLIBS) 

$(TESTS)testing: $(OBJ_TESTS) $(TESTS_DEPS)
	@printf "\n== LINKING TESTS ==\n"
	$(CC) $(INCLUDES) $(TEST_CFLAGS) -o $@ $^ $(TEST_CLIBS) 
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
