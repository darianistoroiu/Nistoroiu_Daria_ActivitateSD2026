#include<stdio.h>
#include<malloc.h>

struct Firma {
	int id;
	int aniActiv;
	float capital;
	char* nume;
	char tip; //in realitate nu exista, am vrut doar sa folosesc char 
};

typedef struct Firma Firma;

struct Firma initializare(int id, int aniActiv, float capital, const char* nume, char tip) {
	Firma f;
	f.id = id;
	f.aniActiv = aniActiv;
	f.capital = capital;
	f.nume = (char*)malloc(sizeof(char) * (strlen(nume) + 1));
	strcpy_s(f.nume, strlen(nume) + 1, nume);
	f.tip = tip;
	return f;


}

void afisare(Firma f) {
	printf("%d. Firma %s are %d ani de activitate, are un capital de %5.2f si este de tipul %c.\n", f.id, f.nume, f.aniActiv, f.capital, f.tip);
}

void modificaAniActiv(Firma* f, int nou) {
	if (nou > 0) {
		f->aniActiv = nou;
	}
}
void afisareVector(Firma* vector, int nrFirme) {
	for (int i = 0;i < nrFirme;i++) {
		afisare(vector[i]);
	}
}
Firma* copiazaPrimeleNelemente(Firma* vector, int nrElemente, int nrElementeCopiate) {
	Firma* vectorNou = NULL;
	vectorNou = (Firma*)malloc(sizeof(Firma) * nrElementeCopiate);
	for (int i = 0;i < nrElementeCopiate;i++) {
		vectorNou[i] = vector[i];
		vectorNou[i].nume = (char*)malloc(sizeof(char) * (strlen(vector[i].nume) + 1));
		strcpy_s(vectorNou[i].nume, strlen(vector[i].nume) + 1, vector[i].nume);
	}
	return vectorNou;
}
void dezalocare(Firma* f) {
	if (f->nume != NULL) {
		free(f->nume);
		f->nume = NULL;
	}
}
void dezalocareVector(Firma** vector, int* N) {
	for (int i = 0; i < (*N);i++) {
		if ((*vector[i]).nume != NULL) {
			free((*vector[i]).nume);
		}
		free(*vector);
		(*vector) = NULL;
		*N = 0;
	}
}

void copiazaFirmeCuVechime(Firma* vector, int N, int vechimeMin, Firma** vectorNou, int* dimensiune) {
	*dimensiune = 0;
	for (int i = 0;i < N;i++) {
		if (vector[i].aniActiv >= vechimeMin) {
			(*dimensiune)++;
		}
	}
	if ((*vectorNou) != NULL) {
		free(*vectorNou);
	}
	*vectorNou = (Firma*)malloc(sizeof(Firma) * (*dimensiune));
	int k = 0;
	for (int i = 0;i < N;i++) {
		if (vector[i].aniActiv >= vechimeMin) {
			(*vectorNou)[k] = vector[i];
			(*vectorNou)[k].nume = (char*)malloc(strlen(vector[i].nume) + 1);
			strcpy_s((*vectorNou)[k].nume, strlen(vector[i].nume) + 1, vector[i].nume);
			k++;

		}
	}
}

Firma getPrimulElementConditionat(Firma* vector, int N, const char* nume) {
	Firma f;
	f.nume = NULL;
	for (int i = 0;i < N;i++) {
		if (strcmp(vector[i].nume, nume) == 0) {
			f = vector[i];
			f.nume = (char*)malloc(strlen(vector[i].nume) + 1);
			strcpy_s(f.nume, strlen(vector[i].nume) + 1, vector[i].nume);
		}
	}
	return f;
}
int main() {
	Firma f;
	f= initializare(1, 10, 20220.2, "emag", 'A');
	afisare(f);
	modificaAniActiv(&f, 11);
	afisare(f);
	//dezalocare(&f);
	
	printf("\n");

	Firma* firme = NULL;
	int nrFirme = 3;
	firme = (Firma*)malloc(sizeof(Firma) * nrFirme);
	firme[0] = initializare(1, 10, 20220.2, "emag", 'A');
	firme[1] = initializare(2, 20, 1020.5, "altex", 'D');
	firme[2] = initializare(3, 22, 5000, "mediagalaxy", 'D');
	afisareVector(firme, nrFirme);

	printf("\n");
	Firma* primeleFirme = NULL;
	int N = 1;
	primeleFirme=copiazaPrimeleNelemente(firme, nrFirme, N);
	afisareVector(primeleFirme, N);
	dezalocareVector(&primeleFirme, &N);
	afisareVector(primeleFirme, N);


	printf("\n Firmele cu o vechime mai mare de 11 ani sunt:\n");
	Firma* uneleFirme = NULL;
	int nrFirmeBune = 0;
	copiazaFirmeCuVechime(firme, nrFirme, 11, &uneleFirme, &nrFirmeBune);
	
	afisareVector(uneleFirme, nrFirmeBune);

	printf("\n Afiseaza firma care se numeste Altex:\n");
	Firma firma = getPrimulElementConditionat(firme, nrFirme, "altex");
	afisare(firma);
	if (firma.nume != NULL) {
		free(firma.nume);
		firma.nume = NULL;
	}
	dezalocareVector(&firme, &nrFirme);

	return 0;
}