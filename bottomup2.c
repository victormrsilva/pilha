#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct Caixa
{
    int a; // algura
    int l; // largura
    int p; // produnfidade
    int v; // valor
    int id; // identificador
} Caixa;

Caixa *caixas;; // vetor de caixas
int h_max;
int n;

// verifica se há caixas nessa altura
int **alturas;

void printCaixa(Caixa c, int pos);

int main(int argc, char *argv[])
{
    clock_t begin = clock();
    if (argc < 3){
        printf("bottomup2 instanceName outputfile\n");
        return -1;
    }

    FILE *arq, *saida; // instance file and output file

    arq = fopen(argv[1], "r");
    saida = fopen(argv[2],"w");
    if (arq == NULL || saida == NULL){
        printf("Erro, nao foi possivel abrir o arquivo\n");
    }
    else {
        fscanf(arq,"%d \n", &n);
        fscanf(arq,"%d \n", &h_max);
        caixas = malloc(sizeof(Caixa)*(2*n)); // posição i%2==0 =: rotação 1; i%2 == 1 => rotação 2

        // lê valores
        for (int i = 0; i < n; i++){
            int v;
            fscanf(arq,"%d \n", &v);
            //printf("%d\n", v);
            caixas[2*i].v = v;
            caixas[2*i+1].v = v;
        }

        for (int i = 0; i < n; i++){
            int a,l,p;
            fscanf(arq,"%d \n", &l);
            fscanf(arq,"%d \n", &a);
            fscanf(arq,"%d \n", &p);
            //printf("%d %d %d\n", l, a, p);
            caixas[2*i].a = a;
            caixas[2*i+1].a = l;
            caixas[2*i].l = l;
            caixas[2*i+1].l = a;
            caixas[2*i].p = p;
            caixas[2*i+1].p = p;
            caixas[2*i].id = i+1;
            caixas[2*i+1].id = i+1;
        }

        // for (int i = 0; i < 2*n; i++){
        //     printCaixa(caixas[i],i);
        // }

        alturas = malloc(sizeof(int*)*(h_max+1));
        for (int i = 0; i <= h_max; i++){
            alturas[i] = malloc(sizeof(int)*(2*n));
            for (int j = 0; j < 2*n; j++){
                alturas[i][j] = -1;
                alturas[0][j] = 0;
            }
        }
        
        // calcula as pilhas e armazena a melhor
        int melhor = 0;
        int altura = 0;
        int caixa_topo = -1;
        for (int h = 0; h <= h_max; h++){
            for (int i = 0; i < 2*n; i++){
                if (alturas[h][i] == -1){
                    continue;
                } else {
                    for (int j = 0; j < 2*n; j++){
                        if (h + caixas[j].a > h_max || caixas[j].l > caixas[i].l || caixas[j].p > caixas[i].p){
                            continue;
                        } else {
                            int valor = alturas[h][i] + caixas[j].v;
                            if (alturas[h+caixas[j].a][j] < valor){
                                alturas[h+caixas[j].a][j] = valor;
                            }
                            if (valor > melhor){
                                melhor = valor;
                                altura = h+caixas[j].a;
                                caixa_topo = j;
                            }
                        }
                    }
                }
            }
        }

        // escreve solução
        fprintf(saida,"%d\n", melhor);
        int qtd = 1;
        int temp_altura = altura-caixas[caixa_topo].a;
        int temp_valor = melhor-caixas[caixa_topo].v;
        int temp_caixa_topo = caixa_topo;
        int *solucao = malloc(sizeof(int)*(altura+1));
        solucao[1] = caixa_topo;
        while (temp_altura > 0){
            for (int i = 0; i < 2*n; i++){
                if (alturas[temp_altura][i] != temp_valor){
                    continue;
                }
                if (caixas[i].l < caixas[temp_caixa_topo].l || caixas[i].p < caixas[temp_caixa_topo].p){
                    continue;
                }
                qtd++;
                solucao[qtd] = i;
                temp_altura = temp_altura - caixas[i].a;
                temp_valor = temp_valor - caixas[i].v;
                temp_caixa_topo = i;
                
                break;
            }            
        }
        fprintf(saida,"%d\n", qtd);
        for (int i = qtd; i > 0 ; i--){
            fprintf(saida,"%d %d\n",caixas[solucao[i]].id, (solucao[i] % 2)+1);
        }
        for (int i = 0; i <= h_max; i++){
            free(alturas[i]);
        }
        free(alturas);
        free(solucao);
        free(caixas);
        fclose(arq);
        fclose(saida);
        clock_t end = clock();
        double time_spent = ((double)end-begin)/((double)CLOCKS_PER_SEC);
        printf("Time: %.3f\n",time_spent);
    }

        

    return 0;
}

void printCaixa(Caixa c, int pos){
    char* rotacao;
    if (pos % 2 == 0) rotacao = "r1";
    else rotacao = "r2";
    printf("caixa%d %s altura %d, largura %d, profundidade %d, valor %d\n",caixas[pos].id, rotacao,c.a, c.l, c.p, c.v);
}
