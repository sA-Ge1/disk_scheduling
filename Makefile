CC = gcc
CFLAGS = -Wall -Iinclude
AR = ar
ARFLAGS = rcs
SRC_DIR = src
OBJ_DIR = obj
LIB_DIR = lib

SOURCES = $(wildcard $(SRC_DIR)/*.c)
OBJECTS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SOURCES))

LIBRARY = $(LIB_DIR)/libdisk_sched.a

all: $(LIBRARY) main

# Compile main program and link with the static library
main: main.c $(LIBRARY)
	$(CC) $(CFLAGS) main.c -L$(LIB_DIR) -ldisk_sched -o disk_scheduler

# Create the static library
$(LIBRARY): $(OBJECTS)
	@mkdir -p $(LIB_DIR)
	$(AR) $(ARFLAGS) $@ $^

# Compile source files to object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up build files
clean:
	rm -rf $(OBJ_DIR) $(LIB_DIR) disk_scheduler
