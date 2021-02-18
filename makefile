
.PHONY: clean demo install uninstall

all:
	+$(MAKE) -C src

clean:
	+$(MAKE) clean -C src

demo:
	+$(MAKE) demo -C src

install:
	+$(MAKE) install -C src

uninstall:
	+$(MAKE) uninstall -C src
