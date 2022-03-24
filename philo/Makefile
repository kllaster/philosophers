NAME			= philo
CC				= gcc
RM				= rm -f
MKDIR			= mkdir -p
DEBUG			= 1

ifeq ($(DEBUG), 1)
	DEBUG_FLAGS	= -g -fsanitize=address
else
	DEBUG_FLAGS	=
endif

PROTECT_FLAGS	= -fno-exceptions -fstack-protector-all
COMMON_FLAGS	= -std=c99 -Wall -Wextra -Werror -Wfloat-equal -MMD -pipe
CFLAGS			= $(COMMON_FLAGS) $(DEBUG_FLAGS) $(PROTECT_FLAGS)

BIN_DIR			= ./
SRC_DIR			= ./src
BUILD_DIR		= ./build
HEADS			= ./include

SRCS 			= ./src/main.c\
				  ./src/init.c\
				  ./src/table.c\
				  ./src/utils.c\

OBJS			= $(notdir $(SRCS))
OBJS			:= $(subst $(SRC_DIR), $(BUILD_DIR), $(SRCS:%.c=%.o))
NAME			:= $(addprefix $(BIN_DIR), $(NAME))
DEPS			= $(OBJS:.o=.d)
VPATH			= $(SRC_DIR):$(HEADS):$(BUILD_DIR)

all:			$(NAME)

$(NAME):		$(OBJS) $(LIBFT) $(GNL)
				$(CC) $(CFLAGS) -I $(HEADS) $(OBJS) $(LIBS) -o $(NAME)

$(BUILD_DIR)/%.o:  $(SRC_DIR)/%.c
				@if [ ! -d $(dir $@) ] ; then $(MKDIR) $(dir $@); fi
				$(CC) $(CFLAGS) -I $(HEADS) -c $< -o $@

clean:
				$(RM) $(OBJS)
				$(RM) $(DEPS)

fclean:			clean
				$(RM) $(NAME)

re:				fclean all

-include $(DEPS)
.PHONY:			all, clean, fclean, re
