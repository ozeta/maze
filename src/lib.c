#include <ctype.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <curses.h>
#include "./headers/structures.h"
#include "./headers/lib.h"
/**
apre uno stream su file
*/
FILE *openStream ( const char *path, const char *mode ) {

	FILE *stream = fopen ( path, mode );

	return stream;
}

/**
chiude uno stream su file
*/
void closeStream ( FILE *stream ) {

	fclose ( stream );
}

void clear_screen( ) {
	#ifdef WINDOWS
		system("cls");
	#else
		system ("clear");
	#endif
}

/**
controlla che il puntatore a test non sia nullo
*/
int check ( void *test, const char *message ) {

	if ( test )
		return 1;
	else {
		char mess[256];
		sprintf (mess, "%s: unable to read memory.", message );
		//dialogPanel ( mess );
		return 0;
	}
}


int UserIntGet (int inf, int sup) {

	char tempString[256];
	int check = 0;
	int res = inf;
	int i;
    while (check == 0 || (res > sup || res < inf)) {
		//dialogPanel ("inserire un peso per l'arco compreso tra i valori");
		scanf ("%s", tempString);
		i = 0;
		if (tempString[i] == '-' || isdigit (tempString[i]) != 0 ) {
			i++;
			while (tempString[i] != '\0' && isdigit (tempString[i]) != 0 ) {
					i++;
			}
			if (tempString[i] == '\0') {
				check = 1;
			}
		} else {
			check = 0;
			//dialogPanel ("valore inserito non valido");
		}
	    res = atoi (tempString);
    }
    return res;
}
