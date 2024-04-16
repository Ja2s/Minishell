NAME		= minishell

CC 			= cc

CFLAGS		= -g3 -Wall -Wextra -Werror

SRC			= main.c dislpay_err.c check_access.c lst_add.c redirection.c fork.c

OBJ 		= $(SRC:.c=.o)

all : $(NAME)

%.o : %.c minishell.h
	$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJ)
	make -sC ft_printf
	make -sC libft
	$(CC) $(OBJ) -L ft_printf -l ftprintf -L libft -l ft -o $(NAME)

clean :
	rm -rf  $(OBJ) 
	make -sC ft_printf clean
	make -sC libft clean

fclean : clean
		rm -f $(NAME)
		make -sC ft_printf fclean
		make -sC libft fclean

re : fclean all

.PHONY : all clean fclean re