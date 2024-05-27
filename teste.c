#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

#define DEVICE "/dev/main"

int main(void)
{
    int i, fd;
    char ch, write_buf[100], read_buf[100];

    fd = open(DEVICE, O_RDWR);

    if (fd == -1)
    { // Fazer a lógica para criuaera o arquivo caso ele nãoi exista
        printf("Deu ruim papai");
        exit(-1);
    }

    printf("l = ler\ne = escrever\nEscolha: ");
    scanf("%c", &ch);

    switch (ch)
    {
    case 'e':
        printf("Insira os dados: ");
        scanf(" ", write_buf);
        write(fd, write_buf, sizeof(write_buf));
        break;

    case 'l':
        read(fd, read_buf, sizeof(read_buf));
        printf("Leitura: %s\n", read_buf);
        break;


    default:
    printf("Coloca o comando certo animal\n");
        break;
    }
    return 0;
}