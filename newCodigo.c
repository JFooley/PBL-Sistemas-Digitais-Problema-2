#include "lib.c"
#include <stdio.h>

// Funções fornecidas (supondo que já estão implementadas em algum lugar)
//void WBR_BG(int R, int G, int B);
//void WBM(int endereco, int R, int G, int B);
//void DP(int endereco, int refX, int refY, int tamanho, int R, int G, int B, int forma);

int main() {
    // Define a cor de fundo verde (R=0, G=3, B=0)
    WBR_BG(0, 3, 0);

    // Endereço de memória inicial (linha de memória em 12 bits)
    int endereco;

    // Tamanho do bloco de memória 80x60 (80 colunas e 60 linhas de blocos 8x8 pixels)
    int larguraTela = 80;
    int alturaTela = 60;
	int y = 0;
	int x = 0;
    // Desativar todos os blocos de fundo para assumirem a cor de fundo verde
    for (y = 0; y < alturaTela; y++) {
        for (x = 0; x < larguraTela; x++) {
            endereco = (y * larguraTela) + x;
            WBM(endereco, 7, 7, 7);  // Bloco desativado
        }
    }

    // Coordenadas para a listra preta (assumindo largura de 1 bloco)
    int larguraListra = 1;
    int posXCentral = larguraTela / 2;  // Centro da tela

    // Desenhar a listra preta vertical no centro da tela
    for (y = 0; y < alturaTela; y++) {
        endereco = (y * larguraTela) + posXCentral;
        WBM(endereco, 0, 0, 0);  // Bloco preto
    }

    // Desenhar um polígono preto no centro para complementar (opcional)
    // DP(int endereco, int refX, int refY, int tamanho, int R, int G, int B, int forma)
    // DP(0, posXCentral, 0, alturaTela * 8, 0, 0, 0, 0);  // Polígono quadrado preto

    return 0;
}

