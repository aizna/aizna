all:
	$(MAKE) -C aizna
	$(MAKE) -C samples

lib:
	$(MAKE) -C aizna

clean:
	$(MAKE) -C aizna clean
	$(MAKE) -C samples clean