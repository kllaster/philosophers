NAME		= philo
CC			= gcc
RM			= rm -f
DEBUG		= 0
ifeq ($(DEBUG), 1)
	DEBUG_FLAGS	= -fsanitize=address -g
endif
CFLAGS		= -Wall -Wextra -Werror -MMD $(DEBUG_FLAGS)
CPPFLAGS	= $(CFLAGS) -march=native -flto -pipe
HEADERS		= /

SRCS =	main.c\
		init.c\
		table.c\
		utils.c\

OBJS = $(SRCS:.c=.o)

$(NAME):		$(OBJS)
				$(CC) $(CPPFLAGS) -I $(HEADERS) $(OBJS) -o $(NAME)

all:			$(NAME)

.c.o:
				$(CC) $(CPPFLAGS) -I $(HEADERS) -o $@ -c $<

$(OBJS):		$(HEADERS)

clean:
				$(RM) $(OBJS)
				$(RM) $(OBJS:.o=.d)

fclean:			clean
				$(RM) $(NAME)

re:				fclean all

re_pr:			clean all

.PHONY:			all clean fclean re re_pr
.SILENT: