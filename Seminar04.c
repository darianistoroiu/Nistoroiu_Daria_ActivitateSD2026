//tabela de dispersie
//mecanismul chaining se refera la mecanismul de tratare al coliziunilor?; chaining- vector de liste, vector alocat dinamic de noduri
//vector cu o dimensiune limitata
//coliziune:inserare de mai multe informatii pe o anumita pozitie in vector
//calcularea unei functii hash->pozitia in vector unde trebuie sa inserez
#include<stdio.h>
#include<malloc.h>
typedef struct Test Test;
typedef struct Nod Nod;
typedef struct HashTable HashTable;

struct Test {
	char* materie;
	int nrStudenti;
	float medie;
};
struct Nod {
	Test info;
	Nod* next;
};
struct HashTable {
	int dimensiune;
	Nod** vector;
};
Test initTest(const char* materie, int nrStudenti, float medie) {
	Test t;
	t.nrStudenti = nrStudenti;
	t.medie = medie;
	t.materie = (char*)malloc(sizeof(char)*(strlen(materie) + 1));
	strcpy_s(t.materie, strlen(materie) + 1, materie);
	return t;
}
void afisareTest(Test t) {
	printf("\n La testul de la materia %s au participat %d studenti si au obstinut media %.2f.\n ", 
		t.materie, t.nrStudenti, t.medie);
}
void inserareLaSfarsit(Nod** cap, Test t) { 
	Nod* nou= (Nod*)malloc(sizeof(Nod));
	nou->next = NULL;
	nou->info = t;
	if ((*cap) == NULL) {
		(*cap) = nou;
	}
	else {
		Nod* aux = *cap;
		while (aux->next!=NULL) {
			aux = aux->next;
		}
		aux->next = nou;
	}
}
HashTable initHashTable(int size) {
	HashTable tabela;
	tabela.dimensiune = size;
	tabela.vector = (Nod**)malloc(sizeof(Nod*)*size);
	for (int i = 0;i < size;i++) {
		tabela.vector[i] = NULL;
	}
	return tabela;
}
int hash(int dim, int nrStud) { //scopul lui hash e sa returneze valori din [0, dim-1]
	return nrStud % dim;
}
void inserareHashTable(HashTable tabela, Test t) {
	if (tabela.dimensiune > 0) {
		int pozitie = hash(tabela.dimensiune, t.nrStudenti);
		if (pozitie >= 0 && pozitie < tabela.dimensiune) {
			inserareLaSfarsit(&(tabela.vector[pozitie]), t);
		}
	}
}
void afisareListaTeste(Nod* cap) {
	while (cap != NULL) {
		afisareTest(cap->info);
		cap = cap->next;
	}
}
void afisareHashTable(HashTable tabela) {
	for (int i = 0;i < tabela.dimensiune;i++) {
		printf("\npozitie:%d ", i);
		afisareListaTeste(tabela.vector[i]);
	}
}
//nr studentilor din prima lista a tabelei
int calculNrStudentiPrimaLista(HashTable tabela) {
	int nrStud = 0;
	Nod* capLista0 = tabela.vector[0];
	while (capLista0 != NULL) {
		nrStud += capLista0->info.nrStudenti;
		capLista0 = capLista0->next;
	}
	return nrStud;
}
void dezalocareLista(Nod** cap) {
	while ((*cap) != NULL) {
		free((*cap)->info.materie);
		Nod* copie = *cap;
		(*cap) = (*cap)->next;
		free(copie);
	}
}
void dezalocareHashTable(HashTable tabela) {
	for (int i = 0;i < tabela.dimensiune;i++) {
		dezalocareLista(&(tabela.vector[i]));
	}
	free(tabela.vector);
}

int main() {
	HashTable tabela = initHashTable(4);
	inserareHashTable(tabela, initTest("SDD", 60, 2.5));
	inserareHashTable(tabela, initTest("POO", 75, 6.5));
	inserareHashTable(tabela, initTest("Java", 48, 5.5));
	afisareHashTable(tabela);

	printf("\nNumarul studentilor din prima lista tabelei este %d", calculNrStudentiPrimaLista(tabela));

	dezalocareHashTable(tabela);
	return 0;
}
//tema implementari pe lista dublu inlatuita
//tema sa facem celelalte inserari si extrageri
//daca ni se da cheia de cautare mergem direct la pozitie in vector, nu il parcurgem