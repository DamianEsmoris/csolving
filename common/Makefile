linklst:
	gcc -c linklst.c linklst.o

all: linklst

clean:
	echo `ls *.o; echo; read -p "are you sure to delete all *.o on the current dir? (y/N) " resp; [[ $${resp,,} =~ (y|yes) ]] && rm -fv *.o || echo -e '\nok... operation cancelled'`
