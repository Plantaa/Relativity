CC       = gcc
BIN      = relativity
OBJDIR   = build
SRCDIR   = src
SRCS     = $(wildcard $(SRCDIR)/*.c)
OBJS     = $(SRCS:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
CFLAGS   = -g3 -std=c99 -Wall -Wextra -Wpedantic -O1
RM       = rm -rf

$(BIN): $(OBJS)
	$(CC) $(OBJS) -o $(BIN) $(LIBS) -lm -lraylib

$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	$(CC) -c $< -o $@ $(CFLAGS)

$(OBJDIR):
	mkdir -p $(OBJDIR)

clean:
	$(RM) $(OBJDIR) $(BIN)
