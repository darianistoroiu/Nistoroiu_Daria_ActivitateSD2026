#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<malloc.h>
#include <string.h>
#include <stdlib.h>

struct Farmacist {
	int codAngajat;
	char* numeFarmacist;
	float salariu;
};
typedef struct Farmacist Farmacist;
typedef struct Nod Nod;
typedef struct ListaDubla ListaDubla;
struct Nod {
	Farmacist info;
	Nod* next;
	Nod* prev;
};
struct ListaDubla {
	Nod* prim;
	Nod* ultim;
};

Farmacist citireFarmacistDinFisier(FILE* file)
{
	char buffer[100];
	char sep[3] = ",\n";
	fgets(buffer, 100, file);
	char* aux;
	Farmacist f;
	aux = strtok(buffer, sep);
	f.codAngajat = atoi(aux);
	aux = strtok(NULL, sep);
	f.numeFarmacist = (char*)malloc(sizeof(char) * (strlen(aux) + 1));
	strcpy_s(f.numeFarmacist, strlen(aux) + 1, aux);
	f.salariu = atof(strtok(NULL, sep));
	return f;
}

void afisareFarmacist(Farmacist f) {
	printf("Farmacistul cu codul %d ", f.codAngajat);
	printf("se numeste %s ", f.numeFarmacist);
	printf("si are un salariu de %.2f lei. ", f.salariu);
}
void inserareFarmacistInLista(ListaDubla* lista, Farmacist farmacist) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = farmacist;
	nou->next = nou->prev = NULL;
	if (lista->ultim != NULL) {
		nou->prev = lista->ultim;
		lista->ultim->next = nou;
		lista->ultim = nou;
	}
	else {
		lista->prim = lista->ultim = nou;
	}
}

ListaDubla* citireListaDinFisier(const char* numeFisier) {
	FILE* file = fopen(numeFisier, "r");
	if (file) {
		ListaDubla* lista = (ListaDubla*)malloc(sizeof(ListaDubla));
		lista->prim = lista->ultim = NULL;
		while (!feof(file)) {
			inserareFarmacistInLista(lista, citireFarmacistDinFisier(file));
		}
		fclose(file);
		return lista;
	}
	else {
		return NULL;
	}
}
void afisareLista(ListaDubla lista) {
	Nod* p = lista.prim;
	while (p) {
		afisareFarmacist(p->info);
		p = p->next;
		printf("\n");
	}
}
//functie care returneaza numele farmacistului cu salariul cel mai mare, daca sunt doi se returneaza ultimul din lista
char* farmacistCautat(ListaDubla lista) {
	Nod* p = lista.prim;
	float maxim = lista.prim->info.salariu;
	p = p->next;
	while (p) {
		if (p->info.salariu > maxim) {
			maxim = p->info.salariu;
		}
		p = p->next;
	}
	p = lista.ultim;
	int ok = 0;
	while (p && !ok) {
		if (p->info.salariu == maxim) {
			ok = 1;
			return p->info.numeFarmacist;
		}
		p = p->prev;
	}
}
//functia care returneaza un farmacist-deep copy in fct de id ul acestuia. daca sunt doi se returneaza primul
Farmacist cautareFarmacistDupaId(ListaDubla lista, int idCautat) {
	Farmacist f;
	f.codAngajat = -1;
	f.numeFarmacist = NULL;
	f.salariu = 0;
	Nod* p = lista.prim;
	int ok = 0;
	while (p && !ok) {
		if (p->info.codAngajat == idCautat) {
			ok = 1;
			f.codAngajat = p->info.codAngajat;
			f.numeFarmacist = (char*)malloc(sizeof(strlen(p->info.numeFarmacist) + 1));
			strcpy_s(f.numeFarmacist, strlen(p->info.numeFarmacist) + 1, p->info.numeFarmacist);
			f.salariu = p->info.salariu;
			return f;
		}
		p = p->next;
	}
	if (ok == 0) {
		return f;
	}
}

int main() {
	ListaDubla* lista = citireListaDinFisier("farmacisti.txt");
	afisareLista(*lista);
	printf("\nFarmacistul cu ce mai mare salariu este %s. ", farmacistCautat(*lista));
	printf("\nFarmacistul cu id-ul cautat este: ");afisareFarmacist(cautareFarmacistDupaId((*lista), 3));
}