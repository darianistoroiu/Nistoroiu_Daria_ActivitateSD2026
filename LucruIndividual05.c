#define  _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

struct StructuraMasina {
	int id;
	int nrUsi;
	float pret;
	char* model;
	char* numeSofer;
	unsigned char serie;
};

typedef struct StructuraMasina Masina;

struct Nod {
	Masina masina;
	struct Nod* next;
};
typedef struct Nod Nod;

struct HashTable {
	int dim;
	Nod** tabela;//vector de pointeri

};
typedef struct HashTable HashTable;
Masina citireMasinaFisier(FILE* file) {
	Masina m;
	char buffer[100];
	char sep[4] = ",;\n";
	fgets(buffer, 100, file);
	m.id = atoi(strtok(buffer, sep));
	m.nrUsi = atoi(strtok(NULL, sep));
	m.pret = atof(strtok(NULL, sep));
	char* aux;
	aux = strtok(NULL, sep);
	m.model = (char*)malloc(sizeof(char) * (strlen(aux) + 1));
	strcpy_s(m.model, strlen(aux) + 1, aux);
	aux = strtok(NULL, sep);
	m.numeSofer = (char*)malloc(sizeof(char) * (strlen(aux) + 1));
	strcpy_s(m.numeSofer, strlen(aux) + 1, aux);
	m.serie = strtok(NULL, sep)[0];
	return m;

}
void afisareMasina(Masina masina) {
	printf("Id:%d\n", masina.id);
	printf("Nr usi:%d\n", masina.nrUsi);
	printf("Pret:%.2f\n", masina.pret);
	printf("Model:%s\n", masina.model);
	printf("Nume sofer:%s\n", masina.numeSofer);
	printf("Serie:%c\n", masina.serie);
}
void afisareListaMasini(Nod* cap) {
	while (cap) {
		afisareMasina(cap->masina);
		cap = cap->next;
		
	}
}
void adaugaMasinaInLista(Nod* cap, Masina masinaNoua) { //inserare in lista existenta
	Nod* p = cap;
	while (p->next) {
		p = p->next;
	}
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->masina = masinaNoua;
	nou->next = NULL;
	p->next = nou;
}

HashTable initializareHashTable(int dimensiune) {
	HashTable ht;
	ht.dim = dimensiune;
	ht.tabela = (Nod**)malloc(dimensiune * sizeof(Nod*));
	for (int i = 0;i < dimensiune;i++) {
		ht.tabela[i] = NULL;
	}
	return ht;
}
int calculeazaHash(const char* nume, int dimensiune) {
	int suma = 0;
	for (int i = 0;i < strlen(nume);i++) {
		suma += nume[i];
	}
	return suma % dimensiune;
}
void inserareMasinaInTabela(HashTable hash, Masina masina) {
	int pozitie = calculeazaHash(masina.numeSofer, hash.dim);
	if (hash.tabela[pozitie] == NULL) {
		hash.tabela[pozitie] = (Nod*)malloc(sizeof(Nod));
		hash.tabela[pozitie]->masina = masina;
		hash.tabela[pozitie]->next = NULL;
	}
	else
	{
		adaugaMasinaInLista(hash.tabela[pozitie], masina);
	}
}
HashTable citireMasiniDinFisier(const char* numeFisier, int dimensiune) {
	HashTable hash = initializareHashTable(dimensiune);
	FILE* f = fopen(numeFisier, "r");
	while (!feof(f)) {
		Masina masina = citireMasinaFisier(f);
		inserareMasinaInTabela(hash, masina);
	}
	fclose(f);
	return hash;
}

void afisareTabelaDeMasini(HashTable ht) {
	for (int i = 0;i < ht.dim;i++) {
		if (ht.tabela[i] != NULL)
		{
			printf("\nMasinile de pe pozitia %d sunt: \n", i);
			afisareListaMasini(ht.tabela[i]);
		}
		else
			printf("\nPe pozitia %d nu avem masini. \n", i);
	}
}
void dezalocareListaMasini(Nod** cap) {
	Nod* p = *cap;
	while (p) {
		Nod* temp = p;
		p = p->next;
		if (temp->masina.model != NULL) {
			free(temp->masina.model);
		}
		if (temp->masina.numeSofer != NULL) {
			free(temp->masina.numeSofer);
		}
		free(temp);
	}
	*cap = NULL;
}
void dezalocareTabelaDeMasini(HashTable* ht) {
	for (int i = 0;i < ht->dim;i++) {
		dezalocareListaMasini(&(ht->tabela[i]));
	}
	free(ht->tabela);
	ht->tabela = NULL;
	ht->dim = 0;
}

float medieLista(Nod* cap) {
	float suma = 0;
	int nrElemente = 0;
	while (cap) {
		suma += cap->masina.pret;
		cap = cap->next;
		nrElemente++;
	}
	if (nrElemente > 0) {
		return suma / nrElemente;
	}
	else
		return 0;
	
}
float* calculeazaPreturiMediiPerClustere(HashTable ht, int* nrClustere) {
	float* preturi = NULL;
	*nrClustere = 0;
	for (int i = 0;i < ht.dim;i++) {
		if (ht.tabela[i] != NULL)
			(*nrClustere)++;
	}
	preturi = (float*)malloc(sizeof(float) * (*nrClustere));
	int contor = 0;
	for (int i = 0;i < ht.dim;i++) {
		if (ht.tabela[i] != NULL) {
			preturi[contor] = medieLista(ht.tabela[i]);
			contor++;
		}

	}
	return preturi;
}
Masina cautareMasinaInLista(Nod* cap, const char* nume) {
	Masina m;
	m.id = -1;
	while (cap && strcmp(cap->masina.numeSofer,nume)!=0) {
		cap = cap->next;
	}
	if (cap) {
		m = cap->masina;
		m.model = (char*)malloc(strlen(cap->masina.model) + 1);
		strcpy_s(m.model, strlen(cap->masina.model) + 1, cap->masina.model);
		m.numeSofer = (char*)malloc(strlen(cap->masina.numeSofer) + 1);
		strcpy_s(m.numeSofer, strlen(cap->masina.numeSofer) + 1, cap->masina.numeSofer);
	}
	return m;
}


Masina getMasinaDupaNumeSofer(HashTable ht, const char* numeCautat) {
	Masina m;
	m.id = -1;
	int poz = calculeazaHash(numeCautat, ht.dim);
	if (poz >= 0 && poz < ht.dim) {
		return cautareMasinaInLista(ht.tabela[poz], numeCautat);
	}
	return m;
}

int main() {

	HashTable ht = citireMasiniDinFisier("masini.txt",7);
	afisareTabelaDeMasini(ht);

	printf("Preturile medii per clustere sunt:\n");
	int nrClustere = 0;
	float *preturi = calculeazaPreturiMediiPerClustere(ht, &nrClustere);
	for (int i = 0;i < nrClustere;i++) {
		printf("%.2f  ", preturi[i]);
	}
	printf("\n");
	printf("\n");
	Masina masina = getMasinaDupaNumeSofer(ht, "Gigel Marius");
	if (masina.id != -1) {
		printf("Masina cautata:\n");
		afisareMasina(masina);
			if (masina.model != NULL) {
				free(masina.model);
			}
			if (masina.numeSofer != NULL) {
				free(masina.numeSofer);
			}

	}
	else {
		printf("Masina nu a fost gasita.\n");
	}
	

	dezalocareTabelaDeMasini(&ht);

	return 0;
}