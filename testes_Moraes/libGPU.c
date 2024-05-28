#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#define DEVICE_FILE "/dev/DRIVERMORAES"
#define BUFFER_SIZE_BITS 64 // 64 bits
#define BUFFER_SIZE_BYTES (BUFFER_SIZE_BITS / 8) // Tamanho do buffer em bytes

int simple_driver_open() {
    int fd = open(DEVICE_FILE, O_RDWR);
    if (fd < 0) {
        perror("Falha ao abrir o dispositivo");
        return -1;
    }
    printf("Dispositivo aberto com sucesso!\n");
    return fd;
}

int simple_driver_close(int fd) {
    int ret = close(fd);
    if (ret < 0) {
        perror("Falha ao fechar o dispositivo");
        return -1;
    }
    printf("Dispositivo fechado com sucesso!\n");
    return 0;
}

ssize_t simple_driver_read_bits(int fd, unsigned char *buffer, size_t num_bits) {
    // Converter o tamanho de bits para bytes
    size_t num_bytes = (num_bits + 7) / 8;

    ssize_t bytes_read = read(fd, buffer, num_bytes);
    if (bytes_read < 0) {
        perror("Falha ao ler do dispositivo");
        return -1;
    }

    printf("Dados lidos do dispositivo: ");
    for (size_t i = 0; i < bytes_read; i++) {
        for (int j = 0; j < 8; j++) {
            printf("%d", (buffer[i] >> (7 - j)) & 1);
        }
        printf(" ");
    }
    printf("\n");

    return bytes_read;
}

ssize_t simple_driver_write_bits(int fd, const unsigned char *buffer, size_t num_bits) {
    // Converter o tamanho de bits para bytes
    size_t num_bytes = (num_bits + 7) / 8;

    ssize_t bytes_written = write(fd, buffer, num_bytes);
    if (bytes_written < 0) {
        perror("Falha ao escrever no dispositivo");
        return -1;
    }

    printf("Dados escritos no dispositivo: ");
    for (size_t i = 0; i < num_bytes; i++) {
        for (int j = 0; j < 8; j++) {
            printf("%d", (buffer[i] >> (7 - j)) & 1);
        }
        printf(" ");
    }
    printf("\n");

    return bytes_written;
}

// Executando as funções para testar o driver
int main() {
    int fd = simple_driver_open();
    if (fd < 0) {
        return EXIT_FAILURE;
    }

    // Escrever dados no dispositivo (64 bits)
    unsigned char write_buffer[BUFFER_SIZE_BYTES] = {0xAB, 0xCD, 0xEF, 0x12, 0x34, 0x56, 0x78, 0x9A};
    ssize_t bytes_written = simple_driver_write_bits(fd, write_buffer, BUFFER_SIZE_BITS);
    if (bytes_written < 0) {
        simple_driver_close(fd);
        return EXIT_FAILURE;
    }

    // Ler dados do dispositivo (64 bits)
    unsigned char read_buffer[BUFFER_SIZE_BYTES];
    ssize_t bytes_read = simple_driver_read_bits(fd, read_buffer, BUFFER_SIZE_BITS);
    if (bytes_read < 0) {
        simple_driver_close(fd);
        return EXIT_FAILURE;
    }

    simple_driver_close(fd);
    return EXIT_SUCCESS;
}