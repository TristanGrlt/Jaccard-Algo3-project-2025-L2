.PHONY: clean dist

dist: clean
	tar -hzcf "$(CURDIR).tar.gz" word/* opt/* main/* jaccard/* holdall/* hashtable/* makefile

clean:
	$(MAKE) -C main clean
