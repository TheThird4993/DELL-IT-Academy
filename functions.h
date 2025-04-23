// Data: 17/04/2025 - 23/04/2025
// Autor: Rodrigo Presa Pacheco
// Finalidade: Trabalho técnico, IT Academy
#pragma once

#include <stdio.h>							// input & output					
#include <stdint.h>							// uint8_t para usar como flags
#include <stdlib.h>							// para alocar dinamicamente a quantidade de empresas e para utilizar o qsort
#include <time.h>							// para gerar num "aleatorios"
#include <string.h>                         // para utilizar o memcopy

#define TAM_MAX 101							// 100 caracteres + "\0"
#define LINHAS 2							// tamanho de linhas na matriz de indices
#define TAM 7								// tamanho do vetor que armazena os indices dos perdedores
#define EVENTS 5							// quantidade de eventos que podem ocorrer

typedef struct Startup Startup;

struct Startup {
	char nome[TAM_MAX], slogan[TAM_MAX];
	unsigned int pts;
	int ano_f, battle_id;
	uint8_t eventos;
	int vet_eventos[EVENTS];
};

int pareamento(Startup*, int, int*);

void sharkFight(Startup*, int, int);

int** funcMatriz(int, Startup*, int);

void liberarMatriz(int**);

int rodada(int, int, Startup*);

int evento(int, int, Startup*);

void batalha(Startup*, int);

int compararPontuacao(const void*, const void*);

void revanche(Startup*);