#include <stdio.h>

// Protótipo da função WBM (escrita na memória de background)
void WBM(unsigned long long mem_address, unsigned long long R, unsigned long long G, unsigned long long B);

// Função para desenhar duas linhas verticais
void draw_vertical_lines() {
    // Dimensões da tela em blocos
    const int width_in_blocks = 80;
    const int height_in_blocks = 60;

    // Coordenadas das colunas onde as linhas serão desenhadas
    const int line1_column = 20;
    const int line2_column = 60;

    // Cor para as linhas (vermelho, por exemplo)
    unsigned long long R = 255;
    unsigned long long G = 0;
    unsigned long long B = 0;

    // Loop para percorrer todas as linhas da tela (em blocos)
    for (int row = 0; row < height_in_blocks; row++) {
        // Calcula o endereço de memória para cada linha nas colunas especificadas
        unsigned long long mem_address_line1 = row * width_in_blocks + line1_column;
        unsigned long long mem_address_line2 = row * width_in_blocks + line2_column;

        // Chama a função WBM para desenhar as linhas nas colunas especificadas
        WBM(mem_address_line1, R, G, B);
        WBM(mem_address_line2, R, G, B);
    }
}

// Exemplo de implementação da função WBM (esta é apenas uma simulação)
void WBM(unsigned long long mem_address, unsigned long long R, unsigned long long G, unsigned long long B) {
    // Em um sistema real, esta função escreveria na memória de background
    // Aqui, apenas imprimimos os valores para demonstração
    printf("Endereço: %llu, R: %llu, G: %llu, B: %llu\n", mem_address, R, G, B);
}

int main() {
    // Chama a função para desenhar as linhas verticais
    draw_vertical_lines();
    return 0;
}

