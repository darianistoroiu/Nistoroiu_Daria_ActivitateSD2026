#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Examen {
	char* materie;
	unsigned int codExamen;
	int nrCredite;
	int studExaminati;
	float* noteStud;
};
typedef struct Examen Examen;
typedef struct Nod Nod;

//creare structura arbore binar de cautare
struct Nod {
	Examen info;
	Nod* st;
	Nod* dr;
};

Examen citireExamenDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = " \n,";
	fgets(buffer, 100, file);
	char* aux;
	aux = strtok(buffer, sep);
	Examen e;
	e.materie = (char*)malloc(strlen(aux) + 1);
	strcpy_s(e.materie, strlen(aux) + 1, aux);
	aux = strtok(NULL, sep);
	e.codExamen = atoi(aux);
	aux = strtok(NULL, sep);
	e.nrCredite = atoi(aux);
	aux = strtok(NULL, sep);
	e.studExaminati = atoi(aux);
	e.noteStud = (float*)malloc(sizeof(float) * e.studExaminati);
	for (int i = 0;i < e.studExaminati;i++) {
		aux = strtok(NULL, sep);
		e.noteStud[i] = atof(aux);
	}
	return e;
}
void afisareExamen(Examen e) {
	printf("Materia:%s\n", e.materie);
	printf("Cod:%d\n", e.codExamen);
	printf("Nr.credite:%d\n", e.nrCredite);
	printf("Studenti examinati: %d\n", e.studExaminati);
	for (int i = 0;i < e.studExaminati;i++) {
		printf("Nota studentului %d este %.2f\n", i+1, e.noteStud[i]);
	}
	printf("\n");
}

void adaugaExamenInArbore(Nod** rad, Examen examenNou) {
	if (*rad == NULL) {
		Nod* nou = malloc(sizeof(Nod));
		nou->info = examenNou;
		nou->dr = nou->st = NULL;
		*rad = nou;
	}
	else {
		if ((*rad)->info.codExamen > examenNou.codExamen) {
			adaugaExamenInArbore(&((*rad)->st), examenNou);
		}
		if ((*rad)->info.codExamen < examenNou.codExamen) {
			adaugaExamenInArbore(&((*rad)->dr), examenNou);
		}
	}

}

Nod* citireArboreDinFisier(const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");
	Nod* rad = NULL;
	if (f) {
		while (!feof(f)) {
			Examen e = citireExamenDinFisier(f);
			adaugaExamenInArbore(&rad, e);
		}
	}
	fclose(f);
	return rad;
}

void afisareExameneDinArbore(Nod* rad) {
	if (rad) {
		afisareExameneDinArbore(rad->st);
		afisareExamen(rad->info);
		afisareExameneDinArbore(rad->dr);
	}
}

void afisarePostordine(Nod* rad) {
	if (rad) {
		afisarePostordine(rad->st);
		afisarePostordine(rad->dr);
		afisareExamen(rad->info);
	}
}

Nod* stergereExamenDupaCod(Nod* rad, unsigned int codCautat) {
	if (rad == NULL) {
		return NULL;
	}

	if (codCautat < rad->info.codExamen) {
		rad->st = stergereExamenDupaCod(rad->st, codCautat);
	}
	else if (codCautat > rad->info.codExamen) {
		rad->dr = stergereExamenDupaCod(rad->dr, codCautat);
	}
	else {

		//cazul in care nodul gasit pentru a fi sters are 0 sau 1 copii
		if (rad->st == NULL) {
			Nod* temp = rad->dr;
			free(rad->info.materie);
			free(rad->info.noteStud);
			free(rad);
			return temp;
		}
		else if (rad->dr == NULL) {
			Nod* temp = rad->st;
			free(rad->info.materie);
			free(rad->info.noteStud);
			free(rad);
			return temp;
		}
		//cazul in care nodul gasit pentru a fi sters are 2 copii
		//in acest caz mergem in subarborele drept al nodului curent(de sters) si cautam cel mai mic element - mergem in stanga pana la capat
		//acest nod gasit are proprietatea de a fi mai mare decat tot ce se afla in subarborele stang al nodului de sters si este totodata mai mic deecat tot ce este in subarborele drept
		//facem schimbul intre acest nod gasit si nodul de sters pentru a nu pierde legaturile
		Nod* temp = rad->dr;
		while (temp->st != NULL) {
			temp = temp->st;
		}
		free(rad->info.materie);
		free(rad->info.noteStud);

		Examen infoNoua = temp->info;
		temp->info.materie = NULL;
		temp->info.noteStud = NULL;

		rad->dr = stergereExamenDupaCod(rad->dr, infoNoua.codExamen);
		rad->info = infoNoua;
	}
	return rad;
}

void dezalocareArbore(Nod** rad) {
	if (rad) {
		dezalocareArbore(&(*rad)->st);
		dezalocareArbore(&(*rad)->dr);
		free((*rad)->info.materie);
		free((*rad)->info.noteStud);
		free(*rad);
		*rad = NULL;

	}
}

int main() {
	/*FILE* f = fopen("examene.txt", "r");
	Examen e1 = citireExamenDinFisier(f);
	afisareExamen(e1);
	Examen e2 = citireExamenDinFisier(f);
	afisareExamen(e2);
	fclose(f);*/

	Nod* rad = citireArboreDinFisier("examene.txt");
	afisareExameneDinArbore(rad);

	printf("Afisare arbore in postordine:\n");
	afisarePostordine(rad);

	unsigned int codDeSters = 101;
	rad = stergereExamenDupaCod(rad, codDeSters);
	printf("Afisare arbore dupa stergere:\n");
	afisareExameneDinArbore(rad);

	dezalocareArbore(&rad);

}