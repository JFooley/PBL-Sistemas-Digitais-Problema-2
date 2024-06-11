#include "lib.c"
#include <stdio.h>

// Função para desenhar duas linhas verticais
void draw_vertical_lines() {
    // Dimensões da tela em blocos
    const int width_in_blocks = 80;
    const int height_in_blocks = 60;

    // Coordenadas das colunas onde as linhas serão desenhadas
    const int line1_column = 20;
    const int line2_column = 60;

    // Cor para as linhas (vermelho, por exemplo)
    unsigned long long R = 6;
    unsigned long long G = 7;
    unsigned long long B = 7;

    // Loop para percorrer todas as linhas da tela (em blocos)
    int row;
    for (row = 0; row < height_in_blocks; row++) {
        // Calcula o endereço de memória para cada linha nas colunas especificadas
        unsigned long long mem_address_line1 = ((row * width_in_blocks) + line1_column);
        unsigned long long mem_address_line2 = ((row * width_in_blocks) + line2_column);

        // Chama a função WBM para desenhar as linhas nas colunas especificadas
        WBM(mem_address_line1, R, G, B);
        WBM(mem_address_line2, R, G, B);
    }
}

void main(){
    // Seta backgroud
    //WBR_BG(0, 0, 7);

    draw_vertical_lines();

    // Memória de sprites
    // unsigned long long i;
    // for (i = 0; i < 400; i++) {
    //     WSM(25, i, 7, 0, 0);
    // }
    
    // Memoria de background
    // WBM(1, 7, 7, 7);

    //DP
    //DP(1, 0, 0, 0, 6, 7, 7, 0);

    // sprites
    // WBR_S(1, 1, 50, 100, 1);
    // WBR_S(2, 2, 100, 100, 1);
    // WBR_S(3, 3, 150, 100, 1);
    // WBR_S(4, 4, 200, 100, 1);
    // WBR_S(5, 5, 250, 100, 1);
    // WBR_S(6, 6, 300, 100, 1);
    // WBR_S(7, 7, 350, 100, 1);
    // WBR_S(8, 8, 400, 100, 1);
    // WBR_S(9, 9, 450, 100, 1);
    // WBR_S(10, 10, 500, 100, 1);
    // WBR_S(11, 11, 50, 200, 1);
    // WBR_S(12, 12, 100, 200, 1);
    // WBR_S(13, 13, 150, 200, 1);
    // WBR_S(14, 14, 200, 200, 1);
    // WBR_S(15, 15, 250, 200, 1);
    // WBR_S(16, 16, 300, 200, 1);
    // WBR_S(17, 17, 350, 200, 1);
    // WBR_S(18, 18, 400, 200, 1);
    // WBR_S(19, 19, 450, 200, 1);
    // WBR_S(20, 20, 500, 200, 1);
    // WBR_S(21, 21, 50, 300, 1);
    // WBR_S(22, 22, 100, 300, 1);
    // WBR_S(23, 23, 150, 300, 1);
    // WBR_S(24, 24, 200, 300, 1);
    // WBR_S(25, 25, 250, 300, 1);    
    // WBR_S(26, 26, 300, 300, 1);
    // WBR_S(27, 27, 350, 300, 1);
    // WBR_S(28, 28, 400, 300, 1);
}
