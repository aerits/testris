##
# testris
#
# @file
# @version 0.1
testris: main.c
	gcc -W -Wall -Wextra -pedantic -o testris main.c -Iinclude -lpthread -Llib -lSDL2 -lSDL2main -lSDL2_ttf

# end
