#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Data{
       int dia;
       int mes;
       int ano;
};

struct Cliente{
       int id;
       char *nome;
       char *email;
       char telefone[8];
       char celular[8];
       float saldo;
       struct Data vencimento;
};

float leNumero(){
    float n;
    scanf("%f", &n);
    fflush(stdin);
    return n;
}

void telaInicial(){
     printf("\t\t\t ------------------ \n");
     printf("\t\t\t| Bem-vindo ao PGC |\n");
     printf("\t\t\t ------------------ \n");
     printf("\n\n\t\t Programa de Gerenciamento de Clientes\n\n\n\n");
     printf("\t\t\t\t:) \n\n\n\n");
     printf("\n\n\n\n\n\nPressione <ENTER> para continuar");
     getchar();
}

void menuPrincipal(){
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
     
     if(op == 1) printf("novo");
     else if(op == 2) printf("apagar");
     else if(op == 3) printf("alterar");
     else if(op == 4) printf("cliente");
     else if(op == 5) printf("todos");
     else exit(0);
             
}

int main(){
    
    telaInicial();
    menuPrincipal();
    
    printf("\n\nFIM");
    
    getchar();
    return 0;
}
