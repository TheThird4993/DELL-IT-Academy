// Data: 17/04/2025 - 23/04/2025
// Autor: Rodrigo Presa Pacheco
// Finalidade: Trabalho técnico, IT Academy

#include "functions.h" // aglomerado de includes e funções utilizadas

int main() {
//----------------------------------------------------------------------------------------------- "criação" das startups
	Startup* empresas;
	unsigned int num_empresas = 0, rodadas = 1;

	srand((unsigned)time(NULL));

	while (1) {
		printf("Digite o numero de empresas participantes(num par, entre 4 e 8):\n");
		scanf("%d", &num_empresas);
		if (num_empresas < 4 || num_empresas > 8 || num_empresas & 1) printf("Numero de empresas invalido\n");
		else break;
	}

	empresas = (Startup*)malloc(sizeof(Startup) * num_empresas);

	for (int i = 0; i < num_empresas; i++) {
		printf("\nDigite o nome da empresa: %d/%d\n", i + 1, num_empresas);
		scanf("%s", empresas[i].nome);

		printf("\nDigite o slogan da empresa: %d/%d\n", i + 1, num_empresas);
		scanf("%s", empresas[i].slogan);

		while (1) {
			printf("\nDigite o ano de fundacao da empresa: %d/%d\n", i + 1, num_empresas);
			scanf("%d", &empresas[i].ano_f);

			if (empresas[i].ano_f < 0 || empresas[i].ano_f > 2025) printf("Ano invalido\n");
			else break;
		}
		for (int j = 0; j < EVENTS; j++) {
			empresas[i].vet_eventos[j] = 0;
		}
		empresas[i].pts = 70;
	}

//----------------------------------------------------------------------------------------------- onde ocorre a batalha em si

	batalha(empresas, num_empresas);

//----------------------------------------------------------------------------------------------- classificações e estatisticas

	qsort(empresas, num_empresas, sizeof(Startup), compararPontuacao);

	if (((rand() % 5) + 1) == ((rand() % 5) + 1)) {
		revanche(empresas);
		qsort(empresas, num_empresas, sizeof(Startup), compararPontuacao);
	}

	printf("\nVencedor:\n1 lugar: %s (Pontuacao: %d)\n%s\n\n", empresas[0].nome, empresas[0].pts, empresas[0].slogan);

	for (int i = 1; i < num_empresas; i++) {
		printf("%d lugar: %s (Pontuacao: %d)\n", i + 1, empresas[i].nome, empresas[i].pts);
	}

	printf("\n\nEstatisticas:\n");
	for (int i = 0; i < num_empresas; i++) {
		printf("startup %s:   pitches: %d, bugs: %d, tracoes: %d, investidores irritados: %d, fake news: %d\n", empresas[i].nome, empresas[i].vet_eventos[0], empresas[i].vet_eventos[1], empresas[i].vet_eventos[2], empresas[i].vet_eventos[3], empresas[i].vet_eventos[4]);
	}

	free(empresas);

//----------------------------------------------------------------------------------------------- fim

	return 0;
}