#include "lib.c"
#include <stdio.h>

// Dimensões da tela em blocos
#define BACKGROUND_WIDTH 80
#define BACKGROUND_HEIGHT 60
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

// Função para limpar a tela
void clear_lines() {
    int i;
    for (i = 0; i < 4800; i++) {
        WBM(i, 7, 7, 7);
    }
}

// Função para desenhar duas linhas verticais
void draw_vertical_lines(int pos, int thickness, unsigned long long R, unsigned long long G, unsigned long long B) {
    // Loop para percorrer todas as linhas da tela (em blocos)
    int row;
    int count_thick;
    int count_pos;
    unsigned long long mem_address_line1;
    for (row = 0; row < BACKGROUND_HEIGHT; row++) {
        count_pos = pos;
        for (count_thick = 0; count_thick < thickness; count_thick++) {
            // Calcula o endereço de memória para cada linha nas colunas especificadas
            mem_address_line1 = ((row * BACKGROUND_WIDTH) + count_pos);

            // Chama a função WBM para desenhar as linhas nas colunas especificadas
            WBM(mem_address_line1, R, G, B);

            count_pos += 1;
        }
    }
}

void draw_horizontal_line(int line, int thickness, unsigned long long R, unsigned long long G, unsigned long long B) {
    int row;
    int j;
    int line_address;
    for (row = 0; row < BACKGROUND_HEIGHT; row++) {
        if (row == line) {
            for (j = 0; j < (BACKGROUND_WIDTH * thickness); j++) {
                line_address = j + (BACKGROUND_WIDTH * (row - 1));
                WBM(line_address, R, G, B);
            }
        }
    }
}

void main(){
    // Desenho    
    // WBR_BG(3, 4, 7);
    // draw_horizontal_line(31, 30, 0, 5, 0);
    // DP(30, 450, 350, 12, 7, 7, 7, 0); // Tasa
    // DP(29, 450, 220, 12, 6, 0, 0, 1); // Telhado
    // DP(1, 430, 355, 3, 2, 2, 0, 0); // porta1
    // DP(2, 430, 395, 3, 2, 2, 0, 0); // porta 2
    // DP(3, 480, 360, 2, 4, 4, 7, 0); // Janela

    int i;
    for (i = 0; i < SCREEN_WIDTH; i++) {
        WBR_S(6, 6, i, 100, 1);
        
        if (i >= SCREEN_WIDTH) [
            i = 0;
        ]
    }



    // Seta backgroud
    // WBR_BG(0, 0, 0);

    // clear_lines();

    // Memória de sprites
    // unsigned long long i;
    // for (i = 0; i < 400; i++) {
    //     WSM(25, i, 7, 0, 0);
    // }
    
    // Memoria de background
    // WBM(1, 7, 7, 7);

    //DP
    //DP(1, 0, 0, 0, 6, 7, 7, 0);

    // // sprites
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
