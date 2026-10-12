#include <stdio.h>
#include <locale.h>
#include <stdlib.h>
#include <conio.h>

int menu(){  //Menu principal
	int Opcoes;
	system("cls");
		printf("|========================================|\n");
		printf("|                                        |\n");
		printf("|        ESCOLHA AS OPCOES ABAIXO:       |\n");
		printf("|                                        |\n");
		printf("|========================================|\n\n");
	   	
		printf(" [1] - Cadastrar: \n");
		printf(" [2] - Gerenciar: \n");
		printf(" [3] - Controle de Frequência: \n");
		printf(" [4] - Controle Financeiro: \n");
		printf(" [5] - Encerrar Sistema: \n");
		printf("\n Opção Selecionada: ");
		scanf("%d", &Opcoes);
		return Opcoes;
}

void altera(char textMensage[30]) {
	int OpcoesAlteracao;
	
	do {
		system("cls");
		printf("|========================================|\n");
		printf("|                                        |\n");
		printf("|               GERENCIAR                |\n");
		printf("|                                        |\n");
		printf("|========================================|\n");
		
		printf("\n [1] - Alterar %s: \n", textMensage);
		printf(" [2] - Deletar %s: \n", textMensage);
		printf(" [3] - Relatorio de %s: \n", textMensage);
		printf(" [4] - Voltar ao Sub Menu: \n");
		printf("\n Opcão Selecionada: ");
		scanf("%d", &OpcoesAlteracao);   
		
		switch(OpcoesAlteracao) {
			case 1:
				break;
			case 2:
				break;
			case 3:
				break;
			case 4:
				printf("\n Voltando...");
				getch();
				Gerenciar();
				break;
					
			default:
				printf("\n Opção Inválida!n");
				getch();
				break;
		}
	} while(OpcoesAlteracao != 4);
}

//1 - FUNÇÃO DE CADASTRO
int cadastrar() {
	int OpcoesCadastro;
	
	do {
		system("cls");
		printf("|========================================|\n");
		printf("|                                        |\n");
		printf("|               CADASTRO                 |\n");
		printf("|                                        |\n");
		printf("|========================================|\n");
		
		printf(" [1] - Cadastro Aluno: \n");
		printf(" [2] - Cadastro Profissionais: \n\n");
		printf(" [3] - Cadastro de Treinos: \n");
		printf(" [4] - Cadastro de Planos: \n");
		printf(" [5] - Cadastro de Avaliações Físicas: \n");
		printf(" [6] - Cadastro de Pacote de Modalidades: \n\n");
		printf(" [7] - Cadastro de Agendamento: \n");
		printf(" [8] - Cadastro de Equipamentos: \n\n");
		printf(" [9] - Voltar ao Menu Inicial: \n");
		printf("\n Opcão Selecionada: ");
		scanf("%d", &OpcoesCadastro);   
		      
		switch(OpcoesCadastro) {
			case 1:
				break;
			case 2:
				break;
			case 3:
				break;
			case 4:
				break;
			case 5:
				break;
			case 6:
				break;
			case 7:
				break;
			case 8:
				break;
			case 9:
				printf("\n Voltando...");
				getch();
				break;
					
			default:
				printf("\n Opção Inválida!");
				getch();
				break;
		}
	} while(OpcoesCadastro != 9);
}

//2 - FUNÇÃO DE GERENCIAR
int Gerenciar() {
	int OpcoesGerenciar;
	
	do {
		system("cls");
		printf("|========================================|\n");
		printf("|                                        |\n");
		printf("|              GERENCIAR                 |\n");
		printf("|                                        |\n");
		printf("|========================================|\n");
			   
		printf("\n [1] - Gerenciar Alunos: \n");
		printf(" [2] - Gerenciar Profissionais: \n\n");
		printf(" [3] - Gerenciar Treinos: \n");
		printf(" [4] - Gerenciar Planos: \n");
		printf(" [5] - Gerenciar Matrículas: \n");
		printf(" [6] - Gerenciar Avaliações Físicas: \n\n");
		printf(" [7] - Gerenciar Agenda de Profissionais: \n");
		printf(" [8] - Gerenciar Equipamentos: \n");
		printf(" [9] - Gerenciar Pacote de Modalidades: \n\n");
		printf(" [10] - Voltar ao Menu Inicial: \n");
		printf("\n Opção Selecionada: ");
		scanf("%d", &OpcoesGerenciar);
		
		switch(OpcoesGerenciar)	{
			case 1:
				//Alunos
				altera("Alunos");
				break;
					
		//===========================================================*/	
		
			case 2:
			  	//Profissionais
			  	altera("Profissionais");
		   		break;
		    		
		//===========================================================*/	
		
			case 3:
			   	//Treinos
			   	altera("Treinos");
				break;
					
		//===========================================================*/	
		
			case 4:
			   	//Planos
			   	altera("Planos");
				break;
					
		//===========================================================*/	
		    case 5:
		    	//Matrículas
		    	altera("Matrículas");
		        break;
		
		//===========================================================*/	
		
			case 6:
			   	//Avaliações Físicas
			   	altera("Avaliações Físicas");
				break;
					
		//===========================================================*/	
		
			case 7:
			   	//Agenda de Profissionais
			   	altera("Agenda de Profissionais");
				break;
					
		//===========================================================*/	
		
			case 8:
			   	//Equipementos
			   	altera("Equipementos");
				break;
					
		//===========================================================*/	
			case 9:
			   	//Pacote de Modalidades
			   	altera("Pacote de Modalidades");
				break;  
			   
			   
		//===========================================================*/		   
			case 10:
			  	printf("\n Voltando...");
				getch();
			   	break;
				   
		//===========================================================*/	
		
			default:
				printf("\n Opção Inválida!");
				getch();
				break;          	
		}
	} while(OpcoesGerenciar != 10);
}

//4 - FUNÇÃO LISTAMENTO DE FREQUENCIA
int frequencia() {
	int Opcoes;
	
	do {
		system("cls");
		printf("|========================================|\n");
		printf("|                                        |\n");
		printf("|        ESCOLHA AS OPCOES ABAIXO:       |\n");
		printf("|                                        |\n");
		printf("|========================================|\n\n");
		   	
		printf(" [1] - Listar Frequecia de Alunos: \n");
		printf(" [2] - Fazer Chamada de Alunos: \n");
		printf(" [3] - Alterar Frequencia de Alunos: \n");
		printf(" [4] - Voltar ao Menu Inicial: \n");
		printf("\n Opção Selecionada: ");
		scanf("%d", &Opcoes);
		
		switch(Opcoes) {
			case 1:
				break;
			case 2:
				break;
			case 3:
				break;
			case 4:
				printf("\n Voltando...");
				getch();
				break;
					
			default:
				printf("\n Opção Inválida!");
				getch();
				break;
		}
	} while(Opcoes != 4);
}

//5 - Função de Controle Financeiro
int financeiro() {
	int Opcoes;
	
	do {
		system("cls");
		printf("|========================================|\n");
		printf("|                                        |\n");
		printf("|        ESCOLHA AS OPCOES ABAIXO:       |\n");
		printf("|                                        |\n");
		printf("|========================================|\n\n");
		   	
		printf(" [1] - Listar Faturamento Mensal: \n");
		printf(" [2] - Listar Faturamento por Planos: \n");
		printf(" [3] - Listar Gastos Mensais: \n");
		printf(" [4] - Pagamentos por Aluno: \n");
		printf(" [5] - Voltar ao Menu Inicial: \n");
		printf("\n Opção Selecionada: ");
		scanf("%d", &Opcoes);
		
		switch(Opcoes) {
			case 1:
				break;
			case 2:
				break;
			case 3:
				break;
			case 4:
				break;
			case 5:
				printf("\n Voltando...");
				getch();
				break;
					
			default:
				printf("\n Opção Inválida!");
				getch();
				break;
		}
	} while(Opcoes != 4);
}

int main() {
	setlocale(LC_ALL, "Portuguese");
	int Opcoes;
	
   	do{	
    	Opcoes=menu();
		switch(Opcoes) {
			case 1:
		    	//Cadastro
				cadastrar();	
			    break;
				
		//===========================================================*/		
		
			case 2:
				//Gerenciar
				Gerenciar();
				break;
				
		//===========================================================*/
				
			case 3:
				//Controle de Frequencia
				frequencia();
				break;	
				
		//===========================================================*/
				
			case 4:
				//Controle Financeiro
				financeiro();
				break;	
				
		//===========================================================*/
		
			case 5:
				printf("\n SISTEMA ENCERRADO! ");
				break;	
					
		//===========================================================*/
					
			default:
				//Sair do Sistema
				printf("Opção inválida!\n");
				getch();
				break;
     	}
   	}while(Opcoes!=5);
}
