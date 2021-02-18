
CC=g++
CFLAGS=-Ofast -fPIC -I$(IDIR) -w

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
	$(CC) -c -o $@ $< $(CFLAGS)


.PHONY: clean demo install prep
.SECONDARY: build build-demo

all: prep build

demo: prep build-demo

prep:
	mkdir -p $(ODIR)
	mkdir -p $(BDIR)

build: $(OBJ)
	$(CC) -shared -o $(BDIR)/libvisualizer.so $^ $(CFLAGS) $(LIBS)

build-demo: $(OBJ) $(ODIR)/demo.o
	$(CC) -o $(DDIR)/visualizer $^ $(CFLAGS) $(LIBS)

clean:
	rm -rf $(ODIR)
	rm -rf $(BDIR)
	rm -rf $(DDIR)/visualizer

install: prep build
	cp $(IDIR)/* /usr/local/include
	cp $(BDIR)/* /usr/local/lib
	ldconfig

uninstall:
	rm -f /usr/local/lib/libvisualizer.so
	rm -f `fdupes --omitfirst ../include /usr/local/include`
	ldconfig