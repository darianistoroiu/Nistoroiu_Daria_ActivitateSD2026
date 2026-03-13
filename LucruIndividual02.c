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

Masina initializare(int id, int nrUsi, float pret, char* model, char* numeSofer, unsigned char serie) {
	Masina m;
	m.id = id;
	m.nrUsi = nrUsi;
	m.pret = pret;
	m.model = (char*)malloc(sizeof(char) * (strlen(model) + 1));
	strcpy_s(m.model, strlen(model) + 1, model);
	m.numeSofer = (char*)malloc(sizeof(char) * (strlen(numeSofer) + 1));
	strcpy_s(m.numeSofer, strlen(numeSofer) + 1, numeSofer);
	m.serie = serie;
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

void afisareVectorMasini(Masina* masini, int nrMasini) {
	for (int i = 0;i < nrMasini;i++) {
		afisareMasina(masini[i]);
		printf("\n");
	}
}
void adaugaMasinaNoua(Masina** masini, int* nrMasini, Masina masinaNoua) {
	Masina* aux = (Masina*)malloc(sizeof(Masina) * ((*nrMasini) + 1));
	for (int i = 0;i < (*nrMasini);i++) {
		aux[i] = (*masini)[i];
	}
	aux[(*nrMasini)] = masinaNoua;
	//aux[i].model = (char*)malloc(sizeof(char) * (strlen(masinaNoua.model) + 1));
	//strcpy_s(aux[i].model, strlen(masinaNoua.model) + 1, masinaNoua.model);

	//aux[i].numeSofer = (char*)malloc(sizeof(char) * (strlen(masinaNoua.numeSofer) + 1));
	//strcpy_s(aux[i].numeSofer, strlen(masinaNoua.numeSofer) + 1, masinaNoua.numeSofer);
	free(*masini);
	(*masini) = aux;
	(*nrMasini)++;

}
Masina citireMasinaFisier(FILE* file) {
	Masina m;
	char buffer[100];
	char sep[4] = ",;\n";
	fgets(buffer, 100, file);
	m.id = atoi(strtok(buffer, sep));
	m.nrUsi= atoi(strtok(NULL, sep));
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

Masina* citireVectorMasiniFisier(const char* numeFisier, int* nrMasiniCitite) {
	FILE* file = fopen(numeFisier, "r");
	Masina* masini = NULL;
	(*nrMasiniCitite) = 0;
	while (!feof(file)) {
		adaugaMasinaNoua(&masini, nrMasiniCitite, citireMasinaFisier(file));
	}
	fclose(file);
	return masini;

}
void dezalocareVectorMasini(Masina** masini, int* nrMasini) {
	for (int i = 0;i < *nrMasini;i++) {
		if((*masini)[i].model!=NULL)
		free((*masini)[i].model);
		if ((*masini)[i].numeSofer != NULL)
		free((*masini)[i].numeSofer);
	}
	free(*masini);
	(*masini) = NULL;
	(*nrMasini) = 0;
}

int main() {
	int nrMasini = 3;
	Masina* masini = (Masina*)malloc(sizeof(Masina)*nrMasini);
	masini[0] = initializare(1, 4, 1500, "vw", "ionel", 'a');
	masini[1] = initializare(2, 4, 1200, "opel", "alex", 'a');
	masini[2] = initializare(3, 4, 15000, "audi", "stefan", 'a');

	afisareVectorMasini(masini, nrMasini);


	printf("Masini din fisier:\n");
	int nrMasiniCitite = 0;
	Masina* m = citireVectorMasiniFisier("masini.txt", &nrMasiniCitite);
	afisareVectorMasini(m, nrMasiniCitite);
	dezalocareVectorMasini(&m, &nrMasiniCitite);
	dezalocareVectorMasini(&masini, &nrMasini);


}
