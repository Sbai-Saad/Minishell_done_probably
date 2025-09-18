NAME = minishell

CC = cc
CFLAGS = -Wall -Wextra -Werror

READLINE_CFLAGS := $(shell pkg-config --cflags readline 2>/dev/null)
READLINE_LDLIBS := $(shell pkg-config --libs readline 2>/dev/null)

INCLUDES = -I includes $(READLINE_CFLAGS) $(if $(READLINE_INC),-I $(READLINE_INC))
LDLIBS = $(if $(READLINE_LDLIBS),$(READLINE_LDLIBS),-lreadline) $(if $(READLINE_LIB),-L $(READLINE_LIB))

SRCS =	srcs/main.c \
		srcs/main_utils.c \
		srcs/tokens.c \
		srcs/token_helpers.c \
		srcs/parsing.c \
		srcs/parsing_helpers.c \
		srcs/parsing_helpers2.c \
		srcs/parsing_helpers3.c \
		srcs/parsing_utils.c \
		srcs/parser_cmd.c \
		srcs/parser_cmd_utils.c \
		srcs/parser_cmd_utils2.c \
		srcs/parser_cmd_utils3.c \
		srcs/parser_cmd_utils4.c \
		srcs/expand.c \
		srcs/expand_utils.c \
		srcs/expand_helpers.c \
		srcs/quotes.c \
		srcs/execute.c \
		srcs/execute_utils.c \
		srcs/execute_utils2.c \
		srcs/execute_utils3.c \
		srcs/execute_utils4.c \
		srcs/execute_pipeline.c \
		srcs/execute_pipeline2.c \
		srcs/execute_pipeline3.c \
		srcs/redirections.c \
		srcs/redirections_utils.c \
		srcs/heredoc.c \
		srcs/heredoc_expand.c \
		srcs/heredoc_expand2.c \
		srcs/heredoc_utils.c \
		srcs/heredoc_utils2.c \
		srcs/heredoc_utils3.c \
		srcs/pipes.c \
		srcs/builtins.c \
		srcs/echo.c \
		srcs/cd.c \
		srcs/pwd.c \
		srcs/export.c \
		srcs/export_utils.c \
		srcs/export_helpers.c \
		srcs/unset.c \
		srcs/env.c \
		srcs/exit.c \
		srcs/exit_utils.c \
		srcs/env_utils.c \
		srcs/env_helpers.c \
		srcs/signals.c \
		srcs/error.c \
		srcs/memory.c \
		srcs/helpers.c \
		srcs/helpers_split.c \
		srcs/helpers_itoa.c \
		srcs/helpers_string.c \

OBJS = $(SRCS:.c=.o)

READLINE_LIB = $(shell brew --prefix readline)/lib
READLINE_INC = $(shell brew --prefix readline)/include

all: $(NAME)

$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(LDLIBS)

%.o: %.c includes/minishell.h
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	@rm -f $(OBJS)

fclean: clean
	@rm -f $(NAME)

re: fclean all