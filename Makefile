NAME		= minishell

CC 			= cc

CFLAGS		= -g3 -Wextra -Werror

SRC			= dislpay_err.c check_access.c lst_add.c redirection.c fork.c minishell.c

OBJ 		= $(SRC:.c=.o)

all : $(NAME)

%.o : %.c minishell.h
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