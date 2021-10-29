PROGNAME = captcha-gd

all: $(PROGNAME)

clean:
	rm -f $(PROGNAME) *.o

$(PROGNAME):
	$(CC) -I/usr/local/include -o $(PROGNAME) captcha-gd.c -L/usr/local/lib -lgd -L.
