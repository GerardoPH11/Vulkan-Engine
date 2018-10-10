IDIR=include
ODIR=intermediate
SDIR=src
LIBS=-lvulkan
CC=g++-8
CFLAGS=-I$(IDIR) -std=c++17
_DEPS = vulkan.h utils.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))
_OBJ = main.o utils.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: $(SDIR)/%.cpp
	$(CC) -c -o $@ $< $(CFLAGS)

vulkan: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 
