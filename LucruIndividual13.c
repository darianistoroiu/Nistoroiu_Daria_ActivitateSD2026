#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct Platforma {
	int id;
	float pret;
	char* nume;
	int nrCategorii;
	int* subcategorii;
};
typedef struct Platforma Platforma;
typedef struct Heap Heap;
typedef struct Nod Nod;

struct Heap {
	int lungime;
	Platforma* vector;
	int nrElemente;
};

struct Nod {
	Platforma info;
	Nod* st;
	Nod* dr;
};

Platforma citirePlatformaDinFisier(FILE* f) {
	char buffer[100];
	char sep[3] = ",\n";
	fgets(buffer, 100, f);
	char* aux;
	Platforma p;
	p.id = atoi(strtok(buffer, sep));
	p.pret = atof(strtok(NULL, sep));
	aux = strtok(NULL, sep);
	p.nume = (char*)malloc(strlen(aux) + 1);
	strcpy_s(p.nume, strlen(aux) + 1, aux);
	p.nrCategorii = atoi(strtok(NULL, sep));
	p.subcategorii = (int*)malloc(sizeof(int) * p.nrCategorii);
	for (int i = 0;i < p.nrCategorii;i++) {
		p.subcategorii[i] = atoi(strtok(NULL, sep));
	}
	return p;
}
void afisarePlatforma(Platforma p) {
	printf("Id:%d\n", p.id);
	printf("Nume:%s\n", p.nume);
	printf("Pret:%f\n", p.pret);
	printf("Nr.categorii:%d\n", p.nrCategorii);
	for (int i = 0;i < p.nrCategorii;i++) {
		printf("In categoria %d sunt %d subcategorii.\n", i+1, p.subcategorii[i]);
	}
	printf("\n");
}

Heap initializareHeap(int lungime) {
	Heap h;
	h.lungime = lungime;
	h.nrElemente = 0;
	h.vector =malloc(sizeof(Platforma) * lungime);
	return h;
}

void filtrare(Heap h, int pozitieNod) {
	int pozSt = 2 * pozitieNod + 1;
	int pozDr = 2 * pozitieNod + 2;
	int pozMin = pozitieNod;
	if (h.vector[pozSt].pret < h.vector[pozMin].pret && pozSt < h.nrElemente) {
		pozMin = pozSt;
	}
	if (h.vector[pozDr].pret < h.vector[pozMin].pret && pozDr < h.nrElemente) {
		pozMin = pozDr;
	}
	if (pozMin != pozitieNod) {
		Platforma aux = h.vector[pozitieNod];
		h.vector[pozitieNod] = h.vector[pozMin];
		h.vector[pozMin] = aux;
		if (pozMin <= (h.nrElemente - 2) / 2) {
			filtrare(h, pozMin);
		}
	}
}

Heap citireHeapDinFisier(const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");
	Heap h = initializareHeap(10);
	if (f) {
		while (!feof(f)) {
			h.vector[h.nrElemente++] = citirePlatformaDinFisier(f);
		}
	}
	fclose(f);
	for (int i = (h.nrElemente - 2) / 2;i >= 0;i--) {
		filtrare(h, i);
	}
	return h;
}

void afisareHeap(Heap h) {
	for (int i = 0;i < h.nrElemente;i++) {
		afisarePlatforma(h.vector[i]);
	}
}

void modificareCelMaiMicPret(Heap h, float pretNou) {
	if (h.nrElemente > 0) {
		Platforma aux = h.vector[0];
		h.vector[0] = h.vector[h.nrElemente - 1];
		h.nrElemente--;
		filtrare(h, 0);
		aux.pret = pretNou;
		h.nrElemente++;
		h.vector[h.nrElemente - 1] = aux;
		for (int i = (h.nrElemente - 2) / 2;i >= 0;i--) {
			filtrare(h, i);
		}
	
	}
}

void inserareInArbore(Nod** cap, Platforma element) {
	if (*cap == NULL) {
		Nod* nou = (Nod*)malloc(sizeof(Nod));
		nou->info.id = element.id;
		nou->info.pret = element.pret;
		nou->info.nume = (char*)malloc(strlen(element.nume) + 1);
		strcpy_s(nou->info.nume, strlen(element.nume) + 1, element.nume);
		nou->info.nrCategorii = element.nrCategorii;
		nou->info.subcategorii = (int*)malloc(sizeof(int) * element.nrCategorii);
		for (int i = 0;i < element.nrCategorii;i++) {
			nou->info.subcategorii[i] = element.subcategorii[i];
		}
		nou->st = nou->dr = NULL;
		*cap = nou;
	}
	else {
		if (element.nrCategorii < (*cap)->info.nrCategorii) {
			inserareInArbore(&((*cap)->st), element);
		}
		if (element.nrCategorii > (*cap)->info.nrCategorii) {
			inserareInArbore(&((*cap)->dr), element);
		}
	}
}

Nod* copiereValoriDinHeap(Heap h, int n) {
	Nod* cap = NULL;
	for (int i = 0;i < n;i++) {
		inserareInArbore(&cap, h.vector[i]);
	}
	return cap;
}

void afisareArbore(Nod* cap) {
	if (cap != NULL) {
		afisareArbore(cap->st);
		afisarePlatforma(cap->info);
		afisareArbore(cap->dr);
		}
}

void afisareNodCuUnSingurDescendent(Nod* cap) {
	if (cap) {
		if (cap->st != NULL && cap->dr == NULL || cap->st==NULL && cap->dr!=NULL) {
			printf("\nPlatforma cu un singur descendent:\n");
			afisarePlatforma(cap->info);
		}
		afisareNodCuUnSingurDescendent(cap->st);
		afisareNodCuUnSingurDescendent(cap->dr);
	}
}

void dezalocareHeap(Heap* h) {
	for (int i = 0;i < h->lungime;i++) {
		free(h->vector[i].nume);
		free(h->vector[i].subcategorii);
	}
	free(h->vector);
	h->vector = NULL;
	h->lungime = 0;
	h->nrElemente = 0;
}

void dezalocareArbore(Nod** cap) {
	if ((*cap) != NULL) {
		dezalocareArbore(&((*cap)->st));
		dezalocareArbore(&((*cap)->dr));
		free((*cap)->info.nume);
		free((*cap)->info.subcategorii);
		free((*cap));
		*cap = NULL;
	}
}
	

int main() {
	Heap h = citireHeapDinFisier("platforme.txt");
	afisareHeap(h);
	printf("\nHeap dupa modificarea celui mai mic pret:\n");
	modificareCelMaiMicPret(h, 2.55);
	afisareHeap(h);
	printf("\nCopiere a primele 4 elemente in arbore binar de cautare din structura heap:\n");
	Nod* arbore = copiereValoriDinHeap(h, 4);
	afisareArbore(arbore);

	printf("Afisare platforme care au un singur descendent in arbore:\n");
	afisareNodCuUnSingurDescendent(arbore);
	dezalocareArbore(&arbore);
	dezalocareHeap(&h);
	return 0;
}