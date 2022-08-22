/* -------------------------------------------------------------------------- */
// Copyright (c) 2022. Nikola Vukićević
/* -------------------------------------------------------------------------- */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define BROJ_MESTA           4
#define BROJ_BOJA            6
#define BROJ_POKUSAJA        8
#define DUZINA_REDA_POKUSAJI 23
#define DONJA_GRANICA        1111
#define GORNJA_GRANICA       6666
#define ZNAK_CRNO            '*'
#define ZNAK_BELO            '+'
#define DEBUG                0
#define CHEAT                0
/* -------------------------------------------------------------------------- */
void reset_strukture_pokusaji(char* p)
{
	int d = BROJ_POKUSAJA * DUZINA_REDA_POKUSAJI;
	int i = 0;

	while (i < d) {
		*(p + i) = ' ';
		++i;
	}
}
/* -------------------------------------------------------------------------- */
void ispis_strukture_pokusaji(char* p, int sirina)
{
	int i, j;

	for(i = 0; i < BROJ_POKUSAJA; ++i) {
		for(j = 0; j < sirina; ++j) {
			printf("%c ", *(p + i * sirina + j));
		}

		printf("\n");
	}
}
/* -------------------------------------------------------------------------- */
void generisanje_lozinke(int lozinka[4])
{
	int i;

	if(!DEBUG) srand(time(NULL));

	for(i = 0; i < BROJ_MESTA; ++i) {
		lozinka[i] = rand() % BROJ_BOJA + 1;
	}
}
/* -------------------------------------------------------------------------- */
void ispis_maske()
{
	printf("= ------------------ =\n");
	printf("=    Master Mind     =\n");
	printf("= ------------------ =\n");
}
/* -------------------------------------------------------------------------- */
void ispis_pojedinacnog_reda(char* p, int red, int sirina)
{
	int i = 0;
	
	while(i < sirina) {
		printf("%c", *(p + red * sirina + i));
		++i;
	}

	printf("\n");
}
/* -------------------------------------------------------------------------- */
void ispis_dosadasnjih_pokusaja(char* p, int red, int sirina)
{
	int i = 0;

	while(i < red) {
		ispis_pojedinacnog_reda(p, i, sirina);
		++i;
	}
}
/* -------------------------------------------------------------------------- */
int da_li_je_u_granicama(int n)
{
	return n >= DONJA_GRANICA && n <= GORNJA_GRANICA;
}
/* -------------------------------------------------------------------------- */
int smestanje_pokusaja_u_niz(int pokusaj_niz[], int pokusaj_int)
{
	int i;
	for(i = 0; i < BROJ_MESTA; ++i) pokusaj_niz[i] = 0;
	i = BROJ_MESTA - 1;
	while(pokusaj_int > 0) {
		pokusaj_niz[i] = pokusaj_int % 10;
		if(pokusaj_niz[i] < 1 || pokusaj_niz[i] > BROJ_BOJA) return 0;
		pokusaj_int   /= 10;
		--i;
	}

	return 1;
}
/* -------------------------------------------------------------------------- */
void ocenjivanje(int lozinka[], int pokusaj[], int ocena[], int d_n)
{
	int i, prebrojavanje[7] = { 0, 0, 0, 0, 0, 0, 0 };

	ocena[0] = 0;
	ocena[1] = 0;

	for(i = 0; i < d_n; i++){
		if (pokusaj[i] == lozinka[i]) {
			++ocena[0];
		}
		else {
			++prebrojavanje[lozinka[i]];
		}
	}

	for(i = 0; i < d_n; ++i) {
		if(pokusaj[i] == lozinka[i])      continue;
		if(prebrojavanje[pokusaj[i]] < 1) continue;
		++ocena[1];
		--prebrojavanje[pokusaj[i]];
	}
}
/* -------------------------------------------------------------------------- */
void upis_pokusaja(char* pokusaji, int red, int pokusaj_niz[], int ocena_niz[], int sirina)
{
	--red;
	*(pokusaji + red * sirina + 0)  = red + 1 + '0';
	*(pokusaji + red * sirina + 1)  = ':';
	*(pokusaji + red * sirina + 3)  = pokusaj_niz[0] + 48;
	*(pokusaji + red * sirina + 5)  = pokusaj_niz[1] + 48;
	*(pokusaji + red * sirina + 7)  = pokusaj_niz[2] + 48;
	*(pokusaji + red * sirina + 9)  = pokusaj_niz[3] + 48;
	*(pokusaji + red * sirina + 11) = '[';
	*(pokusaji + red * sirina + 21) = ']';
}
/* -------------------------------------------------------------------------- */
void upis_ocene(char* pokusaji, int red, int pokusaj_niz[], int ocena_niz[], int sirina)
{
	--red;
	int i    = 13,
		crni = ocena_niz[0],
	    beli = ocena_niz[1];
	
	while (crni > 0) {
		*(pokusaji + red * sirina + i) = ZNAK_CRNO;
		--crni;
		i += 2;
	}

	while (beli > 0) {
		*(pokusaji + red * sirina + i) = ZNAK_BELO;
		--beli;
		i += 2;
	}
}
/* -------------------------------------------------------------------------- */
void praznjenje_bafera()
{
	char c = 'a';
	while(c != '\n' && c != EOF) {
		c = getchar();
	}
}
/* -------------------------------------------------------------------------- */
void obrada_ulaza(char* pokusaji, int* red, int* prekid_korisnik, int lozinka[], int pokusaj[], int ocena[], int sirina)
{
	int p, r;

	printf("%d: ", *red + 1);

	r = scanf("%d", &p);
	praznjenje_bafera(); // bitno!
	
	if(r != 1)                                return;

	*prekid_korisnik = p == 11;

	if(*prekid_korisnik)                      return;
	if(!da_li_je_u_granicama(p))              return;
	if(!smestanje_pokusaja_u_niz(pokusaj, p)) return;

	++(*red);
	ocenjivanje(lozinka, pokusaj, ocena, BROJ_MESTA);
	upis_pokusaja(pokusaji, *red, pokusaj, ocena, sirina);
	upis_ocene(pokusaji, *red, pokusaj, ocena, sirina);
}
/* -------------------------------------------------------------------------- */
void pokusaj_izlaza(int* reseno, int ocena[], int* red)
{
	*reseno = ocena[0] == BROJ_MESTA; 
}
/* -------------------------------------------------------------------------- */
void rutina_poslednji_red(char* pokusaji, int lozinka[], int reseno, int red, int sirina)
{
	system("clear");
	ispis_maske();
	ispis_dosadasnjih_pokusaja(pokusaji, red, sirina);

	printf("----------------------\n");
	printf("l: %d %d %d %d [ x x x x ]\n", lozinka[0], lozinka[1], lozinka[2], lozinka[3]);
	
	if(reseno) {
		printf("POGODAK!\n");
	}
	else {
		printf("Nema pogotka. :(\n");
	}

	printf("----------------------\n");
}
/* -------------------------------------------------------------------------- */
void ispis_debug(int lozinka[], int pokusaj[], int ocena[], int d_n)
{
	int i;
	printf("%d %d %d %d\n", lozinka[0], lozinka[1], lozinka[2], lozinka[3]);
	printf("%d %d %d %d -> ", pokusaj[0], pokusaj[1], pokusaj[2], pokusaj[3]);
	printf("[ ");

	for(i = 1; i <= ocena[0]; i++) {
		printf("%c ", ZNAK_CRNO);
	}

	for(i = 1; i <= ocena[1]; i++) {
		printf("%c ", ZNAK_BELO);
	}

	printf("]\n--------------------\n");
}
/* -------------------------------------------------------------------------- */
int _debug()
{
	int ulaz   = 0,
		reseno = 0,
	    red    = 0,
		i;

	int lozinka[BROJ_MESTA] = { 1 , 1 , 1 , 1 },
		pokusaj[BROJ_MESTA] = { 1 , 1 , 1 , 1 },
		ocena[BROJ_MESTA]   = { 0 , 0 };
	
	char* pokusaji = malloc(BROJ_POKUSAJA * DUZINA_REDA_POKUSAJI);
	
	for(i = 1; i <= 1000; i++) {
		reset_strukture_pokusaji(pokusaji);
		/** ispis_strukture_pokusaji(pokusaji); */
		generisanje_lozinke(lozinka);
		generisanje_lozinke(pokusaj);
		ocenjivanje(lozinka, pokusaj, ocena, BROJ_MESTA);
		ispis_debug(lozinka, pokusaj, ocena, BROJ_MESTA);
	}

	return 0;
}
/* -------------------------------------------------------------------------- */
int main()
{
	if(DEBUG) {
		_debug();
		return 0;
	}

	int ulaz            = 0,
		reseno          = 0,
		prekid_korisnik = 0,
	    red             = 0;

	int lozinka[BROJ_MESTA] = { 1 , 1 , 1 , 1 },
		pokusaj[BROJ_MESTA] = { 1 , 1 , 1 , 1 },
		ocena[BROJ_MESTA]   = { 2 , 1 };

	char* pokusaji = malloc(BROJ_POKUSAJA * DUZINA_REDA_POKUSAJI);
	
	reset_strukture_pokusaji(pokusaji);
	generisanje_lozinke(lozinka);
	
	while (!reseno && !prekid_korisnik && red < BROJ_POKUSAJA) {
		system("clear");
		ispis_maske();
		if(CHEAT) printf("l: %d %d %d %d\n", lozinka[0], lozinka[1], lozinka[2], lozinka[3]);
		ispis_dosadasnjih_pokusaja(pokusaji, red, DUZINA_REDA_POKUSAJI);
		obrada_ulaza(pokusaji, &red, &prekid_korisnik, lozinka, pokusaj, ocena, DUZINA_REDA_POKUSAJI);	
		pokusaj_izlaza(&reseno, ocena, &red);
	}

	rutina_poslednji_red(pokusaji, lozinka, reseno, red, DUZINA_REDA_POKUSAJI);

	return 0;
}
/* -------------------------------------------------------------------------- */

