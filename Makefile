CC=gcc
OUTPUT=tokenizer
CFLAGS=-Wall -g -fsanitize=address -std=c99

$(OUTPUT): $(OUTPUT).c 
	$(CC) $(CFLAGS) -o $(OUTPUT) $(OUTPUT).c

all: $(OUTPUT)

clean:
	rm -f $(OUTPUT)
