SRC=$(wildcard *.mp)
all: $(SRC:.mp=.out)
	latexmk -pdf doc.tex

%.out: %.mp
	mpost $^
	rm *.log

run:
	evince -f doc.pdf&
