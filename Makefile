all: logParse

clean:
	-rm logParse

dt: 
	gcc -o logParse logParse.c

