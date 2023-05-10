CLIENT_PROJECT=client
CLIENT_SOURCES=client.cpp commands.cpp request.cpp helpers.cpp post.cpp buffer.cpp

LIBRARY=nope
INCPATHS=include
LIBPATHS=.
LDFLAGS=
CFLAGS=-g -c -Wall -Werror -Wno-error=unused-variable
CC=g++

# Automatic generation of some important lists
CLIENT_OBJECTS=$(CLIENT_SOURCES:.c=.o)

INCFLAGS=$(foreach TMP,$(INCPATHS),-I$(TMP))
LIBFLAGS=$(foreach TMP,$(LIBPATHS),-L$(TMP))

# Set up the output file names for the different output types
CLIENT_BINARY=$(CLIENT_PROJECT)

all: $(CLIENT_SOURCES) $(CLIENT_BINARY)

$(CLIENT_BINARY): $(CLIENT_OBJECTS)
	$(CC) $(LIBFLAGS) $(CLIENT_OBJECTS) -o $@

.c.o:
	$(CC) $(INCFLAGS) $(CFLAGS) -fPIC $< -o $@

clean:
	rm -rf server subscriber

