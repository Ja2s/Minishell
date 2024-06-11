NAME        = minishell

CC          = cc

CFLAGS      = -g3 -Wextra -Werror

SRC         =	minishell.c \
				main_helper.c \
				$(EXEC_SRC)dislpay_err.c \
				$(EXEC_SRC)check_access.c \
            	$(EXEC_SRC)redirection.c \
				$(EXEC_SRC)fork.c \
        		$(EXEC_SRC)heredoc.c \
        		$(OTHER_SRC)other.c \
				$(OTHER_SRC)builtins.c \
				$(OTHER_SRC)builtins_helper.c \
				$(OTHER_SRC)builtins_nd.c \
				$(OTHER_SRC)builtins_rd.c \
				$(OTHER_SRC)lst_add.c \
				$(OTHER_SRC)env.c \
				$(OTHER_SRC)env_helper.c \
            	$(PARSING_SRC)syntaxe_errors.c \
				$(PARSING_SRC)utilities.c \
            	$(PARSING_SRC)utilities_nd.c \
				$(PARSING_SRC)commands_stocker.c \
            	$(PARSING_SRC)expand/expand.c \
				$(PARSING_SRC)expand/expand_helper.c \
				$(PARSING_SRC)expand/checkers_expand.c \
				$(PARSING_SRC)expand/exitcode_expand.c \
				$(PARSING_SRC)expand/extract_expand.c \
				$(PARSING_SRC)expand/helper_expand.c \
				$(PARSING_SRC)expand/heredoc_expand.c \
				$(PARSING_SRC)expand/utils_expand.c \
				$(PARSING_SRC)expand/utils_expand_nd.c \
				$(PARSING_SRC)/main_functions/main_functions.c \
            	$(PARSING_SRC)/main_functions/main_functions_nd.c \
				$(PARSING_SRC)/main_functions/main_functions_rd.c \
				$(PARSING_SRC)/heredoc/heredoc.c \
				$(PARSING_SRC)/heredoc/heredoc_nd.c \
				$(PARSING_SRC)/heredoc/heredoc_rd.c \
            	$(PARSING_SRC)/redirecter/redirecter.c \
				$(PARSING_SRC)/redirecter/redirecter_nd.c \
				$(PARSING_SRC)syntaxe_errors_nd.c 

PARSING_SRC = parsing/

EXEC_SRC    = exec/

OTHER_SRC   = other/

OBJ         = $(SRC:.c=.o)

all : $(NAME)

%.o : %.c includes/minishell.h
	@$(CC) $(CFLAGS) -c $< -o $@
	printf "\033[1A\033[KCompiling $<\r"

$(NAME): $(OBJ)
	@make -sC libft && $(CC) $(OBJ) -L libft -l ft -lreadline -o $(NAME)
	printf "\033[38;5;220m\033[1A\033[KExecutable $(NAME) created\n\033[0m"

clean :
	@rm -rf  $(OBJ) 
	@make -sC libft clean
	printf "\033[38;5;220m\033[1A\033[KCleaning objects\n\033[0m"

fclean : clean
	@rm -f $(NAME)
	@make -sC libft fclean
	printf "\033[38;5;220m\033[1A\033[KRemoving $(NAME) and libraries\n\033[0m"

re : fclean all

.PHONY : all clean fclean re


