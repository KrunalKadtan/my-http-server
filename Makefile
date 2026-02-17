
CC = gcc
CFLAGS = -Wall -Wextra -g -Iinclude
SRCDIR = src
OBJDIR = obj
BINDIR = .

# Souce files
SOURCES = $(wildcard $(SRCDIR)/*.c)
# Object files
OBJECTS = $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
# Executable name
TARGET = $(BINDIR)/server

# Default TARGET
all: $(TARGET)

# Create directories if they don't exist
$(OBJDIR):
	mkdir -p $(OBJDIR)

# Compile .c files to .o files
$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Link object files to create Executable
$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $^ -o $@

# Clean build artifacts
clean:
	rm -rf $(OBJDIR) $(TARGET)
	@echo "Clean complete"

# Rebuild everything
rebuild: clean all

# Run the server
run: $(TARGET)
	./$(TARGET)

# Help message
help:
	@echo "Available targets:"
	@echo "  all     - Build the server (default)"
	@echo "  clean   - Remove build artifacts"
	@echo "  rebuild - Clean and build"
	@echo "  run     - Build and run the server"
	@echo "  help    - Show this help message"

.PHONY: all clean rebuild run help
