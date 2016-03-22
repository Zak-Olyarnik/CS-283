filenames: L4
	./L4 -p $(PATTERN) -r $(REPLACE) -a filenames -f$(FNAMES)
# removing the space between "-f$(FNAMES) turns it into an
#	optional argument when parsing with getopt() in main.c

filecontents: L4
	./L4 -p $(PATTERN) -r $(BEFORE) -a filecontents -f$(FNAMES)

L4: main.c
	gcc main.c -o L4

clean:
	rm L4
