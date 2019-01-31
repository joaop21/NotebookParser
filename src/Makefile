IDIR=../include
ODIR=obj

CC=gcc
CFLAGS = -Wall -std=c11 -I$(IDIR)

DEPS=$(IDIR)/$(wildcard *.h)
SOURCES=$(wildcard *.c)
SOURCES_OBJ=$(patsubst %.c,$(ODIR)/%.o,$(SOURCES))

print-% : ; @echo $* = $($*)

$(ODIR)/%.o : %.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

program: $(SOURCES_OBJ) $(MY_LIBS_OBJ)
	$(CC) $(CFLAGS) $(wildcard $(ODIR)/*.o) -o notebook

clean:
	rm obj/*.o
	rm notebook
	rm -r ../documentacao/html

doc:
	doxygen ../documentacao/Doxyfile