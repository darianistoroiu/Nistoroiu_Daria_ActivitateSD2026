#include<stdio.h>
#include<malloc.h>

typedef struct Depozit Depozit; //definire alias pt struct Depozit

struct Depozit {
	int id;
	int volum;
	char* denumire; //un pointer ocupa maxim 8 octeti(windows x64) doar pe STACK
	float suprafata;
	char clasa; //memoria STACK este stocata de catre compilator; memoria HEAP gestionata de catre programator
};
struct Depozit initializare(int id, float suprafata, int volum, char* denumire, char clasa) {
	struct Depozit s;
	s.id = id;
	s.suprafata = suprafata;
	s.volum = volum;
	s.denumire = (char*)malloc(sizeof(char) * (strlen(denumire) + 1)); // pun char* in fata lui malloc pentru a face conversia de la void*; malloc este void* pointer generic pentru orice fel de tip de pointer avem nevoie
	strcpy(s.denumire, denumire);
	s.clasa = clasa;
	return s;
}
Depozit copiere(Depozit d) {
	return initializare(d.id, d.suprafata, d.volum, d.denumire, d.clasa);
}
void afisare(struct Depozit s) {
	printf("%d. Depozitul %s de clasa %c are o suprafata de %5.2f si un volum de %d.\n", s.id, s.denumire, s.clasa, s.suprafata, s.volum); //%s-string sir de caractere
}

void afisareVector(struct Depozit* vector, int nrElemente) {
	//afisarea elementelor din vector apeland functia afisare
	for (int i = 0;i < nrElemente;i++) {
		afisare(*(vector + i));
	}
}

struct Depozit* copiazaPrimeleNElemente(Depozit* vector, int nrElemente, int nrElementeCopiate) {
	//copiem intr-un vector nou, pe care il vom returna, primele nrElementeCopiate
	if (nrElementeCopiate < nrElemente) {
		struct Depozit* vectorNou = malloc(sizeof(Depozit) * nrElementeCopiate);
		for (int i = 0;i < nrElementeCopiate;i++)
		{
			vectorNou[i] = copiere(vector[i]);
		}

		return vectorNou;
	}
	else {
		return NULL;
	}
}

void dezalocare(struct Depozit** vector, int* nrElemente) {
	//dezalocam elementele din vector si vectorul
	for (int i = 0;i < *nrElemente;i++) {
		free((*vector)[i].denumire);
	}
	free(*vector);
	*nrElemente = 0;
	*vector = NULL;
}

void copiazaAnumiteElemente(struct Depozit* vector, char nrElemente, float prag, struct Depozit** vectorNou, int* dimensiune) {
	//parametrul prag poate fi modificat in functie de 
	// tipul atributului ales pentru a indeplini o conditie
	//este creat un nou vector cu elementele care indeplinesc acea conditie
	*dimensiune = 0;
	for (int i = 0;i < nrElemente;i++) {
		if (vector[i].suprafata < prag)
			(*dimensiune)++;
	}
	*vectorNou = malloc(sizeof(Depozit) * (*dimensiune));
	*dimensiune = 0;

	for (int i = 0;i < nrElemente;i++) {
		if (vector[i].suprafata < prag) {
			(*vectorNou)[*dimensiune] = copiere(vector[i]);
			(*dimensiune)++;
		}
	}


}

struct Depozit getPrimulElementConditionat(struct Depozit* vector, int nrElemente, const char* numeCautat) {
	//trebuie cautat elementul care indeplineste o conditie
	//dupa atributul de tip char*. Acesta este returnat.
	Depozit s;
	s.id = -1;
	for (int i = 0;i < nrElemente;i++) {
		if (strcmp(numeCautat, vector[i].denumire) == 0) {
			s = copiere(vector[i]);
		}
	}
	return s;
}



int main() {
	struct Depozit depozit = initializare(1, 34.3, 70, "Emag", 'A');
	afisare(depozit);

	int nrDepozite = 4;
	Depozit* vector = (Depozit*)malloc(sizeof(Depozit) * nrDepozite);
	vector[0] = initializare(2, 34, 45, "DPD", 'B');
	vector[1] = initializare(3, 56, 100, "Amazon", 'C');
	vector[2] = initializare(4, 40, 80, "DSC", 'B');
	vector[3] = initializare(5, 34, 45, "Emag", 'A');

	afisareVector(vector, nrDepozite);
	printf("\n\n");
	int nrCopiate = 2;
	Depozit* scurt = copiazaPrimeleNElemente(vector, nrDepozite, nrCopiate);
	//afisareVector(scurt, nrCopiate);
	dezalocare(&scurt, &nrCopiate);
	afisareVector(scurt, nrCopiate);


	printf("Vector filtrat \n");
	Depozit* filtrat = NULL;
	int dimensiune = 0;
	copiazaAnumiteElemente(vector, nrDepozite, 50, &filtrat, &dimensiune);
	afisareVector(filtrat, dimensiune);
	dezalocare(&filtrat, &dimensiune);


	printf("Depozit cautat \n");
	Depozit depozitCautat = getPrimulElementConditionat(vector, nrDepozite, "Amazon");
	afisare(depozitCautat);
	if (depozitCautat.id != -1) {
		free(depozitCautat.denumire);
	}

	free(depozit.denumire);
	dezalocare(&vector, &nrDepozite);
	return 0;
}