NAME = webserv
CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -MMD -g3 -std=c++98
SANITIZE =	-fsanitize=memory                                      \
						-fsanitize=alignment                                   \
						-fsanitize=bool                                        \
						-fsanitize=bounds                                      \
						-fsanitize=builtin                                     \
						-fsanitize=enum                                        \
						-fsanitize=float-cast-overflow                         \
						-fsanitize=float-divide-by-zero                        \
						-fsanitize=function                                    \
						-fsanitize=implicit-integer-sign-change                \
						-fsanitize=implicit-signed-integer-truncation          \
						-fsanitize=implicit-unsigned-integer-truncation        \
						-fsanitize=integer                                     \
						-fsanitize=local-bounds                                \
						-fsanitize=memory                                      \
						-fsanitize-memory-param-retval                         \
						-fsanitize-memory-track-origins=2                      \
						-fsanitize=nonnull-attribute                           \
						-fsanitize=null                                        \
						-fsanitize=nullability-arg                             \
						-fsanitize=nullability-assign                          \
						-fsanitize=nullability-return                          \
						-fsanitize=pointer-overflow                            \
						-fsanitize-recover=all                                 \
						-fsanitize=return                                      \
						-fsanitize=returns-nonnull-attribute                   \
						-fsanitize=shift                                       \
						-fsanitize=signed-integer-overflow                     \
						-fsanitize=undefined                                   \
						-fsanitize=unreachable                                 \
						-fsanitize=unsigned-shift-base                         \
						-fsanitize=vla-bound                                   \
						-fsafe-buffer-usage-suggestions                        \
						-fno-sanitize-trap=all 
						#-fsanitize=leak
						#-fsanitize=address 
OBJ_PATH = .obj/
SRC = srcs/main.cpp \
	  srcs/Server.cpp \
	  srcs/epoll_events.cpp \
	  srcs/print.cpp \
		srcs/signals/signals.cpp \
		srcs/parsing/parsing_utils.cpp \
		srcs/parsing/parse_config.cpp \
		srcs/parsing/location_parsing.cpp \
		srcs/Init.cpp \
		srcs/request.cpp \
		srcs/error.cpp \
		srcs/location.cpp \
		srcs/config.cpp \
		srcs/Files.cpp \
		srcs/directory_listing.cpp \
		srcs/response.cpp 
OBJ_NAME = $(SRC:.cpp=.o)
OBJ = $(addprefix $(OBJ_PATH), $(OBJ_NAME))

.DEFAULT_GOAL := all

$(NAME): $(OBJ)
	@$(CXX) $(CXXFLAGS) -o $(NAME) $(OBJ)

$(OBJ_PATH)%.o: %.cpp
	@mkdir -p $(@D)
	@$(CXX) $(CXXFLAGS) -o $@ -c $<

-include $(OBJ:%.o=%.d)

all: $(NAME)

clean:
	rm -rf .obj

fclean: clean
	rm -rf $(NAME)

re:
	@$(MAKE) -s fclean all

.PHONY: all clean fclean re
