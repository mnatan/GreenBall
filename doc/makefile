CC=pdflatex
CFLAGS=
DIAGRAMS=diagrams
SRC=$(shell find $(DIAGRAMS) -type f -name '*.mp')
all: diagrams
	$(CC) $(CFLAGS) doc.tex
	rm *.log *.aux

diagrams: $(SRC:.mp=.out)

%.out: %.mp
	mpost --tex=mex $^

run:
	evince -f doc.pdf
