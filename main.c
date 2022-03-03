#include <stdlib.h>
#include <stdio.h>

struct estoque {
   unsigned int ProdutoId;
   char nome[15];
   char marca[15];
   char descricao[50];
   double valor;
   unsigned int qtd;
   char estoque;
};

void inserirProdutos(FILE* inserirProd);
void listarProdutos(FILE* listProd);
void excluirProd(FILE* exProd);
void attPreco(FILE* attPr);
void attQtd(FILE* attQtde);

int main(){

    FILE* Produtos;
    int op;

    if((Produtos = fopen("produtos.dat", "rb+")) == NULL){
	    puts("Arquivo não pode ser aberto.");
	}

    puts("Escolha uma opção");
    puts("1- Inserir produtos");
    puts("2- Listar produtos em txt");
    puts("3- Atualizar preço");
    puts("4- Atualizar Quantidade");
    puts("5- Excluir produto");
    printf("--> ");
    scanf("%d", &op);

    system("clear");

    switch(op){
        case 1:
            inserirProdutos(Produtos);
            break;
        case 2:
            listarProdutos(Produtos);
            break;
        case 3:
            attPreco(Produtos);
            break;
        case 4:
            attQtd(Produtos);
            break;
        case 5:
            excluirProd(Produtos);
            break;
        default:
            puts("Opção Inválida");
    }
    
    fclose(Produtos);

    return 0;
}

void inserirProdutos(FILE* inserirProd){
    
    struct estoque produtos = {0, "", "", "", 0.0, 0, 0};
    puts("######     Inserir Produtos     ######");
    printf("Código do produto (1 a 100): ");
    scanf("%d", &produtos.ProdutoId);
    produtos.estoque = 'S';

    fseek(inserirProd, (produtos.ProdutoId - 1) * sizeof(struct estoque), SEEK_SET);

    while(produtos.ProdutoId != 0) {
        getchar();
        printf("Nome do produto: ");
        scanf("%14[^\n]s", produtos.nome);
        getchar();
        printf("Marca do produto: ");
        scanf("%14[^\n]s", produtos.marca);
        getchar();
        printf("Descrição do produto (MÁX 50 caractéres): ");
        scanf("%49[^\n]s", produtos.descricao);
        printf("Valor do produto: ");
        scanf("%lf", &produtos.valor);
        printf("Quantidade do produto: ");
        scanf("%d", &produtos.qtd);

        fwrite(&produtos, sizeof(struct estoque), 1, inserirProd);
        
        system("clear");
        puts("######     Inserir Produtos     ######");
        puts("######    Para sair digite 0    ######");
        printf("Código do produto (1 a 100): ");
        scanf("%d", &produtos.ProdutoId);
    } 
    system("clear");
}

void excluirProd(FILE* exProd){

    unsigned int aux;

    struct estoque produtos = {0, "", "", "", 0.0, 0, 0};
    puts("######     Apagar produto     ######");
    printf("Código do produto (1 a 100): ");
    scanf("%u", &aux);

    fseek(exProd, (aux - 1) * sizeof(struct estoque), SEEK_SET);
    
    fwrite(&produtos, sizeof(struct estoque), 1, exProd);
    
    system("clear");
}

void attPreco(FILE* attPr){

    unsigned int aux;

    struct estoque produtos = {0, "", "", "", 0.0, 0, 0};
    puts("######     Atualizar Preços     ######");
    printf("Código do produto (1 a 100): ");
    scanf("%u", &aux);

    fseek(attPr, (aux - 1) * sizeof(struct estoque), SEEK_SET);
    
    fread(&produtos, sizeof(struct estoque), 1, attPr);
    if(aux == produtos.ProdutoId){

        printf("Valor do produto: ");
        scanf("%lf", &produtos.valor);

        fseek(attPr, (aux - 1) * sizeof(struct estoque), SEEK_SET);
        fwrite(&produtos, sizeof(struct estoque), 1, attPr);
    }
    
    system("clear");
}

void attQtd(FILE* attQtde){

    unsigned int aux;

    struct estoque produtos = {0, "", "", "", 0.0, 0, 0};
    puts("######     Atualizar quantidade     ######");
    printf("Código do produto (1 a 100): ");
    scanf("%u", &aux);

    fseek(attQtde, (aux - 1) * sizeof(struct estoque), SEEK_SET);
    
    fread(&produtos, sizeof(struct estoque), 1, attQtde);
    if(aux == produtos.ProdutoId){

        printf("Quantidade do produto: ");
        scanf("%u", &produtos.qtd);

        if(produtos.qtd == 0){
            produtos.estoque = 'N';
        }

        fseek(attQtde, (aux - 1) * sizeof(struct estoque), SEEK_SET);
        fwrite(&produtos, sizeof(struct estoque), 1, attQtde);
    }
    
    system("clear");
}

void listarProdutos(FILE* listProd){

    FILE* listProdTxt;
    double totalValor;
    long int totalEstoq = 0;

    struct estoque produtos = {0, "", "", "", 0.0, 0, 0};
    listProdTxt = fopen("produtos.txt", "w");
    if(listProdTxt){
        while((fread(&produtos, sizeof(struct estoque), 1, listProd)) != 0)
        {   
            if(produtos.ProdutoId != 0){

                totalValor = produtos.valor * produtos.qtd;
                totalEstoq += produtos.qtd;

                fprintf(listProdTxt, "%5u", produtos.ProdutoId);
                fprintf(listProdTxt, "%19s", produtos.nome);
                fprintf(listProdTxt, "%16s", produtos.marca);
                fprintf(listProdTxt, "%51s", produtos.descricao);
                fprintf(listProdTxt, "%10.2lf", totalValor);
                fprintf(listProdTxt, "%6c\n", produtos.estoque);
            }
        }
    }else{
        puts("Arquivo não pode ser aberto");
    }

    fprintf(listProdTxt, "Total estoque: %ld", totalEstoq);

    fclose(listProdTxt);
}