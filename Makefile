# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: user42 <user42@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/01/21 14:58:12 by pgueugno          #+#    #+#              #
#    Updated: 2021/06/14 18:06:08 by user42           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

################################################################################
#                               Filename output                                #
################################################################################

NAME_MINISHELL		=	minishell

#TEMP A SUPPRIMER ENSUITE
NAME_J			=	jprog
NAME_P			=	pprog

################################################################################
#                         Sources and objects directories                      #
################################################################################

HEADERS_DIR	=	header
SRCS_DIR	=	srcs
OBJS_DIR	=	objs
LIBS_DIR	=	libs
LIBFT_DIR	=	libft
LIBFT_MAKE	=	Makefile
LIB_LIBFT_DIR	= $(LIBS_DIR)/libft.a

################################################################################
#                               Sources filenames                              #
################################################################################

SRCS_MINISHELL		=	main.c prompt.c tokenize.c utils.c exit.c exec_command.c\
						gbcollector.c builtins.c quote.c parse.c pipeline.c\
						expand_env.c parse_env.c redirect.c

################################################################################
#                              Commands and arguments                          #
################################################################################

CC			=	@gcc
CFLAGS		=	-Wall -Wextra -Werror -g3 -I$(HEADERS_DIR) -fsanitize=address
LDFLAGS 	=	-L . $(LIB_LIBFT_DIR) -lreadline -fsanitize=address
RM			=	@rm -f

################################################################################
#                                 Defining colors                              #
################################################################################

_RED		=	\033[31m
_GREEN		=	\033[32m
_YELLOW		=	\033[33m
_CYAN		=	\033[96m

################################################################################
#                                  Compiling                                   #
################################################################################

OBJS_MINISHELL	=	$(addprefix $(OBJS_DIR)/, $(SRCS_MINISHELL:.c=.o))

#TEMP A SUPPRIMER ENSUITE
OBJS_J		=	$(addprefix $(OBJS_DIR)/, $(SRCS_J:.c=.o))
OBJS_P	=	$(addprefix $(OBJS_DIR)/, $(SRCS_P:.c=.o))

all: init_libft init_m $(NAME_MINISHELL)

install_pkg:
			echo "$(_YELLOW)[Downloading readline function dependancies]";\
			echo "user42" | sudo -S apt install libreadline-dev >/dev/null 2>&1
			echo "$(_YELLOW)[Readline function dependancies installed]"

init_m:
		if test -f $(NAME_MINISHELL);\
		then echo "$(_CYAN)[minishell program already created]";\
		else echo "$(_YELLOW)[Initialize minishell program]";\
		fi

init_libft:
			if test -f $(LIB_LIBFT_DIR); \
			then echo "$(_CYAN)[libft.a already created]";\
			else make -j -C $(LIBFT_DIR) -f $(LIBFT_MAKE);\
			fi

$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.c
				@ echo "\t$(_YELLOW) compiling... $*.c"
				$(CC) $(CFLAGS) -c $< -o $@


$(NAME_MINISHELL):	$(OBJS_MINISHELL)
					echo "\t$(_YELLOW)[Creating minishell program]"
					$(CC) $(OBJS_MINISHELL) -o $(NAME_MINISHELL) $(LDFLAGS)
					echo "$(_GREEN)[minishell program created & ready]"

clean:
		echo "$(_RED)[cleaning up .out & objects files]"
		$(RM) $(OBJS_MINISHELL)
		make -C $(LIBFT_DIR) -f $(LIBFT_MAKE) clean

fclean: clean
		echo "$(_RED)[cleaning up .out, objects & library files]"
		$(RM) $(NAME_MINISHELL)
		make -C $(LIBFT_DIR) -f $(LIBFT_MAKE) fclean

re:	fclean all

.SILENT:
		all
.PHONY: all clean fclean re bonus
