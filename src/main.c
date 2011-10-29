#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pgc.h"

struct Data{
       int dia;
       int mes;
       int ano;
};

struct Cliente{
       int id;
       char nome[500];
       char email[500];
       char telefone[8];
       char celular[8];
       float saldo;
       struct Data vencimento;
       int linha;
};

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
         printf("\t\t\t\t| Menu Principal |\n");
         printf("\t\t\t\t ---------------- \n");
         
         printf("\t1- Novo cliente\n");
         printf("\t2- Apagar cliente\n");
         printf("\t3- Alterar informacoes\n");
         printf("\t4- Exibir dados de um cliente\n");
         printf("\t5- Exibir todos os registros\n");
         printf("\t6- Sair\n");
         
         printf("Digite uma opcao e de <ENTER>: ");
         op = leNumero();
         
     }while(op < 1 || op > 6);
     
     return op;
}

/* FIRST DISPLAY */
void telaInicial(){
     printf("\t\t\t ------------------ \n");
     printf("\t\t\t| Bem-vindo ao PGC |\n");
     printf("\t\t\t ------------------ \n");
     printf("\n\n\t\t Programa de Gerenciamento de Clientes\n\n\n\n");
     printf("\t\t\t\t:) \n\n\n\n");
     printf("\n\n\n\n\n\nPressione <ENTER> para continuar");
     getchar();
}

void exibirDados(){
     int id;
     struct Cliente c;
     printf("\n\nDigite o ID do cliente: ");
     id = leNumero();
     if(buildClienteById(&c, id)){
        printf("\nID: %d\nNome:%s\n", c.id, c.nome);
        printf("\n\n<ENTER>");
        getchar();
     }
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
       else if(op == 5) printf("todos");
    }while(op != 6);
    
    printf("\n\nFIM");
    
    getchar();
    return 0;
}
