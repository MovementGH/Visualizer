CC=g++
CFLAGS=-Ofast -fPIC -I$(IDIR)

IDIR=../include
ODIR=obj
BDIR=../build
DDIR=../demo

LIBS=-lsfml-audio -lsfml-graphics -lsfml-window -lsfml-system -lpthread

_DEPS = Inputs.hpp Plugins.hpp Util.hpp Visualizer.hpp Sprite3d.hpp
DEPS=$(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = Inputs.o Plugins.o Util.o Visualizer.o Sprite3d.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: %.cpp $(DEPS)
	mkdir -p $(ODIR)
	$(CC) -c -o $@ $< $(CFLAGS)

libvisualizer.so: $(OBJ)
	mkdir -p $(BDIR)
	$(CC) -shared -o $(BDIR)/$@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean demo install

clean:
	rm -rf $(ODIR)
	rm -rf $(BDIR)
	rm -rf $(DDIR)/visualizer

install: libvisualizer.so
	cp $(IDIR)/* /usr/local/include
	cp $(BDIR)/* /usr/local/lib

demo: $(OBJ) $(ODIR)/main.o
	$(CC) -o $(DDIR)/visualizer $^ $(CFLAGS) $(LIBS)
