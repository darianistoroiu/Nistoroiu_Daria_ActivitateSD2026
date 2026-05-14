//tabela de dispersie

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct Gradinita {
	int id;
	char* nume;
	int nrGrupe;
	int* copiiInGrupa;
	int aniActivitate;
};
typedef struct Gradinita Gradinita;
typedef struct Nod Nod;
typedef struct Hashtable Hashtable;
typedef struct NodLista NodLista;
typedef struct ListaDubla ListaDubla;
struct Nod {
	Gradinita info;
	Nod* next;
};

struct Hashtable {
	int dimensiune;
	Nod** vector;
};
struct NodLista {
	Gradinita info;
	Nod* next;
	Nod* prev;
};
struct ListaDubla {
	NodLista* prim;
	NodLista* ultim;
};
Gradinita citireGradinita(FILE* f) {
	char buffer[100];
	char sep[3] = ",\n";
	fgets(buffer, 100, f);
	char* aux;
	Gradinita g;
	g.id = atoi(strtok(buffer, sep));
	aux = strtok(NULL, sep);
	g.nume = (char*)malloc(strlen(aux) + 1);
	strcpy_s(g.nume, strlen(aux) + 1, aux);
	g.nrGrupe = atoi(strtok(NULL, sep));
	g.copiiInGrupa = (int*)malloc(sizeof(int) * g.nrGrupe);
	for (int i = 0;i < g.nrGrupe;i++) {
		g.copiiInGrupa[i] = atoi(strtok(NULL, sep));
	}
	g.aniActivitate = atoi(strtok(NULL, sep));
	return g;
}
void afisareGradinita(Gradinita g) {
	printf("Id:%d\n", g.id);
	printf("Nume:%s\n", g.nume);
	printf("Nr.grupe:%d\n", g.nrGrupe);
	for (int i = 0;i < g.nrGrupe;i++) {
		printf("In grupa %d sunt %d copii.\n", i+1, g.copiiInGrupa[i]);
	}
	printf("Ani activitate:%d\n", g.aniActivitate);
	printf("\n");
}

void inserareNodLaSfarsit(Nod** cap, Gradinita g) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->next = NULL;
	nou->info = g;
	if ((*cap) == NULL) {
		(*cap) = nou;
	}
	else {
		Nod* aux;
		aux = (*cap);
		while (aux->next != NULL) {
			aux = aux->next;
		}
		aux->next = nou;
	}
}
int hash(int dim,const char* cheie) {
	return (strlen(cheie) + 1) % dim;
}
void inserareInHashtable(Hashtable h, Gradinita g) {
	if (h.dimensiune > 0) {
		int pozitie = hash(h.dimensiune, g.nume);
		if (pozitie >= 0 && pozitie < h.dimensiune) {
			inserareNodLaSfarsit(&(h.vector[pozitie]), g);
		}
	}
}
Hashtable citireTabelaDinFisier(const char* numeFisier, int dimensiune) {
	FILE* f = fopen(numeFisier, "r");
	Hashtable h;
	h.dimensiune = dimensiune;
	h.vector =(Nod**)malloc(sizeof(Nod*)* h.dimensiune);
	for (int i = 0;i < h.dimensiune;i++) {
		h.vector[i] = NULL;
	}
	if (f) {
		while (!feof(f)) {
			Gradinita g = citireGradinita(f);
			inserareInHashtable(h, g);
		}
	}
	return h;
	fclose(f);
}
void afisareListaGradinite(Nod* cap) {
	while (cap != NULL) {
		afisareGradinita(cap->info);
		cap = cap->next;
	}
}
void afisareHashtable(Hashtable h) {
	for (int i = 0;i < h.dimensiune;i++) {
		printf("\npozitia %d\n", i);
		afisareListaGradinite(h.vector[i]);

	}
}

void modificareNume(Hashtable h, const char* numeCautat, const char* numeNou) {
	int pozitie = hash(h.dimensiune, numeCautat);
	Nod* aux = h.vector[pozitie];
	if (aux && strcmp(aux->info.nume, numeCautat) == 0) {
		Gradinita g;
		g = aux->info;
		h.vector[pozitie] = aux->next;
		free(g.nume);
		g.nume = (char*)malloc(strlen(numeNou) + 1);
		strcpy_s(g.nume, strlen(numeNou) + 1, numeNou);
		inserareInHashtable(h, g);
		printf("\nModificarea a fost efectuata!\n");
	}
	else {
			while (aux->next!=NULL && strcmp(aux->next->info.nume, numeCautat) != 0) {
					aux = aux->next;
				}
				if (aux->next == NULL) {
					printf("\nNumele cautat nu a fost gasit!\n");
				}
				else {
						Gradinita g;
						g = aux->next->info;
						aux->next = aux->next->next;
						free(g.nume);
						g.nume = (char*)malloc(strlen(numeNou) + 1);
						strcpy_s(g.nume, strlen(numeNou) + 1, numeNou);
						inserareInHashtable(h, g);
						printf("\nModificarea a fost efectuata!\n");
				}
	}
}


void dezalocareLista(Nod** cap) {
	while((*cap) != NULL) {
		free((*cap)->info.nume);
		free((*cap)->info.copiiInGrupa);
		Nod* copie = (*cap);
		(*cap) = (*cap)->next;
		free(copie);
	}
}
void dezalocareHashtable(Hashtable h) {
	for (int i = 0;i < h.dimensiune;i++) {
		dezalocareLista(&(h.vector[i]));
	}
	free(h.vector);
}
void adaugareLaSfarsitInLista(ListaDubla* lista, Gradinita g) {
	NodLista* nou = (NodLista*)malloc(sizeof(NodLista));
	nou->info = g;
	nou->next = nou->prev = NULL;
	if (lista->prim== NULL) {
		lista->prim = lista->ultim = nou;
	}
	else {
		lista->ultim->next = nou;
		nou->prev = lista->ultim;
		lista->ultim = nou;
	}
}
ListaDubla creareListaDinHastable(Hashtable h) {
	ListaDubla lista;
	lista.prim = lista.ultim = NULL;
	if (h.dimensiune > 0) {
		for (int i = 0;i < h.dimensiune;i++) {
			Nod* aux = h.vector[i];
			while (aux) {
				if (aux->info.aniActivitate % 2 == 0) {
					adaugareLaSfarsitInLista(&lista, aux->info);
				}
				aux = aux->next;
			}
		}
	}
	return lista;
}
void afisareListaDubla(ListaDubla lista) {
	NodLista* aux = lista.prim;
	while (aux != NULL) {
		afisareGradinita(aux->info);
		aux = aux->next;
	}
}

int main() {
	/*FILE* f = fopen("gradinite.txt", "r");
	Gradinita g1 = citireGradinita(f);
	afisareGradinita(g1);
	Gradinita g2 = citireGradinita(f);
	afisareGradinita(g2);*/
	Hashtable h=citireTabelaDinFisier("gradinite.txt", 4);
	afisareHashtable(h);

	modificareNume(h, "Goguta", "Zambilele");
	afisareHashtable(h);

	printf("\nLista dubla formata din Hashtable:\n");
	ListaDubla lista = creareListaDinHastable(h);
	afisareListaDubla(lista);
	return 0;

}