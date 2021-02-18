
.PHONY: clean demo install

all:
	+$(MAKE) -C src

clean:
	+$(MAKE) clean -C src

demo:
	+$(MAKE) demo -C src

install:
	+$(MAKE) install -C src
