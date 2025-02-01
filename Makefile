NAME = ft_ping

CC = gcc
CFLAGS = -Wall -Wextra -Werror
#CFLAGS = -g -fsanitize=address

SRC_DIR = src
OBJ_DIR = obj

SRC = $(SRC_DIR)/error.c $(SRC_DIR)/icmp.c $(SRC_DIR)/main.c $(SRC_DIR)/options.c $(SRC_DIR)/utils.c
OBJ = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC))

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
