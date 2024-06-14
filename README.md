<div align="center">
  <h1>
      Problema 2: Módulo Kernel para GPU
  </h1>

  <h3>
    Gabriel Cordeiro Moraes, 
    Georgenes Caleo Silva Pinheiro, 
    João Gabriel Lima Almeida, 
    João Pedro Silva Bastos
  </h3>

  <p>
    Engenharia de Computação – Universidade Estadual de Feira de Santana (UEFS)
    Av. Transnordestina, s/n, Novo Horizonte
    Feira de Santana – BA, Brasil – 44036-900
  </p>

  <center>gcmorais66@gmail.com, caleosilva75@gmail.com, gabriel.lima.almeida@gmail.com, joaopedro.silvabastos.splash@gmail.com</center>

</div>

# 1. Introdução
Uma Unidade de Processamento Gráfico (GPU) é um hardware especializado na geração da saída gráfica de um sistema computacional. Comumente chamada de placa de vídeo, esse hardware tira o peso do gerenciamento dos elementos gráficos, geração de frames, geração do sinal de video e etc. que normalmente seria realizado pela CPU e que, principalmente quando envolve tridimensionalidade, pode envolver diversos calculos e operações custosas. As placas de vídeo possuem uma operação considerávelmente complexa em relação a outros hardwares, se assemelhando bastante a uma CPU porém especializada para a parte gráfica, já que também possua elementos semelhantes como unidade de lógica aritimética, registradores, as vezes memória dedicada e até o equivalente a sua propria ISA.
Sendo assim, assim como outras peças de hardware, a placa de vídeo também precisa de seu software especializado que realize o gerenciamento e a comunicação com o dispositivo e seja capaz de gerar os sinais digitais corretos para cada operação da GPU.

Pensando nisso, o problema 2 do módulo integrador TEC499 - SISTEMAS DIGITAIS do curso de Engenharia da Computação da UEFS traz como desafio a construção de módulo kernel do linux que realize o gerenciamento de uma placa de vídeo e que possa ser acionado por uma biblioteca. A GPU utilizada foi desenvolvida pelo aluno Gabriel Sá Barreto Alves em suas atividades de iniciação científica e em seu trabalho de conclusão de curso, sendo implementada em FPGA em um kit de desenvolvimento DE1-SoC.

# 2. Metodologia

## 2.1 Funcionamento da placa
Para desenvolver um código capaz de controlar a GPU, primeiro foi necessário entender como a GPU presente na placa funciona. Através do trabalho "Desenvolvimento de uma Arquitetura Baseada em Sprites para criação de Jogos 2D em Ambientes Reconfiguraveis utilizando dispositivos FPGA" disponibilizado por Gabriel, foi explicitado o seguinte funcionamento da GPU:

- A GPU se comunica através de instruções de 64 bits identificadas através do campo opcode (4 bits) do início da palavra. Essas instruções são inseridas na GPU através dos barramentos DATA A e DATA B, onde a palavra deve ser dividida corretamente (a depender da instrução), indo parte para ao DATA A e a outra parte para o DATA B. Quando um sinal de nível lógico alto é passado no barramento START, é inserido nos dois buffers FIFOs (First in, First out) de instrução os valores de DATA A e DATA B e a GPU vai retirando par por par para realizar a operação indicada.

- Saindo da GPU, há um barramento que possui um sinal lógico que indica se as FIFOs estão cheias, o que é importante para evitar a perda de instruções devido a inserção em uma FIFO cheia.

- Na GPU, há duas memórias: uma de sprites, capaz de guardar as informações de cada pixel das 31 sprites que a GPU consegue gerenciar ao mesmo tempo; e uma de Background, que guarda a informação de 4800 blocos de 8x8 pixels que formam o fundo da tela em uma grade 80x60. Além disso, ela possui 32 registradores que guardam o endereço de cada sprite ativa na tela, sendo que o registrador 1 é de uso exclusivo da cor de background da tela.

- A saída gerada pela GPU é um sinal de 640x480 pixels já formatado para o padrão VGA e associado diretamente a porta VGA da placa, sem necessidade de tratamento posterior.

- A GPU é capaz também de gerenciar polígonos na tela que podem ser um quadrado ou um triangulo com tamanhos pre-defindos que são selecionados na instrução.

No geral, a GPU possui 4 instruções: 0000 - Escrita no Banco de Registradores (WBR), 0001 - Escrita na Memória de Sprites (WSM), 0010 - Escrita na Memória de Background (WBM) e 0011 - Definição de um Polígono (DP).

- WBR: Modifica a referencia de memória que o registrador alvo possui adicionando a referencia a uma sprite.
  
- WSM: Modifica o valor de um endereço na memória de sprites, ou seja, modifica o valor de um píxel de uma sprite.

- WBM: Modifica o valor de um endereço na memória de background, ou seja, modifica a cor de um dos blocos do background.

- DP: Define a referencia a um poligono de tamanho, cor e posição específica no registrador selecionado.

## 2.2 Módulo Kernel
O módulo kernel desenvolvido é responsável por facilitar a comunicação entre o sistema operacional e a GPU, permitindo que aplicativos e processos enviem instruções e dados para a GPU e recebam resultados de volta. É interessante destacar que é definido o nome do dispositivo como "gpucjjg", que será utilizado para acessar a GPU.

Dentro do código, foram incluídas variáveis globais e ponteiros que são essenciais para o funcionamento do módulo. Estes incluem uma variável para armazenar o número major do dispositivo, um buffer de 8 bytes para comunicação com o dispositivo e ponteiros voláteis para acessar os registradores de controle e as FIFOs da GPU. Esses elementos são cruciais para garantir a comunicação eficiente entre o módulo de kernel e o Processador Gráfico.

O módulo de kernel inclui funções específicas para abrir, fechar, ler e escrever no dispositivo. A função de abertura (dev_open) garante que apenas um processo por vez possa acessar o dispositivo, enquanto a função de fechamento (dev_release) libera o dispositivo após o término do acesso. A função de escrita (dev_write) é responsável por receber os dados do usuário, interpretar as instruções contidas nesses dados e enviar os comandos correspondentes para a GPU. Por outro lado, a função de leitura (dev_read) copia os dados do dispositivo de volta para o espaço do usuário.

Além disso, foi necessário implementar as funções de inicialização e desinstalação do módulo de kernel. Durante a inicialização, o número major do dispositivo é registrado, permitindo que os processos acessem o dispositivo de maneira exclusiva. Ademais, o endereço físico da ponte é mapeado para um endereço virtual, facilitando o acesso aos registradores e FIFOs da GPU.

## 2.3 Biblioteca
A biblioteca foi desenvolvida para permitir a interação do usuário com a GPU por meio do módulo kernel. Este conjunto de funções é destinado a simplificar o processo de envio de instruções e dados para o Processador Gráfico, abstraindo a complexidade do acesso direto aos registradores e FIFOs da GPU.

A função `simple_driver_open()` é responsável por abrir o dispositivo de GPU para comunicação. Ela utiliza a função `open()` para abrir o arquivo de dispositivo associado à GPU, definido pela constante `DEVICE_FILE`. Em caso de falha na abertura do dispositivo, uma mensagem de erro é exibida.

Por sua vez, a função `simple_driver_close()` fecha o dispositivo de GPU previamente aberto. Ela utiliza a função `close()` para fechar o arquivo de dispositivo, liberando recursos associados a ele.

A função `intToCharArray()` converte um número inteiro de 64 bits que representa a instrução completa, em um array de caracteres de 8 bytes. Ela realiza uma operação de deslocamento de bits para extrair cada byte do número e armazená-lo no array de caracteres para que essa informação seja recebida da maneira correta pelo kernel, uma vez que ele é um Char Device.

A função `fileWriter()` é responsável por escrever uma palavra de 64 bits no dispositivo de GPU. Ela invoca a função `simple_driver_open()` para abrir o dispositivo, escreve os dados no dispositivo utilizando a função `write()` e, em seguida, fecha o dispositivo com a função `simple_driver_close()`.

As demais funções (`WBR_BG()`, `WBR_S()`, `WSM()`, `WBM()` e `DP()`) representam instruções específicas que podem ser enviadas para a GPU (seção 2.1). Cada uma dessas funções recebe parâmetros correspondentes aos campos da instrução a ser enviada e constrói a palavra de 64 bits representando essa instrução. Em seguida, elas chamam a função `fileWriter()` para enviar a instrução para o dispositivo de GPU.

De maneira geral, a biblioteca encapsula a lógica de comunicação com a GPU e fornece uma interface simplificada para o usuário interagir com a GPU por meio do módulo kernel. Ela facilita o desenvolvimento de programas que fazem uso da GPU para processamento gráfico.


# 3. Resultados

Com o módulo kernel já inicializado, o usuário pode simplesmente incluir o arquivo "lib.c", o qual contém as cinco instruções possíveis de serem realizadas com o Processador Gráfico (Figura 1).

Figura 1: Nome de parâmetros recebidos por cada instrução da biblioteca. <br/>
![Imagem 1](https://github.com/JFooley/PBL-Sistemas-Digitais-Problema-2/blob/main/instrucoes1.png)

Embora as instruções indicadas na Figura 1 tenham sido explicadas previamente na seção 2.1, é importante compreender seus parâmetros distintos para utilizá-los adequadamente. A Figura 2 detalha cada um individualmente.

Figura 2: Detalhamento dos parâmetros das instruções. <br/>
![Imagem 2](https://github.com/JFooley/PBL-Sistemas-Digitais-Problema-2/blob/main/instrucoes2.png)

Respeitando as limitações da plataforma onde a GPU está instalada, o usuário tem a liberdade de criar interfaces gráficas conforme desejado, utilizando as instruções fornecidas. Para ilustrar este processo, é interessante examinar o código presente no arquivo "codigo.c", dentro da função "main". Um exemplo dessa saída pode ser observado na Figura 3.

Figura 3: Exemplo de saída. <br/>
![Imagem 2](https://github.com/JFooley/PBL-Sistemas-Digitais-Problema-2/blob/main/exemploSaida.jpeg)

Utilizando apenas as instruções disponíveis na biblioteca, foi possível gerar o cenário acima, o qual inclui não apenas elementos estáticos na tela, mas também o movimento da espaçonave e dos tiros por ela disparados.


# 4. Conclusão

Ao concluir este trabalho, adquirimos um conhecimento aprofundado sobre a interação entre software e hardware e a maneira de integrar essas duas áreas por meio de um driver. Exploramos detalhadamente conceitos de mapeamento e manipulação de memória, especialmente no contexto do kit de desenvolvimento DE1-SoC, e aprimoramos nosso entendimento e utilização da linguagem de programação C.

Cumprimos com êxito todos os requisitos estipulados pelo problema. Utilizamos a interface de conexão entre a HPS e a FPGA do DE1-SoC, implementamos uma biblioteca em C no espaço do usuário para funcionalidades da GPU e desenvolvemos um módulo kernel no HPS para o Processador Gráfico.

Apesar de termos concluído todas as funcionalidades previstas com sucesso, identificamos oportunidades para melhorias futuras. A principal sugestão seria expandir as funcionalidades da GPU, ampliando a biblioteca disponível para oferecer ao usuário final uma gama ainda maior de possibilidades de uso da GPU.


# Referências
