COMPILER = gcc
FLAGS = -Wall -Wextra -Wall -g -std=c89
ROOT = ../..
BUILD_DIR = build
TARGET = test_list

SOURCES = \
$(ROOT)/Unity/src/unity.c \
$(ROOT)/list/list.c \
test_list.c

INCLUDES = \
-I$(ROOT)/Unity/src \
-I$(ROOT)/list/

FLAGS += $(INCLUDES)


OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(SOURCES)))

$(BUILD_DIR)/%.o: %.c | $(BUILD_DIR)
	$(COMPILER) -c $(FLAGS) $< -o $@

$(TARGET): $(OBJECTS) | $(BUILD_DIR)
	$(COMPILER) $(FLAGS) -o $(BUILD_DIR)/$(TARGET) $(OBJECTS)

$(BUILD_DIR):
	mkdir $@

clean:
	rm -rf build/
