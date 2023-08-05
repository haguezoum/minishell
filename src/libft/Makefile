# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile             ########    ## #     ##    ##             ## ##      #
#                                                     +:+ +:+         +:+      #
#    By: aet-tass <aet-tass@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/10/27 22:01:43 by aet-tass          #+#    #+#              #
#    Updated: 2023/   ########    ##      # #    ##    ##  ##           ##     #
#                                                                              #
# **************************************************************************** #

SRCS	= ft_isalnum.c ft_isalpha.c ft_isascii.c ft_isdigit.c ft_isprint.c ft_tolower.c ft_toupper.c\
 ft_bzero.c ft_memchr.c ft_memcmp.c ft_memcpy.c ft_memmove.c ft_memset.c ft_strlen.c ft_strlcpy.c\
  ft_strlcat.c ft_strchr.c ft_strrchr.c ft_strnstr.c ft_strncmp.c ft_atoi.c ft_calloc.c ft_strdup.c\
   ft_strjoin.c ft_split.c ft_putchar_fd.c ft_putstr_fd.c ft_putendl_fd.c ft_putnbr_fd.c ft_substr.c\
	ft_itoa.c ft_strtrim.c ft_strmapi.c ft_striteri.c
OBJS	= ${SRCS:.c=.o}
NAME	= libft.a
HEAD	= includes
AR		= ar -r
CC		= gcc
CFLAGS	=
RM 		= rm -f
BONUS	= ft_lstnew.c ft_lstadd_front.c ft_lstsize.c ft_lstlast.c ft_lstadd_back.c ft_lstdelone.c \
 ft_lstclear.c ft_lstiter.c 
BONUS_O	= ${BONUS:.c=.o}

all:	${NAME}

${NAME}:	${OBJS}
			${AR} ${NAME} ${OBJS}

bonus:	${BONUS_O} ${OBJS} 
		${AR} ${NAME} ${OBJS} ${BONUS_O}

clean: 
		${RM} ${OBJS} ${BONUS_O}

fclean:	clean
		${RM} ${NAME}

re:		fclean all

.PHONY:		all clean fclean re
