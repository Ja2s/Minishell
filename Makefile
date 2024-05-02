NAME		= minishell

CC 			= cc

CFLAGS		= -g3  -Wextra -Werror

SRC			= dislpay_err.c check_access.c builtin.c lst_add.c redirection.c fork.c minishell.c other.c \
				$(PARSING_SRC)syntaxe_errors.c $(PARSING_SRC)utilities.c \
				$(PARSING_SRC)utilities_nd.c $(PARSING_SRC)commands_stocker.c \
				$(PARSING_SRC)expander.c $(PARSING_SRC)heredoc.c \
				$(PARSING_SRC)redirection.c

PARSING_SRC	= parsing/

OBJ 		= $(SRC:.c=.o)

all : $(NAME)

%.o : %.c includes/minishell.h
	$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJ)
	make -sC libft
	$(CC) $(OBJ) -L libft -l ft -lreadline -o $(NAME)

clean :
	rm -rf  $(OBJ) 
	make -sC libft clean

fclean : clean
		rm -f $(NAME)
		make -sC libft fclean

re : fclean all

.PHONY : all clean fclean re