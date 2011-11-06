#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dos.h>
#include "pgc.h"

/* MAIN MENU */
int menuPrincipal(){
     int op;
     struct Data h;
     
     hojeIs(&h);
     
     do{
         system("cls");
         printf("\t\t\t\t ---------------- \n");
         printf("\t\t\t\t| MENU PRINCIPAL |\n");
         printf("\t\t\t\t ---------------- \n");
         
                  
         printf("\nHoje: %.2d/%.2d/%.4d", h.dia, h.mes, h.ano);
         printf("\n\t1- Novo cliente\n");
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

/* SHOW NAME OF ALL CLIENT */
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

/* SEARCH BY NAME */
void pesquisarNome(){
     char pesquisa[500] = { 0 };
     struct ResultadoPesquisa r;
     
     system("cls");
     printf("\t\t\t ------------------ \n");
     printf("\t\t\t| PESQUISAR - NOME |\n");
     printf("\t\t\t ------------------ \n");
     
     printf("\nDigite o nome a ser pesquisado: ");
     gets(pesquisa);
     strupr(pesquisa);
     
     buscarNome(&r, pesquisa);
     
     if(r.qtd == 0) printf("Nenhum resultado obtido...\n");
     
     else{
        int i;
        
        printf("\n%d resultados obtidos\n\n", r.qtd);
        printf("ID  NOME\n");
        for(i = 0; i < r.qtd; i++){
           struct Cliente c;
        
           if(buildClienteById(&c, r.resultado[i]))
              printf("%.2d: %s\n", c.id, c.nome);
        }
        

        free(r.resultado);
        r.resultado = NULL;
     }
     
     printf("\n<ENTER>");
     getchar();
}

/* SEARCH BY BALANCE */
void pesquisarSaldo(){
     float pesquisa;
     struct ResultadoPesquisa r;
     int op;
     
     do{
         system("cls");
         printf("\t\t\t ------------------- \n");
         printf("\t\t\t| PESQUISAR - SALDO |\n");
         printf("\t\t\t ------------------- \n");
         
         printf("\n 1 - Valores maiores que...\n");
         printf(" 2 - Valores menores que...\n");
         printf("Opcao e <ENTER>: ");
         op = leNumero();
     }while(op < 1 || op > 2);
     
     printf("\nValor para pesquisar(casas decimais com .(ponto)): ");
     pesquisa = leNumero();
     
     if(op == MAIOR_QUE)
        buscarSaldo(&r, pesquisa, MAIOR_QUE);
     else
        buscarSaldo(&r, pesquisa, MENOR_QUE);
        
     if(r.qtd == 0) printf("Nenhum resultado obtido...\n");
     
     else{
        int i;
        
        printf("\n%d resultados obtidos\n\n", r.qtd);
        printf("ID  NOME\n");
        for(i = 0; i < r.qtd; i++){
           struct Cliente c;
        
           if(buildClienteById(&c, r.resultado[i]))
              printf("%.2d: %s\n", c.id, c.nome);
        }
        

        free(r.resultado);
        r.resultado = NULL;
     }
     
     printf("\n<ENTER>");
     getchar();
        
}

/* SEARCH BY PAYMENT DAY */
void pesquisarPrazo(){
     struct Data pesquisa;
     struct ResultadoPesquisa r;
 
     system("cls");
     printf("\t\t\t -------------------------------- \n");
     printf("\t\t\t| PESQUISAR - PRAZO DE PAGAMENTO |\n");
     printf("\t\t\t -------------------------------- \n");
     
     printf("\nDia(dd): ");
     pesquisa.dia = leNumero();
     printf("Mes(mm): ");
     pesquisa.mes = leNumero();
     printf("Ano(aa): ");
     pesquisa.ano = leNumero();

     pesquisarPrazoAte(&r, pesquisa);
     
     if(r.qtd == 0) printf("Nenhum resultado obtido...\n");
     
     else{
        int i;
        
        printf("\n%d resultados obtidos\n\n", r.qtd);
        printf("ID  NOME\n");
        for(i = 0; i < r.qtd; i++){
           struct Cliente c;
        
           if(buildClienteById(&c, r.resultado[i]))
              printf("%.2d: %s\n", c.id, c.nome);
        }
        

        free(r.resultado);
        r.resultado = NULL;
     }
     
     printf("\n<ENTER>");
     getchar();
}

/* SEARCH MENU OPTIONS */
void pesquisar(){
     int op;
     
     do{
         system("cls");
         printf("\t\t\t\t ----------- \n");
         printf("\t\t\t\t| PESQUISAR |\n");
         printf("\t\t\t\t ----------- \n");
    
         printf("Pesquisar por:\n");
         printf(" 1 - Nome\n");
         printf(" 2 - Saldo\n");
         printf(" 3 - Prazo maximo de pagamento\n\n");
         printf(" 4 - Voltar ao menu principal\n\n");
         printf("Digite uma opcao e de <ENTER>: ");
         
         op = leNumero();
    }while(op < 1 || op > 4);
    
    if(op == 1) pesquisarNome();
    if(op == 2) pesquisarSaldo();
    if(op == 3) pesquisarPrazo();
    
    return ;
}

/* ALTER CLIENT'S DATA */
void alterarDadosClienteMenu(struct Cliente *c){
     int op;
     
     while(1){
        system("cls");
        printCliente(c);
        printf("\n\nAlterar:\n");
        printf(" 1 - Nome\n");
        printf(" 2 - Telefone\n");
        printf(" 3 - Celular\n");
        printf(" 4 - Email\n");
        printf(" 5 - Saldo\n");
        printf(" 6 - Vencimento\n");
        printf("\n 7 - Finalizar");
        
        do{
           printf("\nDigite uma opcao e de <ENTER>: "); 
           op = leNumero();
        }while(op < 1 || op > 7);
        
        if(op == 7) break;
        else alterarInformacoesCliente(c, op);
     }
}

/* CHOOSE A CLIENT TO ALTER DATA */
void alterarDados(){
     struct Cliente c;
     int id;   
     
     system("cls");
     printf("\t\t\t ----------------------------- \n");
     printf("\t\t\t| ALTERAR DADOS DE UM CLIENTE |\n");
     printf("\t\t\t ----------------------------- \n");
     
     printf("\n\nDigite o ID do cliente: ");
     id = leNumero();
     
     if(!buildClienteById(&c, id))
        alterarDados();
     
     else alterarDadosClienteMenu(&c);
     
}

/* DELETE A CLIENT */
void apagarRegistroCliente(){
     int id;   
     struct Cliente c;
     system("cls");
     printf("\t\t\t\t ---------------- \n");
     printf("\t\t\t\t| APAGAR CLIENTE |\n");
     printf("\t\t\t\t ---------------- \n");
     
     printf("\n\nDigite o ID do cliente: ");
     id = leNumero();
     
     if(buildClienteById(&c, id)){  
        printf("Voce tem certeza que deseja apagar este cliente?\n");
        printCliente(&c);
        printf("\n\n1 - Sim\n2 - Cancelar\nDigite um numero e de <ENTER>: ");
        if(leNumero() == 1) apagarCliente(c.linha);
     }
}

/* SAVE A NEW CLIENT */
void novoCliente(){ 
     struct Cliente c;
     system("cls");
     printf("\t\t\t\t -------------- \n");
     printf("\t\t\t\t| NOVO CLIENTE |\n");
     printf("\t\t\t\t -------------- \n");
     
     printf("Informacoes vazias nao sao aceitas\nDica: use [-]s para quando nao houve informacao\n\n");
     initCliente(&c);
     leInformacoesBasicasCliente(&c);
     salvarNovoCliente(&c);
}
     

/* MAIN FUNCTION */
int main(){
    int op;
    telaInicial();
    
    do{
       op = menuPrincipal();
       
       if(op == 1) novoCliente();
       else if(op == 2) apagarRegistroCliente();
       else if(op == 3) alterarDados();
       else if(op == 4) exibirDados();
       else if(op == 5) exibirTodosComp();
       else if(op == 6) exibirTodosNome();
       else if(op == 7) pesquisar();
    }while(op != 8);
    
    return 0;
}
