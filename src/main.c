#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pgc.h"

/* READ A NUMBER AND RETURN THIS */
float leNumero(){
    float n;
    scanf("%f", &n);
    fflush(stdin);
    return n;
}

/* MAIN MENU */
int menuPrincipal(){
     int op;
     do{
         system("cls");
         printf("\t\t\t\t ---------------- \n");
         printf("\t\t\t\t| MENU PRINCIPAL |\n");
         printf("\t\t\t\t ---------------- \n");
         
         printf("\t1- Novo cliente\n");
         printf("\t2- Apagar cliente\n");
         printf("\t3- Alterar informacoes\n");
         printf("\t4- Exibir dados de um cliente\n");
         printf("\t5- Exibir todos os registros(todas informacoes)\n");
         printf("\t6- Exibir todos os registros(apenas nomes)\n");
         printf("\t7- Pesquisar\n");
         printf("\t8- Sair\n");
         
         printf("Digite uma opcao e de <ENTER>: ");
         op = leNumero();
         
     }while(op < 1 || op > 8);
     
     return op;
}

/* FIRST DISPLAY */
void telaInicial(){
     printf("\t\t\t ------------------ \n");
     printf("\t\t\t| BEM-VINDO AO PGC |\n");
     printf("\t\t\t ------------------ \n");
     printf("\n\n\t\t Programa de Gerenciamento de Clientes\n\n\n\n");
     printf("\t\t\t\t:) \n\n\n\n");
     printf("\n\n\n\n\n\nPressione <ENTER> para continuar");
     getchar();
}

/* SHOW DATA OF ONE CLIENT */
void exibirDados(){
     int id;
     struct Cliente c;
     
     system("cls");
     printf("\t\t\t --------------------- \n");
     printf("\t\t\t| DADOS DE UM CLIENTE |\n");
     printf("\t\t\t --------------------- \n");
     
     printf("\n\nDigite o ID do cliente: ");
     id = leNumero();
     if(buildClienteById(&c, id)){
        printCliente(&c);
        printf("\n\n<ENTER>");
        getchar();
     }
}

/* SHOW DATA OF ALL CLIENT */
void exibirTodosComp(){
     system("cls");
     printf("\t\t\t\t ---------------- \n");
     printf("\t\t\t\t| TODOS CLIENTES |\n");
     printf("\t\t\t\t ---------------- \n");
     
     exibirTodosDados();
     
     printf("\n\nPressione <ENTER> para voltar ao menu principal");
     getchar();
}

void exibirTodosNome(){
     FILE *f = NULL;
     
     system("cls");
     printf("\t\t\t ------------------------------ \n");
     printf("\t\t\t| TODOS CLIENTES - APENAS NOME |\n");
     printf("\t\t\t ------------------------------ \n");
     
     f = fopen(DATABASE_ID_CLIENTE, F_READ);
     if(!f){
        printf("ERRO: Ocorreu um erro ao tentar ler a base de dados\n<ENTER>");
        getchar();
        return ;
     }
     
     printf("ID  NOME\n");
     while(!feof(f)){
        struct Cliente c;
        int id;
        fscanf(f, "%d", &id);
        
        if(buildClienteById(&c, id))
           printf("%.2d: %s\n", c.id, c.nome);
     }
     
     printf("\n\n\n\nPressione <ENTER> para voltar ao menu principal");
     getchar();
}

int main(){
    int op;
    telaInicial();
    
    do{
       op = menuPrincipal();
       
       if(op == 1) printf("novo");
       else if(op == 2) printf("apagar");
       else if(op == 3) printf("alterar");
       else if(op == 4) exibirDados();
       else if(op == 5) exibirTodosComp();
       else if(op == 6) exibirTodosNome();
       else if(op == 7) printf("pesquisar");
    }while(op != 8);
    
    printf("\n\nFIM");
    
    getchar();
    return 0;
}
