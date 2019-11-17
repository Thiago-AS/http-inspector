CC=g++ -std=c++11
CFLAGS=-I$(IDIR) -Wall -pthread

IDIR=../include
ODIR=obj

_DEPS=proxy.hpp helper.hpp request.hpp
DEPS=$(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ=proxy.o helper.o main.o request.o
OBJ=$(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

aracne: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ aracne *~
