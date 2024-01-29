################################################################################
##                               Présentation                                 ##
################################################################################

COLOR_NORM		=	\033[0m
COLOR_RED		=	\033[31m
COLOR_PURPLE	=	\033[35m

################################################################################
##                               SRCS                                         ##
################################################################################

MK = Makefile

# Headers

DIR_HDS				=	includes

RELATIVE_HDS		=	ft_ping.h \
						ft_stat.h

# Code

DIR_GLOBAL						=	srcs
SRC_GLOBAL						=	main.c				\
									ft_openSocket.c     \
									host.c     			\
									utils.c     			\
									memory.c     			\
									hexadump.c       \
									ft_parsing.c  		\
									ft_ping.c  			\
									ft_ping_init.c    	\
									ft_ping_tools.c  	\
									ft_signal.c  		\
									ft_stat.c 			\
									ft_stat_tool.c  	\
									help.c  			\
									tools.c

################################################################################
##                       Compilation Environnement                            ##
################################################################################

NAME	=	ft_ping
CC		=	cc
CFLAGS	=	-Wall -Werror -Wextra -O3
DEBUG   ?= 0

ifeq ($(DEBUG), 1)
    CFLAGS += -D DEBUG
endif
# -g3 -ggdb -fsanitize=address

# Sources

DIRS_SRC			=	$(DIR_GLOBAL)

DIR_OBJ				=	obj

SRCS		=	$(addprefix $(DIR_GLOBAL)/, $(SRC_GLOBAL))					\

HDS			=	$(addprefix $(DIR_HDS)/, $(RELATIVE_HDS))
OBJS		=	$(addprefix $(DIR_OBJ)/, $(SRCS:.c=.o))

DEPENDS		=	$(HDS) $(MK) $(LIBFT_NAME)

################################################################################
##                                 Règles                                     ##
################################################################################

.PHONY: all clean fclean re

all		:	$(NAME)

define src2obj
$(DIR_OBJ)/$(1)/%.o:	$(1)/%.c $(2)
	@mkdir -p $(DIR_OBJ)/$(1)
	@printf "\r\033[K\t[✅]\t$(COLOR_PURPLE)$$< --> $$@\$(COLOR_NORM)"
	@$(CC) $(CFLAGS) -c -o $$@ $$< $(INC_INC)
endef

$(foreach dir,$(DIRS_SRC),$(eval $(call src2obj,$(dir), $(DEPENDS))))

$(NAME)	: $(DEPENDS) $(OBJS)
	@printf "\n[✅]\tCompilation of $(COLOR_PURPLE)$(NAME)\$(COLOR_NORM)\n"
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJS)

clean:
	@printf "[✅]\tDelete $(COLOR_RED)object of $(DIR_GLOBAL)$(COLOR_NORM) of $(NAME)\n"
	@rm -rf $(DIR_OBJ)

fclean: clean
	@printf "[✅]\tDelete $(COLOR_RED)all binary on $(DIR_LIBFT)$(COLOR_NORM)\n"
	@rm -rf $(NAME)

send: fclean 
	@scp -r ../ft_ping vm:/home/jo/

re:	fclean all
