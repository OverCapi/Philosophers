MANDATORY_PATH = ./philo
SRCS_FILE = $(MANDATORY_PATH)/philo.c \
			$(MANDATORY_PATH)/utils/ft_atoi_safe.c

BONUS_PATH = ./philo_bonus
SRCS_BONUS = $(BONUS_PATH)/philo_bonus.c

OBJECT_FILE = $(SRCS_FILE:.c=.o)
OBJECT_FILE_BONUS = $(SRCS_BONUS:.c=.o)

NAME = philo
NAME_BONUS = philo_bonus
CC = cc
CFLAGS = -Wall -Wextra -Werror
LIB    = -lpthread

all: $(NAME)

.c.o:
	$(CC) $(CFLAGS) -c $< -o $(<:.c=.o)

$(NAME): $(OBJECT_FILE)
	$(CC) $(CFLAGS) $(LIB) -o $(MANDATORY_PATH)/$(NAME) $(OBJECT_FILE)

bonus: $(OBJECT_FILE_BONUS)
	$(CC) $(CFLAGS) $(LIB) -o $(BONUS_PATH)/$(NAME_BONUS) $(OBJECT_FILE_BONUS)

clean:
	rm -f $(OBJECT_FILE) $(OBJECT_FILE_BONUS)

fclean: clean
	rm -f $(MANDATORY_PATH)/$(NAME)
	rm -f $(BONUS_PATH)/$(NAME_BONUS)

re: fclean $(NAME)

.PHONY: all bonus clean fclean re