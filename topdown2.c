#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct Caixa {
    int a; // algura
    int l; // largura
    int p; // largura
    int v; // valor
    int id; // identificador
} Caixa;


void printCaixa(Caixa c, int pos);

void empilhamento_maximo(int h, char* filename);

int empilhamento(int h, int largura, int profundidade, int depth);

void printSolucao(int valor, int topo, char* filename);



int **alturas;

int h_max;
int n;
Caixa *caixas;; // vetor de caixas


int main(int argc, char *argv[]){
    clock_t begin = clock();
    if (argc < 3){
        printf("topdown2 instanceName outputFile\n");
        return -1;
    }

    FILE *arq; // instance file

    arq = fopen(argv[1], "r");
    if (arq == NULL){
        printf("Erro, nao foi possivel abrir o arquivo\n");
    }
    else {
        fscanf(arq,"%d \n", &n);
        fscanf(arq,"%d \n", &h_max);
        caixas = malloc(sizeof(Caixa)*(2*n)); // posição i%2==0 =: rotação 1; i%2 == 1 => rotação 2

        // lê valores
        int i = 0;
        for (i = 0; i < n; i++){
            int v;
            fscanf(arq,"%d \n", &v);
            caixas[2*i].v = v;
            caixas[2*i+1].v = v;
        }

        for (i = 0; i < n; i++){
            int a,l,p;
            fscanf(arq,"%d \n", &l);
            fscanf(arq,"%d \n", &a);
            fscanf(arq,"%d \n", &p);
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

        fclose(arq);
        
        alturas = malloc(sizeof(int*)*(h_max+1));
        // inicializa
        for (i = 0; i <= h_max; i++){
            alturas[i] = malloc(sizeof(int)*(2*n));
            for (int j = 0; j < 2*n; j++){
                alturas[i][j] = -1;
            }
        }
        // printf("Começa empilhamento\n");
        empilhamento_maximo(h_max, argv[2]);


        free(caixas);
        for (i = 0; i <= h_max; i++){
            free(alturas[i]);
        }
        free(alturas);
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
    printf("caixa%d %s altura %d, largura %d, profundidade %d, valor %d\n",c.id, rotacao,c.a, c.l, c.p, c.v);
}

void empilhamento_maximo(int h, char* filename){
    
    int melhor = -1;
    int topo = -1;
    int i;
    for (i = 0; i < 2*n; i++){
        int valor = 0;
        valor = empilhamento(h-caixas[i].a, caixas[i].l, caixas[i].p, 0);
        valor = valor  + caixas[i].v;
        alturas[h][i] = valor;
        //
        printf("Iteração %d valor_pilha: %d \n", i, valor);
        if (valor > melhor){
            melhor = valor;
            topo = i;
        }
    }

    printSolucao(melhor, topo, filename);
}

int empilhamento(int h, int largura, int profundidade, int depth){
    if (h < 0)
        return 0;
    int melhor = 0;
    int i;
    for (i = 0; i < 2*n; i++){
        if (caixas[i].l < largura || caixas[i].p < profundidade || h-caixas[i].a < 0){
            continue;
        }
        int valor = 0;
        // melhor altura para aquela caixa já calculada
        if (alturas[h][i] != -1) {
            valor = alturas[h][i];
        } else {
            valor = empilhamento(h-caixas[i].a, caixas[i].l, caixas[i].p, depth++);
            valor = caixas[i].v + valor;
        }

        alturas[h][i] = valor;
        if (melhor < valor){
            melhor = valor;
        }
    }
    return melhor;    
}

void printSolucao(int valor, int topo, char* filename){
    FILE *saida;
    saida = fopen(filename,"w");
    fprintf(saida,"%d\n", valor);
    int qtd = 1;
    int temp_altura = h_max - caixas[topo].a;
    int temp_valor = valor - caixas[topo].v;
    int temp_topo = topo;
    int *solucao = malloc(sizeof(int)*(h_max+1));
    solucao[1] = topo;
    int i = 0;
    int nao_mudou = 1;
    while (nao_mudou != 0){
        nao_mudou = 0;
        for ( i = 0; i < 2*n; i++){
            if (alturas[temp_altura][i] != temp_valor){
                continue;
            }
            if (caixas[i].l < caixas[temp_topo].l || caixas[i].p < caixas[temp_topo].p){
                continue;
            }
            qtd++;
            solucao[qtd] = i;
            temp_altura = temp_altura - caixas[i].a;
            temp_valor = temp_valor - caixas[i].v;
            temp_topo = i;
            nao_mudou = 1;
            break;
        }            
    }
    fprintf(saida,"%d\n", qtd);
    temp_altura = h_max - caixas[topo].a;
    temp_valor = valor - caixas[topo].v;
    temp_topo = topo;
    for (i = qtd; i > 0 ; i--){
        fprintf(saida,"%d %d\n",caixas[solucao[i]].id, (solucao[i] % 2)+1);
    }
    fclose(saida);
    free(solucao);
}