#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#define DEVICE "/dev/main"

int main(void)
{
    int fd;
    char ch, write_buf[100], read_buf[100];

    fd = open(DEVICE, O_RDWR | O_SYNC);

    if (fd == -1) {
        perror("Erro ao abrir o dispositivo");
        exit(EXIT_FAILURE);
    }

    printf("l = ler\ne = escrever\nEscolha: ");
    scanf(" %c", &ch);  // Note the space before %c to skip any whitespace characters

    switch (ch) {
    case 'e': {
        printf("Insira os dados: ");
        scanf(" %[^\n]", write_buf);  // This reads a line of text including spaces

        ssize_t bytes_written = write(fd, write_buf, sizeof(write_buf));
        if (bytes_written == -1) {
            perror("Erro ao escrever no dispositivo");
            close(fd);
            exit(EXIT_FAILURE);
        }
        break;
    }

    case 'l': {
        ssize_t bytes_read = read(fd, read_buf, sizeof(read_buf) - 1);  // Leave space for null terminator
        if (bytes_read == -1) {
            perror("Erro ao ler do dispositivo");
            close(fd);
            exit(EXIT_FAILURE);
        }

        read_buf[bytes_read] = '\0';  // Null-terminate the string
        printf("Leitura: %s\n", read_buf);
        break;
    }

    default:
        printf("Comando inválido\n");
        break;
    }

    close(fd);
    return 0;
}
