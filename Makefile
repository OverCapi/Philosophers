MANDATORY_PATH = ./philo
RUN_PATH = $(MANDATORY_PATH)/run
UTILS_PATH = $(MANDATORY_PATH)/utils

SRCS_RUN_FILE =		$(RUN_PATH)/common_routine.c \
					$(RUN_PATH)/monitoring_routine.c \
					$(RUN_PATH)/philos_routine.c \
					$(RUN_PATH)/run.c

SRCS_UTILS_FILE = 	$(UTILS_PATH)/mutex_getters_setters.c \
					$(UTILS_PATH)/routine_utils.c \
					$(UTILS_PATH)/time.c \
					$(UTILS_PATH)/ft_atoi_safe.c \
					$(UTILS_PATH)/print_error.c

SRCS_FILE_OTHERS =	$(MANDATORY_PATH)/clean.c \
					$(MANDATORY_PATH)/init.c \
					$(MANDATORY_PATH)/parsing.c \
					$(MANDATORY_PATH)/philo.c

SRCS_FILE =			$(SRCS_UTILS_FILE) \
					$(SRCS_RUN_FILE) \
					$(SRCS_FILE_OTHERS)
OBJECT_FILE =		$(SRCS_FILE:.c=.o)

NAME = philo
CC = cc
CFLAGS = -Wall -Wextra -Werror
LIB    = -lpthread
DEBUG  = -g3 #-fsanitize=address

all: $(NAME)

.c.o:
	$(CC) $(CFLAGS) $(DEBUG) -c $< -o $(<:.c=.o)

$(NAME): $(OBJECT_FILE)
	$(CC) $(CFLAGS) $(LIB) $(DEBUG) -o $(MANDATORY_PATH)/$(NAME) $(OBJECT_FILE)

clean:
	rm -f $(OBJECT_FILE)

fclean: clean
	rm -f $(MANDATORY_PATH)/$(NAME)

re: fclean $(NAME)

.PHONY: all clean fclean re