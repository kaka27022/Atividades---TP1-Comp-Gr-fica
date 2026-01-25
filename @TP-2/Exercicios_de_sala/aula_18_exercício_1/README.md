# Atividade Cyrus-Beck (OpenGL)

Implementação do algoritmo de recorte Cyrus-Beck em C++ para a disciplina de Computação Gráfica.

## 🚀 Compilação e Execução

Abra o terminal na pasta do projeto e use os comandos:
** 1. Compilar o projeto **
Este comando gera os arquivos objeto e o executável final:

Bash

make
** 2. Executar o programa **
Este comando inicia a aplicação e abre a janela do exercício:

Bash

make run
** 3. Limpar arquivos temporários **
Use este comando para remover o executável e arquivos .o, deixando a pasta limpa:

Bash

make clean

## 🛠️ Requisitos Técnicos
Compilador: GCC/MinGW (G++)
Bibliotecas: FreeGLUT, OpenGL32, GLU32.
Sistema Operacional: Configurado para Windows (via flags de linkagem no Makefile).

## 🎮 Interação com o Mouse
O programa permite definir a reta de teste de forma interativa:
Clique e Segure: Clique com o botão esquerdo do mouse no ponto onde deseja iniciar a reta.
Arraste: Movimente o mouse mantendo o botão pressionado para definir a direção e o comprimento.
Solte: Ao soltar o botão, o ponto final é definido e o algoritmo de Cyrus-Beck calcula instantaneamente o recorte da reta em relação ao polígono.

![alt text](image.png)
![alt text](image-1.png)



