#include <stdio.h>
#include <stdint.h>

// ASCII-Codes fuer Farben - funktioniert nativ nur auf Linux und Mac!
#define RESET "\033[0m"
#define ROT "\033[31m"
#define GRUEN "\033[32m"
#define GELB "\033[33m"
#define BLAU "\033[34m"

int Spielfeld [6][7]; // Das zweidimensionale Array fuer das Spielfeld.
int minZeile[7]; // Das Array, das die aktuell niedrigste Stelle des Spielfelds fuer jede Spalte angibt.

/**
 * Prueft, ob in umliegenden Feldern Steine vom gleichen Spieler eingeworfen
 * wurden und gibt die Anzahl der zusammenhaengenden Steine -1 zurueck.
 */
int pruefen(int x, int y, int xdir, int ydir, int Spieler) {
    if (x >= 0 && x < 6 && y >= 0 && y < 7) {
	    if (Spielfeld[x][y] == Spieler) {
    	    return pruefen(x + xdir, y + ydir, xdir, ydir, Spieler) + 1;
        }
    }
    return 0;
 }

/**
 * Prueft, ob der uebergebene Spieler an dieser Stelle gewonnen hat.
 */
int gewonnen(int x, int y, int Spieler) {
    return pruefen(x - 1, y, -1, 0, Spieler) + pruefen(x + 1, y, 1, 0, Spieler) >= 3 || // waagerecht
	      pruefen(x, y - 1, 0, -1, Spieler) + pruefen(x, y + 1, 0, 1, Spieler) >= 3 || // senkrecht
	      pruefen(x - 1, y - 1, -1, -1, Spieler) + pruefen(x + 1, y + 1, 1, 1, Spieler) >= 3 || // diagonal von links unten nach rechts oben
	      pruefen(x - 1, y + 1, -1, 1, Spieler) + pruefen(x + 1, y - 1, 1, -1, Spieler) >= 3; // diagonal von links unten nach rechts oben
}

/**
 * Gibt 1 zurueck, wenn kein Zug mehr moeglich ist.
 */
int unentschieden() {
	int u=0;
	for (int i=0;i<6;i++) {
		for (int j=0;j<7;j++) {
			if (Spielfeld[i][j]==0) {
				u=1;
			}
		}
	}
	if (u==0) {
		return 1;
	}
	return 0;
}

/**
 * "Wirft" den tatsaechlichen Spielstein in das Feld, schreibt also die Zahl des
 * Spielers in das Spielfeld-Array.
 */
void Stelleaendern(int Spieler, int x) {
	if (Spieler==1) {
		Spielfeld[minZeile[x-1]][x-1]=1;
		minZeile[x-1]--;
	}
    if (Spieler==2) {
		Spielfeld[minZeile[x-1]][x-1]=2;
		minZeile[x-1]--;
	}
}

/**
 * Gibt das Spielfeld auf der Konsole aus - mit Farben!
 */
void ausgeben() {
	for (int i=0;i<6;i++) {
		for (int j=0;j<7;j++) {
			printf("%s%d%s  ", (Spielfeld[i][j] == 1) ? ROT : (Spielfeld[i][j] == 2 ? GELB : GRUEN), Spielfeld[i][j], RESET);
		}
	    printf("\n");
	}
}

/**
 * Die Methode, fuer das Spiel, wobei G die Zugnummer angibt.
 */
void Spiel(int S, int G){
	printf("\n%sVier Gewinnt:%s\n\n", BLAU, RESET);

    ausgeben();

    // Spieler 1 ist immer bei geraden Zugnummern am Zug.
    if (G%2==0) {
    	printf("\n%sSpieler1%s: In welche Spalte möchten Sie legen?\n", ROT, RESET);
    	scanf("%d", &S);

        while(S<0||S>7) {
        	printf("\n%sSpieler1%s: Falsche Eingabe! Bitte Zahl zwischen 1 und 7 eingeben!\n", ROT, RESET);
        	scanf("%d", &S);
        }
        while (minZeile[S-1]==-1) {
           	printf("\n%sSpieler1%s: Falsche Eingabe! Bitte andere Spalte setzen!\n", ROT, RESET);
           	scanf("%d", &S);
        }
    	if (S>0 && S<=7) {
    		Stelleaendern(1,S); // Spielstein einwerfen
    		if (gewonnen(minZeile[S-1]+1, S-1, 1)==1) {
    		    ausgeben();
    		    printf("%sSpieler 1%s hat gewonnen!!\n", ROT, RESET);
    		}
    		if (unentschieden()==1) {
    		    ausgeben();
    	        printf("\n\nUnentschieden!!");
    		}
    		if (gewonnen(minZeile[S-1]+1, S-1, 1)==0 && unentschieden()==0) {
    		    Spiel(1,G+1);
    		}
    	}
    }

    // Spieler 2 ist immer bei ungeraden Zugnummern am Zug.
    if (G%2==1) {
    	printf("\n%sSpieler2%s: In welche Spalte möchten Sie legen?\n", GELB, RESET);
    	scanf("%d", &S);

        while(S<0||S>7) {
       	    printf("\n%sSpieler2%s: Falsche Eingabe! Bitte Zahl zwischen 1 und 7 eingeben!\n", GELB, RESET);
       	    scanf("%d", &S);
       	}

        while (minZeile[S-1]==-1) {
        	printf("\n%sSpieler2%s: Falsche Eingabe! Bitte andere Spalte setzen!\n", GELB, RESET);
        	scanf("%d", &S);
        }

       	if (S>0 && S<=7 && minZeile[S]>=0) {
       		Stelleaendern(2,S);
       		if (gewonnen(minZeile[S-1]+1, S-1, 2)==1){
       			ausgeben();
       			printf("\n%sSpieler 2%s hat gewonnen!!", GELB, RESET);
       		}
       		if (unentschieden()==1){
       		    ausgeben();
       		 	printf("\nUnentschieden!!");
       		}
       		if (gewonnen(minZeile[S-1]+1, S-1, 2)==0 && unentschieden()==0){
       			Spiel(2,G+1);
       		}
   		}
    }
}

int main (void){
	for (int i=0;i<6;i++){
		for (int j=0;j<7;j++){
			Spielfeld[i][j]=0;
			minZeile[j]=5;
		}
	}
	Spiel(0,0);
}
