#include <stdio.h>

#define TAM 10      // tamanho do tabuleiro (10x10)
#define NAVIO 3     // número de posições de navio pra colocar (pode mudar)
#define H 5         // tamanho fixo das matrizes de habilidade (5x5)

// Função que imprime o tabuleiro com legenda, pra eu entender o que tá acontecendo
void imprimirTabuleiro(int tabuleiro[TAM][TAM]) {
    printf("\n=== TABULEIRO BATALHA NAVAL ===\n");
    printf("Legenda: 0 = Agua | 3 = Navio | 5 = Area da habilidade\n\n");

    for (int i = 0; i < TAM; i++) {        // roda linha por linha
        for (int j = 0; j < TAM; j++) {    // roda coluna por coluna
            printf("%d ", tabuleiro[i][j]);
        }
        printf("\n"); // quebra linha pra próxima
    }
}

// Função que aplica uma matriz de habilidade no tabuleiro,
// pegando como ponto de origem a coordenada (origemX, origemY)
// A matriz de habilidade tem 1 = afeta, 0 = não afeta
// Se a posição no tabuleiro for água (0), marca como 5 (área afetada)
void aplicarHabilidade(int tabuleiro[TAM][TAM], int habilidade[H][H], int origemX, int origemY) {
    // Vamos percorrer toda matriz da habilidade
    for (int i = 0; i < H; i++) {
        for (int j = 0; j < H; j++) {
            // Calcular a posição no tabuleiro pra essa parte da habilidade
            int x = origemX + i - H / 2;  // ajusta pra centralizar matriz na origem
            int y = origemY + j - H / 2;

            // Só mexe se a posição for válida no tabuleiro
            if (x >= 0 && x < TAM && y >= 0 && y < TAM) {
                // Se a matriz da habilidade indica área afetada E a posição do tabuleiro é água,
                // marca essa posição como 5 (área afetada)
                if (habilidade[i][j] == 1 && tabuleiro[x][y] == 0) {
                    tabuleiro[x][y] = 5;
                }
            }
        }
    }
}

int main() {
    int tabuleiro[TAM][TAM] = {0}; // começa tudo com água (0)

    // Posiciona uns navios fixos no tabuleiro só pra mostrar (3 partes cada)
    for (int i = 0; i < NAVIO; i++) tabuleiro[1][2 + i] = 3;   // navio horizontal
    for (int i = 0; i < NAVIO; i++) tabuleiro[4 + i][6] = 3;   // navio vertical
    for (int i = 0; i < NAVIO; i++) tabuleiro[i][i] = 3;       // navio diagonal principal
    for (int i = 0; i < NAVIO; i++) tabuleiro[6 + i][9 - i] = 3; // navio diagonal secundária

    // ----- Construindo a matriz do CONE -----
    // Pensa num triângulo apontando pra baixo, o topo no meio da linha 0,
    // e vai aumentando a base conforme desce as linhas.
    int cone[H][H] = {0};
    for (int i = 0; i <= H / 2; i++) { // só metade do tamanho, pra formar o triângulo
        for (int j = H / 2 - i; j <= H / 2 + i; j++) {
            // marca 1 onde o cone existe, pra depois pintar no tabuleiro
            cone[i][j] = 1;
        }
    }
    // as linhas abaixo do meio ficam zero (fora do cone)

    // ----- Construindo a matriz da CRUZ -----
    // Cruz = linha do meio + coluna do meio
    int cruz[H][H] = {0};
    for (int i = 0; i < H; i++) {
        cruz[i][H / 2] = 1;   // coluna do meio
        cruz[H / 2][i] = 1;   // linha do meio
    }

    // ----- Construindo a matriz do OCTAEDRO (losango) -----
    // A ideia é usar a soma da distância em linha e coluna
    // se soma <= H/2 então pinta, formando um losango no meio da matriz
    int octaedro[H][H] = {0};
    for (int i = 0; i < H; i++) {
        for (int j = 0; j < H; j++) {
            // calcular distância "manhattan" do centro
            int distLinha = i > H/2 ? i - H/2 : H/2 - i;
            int distColuna = j > H/2 ? j - H/2 : H/2 - j;
            if (distLinha + distColuna <= H / 2) {
                octaedro[i][j] = 1;
            }
        }
    }

    // Agora vamos aplicar as habilidades no tabuleiro em pontos fixos
    aplicarHabilidade(tabuleiro, cone, 2, 2);      // cone no ponto (2,2)
    aplicarHabilidade(tabuleiro, cruz, 5, 5);      // cruz no ponto (5,5)
    aplicarHabilidade(tabuleiro, octaedro, 7, 7);  // octaedro no ponto (7,7)

    // Mostrar o resultado final
    imprimirTabuleiro(tabuleiro);

    return 0;
}
