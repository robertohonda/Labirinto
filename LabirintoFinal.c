
#include<stdio.h>
#include<stdlib.h>
#include<time.h>

typedef struct{
    int px;
    int py;
}Entrada;

struct Pilha{
    int px;
    int py;
    struct Pilha *prox;
};

typedef struct Pilha tPilha;

int l, c, achei, cont=0;

char** inicializacao(char** lab);///Inicializa a matriz labirinto

void imprime_lab(char** lab);///Imprime labirinto

void entrada_saida(char** lab);///sorteia a entra e saida

void gera_lab(int px, int py, char** lab);///gera o labirinto

void solucao(int px, int py,char** lab);///solucao do labirinto

void arquivoLabirinto(char** lab);///faz a operação de escrever o labirinto no arquivo

void arquivoSolucao(char** lab);///az a operação de escrever a solucao do labirinto no arquivo

char** carregaLabirinto(char** lab);///Retorna a matriz carregada

tPilha* insere_pilha(tPilha* pilha, int px, int py);

tPilha* remove_pilha(tPilha* pilha);



Entrada entrada_global;///Posição da entrada

int main()
{
    char** lab;
    int i, j, save, load, opcao=1, novo;
    while(opcao)
    {
        novo = -1;
        opcao = 1;
        do{
            printf("Deseja carregar um labirinto? (1 - Sim  0 - Nao)\n");
            scanf("\n%d", &load);
        }while(load!=0&&load!=1);
        while(load&&opcao)
        {
            lab = carregaLabirinto(lab);

            if(lab==NULL)
            {
                printf("Deseja tentar novamente?(1 - Sim  0 - Nao)\n");
                scanf("\n%d", &opcao);
            }
            else
            {
                novo = 0;
                break;
            }
        }
        if(novo==-1||load==0)
        {
            lab = inicializacao(lab);

            srand (time(NULL));
            entrada_saida(lab); ///Sorteia entrada, cria labirinto e gera saida
        }

        imprime_lab(lab);
        achei =0;

        do{
            printf("\nDeseja salva no arquivo o labirinto? (1 - Sim 0 - Nao)\n");
            scanf("\n%d", &save);
        }while(save!=0&&save!=1);

        if(save)
            arquivoLabirinto(lab);

        cont = 0;
        solucao(entrada_global.px, entrada_global.py, lab);

        printf("\n");
        imprime_lab(lab);
        printf("\nCusto: %d passos", cont);
        do{
            printf("\nDeseja salvar a solucao no arquivo? (1 - Sim 0 - Nao)\n");
            scanf("\n%d", &save);
        }while(save!=0&&save!=1);
        if(save)
            arquivoSolucao(lab);
        do
        {
            printf("\nDeseja continuar? (1 - Sim  0 - Nao)\n");
            scanf("\n%d", &opcao);
        }while(opcao!=0&&opcao!=1);
    }

    return 0;
}

char** inicializacao(char** lab)
{
    int i, j;

    printf("\nEntre com o tamanho do labirinto: mxn \n");
    scanf("\n%d%d", &l, &c);

    lab = (char**)malloc(l*sizeof(char*));

    for(i=0;i<l;i++)
        lab[i] = (char*)malloc(c*sizeof(char));

    for(i=0;i<l;i++)///inicia a matriz lab com 1
        for(j=0;j<c;j++)
            lab[i][j] = '1';
    return lab;
}

void imprime_lab(char** lab)
{
    int i, j;

    for(i=0;i<l;i++)///imprime labirinto
    {
        for(j=0;j<c;j++)
        {
            if(lab[i][j]=='-')
                printf("0 ");
            else
                printf("%c ", lab[i][j]);
        }
        printf("\n");
    }
}

void gera_lab(int px, int py, char** lab)
{

    int i, j, passo, k=0, ir, caminho, flag;

    tPilha* pilha=NULL;

    pilha = insere_pilha(pilha, px, py);
    while(pilha!=NULL)
    {
        px = pilha->px;
        py = pilha->py;
        lab[px][py] = '0';///posição = 0
        caminho = rand()%4;///sorteia para onde ir
        flag = 0;
        k=0;
        while(k!=4)///verifica se tem lado pra ir
        {
            if(caminho == 0 && px-2>=1 && lab[px-2][py] =='1' && flag==0)///Cima
            {
                flag = 1;
                lab[px-1][py] = '0';
                pilha = insere_pilha(pilha, px-2, py);
                ///gera_lab(px-2, py, lab);
                break;
            }
            if(caminho == 1 && py+2<c-1 && lab[px][py+2] == '1' && flag==0)///Direita
            {
                flag = 1;
                lab[px][py+1] = '0';
                pilha = insere_pilha(pilha, px, py+2);
                ///gera_lab(px, py+2, lab);
                break;
            }
            if(caminho == 2 &&px+2<l-1&& lab[px+2][py] == '1' &&flag ==0)///Baixo
            {
                flag = 1;
                lab[px+1][py] = '0';
                pilha = insere_pilha(pilha, px+2, py);
                ///gera_lab(px+2, py, lab);
                break;
            }
            if(py-2>=1 &&lab[px][py-2] == '1' &&flag==0)///Esquerda
            {
                flag = 1;
                lab[px][py-1] = '0';
                pilha = insere_pilha(pilha, px, py-2);
                ///gera_lab(px, py-2, lab);
                break;
            }
            caminho = (caminho + 1)% 4;
            k++;
        }
        if(flag==0)
            pilha = remove_pilha(pilha);
    }
}


void entrada_saida(char** lab)
{
    int canto_entrada, canto_saida, entrada, saida, controle=1;

    canto_entrada = rand()%4;///sorteia um canto
    switch(canto_entrada)
    {
        case 0:///canto de cima
            entrada = 1+rand()%(c-2);///sorteia entrada
            lab[0][entrada]='E';///entrada recebe '0'
            entrada_global.px = 0;
            entrada_global.py = entrada;
            gera_lab(1, entrada, lab);///chama a função labirinto pra a proxima posição
            break;
        case 1:///canto de baixo
            entrada = 1+rand()%(c-2);///sorteia entrada
            lab[l-1][entrada]='E';///entrada recebe '0'
            entrada_global.px = l-1;
            entrada_global.py = entrada;
            gera_lab(l-2, entrada, lab);///chama a função labirinto pra a proxima posição
            break;
        case 2:///canto direito
            entrada = 1+rand()%(l-2);///sorteia entrada
            lab[entrada][c-1]='E';///entrada recebe '0'
            entrada_global.px = entrada;
            entrada_global.py = c-1;
            gera_lab(entrada, c-2, lab);///chama a função labirinto pra a proxima posição
            break;
        case 3:///canto esquerdo
            entrada = 1+rand()%(l-2);///sorteia entrada
            lab[entrada][0]='E';///entrada recebe '0'
            entrada_global.px = entrada;
            entrada_global.py = 0;
            gera_lab(entrada, 1, lab);///chama a função labirinto pra a proxima posição
    }
    do
    {
        canto_saida = rand()%4;///sorteia um canto
        switch(canto_saida)
        {
            case 0:///canto de cima
                saida = 2+rand()%(c-5);///sorteia uma saida
                if(canto_entrada!=canto_saida || abs(entrada - saida)>1)///verifica se a saida não é igual ou vizinha da entrada
                {
                    lab[0][saida] = 'S';
                    lab[1][saida] = '0';///saida recebe '0'
                    controle = 0;///não sorteia novamente a saida
                    if(lab[1][saida-1] == '1' && lab[1][saida+1]=='1' && lab[2][saida] == '1')
                        lab[1][saida-1] = '0';
                }
                break;
            case 1:///canto de baixo
                saida = 2+rand()%(c-5);///sorteia uma saida
                if(canto_entrada!=canto_saida || abs(entrada - saida)>1)///verifica se a saida não é igual ou vizinha da entrada
                {
                    lab[l-1][saida] = 'S';
                    lab[l-2][saida] = '0';///saida recebe '0'
                    controle = 0;///não sorteia novamente a saida
                    if(lab[l-2][saida-1] == '1' && lab[l-2][saida+1]=='1' && lab[l-3][saida] == '1')
                        lab[l-2][saida-1] = '0';
                }
                break;
            case 2:///canto direito
                saida = 2+rand()%(l-5);///sorteia uma saida
                if(canto_entrada!=canto_saida || abs(entrada - saida)>1)///verifica se a saida não é igual ou vizinha da entrada
                {
                    lab[saida][c-1]= 'S';
                    lab[saida][c-2] = '0';///saida recebe '0'
                    controle=0;///não sorteia novamente a saida
                    if(lab[saida-1][c-2] == '1' && lab[saida+1][c-2]=='1'&& lab[saida][c-3] == '1')
                        lab[saida-1][c-2] = '0';
                }
                break;
            case 3:///canto esquerdo
                saida = 2+rand()%(l-5);///sorteia uma saida
                if(canto_entrada!=canto_saida || abs(entrada - saida)>1)///verifica se a saida não é igual ou vizinha da entrada
                {
                    lab[saida][0] = 'S';
                    lab[saida][1] = '0';///saida recebe '0'
                    controle = 0;///não sorteia novamente a saida
                    if(lab[saida-1][1] == '1' && lab[saida+1][1]=='1' && lab[saida][2] == '1')
                        lab[saida-1][1] = '0';
                }
        }
    }while(controle == 1);///se a saida for vizinha ou igual a entrada, sorteia novamente a saida
}

void solucao(int px, int py,char** lab)
{
    int i, j, flag;
    tPilha *pilha = NULL, *p;

    pilha = insere_pilha(pilha, px, py);
    while(pilha!=NULL)
    {
        px = pilha->px;
        py = pilha->py;
        cont++;
        if((py==c-1||px==l-1||px==0||py==0)&&lab[px][py]=='S')///Verifica se encontrou a saida
        {
            lab[px][py] = 'x';
            while(pilha!=NULL)
                pilha = remove_pilha(pilha);
        }
        else
        {
            lab[px][py] = 'x';
            flag = 0;
            if(px-1>=0&&(lab[px-1][py] == '0' ||lab[px-1][py] == 'S') && flag == 0)///cima
            {
                flag=1;
                pilha = insere_pilha(pilha, px-1, py);
                ///solucao(px-1, py, lab);
            }
            if(py+1<c&&(lab[px][py+1] == '0'||lab[px][py+1] == 'S') && flag == 0)///direita
            {
                flag = 1;
                pilha = insere_pilha(pilha, px, py+1);
                ///solucao(px, py+1, lab);
            }
            if(px+1<l&&(lab[px+1][py] == '0'||lab[px+1][py] == 'S') && flag == 0)///baixo
            {
                flag = 1;
                pilha = insere_pilha(pilha, px+1, py);
                ///solucao(px+1, py, lab);
            }
            if (py-1>=0&&(lab[px][py-1] == '0'||lab[px][py-1] == 'S') && flag == 0)///esquerda
            {
                flag = 1;
                pilha = insere_pilha(pilha, px, py-1);
                ///solucao(px, py-1, lab);
            }
            if(flag==0)
            {
                lab[px][py] = '-';
                pilha = remove_pilha(pilha);
                cont=cont-2;
            }
        }
    }
}

void arquivoLabirinto(char** lab)
{
    FILE *arquivo;
    int i, j;

    arquivo = fopen("Labirinto.txt", "w");

    fprintf(arquivo, "%d %d", l, c);
    for(i=0;i<l;i++)
    {
        fputc('\n', arquivo);
        for(j=0;j<c;j++)
        {
            fputc(lab[i][j], arquivo);
        }
    }
    fputc('\n', arquivo);

    fclose(arquivo);
}

void arquivoSolucao(char** lab)
{
    FILE *arquivo;
    int i, j;

    arquivo = fopen("Solucao.txt", "a+t");
    fprintf(arquivo, "%d %d", l, c);
    for(i=0;i<l;i++)
    {
        fputc('\n', arquivo);
        for(j=0;j<c;j++)
        {
            fputc(lab[i][j], arquivo);
        }
    }
    fputc('\n', arquivo);

    fclose(arquivo);
}
char** carregaLabirinto(char** lab)
{
    int num_lab, i=1, j, k;

    FILE *arquivo;

    arquivo = fopen("Labirinto.txt", "r");
    if(arquivo == NULL)
    {
        printf("\nErro: Arquivo nao encontrado!\n");
        return NULL;
    }
    else
    {
        if(fscanf(arquivo, "%d %d\n", &l, &c)!=EOF)
        {
            //printf("%d\n", i);
            printf("Linhas: %d\n", l);
            printf("Colunas: %d\n", c);

            lab = (char**)malloc(l*sizeof(char*));
            for(i=0;i<l;i++)
                lab[i] = (char*)malloc(c*sizeof(char));
            for(j=0;j<l;j++)
            {
                for(k=0;k<c;k++)
                {
                    lab[j][k] = fgetc(arquivo);
                    if(lab[j][k] == 'E')
                    {
                        entrada_global.px = j;
                        entrada_global.py = k;
                    }
                }
                fgetc(arquivo);
            }
            fclose(arquivo);
            return lab;
        }
        else
        {
            printf("Ocorreu um erro na leitura!\n");
            fclose(arquivo);
            return NULL;
        }
    }

}

tPilha* insere_pilha(tPilha* pilha, int px, int py)
{
    tPilha *p = (tPilha*)malloc(sizeof(tPilha));
    p->px = px;
    p->py = py;
    p->prox = pilha;

    return p;
}

tPilha* remove_pilha(tPilha* pilha)
{
    tPilha* p;

    if(pilha==NULL)
        return pilha;

    p = pilha;
    pilha = pilha->prox;
    free(p);
    return pilha;
}




