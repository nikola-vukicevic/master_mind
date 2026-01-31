/* -------------------------------------------------------------------------- */
// Copyright (c) 2022. Nikola Vukićević
/* -------------------------------------------------------------------------- */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
// ------------------------------
#define BROJ_MESTA           4
#define BROJ_BOJA            6
#define BROJ_POKUSAJA        6
#define SIRINA_ULAZ          5   // BROJ_MESTA + 1
#define SIRINA_BAFER_ULAZ    100 // Br. dozvoljenih zankova pri upisu kombinacije
#define MAX_BROJ_MESTA       8
#define MAX_BROJ_BOJA        8
#define SIRINA_REDA_POKUSAJI 22 // BROJ_MESTA * 4 + 3 ("1: ") + 3 (dve zagrade [] i razmak)
#define ZNAK_CRNO            '*'
#define ZNAK_BELO            '+'
#define ZNAK_MIN_CIFRA       '1'
#define ZNAK_MAX_CIFRA       '6'
#define CHAR_0               48
#define DEBUG                0
#define CHEAT                0
// #define DONJA_GRANICA        1111 // premešteno u init funkciju
// #define GORNJA_GRANICA       6666 // premešteno u init funkciju
/* -------------------------------------------------------------------------- */
void reset_strukture_pokusaji(char *pokusaji, int broj_pokusaja, int sirina_reda_pokusaji)
{
	int d = broj_pokusaja * sirina_reda_pokusaji;
	int i = 0;

	while (i <= d) {
		*(pokusaji + i) = ' ';
		++i;
	}
}
/* -------------------------------------------------------------------------- */
void ispis_strukture_pokusaji(char *pokusaji, int sirina, int broj_pokusaja)
{
	int i, j;

	for (i = 0; i < broj_pokusaja; ++i) {
		for (j = 0; j < sirina; ++j) {
			printf("%c ", *(pokusaji + i * sirina + j));
		}

		printf("\n");
	}
}
/* -------------------------------------------------------------------------- */
void generisanje_lozinke(int lozinka[], int duzina)
{
	if (!DEBUG) {
		srand(time(NULL));
	}

	for (int i = 0; i < duzina; ++i) {
		lozinka[i] = rand() % BROJ_BOJA + 1;
	}
}
/* -------------------------------------------------------------------------- */
void ispis_maske(int br_mesta)
{
	switch (br_mesta) {
		case 4:
			printf("= ------------------ =\n");
			printf("=    Master Mind     =\n");
			printf("= ------------------ =\n");
		break;
		case 5:
			printf("= ---------------------- =\n");
			printf("=      Master Mind       =\n");
			printf("= ---------------------- =\n");
		break;
		case 6:
			printf("= -------------------------- =\n");
			printf("=        Master Mind         =\n");
			printf("= -------------------------- =\n");
		break;
		default: break;
	}
}
/* -------------------------------------------------------------------------- */
void ispis_pojedinacnog_reda(char *str_red, int red, int sirina)
{
	int i = 0;

	while (i < sirina) {
		printf("%c", *(str_red + red * sirina + i));
		++i;
	}

	printf("\n");
}
/* -------------------------------------------------------------------------- */
void ispis_dosadasnjih_pokusaja(char *str_red, int red, int sirina)
{
	int i = 0;

	while (i < red) {
		ispis_pojedinacnog_reda(str_red, i, sirina);
		++i;
	}
}
/* -------------------------------------------------------------------------- */
int da_li_je_u_granicama(int n, int donja_granica, int gornja_granica)
{
	return n >= donja_granica && n <= gornja_granica;
}
/* -------------------------------------------------------------------------- */
int smestanje_pokusaja_u_niz(int pokusaj_niz[], int pokusaj_int, int broj_mesta)
{
	for (int i = 0; i < broj_mesta; ++i)
		pokusaj_niz[i] = 0;

	int i = broj_mesta - 1;

	while (pokusaj_int > 0) {
		pokusaj_niz[i] = pokusaj_int % 10;

		if (pokusaj_niz[i] < (ZNAK_MIN_CIFRA - CHAR_0) ||
		    pokusaj_niz[i] > (ZNAK_MAX_CIFRA - CHAR_0)) {
			return 0;
		}

		pokusaj_int /= 10;
		--i;
	}

	return 1;
}
/* -------------------------------------------------------------------------- */
void ocenjivanje(int lozinka[], int pokusaj[], int ocena[], int d_n)
{
	int prebrojavanje[MAX_BROJ_BOJA + 1] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };

	ocena[0] = 0;
	ocena[1] = 0;

	for (int i = 0; i < d_n; ++i) {
		if (pokusaj[i] == lozinka[i]) {
			++ocena[0];
		} else {
			++prebrojavanje[lozinka[i]];
		}
	}

	for (int i = 0; i < d_n; ++i) {
		if (pokusaj[i] == lozinka[i]) {
			continue;
		}

		if (prebrojavanje[pokusaj[i]] < 1) {
			continue;
		}
		
		++ocena[1];
		--prebrojavanje[pokusaj[i]];
	}
}
/* -------------------------------------------------------------------------- */
void upis_pokusaja(char *pokusaji, int red, int pokusaj_niz[], int ocena_niz[], int sirina, int broj_mesta)
{
	--red;

	*(pokusaji + red * sirina + 0)  = red + 1 + '0';
	*(pokusaji + red * sirina + 1)  = ':';
	*(pokusaji + red * sirina + 2)  = ' ';

	int indeks_upis = 3;

	for (int i = 0; i < broj_mesta; ++i ) {
		*(pokusaji + red * sirina + indeks_upis)     = pokusaj_niz[i] + CHAR_0;
		*(pokusaji + red * sirina + indeks_upis + 1) = ' ';
		indeks_upis += 2;
	}
}
/* -------------------------------------------------------------------------- */
void upis_ocene(char *pokusaji, int red, int pokusaj_niz[], int ocena_niz[], int sirina, int broj_mesta)
{
	--red;

	int i    = broj_mesta * 2 + 3, // 3 = sirina niske "#: " na početku reda
		crni = ocena_niz[0],       // (npr. "1: ")
		beli = ocena_niz[1];

	*(pokusaji + red * sirina + i)     = '[';
	*(pokusaji + red * sirina + i + 1) = ' ';

	i += 2;

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

	*(pokusaji + (red + 1) * sirina - 1) = ']';
}
/* -------------------------------------------------------------------------- */
void praznjenje_bafera()
{
	char c = 'a';

	while (c != '\n' && c != EOF) {
		c = getchar();
	}
}
/* -------------------------------------------------------------------------- */
int formiranje_ulaz_int(char *str)
{
	int i      = BROJ_MESTA - 1;
	int izlaz  = 0;
	int stepen = 1;

	while (i >= 0) {
		izlaz = (str[i] - CHAR_0) * stepen + izlaz;
		stepen *= 10;
		--i;
	}

	return izlaz;
}
/* -------------------------------------------------------------------------- */
void obrada_ulaza_parser(char *s, int *prekid_korisnik, int *ulaz_int, int donja_granica, int broj_mesta)
{
	int i = 0, n = 0;
	char *str_p = (char *) malloc(MAX_BROJ_MESTA + 1);

	while (*(s + i) && n < broj_mesta) {
		char c = *(s + i);

		// Dozvoljeni znakovi su:
		// - cifre (između min cifre i max cifre)
		// - znak 'q' (samo ako je na prvom mestu)
		if ((c >= ZNAK_MIN_CIFRA && c <= ZNAK_MAX_CIFRA) || (c == 'q' && i == 0)) {
			str_p[n] = c;
			n++;
		}

		++i;
	}

	if (n == 1 && str_p[0] == 'q') {
		*prekid_korisnik = 1;
		*ulaz_int        = donja_granica;
		return;
	}

	if (n != BROJ_MESTA) {
		*ulaz_int = -1;
		return;
	}

	*ulaz_int = formiranje_ulaz_int(str_p);
}
/* -------------------------------------------------------------------------- */
void generisanje_donje_i_gornje_granice(int *donja_granica, int *gornja_granica)
{
	int i = 1;

	*donja_granica  = ZNAK_MIN_CIFRA - CHAR_0;
	*gornja_granica = ZNAK_MAX_CIFRA - CHAR_0;

	while (i < BROJ_MESTA) {
		*donja_granica  = (*donja_granica)  * 10 + ZNAK_MIN_CIFRA - CHAR_0;
		*gornja_granica = (*gornja_granica) * 10 + ZNAK_MAX_CIFRA - CHAR_0;
		++i;
	}
}
/* -------------------------------------------------------------------------- */
void obrada_ulaza(char *pokusaji, int *red, int *prekid_korisnik, int lozinka[], int pokusaj[], int ocena[], int sirina, int donja_granica, int gornja_granica, int broj_mesta)
{
	int  ulaz_int, rez_scanf;
	char *ulaz_str = (char *) malloc(SIRINA_BAFER_ULAZ + 1);

	printf("%d: ", *red + 1);

	// rez_scanf = scanf("%SIRINA_ULAZ[^\n]s", ulaz_str);
	rez_scanf = scanf("%s", ulaz_str);
	praznjenje_bafera(); // bitno!

	if (rez_scanf != 1)
		return;

	obrada_ulaza_parser(ulaz_str, prekid_korisnik, &ulaz_int, donja_granica, broj_mesta);

	if (*prekid_korisnik)
		return;

	if (!da_li_je_u_granicama(ulaz_int, donja_granica, gornja_granica))
		return;

	if (!smestanje_pokusaja_u_niz(pokusaj, ulaz_int, broj_mesta))
		return;

	++(*red);

	ocenjivanje(lozinka, pokusaj, ocena, BROJ_MESTA);
	upis_pokusaja(pokusaji, *red, pokusaj, ocena, sirina, broj_mesta);
	upis_ocene(pokusaji, *red, pokusaj, ocena, sirina, broj_mesta);
}
/* -------------------------------------------------------------------------- */
void pokusaj_izlaza(int *reseno, int ocena[], int *red, int broj_mesta)
{
	*reseno = ocena[0] == broj_mesta;
}
/* -------------------------------------------------------------------------- */
void ispis_donje_linije(int broj_mesta)
{
	int n = broj_mesta * 4 + 6; // Praktično: SIRINA_REDA_POKUSAJI
								// (Gore stoji šta znači "+7")

	char s[n + 1];

	for (int i = 0; i < n; ++i)
		s[i] = '-';

	s[n] = '\0';

	printf("%s\n", s);
}
/* -------------------------------------------------------------------------- */
void ispis_lozinke(int *lozinka, int broj_mesta, int reseno)
{
	ispis_donje_linije(broj_mesta);

	printf("L: ");

	for (int i = 0; i < broj_mesta; ++i) {
		printf("%d ", lozinka[i]);
	}

	printf("[ ");

	for (int i = 0; i < broj_mesta; ++i) {
		if (reseno) {
			printf("✓ ");
		} else {
			printf("x ");
		}
	}

	printf("]\n");

	if (reseno) {
		printf("POGODAK!\n");
	} else {
		printf("Nema pogotka. :(\n");
	}

	ispis_donje_linije(broj_mesta);
}
/* -------------------------------------------------------------------------- */
void rutina_poslednji_red(char *pokusaji, int lozinka[], int reseno, int red, int sirina, int broj_mesta)
{
	system("clear");
	ispis_maske(broj_mesta);
	ispis_dosadasnjih_pokusaja(pokusaji, red, sirina);
	ispis_lozinke(lozinka, broj_mesta, reseno);
}
/* -------------------------------------------------------------------------- */
void ispis_debug(int lozinka[], int pokusaj[], int ocena[], int d_n)
{
	int i;
	printf("%d %d %d %d\n", lozinka[0], lozinka[1], lozinka[2], lozinka[3]);
	printf("%d %d %d %d -> ", pokusaj[0], pokusaj[1], pokusaj[2], pokusaj[3]);
	printf("[ ");

	for (i = 1; i <= ocena[0]; i++) {
		printf("%c ", ZNAK_CRNO);
	}

	for (i = 1; i <= ocena[1]; i++) {
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

	char *pokusaji = malloc(BROJ_POKUSAJA * SIRINA_REDA_POKUSAJI);

	for (i = 1; i <= 1000; i++) {
		reset_strukture_pokusaji(pokusaji, BROJ_POKUSAJA, SIRINA_REDA_POKUSAJI);
		/** ispis_strukture_pokusaji(pokusaji); */
		generisanje_lozinke(lozinka, BROJ_MESTA);
		generisanje_lozinke(pokusaj, BROJ_MESTA);
		ocenjivanje(lozinka, pokusaj, ocena, BROJ_MESTA);
		ispis_debug(lozinka, pokusaj, ocena, BROJ_MESTA);
	}

	return 0;
}
/* -------------------------------------------------------------------------- */
void __init(char *pokusaji, int *lozinka, int *donja_granica, int *gornja_granica, int broj_pokusaja, int sirina_reda_pokusaji, int broj_mesta)
{
	generisanje_donje_i_gornje_granice(donja_granica, gornja_granica);
	reset_strukture_pokusaji(pokusaji, broj_pokusaja, sirina_reda_pokusaji);
	generisanje_lozinke(lozinka, broj_mesta);
}
/* -------------------------------------------------------------------------- */
int main()
{
	if (DEBUG) {
		_debug();
		return 0;
	}

	int reseno          = 0,
		prekid_korisnik = 0,
		red             = 0,
		donja_granica   = -1,
		gornja_granica  = -1,
		lozinka[BROJ_MESTA],
		pokusaj[BROJ_MESTA],
		ocena[BROJ_MESTA] = { -1 , -1 };

	char *pokusaji = (char *) malloc(BROJ_POKUSAJA * SIRINA_REDA_POKUSAJI + 1);

	__init(pokusaji, lozinka, &donja_granica, &gornja_granica, BROJ_POKUSAJA, SIRINA_REDA_POKUSAJI, BROJ_MESTA);

	while (!reseno && !prekid_korisnik && red < BROJ_POKUSAJA) {
		system("clear");
		ispis_maske(BROJ_MESTA);

		if (CHEAT)
			printf("l: %d %d %d %d\n", lozinka[0], lozinka[1], lozinka[2], lozinka[3]);
		
		ispis_dosadasnjih_pokusaja(pokusaji, red, SIRINA_REDA_POKUSAJI);
		obrada_ulaza(pokusaji, &red, &prekid_korisnik, lozinka, pokusaj, ocena, SIRINA_REDA_POKUSAJI, donja_granica, gornja_granica, BROJ_MESTA);
		pokusaj_izlaza(&reseno, ocena, &red, BROJ_MESTA);
	}

	rutina_poslednji_red(pokusaji, lozinka, reseno, red, SIRINA_REDA_POKUSAJI, BROJ_MESTA);
	free(pokusaji);

	return 0;
}
/* -------------------------------------------------------------------------- */
