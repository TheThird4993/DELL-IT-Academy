// Data: 17/04/2025 - 23/04/2025
// Autor: Rodrigo Presa Pacheco
// Finalidade: Trabalho técnico, IT Academy

#include "functions.h"

// atribuição de "battle ids" e número de duplas que irão se enfrentar
int pareamento(Startup* empresas, int quant, int* vet_perd) {
	int* vet;
	int vet_perdedores[TAM];
	int num_duplas = 0, eliminados = 0;

	// debug 1
	//int vet_teste[TAM + 1] = { 0 };
	//

	for (int i = 0; i < TAM; i++) {
		vet_perdedores[i] = vet_perd[i];
	}

	
	for (int i = 0; i < quant; i++) {
		empresas[i].battle_id = -1;	   
	//	vet_teste[i] = -1;			   
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
			//vet_teste[j] = var;
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

// criação dinâmica de uma matriz que armazenará os indices do vetor empresas[] que contem o mesmo "battle_id" que o valor da coluna
// exemplo: emp[2].battle_id -> 0, emp[5].battle_id -> 0; matriz[0][0] = 2, matriz[1][0] = 5.
//          emp[0].battle_id -> 1, emp[3].battle_id -> 1; matriz[0][1] = 0, matriz[1][1] = 3.
//          e assim em diante...
int** funcMatriz(int num_bat, Startup* emp, int num_emp) {
	int** mat;
	int colunas = num_bat;

	mat = (int**)malloc(LINHAS * sizeof(int*));

	for (int l = 0; l < LINHAS; l++) {
		mat[l] = (int*)malloc(colunas * sizeof(int)); // numero de colunas é igual ao num de batalhas que irão ocorrer
	}

	//--------------------------------------------------// preencher a matriz com '-1' para sinalizar os indices que ainda não foram preenchidos
	for (int l = 0; l < LINHAS; l++) {					//
		for (int k = 0; k < colunas; k++) {				//
			mat[l][k] = -1;								//
		}												//
	}													//
	//--------------------------------------------------//

	for (int i = 0, var = 0; i < num_emp; i++) { // se a "battle_id" de uma emp[] for -1, isso significa que essa empresa ja perdeu
		var = emp[i].battle_id;
		if (var == -1) continue;
		if (mat[0][var] == -1) mat[0][var] = i;
		else mat[1][var] = i;
	}

	return mat;
}

// seleção aleatoria entre as duas empresas para desempate
void sharkFight(Startup* empresas, int emp_a, int emp_b) {
	switch ((rand() % 2) + 1) {
	case 1: empresas[emp_a].pts += 2;
		break;
	case 2: empresas[emp_b].pts += 2;
		break;
	};
	return;
}

// para dar free na matriz
void liberarMatriz(int** matriz) {
	for (int i = 0; i < LINHAS; i++) {
		free(matriz[i]); // Libera cada linha
	}
	free(matriz); // Libera o array de ponteiros
}

// onde as "batalhas" ocorrem (por mais que o nome da função em si seja "rodada")
int rodada(int emp_1, int emp_2, Startup* empresas) {
	int sel_e = 0, rodadas = 1;

	for (int var = emp_1, i = 0, j = 0; i != 1;) {
		printf("\nBatalha %d\n", rodadas++);
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

		if (sel_e < 0 || sel_e > 5) {
			printf("\nEvento invalido\n");
			rodadas--;
		}
		else if (!sel_e) {
			if (var == emp_2) i = 1;
			var = emp_2;
			continue;
		}

		if (evento(sel_e, var, empresas)) continue;
		else {
			printf("\nEvento ja ocorreu com esta startup nesta rodada\n");
			rodadas--;
		}
	}

	if (empresas[emp_1].pts == empresas[emp_2].pts) sharkFight(empresas, emp_1, emp_2);

	if (empresas[emp_1].pts > empresas[emp_2].pts) {
		printf("\n%s venceu a batalha!\n +30 pontos\n", empresas[emp_1].nome);
		empresas[emp_1].pts += 30;
		return emp_2;
	}
	else if (empresas[emp_2].pts > empresas[emp_1].pts) {
		printf("\n%s venceu a batalha!\n +30 pontos\n", empresas[emp_2].nome);
		empresas[emp_2].pts += 30;
		return emp_1;
	}

	return -1;
}

// switch case para a seleção do evento escolhido pelo usuário
// utilizei bitwise e flags para determinar se um certo evento ja ocorreu ou não, e um and com o valor negado para desligar o bit da flag
int evento(int num, int num_emp, Startup* emp) {
	switch (num) {
	case 1: {
		if (emp[num_emp].eventos & 1) {
			printf("\nA startup %s fez um pitch convincente!\n +6 pontos\n", emp[num_emp].nome);
			emp[num_emp].pts += 6;
			emp[num_emp].eventos &= ~1;
			emp[num_emp].vet_eventos[0]++;
		}
		else return 0;
	}
		  break;

	case 2: {
		if (emp[num_emp].eventos & 2) {
			printf("\nA startup %s apresentou um produto com bugs!\n -4 pontos\n", emp[num_emp].nome);
			emp[num_emp].pts -= 4;
			emp[num_emp].eventos &= ~2;
			emp[num_emp].vet_eventos[1]++;
		}
		else return 0;
	}
		  break;

	case 3: {
		if (emp[num_emp].eventos & 4) {
			printf("\nA startup %s teve uma boa tracao de usuarios!\n +3 pontos\n", emp[num_emp].nome);
			emp[num_emp].pts += 3;
			emp[num_emp].eventos &= ~4;
			emp[num_emp].vet_eventos[2]++;
		}
		else return 0;
	}
		  break;

	case 4: {
		if (emp[num_emp].eventos & 8) {
			printf("\nA startup %s fez um investidor ficar irritado!\n -6 pontos\n", emp[num_emp].nome);
			emp[num_emp].pts -= 6;
			emp[num_emp].eventos &= ~8;
			emp[num_emp].vet_eventos[3]++;
		}
		else return 0;
	}
		  break;

	case 5: {
		if (emp[num_emp].eventos & 16) {
			printf("\nA startup %s apresentou fake news no pitch!\n -8 pontos\n", emp[num_emp].nome);
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

// função onde o usuário seleciona as batalhas disponiveis para administar
// onde maior parte do debug foi feito
void batalha(Startup* empresas, int num_empresas) {
	int** matriz_batalhas;
	int sel = 0, cont_bat = 1, cont = 0, indice = 0;
	static int vet_cont = 0, teste = 0;
	int um_vencedor = 0, num_batalhas = 0, num_sel = 0;
	int perdedor[TAM] = { 0 };
	//Empresa* empresas;

	teste = num_empresas - 1;

	for (int i = 0; i < TAM; i++) perdedor[i] = -1;

	while (1) {
		num_batalhas = pareamento(empresas, num_empresas, perdedor);
		matriz_batalhas = funcMatriz(num_batalhas, empresas, num_empresas);

		//-------------------------------------------------------------+ debug 1
		//empresas = (Empresa*)malloc(num_empresas * sizeof(Empresa)); |
		//memcpy(empresas, emp, num_empresas * sizeof(Empresa));	   |
		//-------------------------------------------------------------+

		if (!num_batalhas) break;

		printf("\nRodada %d", cont_bat++);

		um_vencedor = 0;

		while (!um_vencedor) {
			// sel de batalha
			printf("\nDigite o numero da batalha que deseja administrar: \n");

			//----------------------------------------------------------------------------------------------------------+ debug 2
			/*printf("\n");																								|
			for (int k = 0; k < num_batalhas; k++) printf("elemento [0][%d] da matriz: %d\n", k, matriz_batalhas[0][k]);|
			printf("\n");																								|
			printf("\n");																								|
			for (int k = 0; k < num_batalhas; k++) printf("elemento [1][%d] da matriz: %d\n", k, matriz_batalhas[1][k]);|
			printf("\n");																								|
			//----------------------------------------------------------------------------------------------------------+*/

			num_sel = 0;

			// for para exibição das batalhas disponiveis
			for (int i = 0, j = 1; i < num_batalhas; i++, j++) {
				while (matriz_batalhas[0][i] == -1) i++;
				if ((i + 1) > num_batalhas) break;
				printf("[%d] batalha %s vs %s\n", j, empresas[matriz_batalhas[0][i]].nome, empresas[matriz_batalhas[1][i]].nome);
				num_sel++;
			}

			printf("\n");

			while (1) {
				scanf("%d", &sel);

				//-------------------------------------------------------+ debug 3
				//printf("\n\nvalor: %d\n", matriz_batalhas[0][sel - 1]);|
				//-------------------------------------------------------+

				// como a batalha num [1] mostrada na tela anterior pode não se referir a batalha desejada,
				// esse for garante que se o usuario escolher a batalha [x] ela realmente vai acontecer, e não outra
				for (indice = 0, cont = 0; indice < num_batalhas; indice++) {
					if (matriz_batalhas[0][indice] != -1) cont++;
					if (cont == sel) break;

					//---------------------------------+ debug 4
					//printf("\nindice: %d\n", indice);|
					//---------------------------------+
				}

				//------------------------------------------------------+ debug 5
				//printf("\nindice: %d\n", indice);						|
				//printf("\ncont: %d\n", cont);							|
				//														|
				//printf("\nvalor: %d\n\n", matriz_batalhas[0][indice]);|
				//------------------------------------------------------+
				 
				// caso o usuario seleciona uma batalha que não existe
				if (sel <= 0 || sel > num_sel) printf("Selecao invalida\n");
				else break;
			}

			for (int i = 0; i < num_empresas; i++) empresas[i].eventos = 31; /// 0001 1111 -> flags para eventos

			// vetor para armazenar os indices de empresas[] que perderam, para estas serem excluidas das proximas rodadas
			perdedor[vet_cont++] = rodada(matriz_batalhas[0][indice], matriz_batalhas[1][indice], empresas);

			for (int i = 0; i < 2; i++) matriz_batalhas[i][indice] = -1;

			//--------------------------------------------------------------+ debug 7
			//printf("\n\nperdedores: ");									|
			//for (int i = 0; i < vet_cont; i++) printf("%d ", perdedor[i]);|
			//printf("\n\n");												|
			//--------------------------------------------------------------+

			// calculo para checar se todos - 1 nessa rodada foram derrotados
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

			//--------------------------------------------+ debug 8
			//printf("\n\ncont: %d\n", cont_bat);		  |
			//printf("um_vencedor %d:\n\n ", um_vencedor);|
			//--------------------------------------------+
		}


		liberarMatriz(matriz_batalhas);
	}

	liberarMatriz(matriz_batalhas);

	// return
	return;
}

// ordena em ordem decrescente de pontuação
int compararPontuacao(const void* a, const void* b) {
	Startup* empresaA = (Startup*)a;
	Startup* empresaB = (Startup*)b;

	return (empresaB->pts - empresaA->pts);
}

// [6] Feature extra, 1/5 de chance da startup em segundo lugar desafiar a campeã para uma revanche pelo título
void revanche(Startup* emp) {
	for (int i = 0; i < 2; i++) emp[i].eventos = 31;
	printf("+-----------------------+");
	printf("\n| Revanche pelo titulo! |\n");
	printf("+-----------------------+");
	emp[1].pts += 20; //isso para a startup desafiante ter uma chance
	rodada(0, 1, emp);
	return;
}
