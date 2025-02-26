##########################################################################################################################

# Target executable name
NAME = btc

# Compiler and compilation flags
CC = c++
CFLAGS = -Wall -Wextra -Werror -std=c++98

# Directory structure
OBJ_DIR = obj/
SRC = $(wildcard src/*.cpp)
OBJS = $(patsubst src/%, $(OBJ_DIR)%, $(SRC:.cpp=.o))

##########################################################################################################################

# General rules
all: $(NAME) $(CONFS)

clean:
	@rm -rf $(OBJS)
	@rm -rf $(OBJ_DIR)
	@echo "$(RED)Removing:$(DEFAULT) All objects from $(NAME)"

fclean: clean
	@rm -rf $(NAME)
	@echo "$(RED)Removing:$(DEFAULT) Program $(NAME)"
	@rm -rf $(CONFS)
	@echo "$(RED)Removing:$(DEFAULT) $(CONFS)"

re: fclean all

##########################################################################################################################

# Build rules
$(NAME): $(OBJ_DIR) $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME)
	@echo "$(MAGENTA)Program $(NAME) created successfully.$(DEFAULT)"

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)
	@echo "$(GREEN)Creating:$(DEFAULT) $(OBJ_DIR) directory"

$(OBJ_DIR)%.o: src/%.cpp | $(OBJ_DIR)
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "$(GREEN)Compiling:$(DEFAULT) $<"


##########################################################################################################################

# Color codes for improved readability
BOLD    := \033[1m
BLACK   := \033[30;1m
RED     := \033[31;1m
GREEN   := \033[32;1m
YELLOW  := \033[33;1m
BLUE    := \033[34;1m
MAGENTA := \033[35;1m
CYAN    := \033[36;1m
WHITE   := \033[37;1m
DEFAULT := \033[0m

.PHONY: all clean fclean re git gitignore


