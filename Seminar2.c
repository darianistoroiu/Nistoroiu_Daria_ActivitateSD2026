#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//trebuie sa folositi fisierul masini.txt
//sau va creati un alt fisier cu alte date

struct StructuraMasina {
	int id;
	int nrUsi;
	float pret;
	char* model;
	char* numeSofer;
	unsigned char serie;
};
typedef struct StructuraMasina Masina;
typedef struct Nod Nod;
typedef struct ListaDubla ListaDubla;

//creare structura pentru un nod dintr-o lista dublu inlantuita

//LDI poate fi parcursa si de la inceput catre sfarsit si invers; ea se foloseste tocmai pt aceste deplasari
//

struct Nod {
	Masina info;
	Nod* next;
	Nod* prev;
};


//creare structura pentru Lista Dubla 

struct ListaDubla { //incapsulare
	Nod* prim;
	Nod* ultim;
};

Masina citireMasinaDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	fgets(buffer, 100, file);
	char* aux;
	Masina m1;
	aux = strtok(buffer, sep);
	m1.id = atoi(aux);
	m1.nrUsi = atoi(strtok(NULL, sep));
	m1.pret = atof(strtok(NULL, sep));
	aux = strtok(NULL, sep);
	m1.model = malloc(strlen(aux) + 1);
	strcpy_s(m1.model, strlen(aux) + 1, aux);

	aux = strtok(NULL, sep);
	m1.numeSofer = malloc(strlen(aux) + 1);
	strcpy_s(m1.numeSofer, strlen(aux) + 1, aux);

	m1.serie = *strtok(NULL, sep);
	return m1;
}

void afisareMasina(Masina masina) {
	printf("Id: %d\n", masina.id);
	printf("Nr. usi : %d\n", masina.nrUsi);
	printf("Pret: %.2f\n", masina.pret);
	printf("Model: %s\n", masina.model);
	printf("Nume sofer: %s\n", masina.numeSofer);
	printf("Serie: %c\n\n", masina.serie);
}

void afisareListaMasini(ListaDubla lista) {
	//afiseaza toate elemente de tip masina din lista dublu inlantuita
	//prin apelarea functiei afisareMasina()
	Nod* p = lista.prim;
	while (p) {
		afisareMasina(p->info);
		p = p->next;
	}
}

void afisareInversaListaMasini(ListaDubla lista) {
	Nod* p = lista.ultim;
	while (p) {
		afisareMasina(p->info);
		p = p->prev;
	}
}

void adaugaMasinaInLista(ListaDubla* lista, Masina masinaNoua) {
	//adauga la final in lista primita o noua masina pe care o primim ca parametru
	Nod* nou = malloc(sizeof(Nod));
	nou->info = masinaNoua;
	nou->next = nou->prev = NULL;
	if (lista->ultim != NULL) {
		nou->prev = lista->ultim;
		lista->ultim->next = nou;
		lista->ultim = nou;
	}
	else {
		lista->ultim = lista->prim = nou;
	}
}

void adaugaLaInceputInLista(ListaDubla* lista, Masina masinaNoua) {
	//adauga la inceputul listei dublu inlantuite o noua masina pe care o primim ca parametru
	Nod* nou = malloc(sizeof(Nod));
	nou->info = masinaNoua;
	nou->next = nou->prev = NULL;
	if (lista->prim) {
		nou->next = lista->prim;
		lista->prim->prev = nou;
		lista->prim = nou;
	}
	else {
		lista->ultim = lista->prim = nou;
	}
}

ListaDubla* citireLDMasiniDinFisier(const char* numeFisier) {
	//functia primeste numele fisierului, il deschide si citeste toate masinile din fisier
	//prin apelul repetat al functiei citireMasinaDinFisier()
	//ATENTIE - la final inchidem fisierul/stream-ul
	FILE* file = fopen(numeFisier, "r");
	if (file) {
		ListaDubla* lista = malloc(sizeof(ListaDubla));
		lista->prim = NULL;
		lista->ultim = NULL;
		while (!feof(file)) { //cat timp nu am ajuns la sfarsitul fisierului
			adaugaMasinaInLista(lista, citireMasinaDinFisier(file));
		}
		fclose(file);
		return lista;
	}
	else {
		//return NULL; nu e ok
		//
		ListaDubla* lista = malloc(sizeof(ListaDubla));
		lista->prim = NULL;
		lista->ultim = NULL;
		return lista;
	}

}

void dezalocareLDMasini(ListaDubla** lista) {
	//sunt dezalocate toate masinile si lista dublu inlantuita de elemente
	Nod* p = (*lista)->prim;
	while (p) {
		free(p->info.model);
		free(p->info.numeSofer);
		Nod* aux = p;
		p = p->next;
		free(aux);
	}
	free(*lista);
	(*lista) = NULL;
}

float calculeazaPretMediu(ListaDubla lista) { //* permite accesarea si modificarea listei
	//calculeaza pretul mediu al masinilor din lista.
	float suma = 0;
	int count = 0;
	Nod* p = lista.prim;
	while (p) {
		suma += p->info.pret;
		count++;
		p = p->next;

	}
	if (count > 0)
		return suma / count;
	else
		return 0;
}

void stergeMasinaDupaID(ListaDubla* lista, int id) {
	//sterge masina cu id-ul primit.
	//tratati situatia ca masina se afla si pe prima pozitie, si pe ultima pozitie

	if (lista->prim->info.id == id) {
		Nod* aux = lista->prim;
		lista->prim = aux->next;
		if (lista->prim) {
			lista->prim->prev = NULL;
		}
		else {
			lista->ultim = NULL;
		}
		free(aux->info.model);
		free(aux->info.numeSofer);
		free(aux);
		return;
	}
	Nod* p = lista->prim;
	while (p && p->info.id != id) {
		p = p->next;
	}
	if (p) {
		p->prev->next = p->next;
		if (p->next) {
			p->next->prev = p->prev;
		}
		else {
			lista->ultim = p->prev;
		}
		free(p->info.model);
		free(p->info.numeSofer);
		free(p);
	}

}

char* getNumeSoferMasinaScumpa(ListaDubla lista) {
	//cauta masina cea mai scumpa si 
	//returneaza numele soferului acestei maasini.
	if (lista.prim) {
		float pretMaxim = lista.prim->info.pret;
		char* sofer = lista.prim->info.numeSofer;
		Nod* p = lista.prim->next;
		while (p) {
			if (p->info.pret > pretMaxim) {
				pretMaxim = p->info.pret;
				sofer = p->info.numeSofer;
			}
			p = p->next;
		}
		char* numeSofer = malloc(sizeof(char) * (strlen(sofer) + 1));
		strcpy(numeSofer, sofer);
		return numeSofer;
	}

	return NULL;
}

int main() {
	ListaDubla* lista = citireLDMasiniDinFisier("masini.txt");
	afisareListaMasini(*lista);

	printf("Pret mediu: %.2f", calculeazaPretMediu(*lista));

	printf("\n");

	printf("Sofer bogat: %s", getNumeSoferMasinaScumpa(*lista));

	printf("\n");
	printf("\n");

	stergeMasinaDupaID(lista, 5);

	afisareListaMasini(*lista);

	dezalocareLDMasini(&lista);

	return 0;
}