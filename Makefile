CC       = gcc
BIN      = relativity
OBJDIR   = build
SRCS     = main.c coordinate.c utils.c clear_button.c aside.c legend.c
OBJS     = $(SRCS:%.c=$(OBJDIR)/%.o)
CFLAGS   = -g3 -std=c99 -Wall -Wextra -Wpedantic -O1
RM       = rm -rf

$(BIN): $(OBJS)
	$(CC) $(OBJS) -o $(BIN) $(LIBS) -lm -lraylib

$(OBJDIR)/%.o: %.c | $(OBJDIR)
	$(CC) -c $< -o $@ $(CFLAGS)

$(OBJDIR):
	mkdir -p $(OBJDIR)

clean:
	$(RM) $(OBJDIR) $(BIN)
