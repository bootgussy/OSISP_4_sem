TARGET = dirwalk

CC = gcc

CFLAGS = -Wall -Wextra -pedantic -std=c11

SRCS = dirwalk.c

OBJS = $(SRCS:.c=.o)
OBJDIR = build/debug

all: $(OBJDIR) $(TARGET)

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $@

$(OBJDIR)/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJDIR) $(TARGET)

.PHONY: all clean
