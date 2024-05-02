# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jgavairo <jgavairo@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/11/09 13:57:30 by jgavairo          #+#    #+#              #
#    Updated: 2024/05/02 15:50:16 by jgavairo         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = cc
CFLAGS = -Wall -Werror -Wextra
SRC = ft_putnbr_fd.c ft_strcmp.c ft_putstr_fd.c ft_putendl_fd.c ft_putchar_fd.c ft_striteri.c ft_strmapi.c ft_itoa.c ft_split.c ft_strtrim.c ft_strjoin.c ft_substr.c ft_calloc.c ft_strdup.c ft_bzero.c ft_isalnum.c ft_isalpha.c ft_isascii.c ft_isdigit.c ft_isprint.c ft_memchr.c ft_memcmp.c ft_memcpy.c ft_memset.c ft_strchr.c ft_strlcat.c ft_strlcpy.c ft_strlen.c ft_strncmp.c ft_strnstr.c ft_strrchr.c ft_tolower.c ft_toupper.c ft_memmove.c ft_atoi.c
OBJ = $(SRC:.c=.o)
DEP = $(OBJ:.o=.d)
BONUS = ft_lstmap_bonus.c ft_lstiter_bonus.c ft_lstclear_bonus.c ft_lstdelone_bonus.c ft_lstadd_back_bonus.c ft_lstlast_bonus.c ft_lstnew_bonus.c ft_lstadd_front_bonus.c ft_lstsize_bonus.c
BONUS_OBJ = $(BONUS:.c=.o)
BONUS_DEP = $(BONUS_OBJ:.o=.d)
NAME = libft.a

.PHONY : all bonus clean fclean re

all : $(NAME)

%.o : %.c
	$(CC) $(CFLAGS) -MMD -c $< -o $@

-include $(DEP) $(BONUS_DEP)

$(NAME) : $(OBJ)
	ar rcs $(NAME) $(OBJ)

bonus:
	$(MAKE) SRC="$(SRC) $(BONUS)" DEP="$(DEP) $(BONUS_DEP)"

clean :
	rm -rf $(OBJ) $(BONUS_OBJ) $(DEP) $(BONUS_DEP)

fclean : clean
	rm -f $(NAME)

re : fclean all