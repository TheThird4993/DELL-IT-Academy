// Data: 17/04/2025
// Autor: Rodrigo Presa Pacheco
// Finalidade: Trabalho técnico, IT Academy

#include <stdio.h>							// input & output					
#include <stdint.h>							// uint8_t para usar como flags
#include <stdlib.h>							// para alocar dinamicamente a quantidade de empresas
#include <time.h>							// para gerar num "aleatorios"
#include <string.h>

#define TAM_MAX 101							// 100 caracteres + "\0"
#define LINHAS 2
#define TAM 7
#define EVENTS 5

typedef struct Empresa Empresa;

struct Empresa {
	char nome[TAM_MAX], slogan[TAM_MAX];
	unsigned int pts;
	int ano_f, battle_id;
	uint8_t eventos;
	int vet_eventos[EVENTS];
};

int pareamento(Empresa*, int, int*);

void sharkFight(Empresa*, int, int);

int** funcMatriz(int, Empresa*, int);

void liberarMatriz(int**);

int rodada(int, int, Empresa*);

int evento(int, int, Empresa*);

int* batalha(Empresa*, int);

int compararPontuacao(const void*, const void*);

void revanche();

int main() {
	Empresa* empresas;
	unsigned int num_empresas = 0, rodadas = 1;
	int* perdedores;

	srand((unsigned)time(NULL));

	while (1) {
		printf("Digite o numero de empresas participantes(num par, entre 4 e 8):\n");
		scanf("%d", &num_empresas);
		if (num_empresas < 4 || num_empresas > 8 || num_empresas & 1) printf("Numero de empresas invalido\n");
		else break;
	}

	empresas = (Empresa*)malloc(sizeof(Empresa) * num_empresas);

	for (int i = 0; i < num_empresas; i++) {
		printf("\nDigite o nome da empresa: %d/%d\n", i + 1, num_empresas);
		scanf("%s", empresas[i].nome);

		printf("\nDigite o slogan da empresa: %d/%d\n", i + 1, num_empresas);
		scanf("%s", empresas[i].slogan);

		while (1) {
			printf("\nDigite o ano de fundacao da empresa: %d/%d\n", i + 1, num_empresas);
			scanf("%d", &empresas[i].ano_f);

			if (empresas[i].ano_f < 0) printf("Ano invalido\n");
			else break;
		}
		for (int j = 0; j < EVENTS; j++) {
			empresas[i].vet_eventos[j] = 0;
		}
		empresas[i].pts = 70;
	}

	perdedores = (int*)malloc(sizeof(int) * TAM);
	memcpy(perdedores, batalha(empresas, num_empresas), TAM * sizeof(int));

	int vencedor = -1;
	for (int i = 0; i < num_empresas; i++) {
		int encontrado = 0;
		for (int j = 0; j < TAM; j++) {
			if (perdedores[j] == i) {
				encontrado = 1;
				break;
			}
		}
		if (!encontrado) {
			vencedor = i;
			break;
		}
	}

	printf("\nVencedor:\n1 lugar: %s (Pontuacao: %d)\n%s\n\n", empresas[vencedor].nome, empresas[vencedor].pts, empresas[vencedor].slogan);

	qsort(empresas, num_empresas, sizeof(Empresa), compararPontuacao);

	for (int i = 1; i < num_empresas; i++) {
		printf("%d lugar: %s (Pontuacao: %d)\n", i + 1, empresas[i].nome, empresas[i].pts);
	}

	printf("\n\nEstatisticas:\n");
	for (int i = 0; i < num_empresas; i++) {
		printf("startup %s:   pitches: %d, bugs: %d, trações: %d, investidores irritados: %d, fake news: %d\n", empresas[i].nome, empresas[i].vet_eventos[0], empresas[i].vet_eventos[1], empresas[i].vet_eventos[2], empresas[i].vet_eventos[3], empresas[i].vet_eventos[4]);
	}

	free(perdedores);
	free(empresas);
	printf("\nisover\n");
	return 0;
}

int pareamento(Empresa* empresas, int quant, int* vet_perd) {
	int* vet;
	int vet_perdedores[TAM];
	int num_duplas = 0, eliminados = 0;
	int vet_teste[TAM + 1] = { 0 };

	for (int i = 0; i < TAM; i++) {
		vet_perdedores[i] = vet_perd[i];
	}

	for (int i = 0; i < quant; i++) {
		empresas[i].battle_id = -1;
		vet_teste[i] = -1;
	}

	for (int i = 0; i < TAM; i++) {
		if (vet_perdedores[i] != -1) {
			eliminados++;
		}
	}

	num_duplas = (quant - eliminados) >> 1;

	if (!num_duplas) return 0;

	vet = (int*)malloc(sizeof(int) * num_duplas);

	for (int j = 0; j < num_duplas; j++) {
		vet[j] = 2;
	}

	for (int j = 0, cont = 0; j < quant; j++) {
		int var = (rand() % (num_duplas));
		for (int k = 0; k < TAM; k++) {
			if (j == vet_perdedores[k]) {
				j++;
				vet_perdedores[k] = -1;
				k = -1;
			}
		}
		if (vet[var]) {
			empresas[j].battle_id = var;
			vet_teste[j] = var;
			vet[var]--;
		}
		else j--;

		cont = 0;

		for (int l = 0; l < quant; l++) {
			if (empresas[l].battle_id != -1) cont++;
		}
		if (cont == (num_duplas << 1)) break;
	}


	free(vet);
	return num_duplas;
}

int** funcMatriz(int num_bat, Empresa* emp, int num_emp) {
	int** mat;
	int colunas = num_bat;

	mat = (int**)malloc(LINHAS * sizeof(int*));

	for (int l = 0; l < LINHAS; l++) {
		mat[l] = (int*)malloc(colunas * sizeof(int));
	}

	//--------------------------------------------------// preencher a matriz com '-1'
	for (int l = 0; l < LINHAS; l++) {					//
		for (int k = 0; k < colunas; k++) {				//
			mat[l][k] = -1;								//
		}												//
	}													//
	//--------------------------------------------------//

	for (int i = 0, var = 0; i < num_emp; i++) {
		var = emp[i].battle_id;
		if (var == -1) continue;
		if (mat[0][var] == -1) mat[0][var] = i;
		else mat[1][var] = i;
	}

	return mat;
}

void sharkFight(Empresa* empresas, int emp_a, int emp_b) {
	switch ((rand() % 2) + 1) {
	case 1: empresas[emp_a].pts += 2;
		break;
	case 2: empresas[emp_b].pts += 2;
		break;
	};
	return;
}

void liberarMatriz(int** matriz) {
	for (int i = 0; i < LINHAS; i++) {
		free(matriz[i]); // Libera cada linha
	}
	free(matriz); // Libera o array de ponteiros
}

int rodada(int emp_1, int emp_2, Empresa* empresas) {
	int sel_e = 0, rodadas = 1;

	for (int var = emp_1, i = 0, j = 0; i != 1;) {
		printf("Batalha %d\n", rodadas++);
		printf("\n| Empresas:  | %s | vs | %s |\n", empresas[emp_1].nome, empresas[emp_2].nome);
		printf("| Pontuacao: |   %d   |    |   %d   |\n\n", empresas[emp_1].pts, empresas[emp_2].pts);
		printf("Digite o evento desejado para a startup %s\n", empresas[var].nome);
		printf("[0] Nada(Proxima startup): +0 pontos\n");
		printf("[1] Pitch convincente: +6 pontos\n");
		printf("[2] Produto com bugs: -4 pontos\n");
		printf("[3] Boa tracao de usuario: +3 pontos\n");
		printf("[4] Investidor irritado: -6 pontos\n");
		printf("[5] Fake news no pitch: -8 pontos\n\n");
		scanf("%d", &sel_e);

		if (sel_e < 0 || sel_e > 5) printf("Evento invalido\n");
		else if (!sel_e) {
			if (var == emp_2) i = 1;
			var = emp_2;
			continue;
		}

		if (evento(sel_e, var, empresas)) continue;
		else {
			printf("Evento ja ocorreu com esta startup nesta rodada\n\n");
			rodadas--;
		}
	}

	if (empresas[emp_1].pts == empresas[emp_2].pts) sharkFight(empresas, emp_1, emp_2);

	if (empresas[emp_1].pts > empresas[emp_2].pts) {
		printf("\n%s venceu a batalha!\n +30 pontos\n\n", empresas[emp_1].nome);
		empresas[emp_1].pts += 30;
		return emp_2;
	}
	else if (empresas[emp_2].pts > empresas[emp_1].pts) {
		printf("\n%s venceu a batalha!\n +30 pontos\n\n", empresas[emp_2].nome);
		empresas[emp_2].pts += 30;
		return emp_1;
	}

	return -1;
}

int evento(int num, int num_emp, Empresa* emp) {
	switch (num) {
	case 1: {
		if (emp[num_emp].eventos & 1) {
			printf("A startup %s fez um pitch convincente!\n +6 pontos\n\n", emp[num_emp].nome);
			emp[num_emp].pts += 6;
			emp[num_emp].eventos &= ~1;
			emp[num_emp].vet_eventos[0]++;
		}
		else return 0;
	}
		  break;

	case 2: {
		if (emp[num_emp].eventos & 2) {
			printf("A startup %s apresentou um produto com bugs!\n -4 pontos\n\n", emp[num_emp].nome);
			emp[num_emp].pts -= 4;
			emp[num_emp].eventos &= ~2;
			emp[num_emp].vet_eventos[1]++;
		}
		else return 0;
	}
		  break;

	case 3: {
		if (emp[num_emp].eventos & 4) {
			printf("A startup %s teve uma boa tracao de usuarios!\n +3 pontos\n\n", emp[num_emp].nome);
			emp[num_emp].pts += 3;
			emp[num_emp].eventos &= ~4;
			emp[num_emp].vet_eventos[2]++;
		}
		else return 0;
	}
		  break;

	case 4: {
		if (emp[num_emp].eventos & 8) {
			printf("A startup %s fez um investidor ficar irritado!\n -6 pontos\n\n", emp[num_emp].nome);
			emp[num_emp].pts -= 6;
			emp[num_emp].eventos &= ~8;
			emp[num_emp].vet_eventos[3]++;
		}
		else return 0;
	}
		  break;

	case 5: {
		if (emp[num_emp].eventos & 16) {
			printf("A startup %s apresentou fake news no pitch!\n -8 pontos\n\n", emp[num_emp].nome);
			emp[num_emp].pts -= 8;
			emp[num_emp].eventos &= ~16;
			emp[num_emp].vet_eventos[4]++;
		}
		else return 0;
	}
		  break;
	}
	return 1;
}

int* batalha(Empresa* empresas, int num_empresas) {
	int** matriz_batalhas;
	int sel = 0, cont_bat = 1, cont = 0, indice = 0;
	static int vet_cont = 0, teste = 0;
	int um_vencedor = 0, num_batalhas = 0, num_sel = 0;
	int perdedor[TAM] = { 0 };
	//Empresa* empresas;

	teste = num_empresas - 1;

	for (int i = 0; i < TAM; i++) perdedor[i] = -1;

	while (1) {
		//empresas = (Empresa*)malloc(num_empresas * sizeof(Empresa));
		//memcpy(empresas, emp, num_empresas * sizeof(Empresa));
		num_batalhas = pareamento(empresas, num_empresas, perdedor);
		matriz_batalhas = funcMatriz(num_batalhas, empresas, num_empresas);

		if (!num_batalhas) break;

		printf("\nRodada %d", cont_bat++);

		um_vencedor = 0;

		while (!um_vencedor) {

			printf("\nDigite o numero da batalha que deseja administrar: \n");

			/*printf("\n");
			for (int k = 0; k < num_batalhas; k++) printf("elemento [0][%d] da matriz: %d\n", k, matriz_batalhas[0][k]);
			printf("\n");
			printf("\n");
			for (int k = 0; k < num_batalhas; k++) printf("elemento [1][%d] da matriz: %d\n", k, matriz_batalhas[1][k]);
			printf("\n");*/

			num_sel = 0;

			for (int i = 0, j = 1; i < num_batalhas; i++, j++) {
				while (matriz_batalhas[0][i] == -1) i++;
				if ((i + 1) > num_batalhas) break;
				printf("[%d] batalha %s vs %s\n", j, empresas[matriz_batalhas[0][i]].nome, empresas[matriz_batalhas[1][i]].nome);
				num_sel++;
			}

			while (1) {
				scanf("%d", &sel);
				//printf("\n\nvalor: %d\n", matriz_batalhas[0][sel - 1]);

				for (indice = 0, cont = 0; indice < num_batalhas; indice++) {
					if (matriz_batalhas[0][indice] != -1) cont++;
					if (cont == sel) break;
					//printf("\nindice: %d\n", indice);
				}

				//printf("\nindice: %d\n", indice);
				//printf("\ncont: %d\n", cont);


				//printf("\nvalor: %d\n\n", matriz_batalhas[0][indice]);

				if (sel <= 0 || sel > num_sel) printf("Selecao invalida\n");
				else break;
			}

			for (int i = 0; i < num_empresas; i++) empresas[i].eventos = 31; /// 0001 1111 -> flags para eventos

			perdedor[vet_cont++] = rodada(matriz_batalhas[0][indice], matriz_batalhas[1][indice], empresas);

			for (int i = 0; i < 2; i++) matriz_batalhas[i][indice] = -1;

			//printf("\n\nperdedores: ");
			//for (int i = 0; i < vet_cont; i++) printf("%d ", perdedor[i]);
			//printf("\n\n");

			if (cont_bat == 2) {
				for (int i = 0; i < (num_empresas >> 1); i++) {
					um_vencedor = 1;
					if (perdedor[i] == -1) um_vencedor = 0;
				}
			}
			else if (num_empresas == 8 || num_empresas == 6) {
				for (int i = 0; i < num_empresas - 2; i++) {
					um_vencedor = 1;
					if (perdedor[i] == -1) um_vencedor = 0;
				}
			}
			else if (num_empresas == 4) {
				for (int i = 0; i <= num_empresas - 2; i++) {
					um_vencedor = 1;
					if (perdedor[i] == -1) um_vencedor = 0;
				}
			}
			//printf("\n\ncont: %d\n", cont_bat);
			//printf("um_vencedor %d:\n\n ", um_vencedor);
		}


		liberarMatriz(matriz_batalhas);
	}

	liberarMatriz(matriz_batalhas);
	return perdedor;
}

int compararPontuacao(const void* a, const void* b) {
	Empresa* empresaA = (Empresa*)a;
	Empresa* empresaB = (Empresa*)b;

	// ordena em ordem decrescente de pontuação
	return (empresaB->pts - empresaA->pts);
}
