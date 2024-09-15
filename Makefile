# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/04/13 11:30:59 by ivalimak          #+#    #+#              #
#    Updated: 2024/09/15 20:06:53 by ivalimak         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	=	libti42.a

BUILD	=	normal

CC				=	cc
cflags.common	=	-Wall -Wextra -Werror
cflags.debug	=	-g
cflags.debugm	=	$(cflags.debug) -D DEBUG_MSG=1
cflags.fsan		=	$(cflags.debug) -fsanitize=address,undefined
cflags.normal	=	-Ofast
cflags.extra	=	
CFLAGS			=	$(cflags.common) $(cflags.$(BUILD)) $(cflags.extra)

SRCDIR	=	src
OBJDIR	=	obj
INCDIR	=	inc

INC		=	-I$(INCDIR)

UTILSDIR	=	utils

UTILSFILES	=	alloc.c \
				codes.c \
				flist.c \
				hmap.c \
				list.c \
				str.c

FILES	=	getflag.c \
			getnum.c \
			getstr.c \
			load.c \
			tgoto.c \
			$(addprefix $(UTILSDIR)/, $(UTILSFILES))

SRCS	=	$(addprefix $(SRCDIR)/, $(FILES))
OBJS	=	$(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRCS))

all: $(NAME)

$(NAME): $(OBJDIR) $(OBJS)
	@printf "\e[1;34mTI42 >\e[m Creating %s\n" $@
	@ar -crs $(NAME) $(OBJS)
	@printf "\e[1;34mTI42 >\e[m \e[1mDone!\e[m\n"

$(OBJDIR):
	@printf "\e[1;34mTI42 >\e[m Creating objdirs\n"
	@mkdir -p $(OBJDIR)/$(UTILSDIR)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@printf "\e[1;34mTI42 >\e[m Compiling %s\n" $@
	@$(CC) $(CFLAGS) $(INC) -c $< -o $@

clean:
	@rm -f $(OBJS)

fclean: clean
	@rm -rf $(OBJDIR)
	@rm -f a.out
	@rm -f $(NAME)

re: fclean all

.PHONY: all test clean fclean re
