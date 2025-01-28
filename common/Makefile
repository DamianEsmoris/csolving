linklst.c:
	gcc -c linklst.c linklst.o

all: linklst.c

clean:
	ls *.o; read -p "are you sure to delete all *.o on the current dir? (y/N) " resp;
	if [[ "$$resp" =~ "y" ]]; then rm -fv *.o; else echo -e '\nok... operation cancelled'; fi
