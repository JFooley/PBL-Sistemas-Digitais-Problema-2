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

# 3. Resultados


# 4. Conclusão


# Referências
