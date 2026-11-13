#include <stdio.h>
#include <locale.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <ctype.h>

// Entidades do Gerenciar (Serve como auxiliar de verificação Ex: case).
enum { // define em sequência depois usa pra chamar a posição.
	ENT_ALUNO,          // ENT_ALUNO vale 0 é a posição de "Alunos" no nomesEntidades. o altera() usa pra reaproveitar o menu.
	ENT_PROFISSIONAL,   // 1
	ENT_TREINO,         // 2
	ENT_PLANO,          // 3
	ENT_MATRICULA,      // 4
	ENT_AVALIACAO,      // 5
	ENT_AGENDA,         // 6
	ENT_EQUIPAMENTO,    // 7
	ENT_PACOTE,         // 8
	ENT_ATIVIDADE       // 9
};

// Nomes das entidades, na mesma ordem do enum.
char nomesEntidades[10][31] = {
	"Alunos",
	"Profissionais",
	"Treinos",
	"Planos",
	"Matrículas",
	"Avaliações Físicas",
	"Agenda de Profissionais",
	"Equipamentos",
	"Pacote de Modalidades",
	"Atividades"
};

// Structs:
typedef struct {
	int dia;
	int mes;
	int ano;
} Data;

typedef struct {
	int  codigo;
	char nome[61];
	char cpf[16];
	Data dataNascimento;
	char telefone[16];
	char email[61];
	int  ativo;
} Aluno;

typedef struct {
	int  codigo;
	char nome[61];
	char cpf[15];
	char telefone[17];
	char funcao[31];
	int  ativo;
} Profissional;

typedef struct {
	int   codigo;
	char  nome[61];
	char  descricao[101];
	int   duracaoMin;
	int   capacidade;
	float valorAula;
	int   ativo;
} Atividade;

typedef struct {
	int   codigo;
	char  nome[31];
	int   meses;
	float valorMensal;
	float porcentagemDesconto;
	int   ativo;
} Plano;

typedef struct {
	int   codigo;
	char  nome[61];
	int   qtdAtivPacote;     // diferente do contador global.
	int   codAtividades[5];
	float valor;
	int   ativo;
} Pacote;

typedef struct {
	int   codigo;
	int   codAluno;
	int   codPlano;
	int   codPacote;
	Data  dataInicio;
	Data  dataTermino;
	float valorTotal;
	int   situacao;     // 1 = ativa, 2 = cancelada.
} Matricula;

typedef struct {
	int   codigo;
	int   codMatricula;
	int   mesRef;
	int   anoRef;
	float valor;
	Data  vencimento;
	Data  dataPagamento;
	int   situacao;   // 1 = pendente, 2 = paga, 3 = cancelada.
} Cobranca;

typedef struct {
	int   codigo;
	int   codMatricula;
	Data  data;
	float valorPago;
	float valorDevolvido;
	float valorRetido;
} Cancelamento;

typedef struct {
	int  codigo;
	int  codAluno;
	int  codProfissional;
	char nome[31];
	char objetivo[61];
	char exercicios[10][41];
	Data dataCriacao;
	int  ativo;
} Treino;

typedef struct {
	int   codigo;
	int   codAluno;
	int   codProfissional;
	Data  data;
	float peso;
	float altura;
	float imc;
	float percGordura;
	char  observacoes[101];
	int   ativo;
} Avaliacao;

typedef struct {
	int codigo;
	int codProfissional;
	int codAtividade;
	int diaSemana;      
	int horaInicio;     
	int horaFim;
	int ativo;
} Agenda;

typedef struct {
	int   codigo;
	char  nome[61];
	char  tipo[31];
	int   quantidade;
	Data  dataAquisicao;
	float valor;
	int   situacao;   // 1 = bom, 2 = manutenção, 3 = inativo
	Data  dataUltimaManutencao;
	int   ativo;
} Equipamento;

typedef struct {
	int  codigo;
	int  codAluno;
	int  codAtividade;
	Data data;
	int  presente;
} Presenca;

typedef struct {
	int   codigo;
	char  descricao[61];
	char  categoria[31];
	float valor;
	Data  data;
	int   ativo;
} Gasto;

// OBS: São variáveis globais, se preferir mudar avisar no grupo e seguir as regras do relatorio.
Aluno        alunos;
Profissional profissionais;
Atividade    atividades;
Plano        planos;
Pacote       pacotes;
Matricula    matriculas;
Cobranca     cobrancas;
Cancelamento cancelamentos;
Treino       treinos;
Avaliacao    avaliacoes;
Agenda       agendas;
Equipamento  equipamentos;
Presenca     presencas;
Gasto        gastos;

// Protótipos
int  menu(void);
void altera(int codEntidade);
void cadastrar(void);
void Gerenciar(void);
void frequencia(void);
void financeiro(void);
void operacoes(void);
void relatorios(void);
void alterarAvaliacao(void);
void excluirAvaliacao(void);
void listarAvaliacoes(void);
int existeAlunoAvaliacao(int codigo);
int existeProfissionalAvaliacao(int codigo);
int existeAtividade(int codigo);
void cadastrarAtividade(void);
void alterarAtividade(void);
void excluirAtividade(void);
void listarAtividades(void);

// Função de Busca
int busca(FILE *arq, int cod, int codEnt) {
	rewind(arq);
	
	Aluno tempAluno;
	Profissional tempProfissional;
	Avaliacao tempAvaliacao;
	Atividade tempAtividade;
	
	switch(codEnt) {
		case ENT_ALUNO:
			fread(&tempAluno, sizeof(Aluno), 1, arq);
			while(!feof(arq) && tempAluno.codigo != cod) {
				fread(&tempAluno, sizeof(Aluno), 1, arq);
			}
			if(!feof(arq)) return (ftell(arq) - sizeof(Aluno));
			else return -1;   
			break;
		case ENT_PROFISSIONAL:
			fread(&tempProfissional, sizeof(Profissional), 1, arq);
			while(!feof(arq) && tempProfissional.codigo != cod) {
				fread(&tempProfissional, sizeof(Profissional), 1, arq);
			}
			if(!feof(arq)) return (ftell(arq) - sizeof(Profissional));
			else return -1;   
			break;
			case ENT_AVALIACAO:
            while(fread(&tempAvaliacao, sizeof(Avaliacao), 1, arq) == 1)
                {
                    if(tempAvaliacao.codigo == cod)
                        return (int)(ftell(arq) - sizeof(Avaliacao));
                }
                return -1;
            break;
		case ENT_ATIVIDADE:
			while(fread(&tempAtividade, sizeof(Atividade), 1, arq) == 1) {
				if(tempAtividade.codigo == cod) return (int)(ftell(arq) - sizeof(Atividade));
			}
			return -1;
			break;
	}
}

int existeAlunoAvaliacao(int codigo) {
	FILE *arq;
	Aluno temp;
	int pos, achou = 0;

	arq = fopen("alunos.bin", "rb");
	if(arq == NULL) return 0;

	pos = busca(arq, codigo, ENT_ALUNO);
	if(pos != -1) {
		fseek(arq, pos, 0);
		fread(&temp, sizeof(Aluno), 1, arq);
		if(temp.ativo == 1) achou = 1;
	}
	fclose(arq);
	return achou;
}

int existeProfissionalAvaliacao(int codigo) {
	FILE *arq;
	Profissional temp;
	int pos, achou = 0;

	arq = fopen("profissionais.bin", "rb");
	if(arq == NULL) return 0;

	pos = busca(arq, codigo, ENT_PROFISSIONAL);
	if(pos != -1) {
		fseek(arq, pos, 0);
		fread(&temp, sizeof(Profissional), 1, arq);
		if(temp.ativo == 1) achou = 1;
	}
	fclose(arq);
	return achou;
}

int nomeDoAluno(int codigo, char destino[]) {
	FILE *arq;
	Aluno temp;
	int pos, achou = 0;

	arq = fopen("alunos.bin", "rb");
	if(arq == NULL) return 0;

	pos = busca(arq, codigo, ENT_ALUNO);
	if(pos != -1) {
		fseek(arq, pos, 0);
		fread(&temp, sizeof(Aluno), 1, arq);
		strcpy(destino, temp.nome);
		achou = 1;
	}
	fclose(arq);
	return achou;
}

int nomeDoProfissional(int codigo, char destino[]) {
	FILE *arq;
	Profissional temp;
	int pos, achou = 0;

	arq = fopen("profissionais.bin", "rb");
	if(arq == NULL) return 0;

	pos = busca(arq, codigo, ENT_PROFISSIONAL);
	if(pos != -1) {
		fseek(arq, pos, 0);
		fread(&temp, sizeof(Profissional), 1, arq);
		strcpy(destino, temp.nome);
		achou = 1;
	}
	fclose(arq);
	return achou;
}

int temProfissionalAtivo(void) 
{
    FILE *arq;
    Profissional temp;
    int encontrou = 0;

    arq = fopen("profissionais.bin", "rb");

    if(arq == NULL)
        return 0;

    while(fread(&temp, sizeof(Profissional), 1, arq) == 1) {
        if(temp.ativo == 1)
            encontrou = 1;
    }

    fclose(arq);

    return encontrou;
}

int temAlunoAtivo(void) {
    FILE *arq;
    Aluno temp;
    int encontrou = 0;

    arq = fopen("alunos.bin", "rb");

    if(arq == NULL)
        return 0;

    while(fread(&temp, sizeof(Aluno), 1, arq) == 1) {
        if(temp.ativo == 1)
            encontrou = 1;
    }

    fclose(arq);

    return encontrou;
}
// Funções de Validação
int validaExcedeu(char texto[]) {
	fflush(stdin);
    if(texto[strlen(texto)-1] != '\n') return 1;
	else {
		texto[strlen(texto)-1] = '\0';
		return 0;
	} 
}

int validaCPF(char cpf[]) {
	int i;
	if(strlen(cpf) == 14) {
		if(cpf[3] == '.' && cpf[7] == '.' && cpf[11] == '-') {
			for(i = 0; i < 14; i++) {
				if(i != 3 && i != 7 && i != 11) {
					if(!isdigit(cpf[i])) return 0;
				}
			}
			return 1;
		} else return 0;
		
	}
	return 0;
}

int validaTelefone(char tel[]) {
	int i;
    if(strlen(tel) == 14) { // Formato: (XX) XXXX-XXXX
        if(tel[0] == '(' && tel[3] == ')' && tel[4] == ' ' && tel[9] == '-') {
            for(i = 0; i < 14; i++) {
                if(i != 0 && i != 3 && i != 4 && i != 9) {
                    if(!isdigit(tel[i])) return 0;
                }
            }
            return 1;
        }
    } else if(strlen(tel) == 15) { // Formato: (XX) XXXXX-XXXX
        if(tel[0] == '(' && tel[3] == ')' && tel[4] == ' ' && tel[10] == '-') {
            for(i = 0; i < 15; i++) {
                if(i != 0 && i != 3 && i != 4 && i != 10) {
                    if(!isdigit(tel[i])) return 0;
                }
            }
            return 1;
        }
    }
    return 0;
}

int validaData(Data d) {
	if (d.ano < 1900 || d.ano > 2100) return 0; // Valida o Ano
    if (d.mes < 1 || d.mes > 12) return 0; // Valida o Mês

    // Valida os Dias Baseado no Mês
    if (d.mes == 1 || d.mes == 3 || d.mes == 5 || d.mes == 7 || d.mes == 8 || d.mes == 10 || d.mes == 12) {
        if (d.dia < 1 || d.dia > 31) return 0;
    } else if (d.mes == 4 || d.mes == 6 || d.mes == 9 || d.mes == 11) {
        if (d.dia < 1 || d.dia > 30) return 0;
    } else if (d.mes == 2) {
        // Validação Ano Bissexto Fevereiro
        int bissexto = 0;
        if ((d.ano % 4 == 0 && d.ano % 100 != 0) || (d.ano % 400 == 0)) bissexto = 1;

        if (bissexto && (d.dia < 1 || d.dia > 29)) return 0;
        if (!bissexto && (d.dia < 1 || d.dia > 28)) return 0;
    }

    return 1;
}

int validaEmail(char email[]) {
	int i, contArroba = 0, contPonto = 0;
    
    for (i = 0; i < strlen(email); i++) {
        char c = email[i];

        if (c == '@') {
            contArroba++;
            if (contArroba > 1) return 0;
        } else if (contArroba == 0) { // Antes do @
            if (!isalnum(c) && c != '-' && c != '.') return 0;
        } else { // Depois do @
            if (c == '.') contPonto++;
            else if (!isalnum(c)) return 0;
        }
    }

    if (contArroba == 1 && contPonto >= 1) return 1;
    return 0;
}

// Menu principal
int menu(void){
	int Opcoes;
	system("cls");
		printf("==============================================\n");
		printf("           ACADEMIA - PROJECT ALPHA\n");
		printf("           Sistema de Gerenciamento\n");
		printf("==============================================\n");
		printf("  MENU PRINCIPAL\n");
		printf("\n");
		printf("  [1]  Cadastros\n");
		printf("       Alunos, planos, pacotes e equipamentos\n");
		printf("  [2]  Gerenciar\n");
		printf("       Alterar, excluir e listar registros\n");
		printf("  [3]  Operações\n");
		printf("       Matrícula, recebimento e cancelamento\n");
		printf("  [4]  Controle de Frequência\n");
		printf("       Chamada e histórico de presença\n");
		printf("  [5]  Controle Financeiro\n");
		printf("       Faturamento, gastos e pagamentos\n");
		printf("  [6]  Relatórios\n");
		printf("       Consultas com filtros\n");
		printf("  [0]  Encerrar sistema\n");        // <--- OBS: encerrar padronizado com 0 em todos menus ok.
		printf("----------------------------------------------\n");
		printf("  Opção: ");
		Opcoes = 0;
		scanf("%d", &Opcoes);
		fflush(stdin);
		return Opcoes;
}

// Funções de Alteração
void alterarAluno(void) {
	FILE *arq;
	int OpcoesAlteracao, matricula, i, pos, tempPos;

	do {
		system("cls");
		fflush(stdin);
		printf("==============================================\n");
		printf("  ALTERAR - Alunos\n");
		printf("==============================================\n\n");
		printf("  [1]  Matrícula\n");
		printf("  [2]  Nome\n");
		printf("  [3]  CPF\n");
		printf("  [4]  Data Nascimento\n");
		printf("  [5]  Telefone\n");
		printf("  [6]  E-mail\n");
		printf("  [7]  Ativo\n");
		printf("  [0]  Voltar\n\n");
		printf("----------------------------------------------\n");
		printf("  Opção: ");
		scanf("%d", &OpcoesAlteracao);
		
		if(OpcoesAlteracao != 0) {
			arq = fopen("alunos.bin", "rb+");
			if(arq == NULL) printf("Erro na Abertura do Arquivo!\n\n");
			else {
				printf("\nInfome a Matrícula do Aluno: ");
				scanf("%d", &matricula);
				
				pos = busca(arq, matricula, ENT_ALUNO);
				if(pos != -1) {
					fseek(arq, pos, 0);
					fread(&alunos, sizeof(Aluno), 1, arq);
					fseek(arq, pos, 0);
					
					fflush(stdin);
					switch(OpcoesAlteracao) {
						case 1:
							do {
								do {
									printf("Informe a Matrícula do Aluno: ");
									scanf("%d", &alunos.codigo);
									if(alunos.codigo < 1 || alunos.codigo > 1000) printf("A Matrícula Deve Estar Entre 1 e 1000!\n\n");
								} while(alunos.codigo < 1 || alunos.codigo > 1000);
								
								tempPos = busca(arq, alunos.codigo, ENT_ALUNO);
								if(tempPos != -1) printf("Matrícula Já Cadastrada!\n\n");
							} while(tempPos != -1);
							fwrite(&alunos, sizeof(Aluno), 1, arq);
							printf("Alteração Realizada com Sucesso!\n");
							break;
						case 2:
							printf("Informe o Novo Nome: ");
							fgets(alunos.nome, 61, stdin);
							if(validaExcedeu(alunos.nome) == 1) printf("Somente os Primeiros 60 Caracteres Serão Cadastrados!\n\n");
							fwrite(&alunos, sizeof(Aluno), 1, arq);
							printf("Alteração Realizada com Sucesso!\n");
							break;
						case 3:
							do {
								printf("Informe o Novo CPF: ");
								fgets(alunos.cpf, 15, stdin);
								alunos.cpf[15] = '\0';
								
								if(validaCPF(alunos.cpf) == 0) printf("Formato de CPF Inválido! \nInsira este Formato XXX.XXX.XXX-XX \nPreencha no Lugar do X Somente Números!\n\n");
							} while(validaCPF(alunos.cpf) != 1);
							fwrite(&alunos, sizeof(Aluno), 1, arq);
							printf("Alteração Realizada com Sucesso!\n");
							break;
						case 4:
							do {
								printf("Informe a Nova Data de Nascimento (DD MM AAAA): ");
								scanf("%d%d%d", &alunos.dataNascimento.dia, &alunos.dataNascimento.mes, &alunos.dataNascimento.ano);
								
								if(validaData(alunos.dataNascimento) == 0) printf("Insira Uma Data Valida! \nAno Entre 1900 e 2100 \nMês Entre 1 e 12 \nE os Dias Válidos de Cada Mês\n\n");
							} while(validaData(alunos.dataNascimento) != 1);
							fwrite(&alunos, sizeof(Aluno), 1, arq);
							printf("Alteração Realizada com Sucesso!\n");
							break;
						case 5:
							do {
								printf("Informe o Novo Telefone: ");
								fgets(alunos.telefone, 17, stdin);
								alunos.telefone[strlen(alunos.telefone)-1] = '\0';
								
								if(validaTelefone(alunos.telefone) == 0) printf("Formato de Telefone Inválido! \nInsira este Formato (XX) XXXX-XXXX ou (XX) XXXXX-XXXX \nPreencha no Lugar do X Somente Números!\n\n");
							} while(validaTelefone(alunos.telefone) != 1);
							fwrite(&alunos, sizeof(Aluno), 1, arq);
							printf("Alteração Realizada com Sucesso!\n");
							break;
						case 6:
							do {
								printf("Informe o Novo E-mail: ");
								fgets(alunos.email, 61, stdin);
								if(validaExcedeu(alunos.email) == 1) printf("Somente os Primeiros 60 Caracteres Serão Cadastrados!\n\n");
								
								if(validaEmail(alunos.email) == 0) printf("Email Deve conter um @ e um . depois Dele \nAntes do @ só pode Conter - ou _ \nDepois do @ Apenas .\n\n");
							} while(validaEmail(alunos.email) != 1);
							fwrite(&alunos, sizeof(Aluno), 1, arq);
							printf("Alteração Realizada com Sucesso!\n");
							break;
						case 7:
							do {
								printf("Informe a Situação (1- Ativo / 0- Inativo): ");
								scanf("%d", &alunos.ativo);
								
								if(alunos.ativo != 1 && alunos.ativo != 0) printf("Informe Uma Situação Válida!\n\n");
							} while(alunos.ativo != 1 && alunos.ativo != 0);
							fwrite(&alunos, sizeof(Aluno), 1, arq);
							printf("\nAlteração Realizada com Sucesso!\n");
							break;
						default:
							printf("\nOpção Inválida!");
							break;
					}
				} else printf("Aluno não Encontrado!\n\n");
			}
			fclose(arq);
		}
		system("pause");
	} while(OpcoesAlteracao != 0);
}

void alterarProfissional(void) {
	FILE *arq;
	int OpcoesAlteracao, matricula, i, pos;

	do {
		system("cls");
		printf("==============================================\n");
		printf("  ALTERAR - Profissionais\n");
		printf("==============================================\n\n");
		printf("  [1]  Matrícula\n");
		printf("  [2]  Nome\n");
		printf("  [3]  CPF\n");
		printf("  [4]  Telefone\n");
		printf("  [5]  Função\n");
		printf("  [6]  Ativo\n");
		printf("  [0]  Voltar\n\n");
		printf("----------------------------------------------\n");
		printf("  Opção: ");
		scanf("%d", &OpcoesAlteracao);

		if(OpcoesAlteracao != 0) {
			arq = fopen("profissionais.bin", "rb+");
			if(arq == NULL) printf("Erro na Abertura do Arquivo!\n\n");
			else {
				printf("\nInforme a Matrícula do Profissional: ");
				scanf("%d", &matricula);
		
				pos = busca(arq, matricula, ENT_PROFISSIONAL);
				if(pos != -1) {
					fseek(arq, pos, 0);
					fread(&profissionais, sizeof(Profissional), 1, arq);
					fseek(arq, pos, 0);
					
					fflush(stdin);
					switch (OpcoesAlteracao) {
						case 1:
							do {
								do {
									printf("Informe a Matrícula do Profissional: ");
									scanf("%d", &profissionais.codigo);
									if(profissionais.codigo < 1 || profissionais.codigo > 1000) printf("A Matrícula Deve Estar Entre 1 e 1000!\n\n");
								} while(profissionais.codigo < 1 || profissionais.codigo > 1000);
								
								pos = busca(arq, profissionais.codigo, ENT_PROFISSIONAL);
								if(pos != -1) printf("Matrícula Já Cadastrada!\n\n");
							} while(pos != -1);
							fwrite(&profissionais, sizeof(Profissional), 1, arq);
							printf("\nAlteração Realizada com Sucesso!\n");
							break;
						case 2:
							printf("Informe o Novo Nome: ");
							fgets(profissionais.nome, 61, stdin);
							if(validaExcedeu(profissionais.nome) == 1) printf("Somente os Primeiros 60 Caracteres Serão Cadastrados!\n\n");
							fwrite(&profissionais, sizeof(Profissional), 1, arq);
							printf("Alteração Realizada com Sucesso!\n");
							break;
						case 3:
							do {
								printf("Informe o Novo CPF: ");
								fgets(profissionais.cpf, 15, stdin);
								profissionais.cpf[15] = '\0';
								
								if(validaCPF(profissionais.cpf) == 0) printf("Formato de CPF Inválido! \nInsira este Formato XXX.XXX.XXX-XX \nPreencha no Lugar do X Somente Números!\n\n");
							} while(validaCPF(profissionais.cpf) != 1);
							fwrite(&profissionais, sizeof(Profissional), 1, arq);
							printf("Alteração Realizada com Sucesso!\n");
							break;
						case 4:
							do {
								printf("Informe o Novo Telefone: ");
								fgets(profissionais.telefone, 17, stdin);
								profissionais.telefone[strlen(profissionais.telefone)-1] = '\0'; // Substitui \n por \0
								
								if(validaTelefone(profissionais.telefone) == 0) printf("Formato de Telefone Inválido! \nInsira este Formato (XX) XXXX-XXXX ou (XX) XXXXX-XXXX \nPreencha no Lugar do X Somente Números!\n\n");
							} while(validaTelefone(profissionais.telefone) != 1);
							fwrite(&profissionais, sizeof(Profissional), 1, arq);
							printf("Alteração Realizada com Sucesso!\n");
							break;
						case 5:
							printf("Informe a Nova Função: ");
							fgets(profissionais.funcao, 31, stdin);
							if(validaExcedeu(profissionais.funcao) == 1) printf("Somente os Primeiros 30 Caracteres Serão Cadastrados!\n\n");
							fwrite(&profissionais, sizeof(Profissional), 1, arq);
							printf("Alteração Realizada com Sucesso!\n");
							break;
						case 6:
							do {
								printf("Informe a Situação (1- Ativo / 0- Inativo): ");
							scanf("%d", &profissionais.ativo);
								
								if(profissionais.ativo != 1 && profissionais.ativo != 0) printf("Informe Uma Situação Válida!\n\n");
							} while(profissionais.ativo != 1 && profissionais.ativo != 0);
							fwrite(&profissionais, sizeof(Profissional), 1, arq);
							printf("Alteração Realizada com Sucesso!\n");
							break;
						default:
							printf("\nOpção Inválida!\n");
							break;
					}
				} else printf("\nProfissional Não Encontrado!\n");
			}
			fclose(arq);
		}
		system("pause");
	} while (OpcoesAlteracao != 0);
}
void alterarAvaliacao(void) {
	FILE *arq;
	int opcao, codigo, pos, aux, alterou;

	do {
		system("cls");
		printf("==============================================\n");
		printf("       ALTERAR - AVALIAÇÕES FÍSICAS\n");
		printf("==============================================\n\n");
		printf("  [1]  Aluno\n");
		printf("  [2]  Profissional\n");
		printf("  [3]  Data\n");
		printf("  [4]  Peso\n");
		printf("  [5]  Altura\n");
		printf("  [6]  Percentual de Gordura\n");
		printf("  [7]  Observações\n");
		printf("  [8]  Ativo\n");
		printf("  [0]  Voltar\n\n");
		printf("----------------------------------------------\n");
		printf("  Opção: ");
		scanf("%d", &opcao);

		if(opcao != 0) {
			arq = fopen("avaliacoes.bin", "rb+");
			if(arq == NULL) printf("\nNenhuma Avaliação Cadastrada!\n\n");
			else {
				printf("\nInforme o Código da Avaliação: ");
				scanf("%d", &codigo);

				pos = busca(arq, codigo, ENT_AVALIACAO);
				if(pos != -1) {
					fseek(arq, pos, 0);
					fread(&avaliacoes, sizeof(Avaliacao), 1, arq);
					alterou = 1;

					switch(opcao) {
						case 1:
							printf("Informe o Novo Código do Aluno: ");
							scanf("%d", &aux);
							if(existeAlunoAvaliacao(aux)) avaliacoes.codAluno = aux;
							else {
								printf("Aluno Não Encontrado ou Inativo!\n\n");
								alterou = 0;
							}
							break;
						case 2:
							printf("Informe o Novo Código do Profissional: ");
							scanf("%d", &aux);
							if(existeProfissionalAvaliacao(aux)) avaliacoes.codProfissional = aux;
							else {
								printf("Profissional Não Encontrado ou Inativo!\n\n");
								alterou = 0;
							}
							break;
						case 3:
							do {
								printf("Informe a Nova Data (DD MM AAAA): ");
								scanf("%d%d%d", &avaliacoes.data.dia, &avaliacoes.data.mes, &avaliacoes.data.ano);
								if(validaData(avaliacoes.data) == 0) printf("Data Inválida!\n\n");
							} while(validaData(avaliacoes.data) != 1);
							break;
						case 4:
							do {
								printf("Informe o Novo Peso (KG): ");
								scanf("%f", &avaliacoes.peso);
								if(avaliacoes.peso < 1 || avaliacoes.peso > 500) printf("O Peso Deve Estar Entre 1 e 500 KG!\n\n");
							} while(avaliacoes.peso < 1 || avaliacoes.peso > 500);
							break;
						case 5:
							do {
								printf("Informe a Nova Altura (M): ");
								scanf("%f", &avaliacoes.altura);
								if(avaliacoes.altura < 0.5 || avaliacoes.altura > 2.5) printf("A Altura Deve Estar Entre 0.5 e 2.5 M!\n\n");
							} while(avaliacoes.altura < 0.5 || avaliacoes.altura > 2.5);
							break;
						case 6:
							do {
								printf("Informe o Novo Percentual de Gordura: ");
								scanf("%f", &avaliacoes.percGordura);
								if(avaliacoes.percGordura < 0 || avaliacoes.percGordura > 100) printf("Percentual Inválido! (0 a 100)\n\n");
							} while(avaliacoes.percGordura < 0 || avaliacoes.percGordura > 100);
							break;
						case 7:
							fflush(stdin);
							printf("Informe as Novas Observações: ");
							fgets(avaliacoes.observacoes, 101, stdin);
							if(validaExcedeu(avaliacoes.observacoes) == 1) printf("Somente os Primeiros 100 Caracteres Serão Cadastrados!\n\n");
							break;
						case 8:
							do {
								printf("Informe a Situação (1- Ativo / 0- Inativo): ");
								scanf("%d", &avaliacoes.ativo);
								if(avaliacoes.ativo != 1 && avaliacoes.ativo != 0) printf("Informe Uma Situação Válida!\n\n");
							} while(avaliacoes.ativo != 1 && avaliacoes.ativo != 0);
							break;
						default:
							printf("\nOpção Inválida!\n");
							alterou = 0;
							break;
					}

					if(alterou) {
						avaliacoes.imc = avaliacoes.peso / (avaliacoes.altura * avaliacoes.altura);
						fseek(arq, pos, 0);
						fwrite(&avaliacoes, sizeof(Avaliacao), 1, arq);
						printf("\nAlteração Realizada com Sucesso!\n");
					}
				} else printf("\nAvaliação Não Encontrada!\n");
				fclose(arq);
			}
		}
		system("pause");
	} while(opcao != 0);
}
// Funções de Exclusão
void excluirAluno(void) {
	FILE *arq, *temp;
	int matricula, pos, i;
	
	system("cls");
	printf("==============================================\n");
	printf("  EXCLUIR - Alunos\n");
	printf("==============================================\n\n");
	
	arq = fopen("alunos.bin", "rb");
	if(arq == NULL) printf("Erro na Abertura do Arquivo!\n\n");
	else {
		printf("Informe a Matrícula do Aluno: ");
		scanf("%d", &matricula);
		
		pos = busca(arq, matricula, ENT_ALUNO);
		if(pos != -1) {
			fseek(arq, pos, 0);
			fread(&alunos, sizeof(Aluno), 1, arq);
			printf("==============================================\n");
			printf("Matrícula: %d\n", alunos.codigo);
			printf("Nome: %s", alunos.nome);
			printf("CPF: %s", alunos.cpf);
			printf("Data de Nascimento: %d/%d/%d\n", alunos.dataNascimento.dia, alunos.dataNascimento.mes, alunos.dataNascimento.ano);
			printf("Telefone: %s", alunos.telefone);
			printf("E-mail: %s", alunos.email);
			printf("Ativo: ");
			if(alunos.ativo == 1) printf("Sim\n");
			else printf("Não\n");
			printf("==============================================\n");
			printf("Deseja excluir? S/N: ");
			if(toupper(getche()) == 'S') {
				temp = fopen("auxiliar.bin", "wb");
				rewind(arq);
				while(fread(&alunos, sizeof(Aluno), 1, arq) == 1)
				if(alunos.codigo != matricula) fwrite(&alunos, sizeof(Aluno), 1, temp);
				fclose(arq);
				fclose(temp);
				remove("alunos.bin");
				rename("auxiliar.bin", "alunos.bin");
				printf("\nO Aluno Foi Excluido com Sucesso!\n");
			}
		} else printf("\nAluno Não Encontrado!\n");
	}
	fclose(arq);
	system("pause");
}

void excluirProfissional(void) {
	FILE *arq, *temp;
	int matricula, pos, i;
	
	system("cls");
	printf("==============================================\n");
	printf("  EXCLUIR - Profissionais\n");
	printf("==============================================\n\n");
	
	arq = fopen("profissionais.bin", "rb");
	if(arq == NULL) printf("Erro na Abertura do Arquivo!\n\n");
	else {
		printf("Informe a Matrícula do Profissional: ");
		scanf("%d", &matricula);
		
		pos = busca(arq, matricula, ENT_PROFISSIONAL);
		if(!feof(arq)) {
			fseek(arq, pos, 0);
			fread(&profissionais, sizeof(Profissional), 1, arq);
			printf("==============================================\n");
			printf("Matrícula: %d\n", profissionais.codigo);
			printf("Nome: %s", profissionais.nome);
			printf("CPF: %s", profissionais.cpf);
			printf("Telefone: %s", profissionais.telefone);
			printf("Função: %s", profissionais.funcao);
			printf("Ativo: ");
			if(profissionais.ativo == 1) printf("Sim\n");
			else printf("Não\n");
			printf("==============================================\n");
			printf("Deseja excluir? S/N: ");
			if(toupper(getche()) == 'S') {
			    temp = fopen("auxiliar.bin", "wb");
			    rewind(arq);
			    while(fread(&profissionais, sizeof(Profissional), 1, arq) == 1)
			    if(profissionais.codigo != matricula) fwrite(&profissionais, sizeof(Profissional), 1, temp);
			    fclose(arq);
			    fclose(temp);
				remove("profissionais.bin");
				rename("auxiliar.bin", "profissionais.bin");
				printf("\nO Profissional Foi Excluido com Sucesso!\n");
			}
		} else printf("\nProfissional Não Encontrado!\n");
	}
	fclose(arq);
	system("pause");
}

void excluirAvaliacao(void) 
{
    FILE *arq, *temp;
    int codigo, pos, confirmou = 0;
    char nomeA[61], nomeP[61];
    system("cls");
    printf("==============================================\n");
    printf("       EXCLUIR - AVALIAÇÕES FÍSICAS\n");
    printf("==============================================\n\n");
    arq = fopen("avaliacoes.bin", "rb");
    if(arq == NULL) 
    {
        printf("Nenhuma Avaliação Cadastrada!\n");
    }
    else 
    {
        printf("Informe o Código da Avaliação: ");
        scanf("%d", &codigo);
        pos = busca(arq, codigo, ENT_AVALIACAO);
        if(pos != -1) 
        {
            fseek(arq, pos, 0);
            fread(&avaliacoes, sizeof(Avaliacao), 1, arq);
            printf("\nCódigo: %d\n", avaliacoes.codigo);
            if(!nomeDoAluno(avaliacoes.codAluno, nomeA)) strcpy(nomeA, "(não encontrado)");
            if(!nomeDoProfissional(avaliacoes.codProfissional, nomeP)) strcpy(nomeP, "(não encontrado)");
            printf("Aluno: %d - %s\n", avaliacoes.codAluno, nomeA);
            printf("Profissional: %d - %s\n", avaliacoes.codProfissional, nomeP);
            printf("Data: %02d/%02d/%d\n", avaliacoes.data.dia, avaliacoes.data.mes, avaliacoes.data.ano);
            printf("Peso: %.2f KG\n", avaliacoes.peso);
            printf("Altura: %.2f M\n", avaliacoes.altura);
            printf("IMC: %.2f\n", avaliacoes.imc);
            printf("Gordura: %.2f%%\n", avaliacoes.percGordura);
            printf("Observações: %s\n", avaliacoes.observacoes);
            printf("\nDeseja excluir? S/N: ");
            if(toupper(getche()) == 'S')
                confirmou = 1;
            if(confirmou) 
            {
                temp = fopen("auxiliar.bin", "wb");
                if(temp == NULL) 
                {
                    printf("\nErro ao Criar Arquivo Temporário!\n");
                }
                else 
                {
                    rewind(arq);
                    while(fread(&avaliacoes, sizeof(Avaliacao), 1, arq) == 1) 
                        if(avaliacoes.codigo != codigo) 
                            fwrite(&avaliacoes, sizeof(Avaliacao), 1, temp);
                    fclose(arq);
                    arq = NULL;
                    fclose(temp);
                    remove("avaliacoes.bin");
                    rename("auxiliar.bin", "avaliacoes.bin");
                    printf("\nAvaliação Excluída com Sucesso!\n");
                }
            }
        }
        else 
            printf("\nAvaliação Não Encontrada!\n");
        if(arq != NULL)
            fclose(arq);
    }
    system("pause");
}
// Funções de Listagem
void listarAlunos(void) {
	FILE *arq;
	int i;
	
	system("cls");
	printf("==============================================\n");
	printf("  LISTAGEM - Alunos\n");
	printf("==============================================\n");
	
	arq = fopen("alunos.bin", "rb");
	if(arq == NULL) printf("Erro na Abertura do Arquivo!\n\n");
	else {
		while(fread(&alunos, sizeof(Aluno), 1, arq) == 1) {
			printf("Matrícula: %d\n", alunos.codigo);
			printf("Nome: %s\n", alunos.nome);
			printf("CPF: %s\n", alunos.cpf);
			printf("Data de Nascimento: %d/%d/%d\n", alunos.dataNascimento.dia, alunos.dataNascimento.mes, alunos.dataNascimento.ano);
			printf("Telefone: %s\n", alunos.telefone);
			printf("E-mail: %s\n", alunos.email);
			printf("Ativo: ");
			if(alunos.ativo == 1) printf("Sim\n");
			else printf("Não\n");
			printf("==============================================\n");
		}
	}
	fclose(arq);
	system("pause");
}

void listarProfissionais(void) {
	FILE *arq;
	int i;
	
	system("cls");
	printf("==============================================\n");
	printf("  LISTAGEM - Profissionais\n");
	printf("==============================================\n");
	
	arq = fopen("profissionais.bin", "rb");
	if(arq == NULL) printf("Erro na Abertura do Arquivo!\n\n");
	else {
		while(fread(&profissionais, sizeof(Profissional), 1, arq) == 1) {
			printf("Matrícula: %d\n", profissionais.codigo);
			printf("Nome: %s\n", profissionais.nome);
			printf("CPF: %s\n", profissionais.cpf);
			printf("Telefone: %s\n", profissionais.telefone);
			printf("Função: %s\n", profissionais.funcao);
			printf("Ativo: ");
			if(profissionais.ativo == 1) printf("Sim\n");
			else printf("Não\n");
			printf("==============================================\n");
		}
	}
	fclose(arq);
	system("pause");
}

void listarAvaliacoes(void) {
	FILE *arq;
	int codigoAluno = 0, encontrou = 0, opc;
	char nomeA[61], nomeP[61];

	system("cls");
	printf("==============================================\n");
	printf("    HISTÓRICO DE AVALIAÇÕES FÍSICAS\n");
	printf("==============================================\n\n");
	arq = fopen("avaliacoes.bin", "rb");
	if(arq == NULL) printf("\nNenhuma Avaliação Cadastrada!\n");
	else {
		do {
			printf("==============================================\n");
			printf("    SELECIONAR POR CÓDIGO OU LISTA COMPLETA\n");
			printf("==============================================\n\n");
			printf("[1] : Código do aluno\n");
			printf("[2] : Lista completa\n");
			printf("Opção: ");
			scanf("%d", &opc);
			if(opc != 1 && opc != 2) printf("\nOpção Inválida!\n\n");
		} while(opc != 1 && opc != 2);

		if(opc == 1) {
			printf("Informe o Código do Aluno: ");
			scanf("%d", &codigoAluno);
		}

		while(fread(&avaliacoes, sizeof(Avaliacao), 1, arq) == 1) {
			if(opc == 2 || (avaliacoes.codAluno == codigoAluno && avaliacoes.ativo == 1)) {
				encontrou = 1;
				if(!nomeDoAluno(avaliacoes.codAluno, nomeA)) strcpy(nomeA, "(não encontrado)");
				if(!nomeDoProfissional(avaliacoes.codProfissional, nomeP)) strcpy(nomeP, "(não encontrado)");
				printf("\n==============================================\n");
				printf("Código da Avaliação: %d\n", avaliacoes.codigo);
				printf("Aluno: %d - %s\n", avaliacoes.codAluno, nomeA);
				printf("Profissional: %d - %s\n", avaliacoes.codProfissional, nomeP);
				printf("Data: %02d/%02d/%d\n", avaliacoes.data.dia, avaliacoes.data.mes, avaliacoes.data.ano);
				printf("Peso: %.2f KG\n", avaliacoes.peso);
				printf("Altura: %.2f M\n", avaliacoes.altura);
				printf("IMC: %.2f\n", avaliacoes.imc);
				printf("Percentual de Gordura: %.2f%%\n", avaliacoes.percGordura);
				printf("Observações: %s\n", avaliacoes.observacoes);
				printf("Ativo: ");
				if(avaliacoes.ativo == 1) printf("Sim\n");
				else printf("Não\n");
				printf("==============================================\n");
			}
		}

		if(!encontrou) {
			if(opc == 1) printf("\nNenhuma Avaliação Encontrada Para Este Aluno!\n");
			else printf("\nNenhuma Avaliação Cadastrada!\n");
		}
		fclose(arq);
	}
	system("pause");
}

// ===================== MENU ATIVIDADES (parte do Luiggi) =====================
int existeAtividade(int codigo) {
	FILE *arq;
	Atividade temp;
	int pos, achou = 0;

	arq = fopen("atividades.bin", "rb");
	if(arq == NULL) return 0;

	pos = busca(arq, codigo, ENT_ATIVIDADE);
	if(pos != -1) {
		fseek(arq, pos, 0);
		fread(&temp, sizeof(Atividade), 1, arq);
		if(temp.ativo == 1) achou = 1;
	}
	fclose(arq);
	return achou;
}

int nomeAtividadeExiste(char nome[], int codIgnorar) {
	FILE *arq;
	Atividade temp;
	int achou = 0;

	arq = fopen("atividades.bin", "rb");
	if(arq == NULL) return 0;

	while(!achou && fread(&temp, sizeof(Atividade), 1, arq) == 1) {
		if(temp.codigo != codIgnorar && stricmp(temp.nome, nome) == 0) achou = 1;
	}
	fclose(arq);
	return achou;
}

void cadastrarAtividade(void) {
	FILE *arq;
	int pos, repetido;

	arq = fopen("atividades.bin", "ab+");
	if(arq == NULL) printf("Erro na Abertura do Arquivo!\n\n");
	else {
		do {
			do {
				printf("Informe o Código da Atividade: ");
				scanf("%d", &atividades.codigo);
				if(atividades.codigo < 1 || atividades.codigo > 1000) printf("O Código Deve Estar Entre 1 e 1000!\n\n");
			} while(atividades.codigo < 1 || atividades.codigo > 1000);

			pos = busca(arq, atividades.codigo, ENT_ATIVIDADE);
			if(pos != -1) printf("Código Já Cadastrado!\n\n");
		} while(pos != -1);

		do {
			fflush(stdin);
			printf("Informe o Nome da Atividade: ");
			fgets(atividades.nome, 61, stdin);
			if(validaExcedeu(atividades.nome) == 1) printf("Somente os Primeiros 60 Caracteres Serão Cadastrados!\n\n");
			repetido = nomeAtividadeExiste(atividades.nome, 0);
			if(repetido) printf("Já Existe uma Atividade com Esse Nome!\n\n");
		} while(repetido);

		fflush(stdin);
		printf("Informe uma Breve Descrição da Atividade: ");
		fgets(atividades.descricao, 101, stdin);
		if(validaExcedeu(atividades.descricao) == 1) printf("Somente os Primeiros 100 Caracteres Serão Cadastrados!\n\n");

		do {
			printf("Informe a Duração da Atividade (Minutos): ");
			scanf("%d", &atividades.duracaoMin);
			if(atividades.duracaoMin < 1) printf("A Duração Deve Ser Maior que Zero!\n\n");
		} while(atividades.duracaoMin < 1);

		do {
			printf("Informe o Número de Vagas: ");
			scanf("%d", &atividades.capacidade);
			if(atividades.capacidade < 1) printf("O Número de Vagas Deve Ser Maior que Zero!\n\n");
		} while(atividades.capacidade < 1);

		do {
			printf("Informe o Valor da Aula: R$ ");
			scanf("%f", &atividades.valorAula);
			if(atividades.valorAula <= 0) printf("O Valor Deve Ser Maior que Zero!\n\n");
		} while(atividades.valorAula <= 0);

		atividades.ativo = 1;
		fseek(arq, 0, SEEK_END);
		fwrite(&atividades, sizeof(Atividade), 1, arq);
		fclose(arq);
		printf("\n==============================================\n");
		printf("     ATIVIDADE CADASTRADA COM SUCESSO!\n");
		printf("==============================================\n");
	}
}

void alterarAtividade(void) {
	FILE *arq;
	int opcao, codigo, pos, repetido;

	do {
		system("cls");
		printf("==============================================\n");
		printf("  ALTERAR - Atividades\n");
		printf("==============================================\n\n");
		printf("  [1]  Nome\n");
		printf("  [2]  Descrição\n");
		printf("  [3]  Duração\n");
		printf("  [4]  Vagas\n");
		printf("  [5]  Valor da Aula\n");
		printf("  [6]  Ativo\n");
		printf("  [0]  Voltar\n\n");
		printf("----------------------------------------------\n");
		printf("  Opção: ");
		scanf("%d", &opcao);

		if(opcao != 0) {
			arq = fopen("atividades.bin", "rb+");
			if(arq == NULL) printf("\nNenhuma Atividade Cadastrada!\n\n");
			else {
				printf("\nInforme o Código da Atividade: ");
				scanf("%d", &codigo);

				pos = busca(arq, codigo, ENT_ATIVIDADE);
				if(pos != -1) {
					fseek(arq, pos, 0);
					fread(&atividades, sizeof(Atividade), 1, arq);
					fseek(arq, pos, 0);

					fflush(stdin);
					switch(opcao) {
						case 1:
							do {
								printf("Informe o Novo Nome: ");
								fgets(atividades.nome, 61, stdin);
								if(validaExcedeu(atividades.nome) == 1) printf("Somente os Primeiros 60 Caracteres Serão Cadastrados!\n\n");
								repetido = nomeAtividadeExiste(atividades.nome, atividades.codigo);
								if(repetido) printf("Já Existe uma Atividade com Esse Nome!\n\n");
							} while(repetido);
							fwrite(&atividades, sizeof(Atividade), 1, arq);
							printf("Alteração Realizada com Sucesso!\n");
							break;
						case 2:
							printf("Informe a Nova Descrição: ");
							fgets(atividades.descricao, 101, stdin);
							if(validaExcedeu(atividades.descricao) == 1) printf("Somente os Primeiros 100 Caracteres Serão Cadastrados!\n\n");
							fwrite(&atividades, sizeof(Atividade), 1, arq);
							printf("Alteração Realizada com Sucesso!\n");
							break;
						case 3:
							do {
								printf("Informe a Nova Duração (Minutos): ");
								scanf("%d", &atividades.duracaoMin);
								if(atividades.duracaoMin < 1) printf("A Duração Deve Ser Maior que Zero!\n\n");
							} while(atividades.duracaoMin < 1);
							fwrite(&atividades, sizeof(Atividade), 1, arq);
							printf("Alteração Realizada com Sucesso!\n");
							break;
						case 4:
							do {
								printf("Informe o Novo Número de Vagas: ");
								scanf("%d", &atividades.capacidade);
								if(atividades.capacidade < 1) printf("O Número de Vagas Deve Ser Maior que Zero!\n\n");
							} while(atividades.capacidade < 1);
							fwrite(&atividades, sizeof(Atividade), 1, arq);
							printf("Alteração Realizada com Sucesso!\n");
							break;
						case 5:
							do {
								printf("Informe o Novo Valor da Aula: R$ ");
								scanf("%f", &atividades.valorAula);
								if(atividades.valorAula <= 0) printf("O Valor Deve Ser Maior que Zero!\n\n");
							} while(atividades.valorAula <= 0);
							fwrite(&atividades, sizeof(Atividade), 1, arq);
							printf("Alteração Realizada com Sucesso!\n");
							break;
						case 6:
							do {
								printf("Informe a Situação (1- Ativo / 0- Inativo): ");
								scanf("%d", &atividades.ativo);
								if(atividades.ativo != 1 && atividades.ativo != 0) printf("Informe Uma Situação Válida!\n\n");
							} while(atividades.ativo != 1 && atividades.ativo != 0);
							fwrite(&atividades, sizeof(Atividade), 1, arq);
							printf("Alteração Realizada com Sucesso!\n");
							break;
						default:
							printf("\nOpção Inválida!\n");
							break;
					}
				} else printf("\nAtividade Não Encontrada!\n");
				fclose(arq);
			}
		}
		system("pause");
	} while(opcao != 0);
}

void excluirAtividade(void) {
	FILE *arq, *temp;
	int codigo, pos;

	system("cls");
	printf("==============================================\n");
	printf("  EXCLUIR - Atividades\n");
	printf("==============================================\n\n");

	arq = fopen("atividades.bin", "rb");
	if(arq == NULL) printf("Nenhuma Atividade Cadastrada!\n\n");
	else {
		printf("Informe o Código da Atividade: ");
		scanf("%d", &codigo);

		pos = busca(arq, codigo, ENT_ATIVIDADE);
		if(pos != -1) {
			fseek(arq, pos, 0);
			fread(&atividades, sizeof(Atividade), 1, arq);
			printf("==============================================\n");
			printf("Código: %d\n", atividades.codigo);
			printf("Nome: %s\n", atividades.nome);
			printf("Descrição: %s\n", atividades.descricao);
			printf("Duração: %d Minutos\n", atividades.duracaoMin);
			printf("Vagas: %d\n", atividades.capacidade);
			printf("Valor da Aula: R$ %.2f\n", atividades.valorAula);
			printf("Ativo: ");
			if(atividades.ativo == 1) printf("Sim\n");
			else printf("Não\n");
			printf("==============================================\n");
			printf("Deseja excluir? S/N: ");
			if(toupper(getche()) == 'S') {
				temp = fopen("auxiliar.bin", "wb");
				if(temp == NULL) printf("\nErro ao Criar Arquivo Temporário!\n");
				else {
					rewind(arq);
					while(fread(&atividades, sizeof(Atividade), 1, arq) == 1)
						if(atividades.codigo != codigo) fwrite(&atividades, sizeof(Atividade), 1, temp);
					fclose(arq);
					arq = NULL;
					fclose(temp);
					remove("atividades.bin");
					rename("auxiliar.bin", "atividades.bin");
					printf("\nAtividade Excluída com Sucesso!\n");
				}
			}
		} else printf("\nAtividade Não Encontrada!\n");
		if(arq != NULL) fclose(arq);
	}
	system("pause");
}

void listarAtividades(void) {
	FILE *arq;
	int encontrou = 0;

	system("cls");
	printf("==============================================\n");
	printf("  LISTAGEM - Atividades\n");
	printf("==============================================\n");

	arq = fopen("atividades.bin", "rb");
	if(arq == NULL) printf("\nNenhuma Atividade Cadastrada!\n\n");
	else {
		while(fread(&atividades, sizeof(Atividade), 1, arq) == 1) {
			encontrou = 1;
			printf("Código: %d\n", atividades.codigo);
			printf("Nome: %s\n", atividades.nome);
			printf("Descrição: %s\n", atividades.descricao);
			printf("Duração: %d Minutos\n", atividades.duracaoMin);
			printf("Vagas: %d\n", atividades.capacidade);
			printf("Valor da Aula: R$ %.2f\n", atividades.valorAula);
			printf("Ativo: ");
			if(atividades.ativo == 1) printf("Sim\n");
			else printf("Não\n");
			printf("==============================================\n");
		}
		if(!encontrou) printf("\nNenhuma Atividade Cadastrada!\n\n");
		fclose(arq);
	}
	system("pause");
}
// =================== FIM DO MENU ATIVIDADES ===================

void altera(int codEntidade) {
	int OpcoesAlteracao;

	do {
		system("cls");
		printf("==============================================\n");
		printf("  GERENCIAR - %s\n", nomesEntidades[codEntidade]);
		printf("==============================================\n");
		printf("\n");
		printf("  [1]  Alterar\n");
		printf("       Corrige os dados de um registro\n");
		printf("  [2]  Excluir\n");
		printf("       Desativa um registro do sistema\n");
		printf("  [3]  Relatório\n");
		printf("       Lista os registros cadastrados\n");
		printf("  [0]  Voltar ao submenu\n");
		printf("----------------------------------------------\n");
		printf("  Opção: ");
		OpcoesAlteracao = 0;
		scanf("%d", &OpcoesAlteracao);
		fflush(stdin);

		switch(OpcoesAlteracao) {
			case 1:
				// ================= ALTERAR: =================
				switch(codEntidade) {
					case ENT_ALUNO:
						alterarAluno();
						break;
					case ENT_PROFISSIONAL:
						alterarProfissional();
						break;
					case ENT_TREINO:
						// alterarTreino();
						break;
					case ENT_PLANO:
						// alterarPlano();
						break;
					case ENT_MATRICULA:
						// alterarMatricula();
						break;
					case ENT_AVALIACAO:
						alterarAvaliacao();
						break;
					case ENT_AGENDA:
						// alterarAgenda();
						break;
					case ENT_EQUIPAMENTO:
						// alterarEquipamento();
						break;
					case ENT_PACOTE:
						// alterarPacote();
						break;
					case ENT_ATIVIDADE:
						alterarAtividade();
						break;
				}
				break;

			case 2:
				// ========== EXCLUIR: ============ (OBS: Matrícula não é excluída: chama o cancelamento)
				switch(codEntidade) {
					case ENT_ALUNO:
						excluirAluno();
						break;
					case ENT_PROFISSIONAL:
						excluirProfissional();
						break;
					case ENT_TREINO:
						// excluirTreino();
						break;
					case ENT_PLANO:
						// excluirPlano();
						break;
					case ENT_MATRICULA:
						// efetuarCancelamento();
						break;
					case ENT_AVALIACAO:
						excluirAvaliacao();
						break;
					case ENT_AGENDA:
						// excluirAgenda();
						break;
					case ENT_EQUIPAMENTO:
						// excluirEquipamento();
						break;
					case ENT_PACOTE:
						// excluirPacote();
						break;
					case ENT_ATIVIDADE:
						excluirAtividade();
						break;
				}
				break;

			case 3:
				// =========== RELATÓRIO / LISTAR: =============
				switch(codEntidade) {
					case ENT_ALUNO:
						listarAlunos();
						break;
					case ENT_PROFISSIONAL:
						listarProfissionais();
						break;
					case ENT_TREINO:
						// listarTreinos();
						break;
					case ENT_PLANO:
						// listarPlanos();
						break;
					case ENT_MATRICULA:
						// listarMatriculas();
						break;
					case ENT_AVALIACAO:
						listarAvaliacoes();
						break;
					case ENT_AGENDA:
						// listarAgenda();
						break;
					case ENT_EQUIPAMENTO:
						// listarEquipamentos();
						break;
					case ENT_PACOTE:
						// listarPacotes();
						break;
					case ENT_ATIVIDADE:
						listarAtividades();
						break;
				}
				break;

			case 0:
				printf("\n Voltando...\n");
				getch();
				break;

			default:
				printf("\n Opção Inválida!\n");
				getch();
				break;
		}
	} while(OpcoesAlteracao != 0);
}

//1 - FUNÇÃO DE CADASTRO
  void cadastrar(void) {
	FILE *arq;
	int OpcoesCadastro, pos, ok;

	do {
		system("cls");
		printf("==============================================\n");
		printf("                  CADASTROS\n");
		printf("==============================================\n");
		printf("  PESSOAS\n");
		printf("  [1]  Alunos\n");
		printf("  [2]  Profissionais\n");
		printf("\n");
		printf("  SERVIÇOS E ACOMPANHAMENTO\n");
		printf("  [3]  Treinos\n");
		printf("  [4]  Planos\n");
		printf("  [5]  Avaliações físicas\n");
		printf("  [6]  Pacotes de modalidades\n");
		printf("  [7]  Agenda de profissionais\n");
		printf("\n");
		printf("  ESTRUTURA E ATIVIDADES\n");
		printf("  [8]  Equipamentos\n");
		printf("  [9]  Atividades\n");
		printf("\n");
		printf("  [0]  Voltar ao menu principal\n");
		printf("----------------------------------------------\n");
		printf("  Opção: ");
		OpcoesCadastro = 0;
		scanf("%d", &OpcoesCadastro);
		fflush(stdin);

		switch(OpcoesCadastro) {
			case 1:
				// Alunos
				arq = fopen("alunos.bin", "ab+");
				if(arq == NULL) printf("Erro na Abertura do Arquivo\n\n");
				else {
					do {
						do {
							printf("Informe a Matrícula do Aluno: ");
						scanf("%d", &alunos.codigo);
							if(alunos.codigo < 1 || alunos.codigo > 1000) printf("A Matrícula Deve Estar Entre 1 e 1000!\n\n");
						} while(alunos.codigo < 1 || alunos.codigo > 1000);
						
						pos = busca(arq, alunos.codigo, ENT_ALUNO);
						if(pos != -1) printf("Matrícula Já Cadastrada!\n\n");
					} while(pos != -1);
					
					fflush(stdin);
					printf("Informe o Nome do Aluno: ");
					fgets(alunos.nome, 61, stdin);
					if(validaExcedeu(alunos.nome) == 1) printf("Somente os Primeiros 60 Caracteres Serão Cadastrados!\n\n");
					
					do {
						fflush(stdin);
						printf("Informe o CPF do Aluno: ");
						fgets(alunos.cpf, 15, stdin);
						alunos.cpf[15] = '\0';
						
						if(validaCPF(alunos.cpf) == 0) printf("Formato de CPF Inválido! \nInsira este Formato XXX.XXX.XXX-XX \nPreencha no Lugar do X Somente Números!\n\n");
					} while(validaCPF(alunos.cpf) != 1);
				
					do {
						printf("Informe a Data de Nascimento (DD MM AAAA): ");
						scanf("%d%d%d", &alunos.dataNascimento.dia, &alunos.dataNascimento.mes, &alunos.dataNascimento.ano);
								
						if(validaData(alunos.dataNascimento) == 0) printf("Insira Uma Data Valida! \nAno Entre 1900 e 2100 \nMês Entre 1 e 12 \nE os Dias Válidos de Cada Mês\n\n");
					} while(validaData(alunos.dataNascimento) != 1);
					
					do {
						fflush(stdin);
						printf("Informe o Número de Telefone: ");
						fgets(alunos.telefone, 17, stdin);
						alunos.telefone[strlen(alunos.telefone)-1] = '\0';
						
						if(validaTelefone(alunos.telefone) == 0) printf("Formato de Telefone Inválido! \nInsira este Formato (XX) XXXX-XXXX ou (XX) XXXXX-XXXX \nPreencha no Lugar do X Somente Números!\n\n");
					} while(validaTelefone(alunos.telefone) != 1);
					
					do {
						fflush(stdin);
						printf("Informe um E-mail de Contato: ");
						fgets(alunos.email, 61, stdin);
						if(validaExcedeu(alunos.email) == 1) printf("Somente os Primeiros 60 Caracteres Serão Cadastrados!\n\n");
						
						if(validaEmail(alunos.email) == 0) printf("Email Deve conter um @ e um . depois Dele \nAntes do @ só pode Conter - ou _ \nDepois do @ Apenas .\n\n");
					} while(validaEmail(alunos.email) != 1);
					
					alunos.ativo = 1;
					fwrite(&alunos, sizeof(Aluno), 1, arq);
				}
				fclose(arq);
				break;
			case 2:
				// Profissionais
				arq = fopen("profissionais.bin", "ab+");
				if(arq == NULL) printf("Erro na Abertura do Arquivo\n\n");
				else {
					do {
						do {
							printf("Informe a Matrícula do Profissional: ");
							scanf("%d", &profissionais.codigo);
							if(profissionais.codigo < 1 || profissionais.codigo > 1000) printf("A Matrícula Deve Estar Entre 1 e 1000!\n\n");
						} while(profissionais.codigo < 1 || profissionais.codigo > 1000);
						
						pos = busca(arq, profissionais.codigo, ENT_PROFISSIONAL);
						if(pos != -1) printf("Matrícula Já Cadastrada!\n\n");
					} while(pos != -1);
					
					fflush(stdin);
					printf("Informe o Nome do Profissional: ");
					fgets(profissionais.nome, 61, stdin);
					if(validaExcedeu(profissionais.nome) == 1) printf("Somente os Primeiros 60 Caracteres Serão Cadastrados!\n\n");
					
					do {
						fflush(stdin);
						printf("Informe o CPF do Profissional: ");
						fgets(profissionais.cpf, 15, stdin);
						profissionais.cpf[15] = '\0';
						
						if(validaCPF(profissionais.cpf) == 0) printf("Formato de CPF Inválido! \nInsira este Formato XXX.XXX.XXX-XX \nPreencha no Lugar do X Somente Números!\n\n");
					} while(validaCPF(profissionais.cpf) != 1);
					
					do {
						fflush(stdin);
						printf("Informe o Telefone do Profissional: ");
						fgets(profissionais.telefone, 17, stdin);
						profissionais.telefone[strlen(profissionais.telefone)-1] = '\0'; // Substitui \n por \0
						
						if(validaTelefone(profissionais.telefone) == 0) printf("Formato de Telefone Inválido! \nInsira este Formato (XX) XXXX-XXXX ou (XX) XXXXX-XXXX \nPreencha no Lugar do X Somente Números!\n\n");
					} while(validaTelefone(profissionais.telefone) != 1);
					
					fflush(stdin);
					printf("Informe a Função do Profissional: ");
					fgets(profissionais.funcao, 31, stdin);
					if(validaExcedeu(profissionais.funcao) == 1) printf("Somente os Primeiros 30 Caracteres Serão Cadastrados!\n\n");
					
					profissionais.ativo = 1;
					fwrite(&profissionais, sizeof(Profissional), 1, arq);
				}
				fclose(arq);
				break;
			case 3:
				break;
			case 4:
				break;
			case 5:
				if(!temAlunoAtivo()) 
					printf("\nNão Existem Alunos Ativos Cadastrados!\n\n");
				else 
                if(!temProfissionalAtivo()) 
					printf("\nNão Existem Profissionais Ativos Cadastrados!\n\n");
				else 
                {
					arq = fopen("avaliacoes.bin", "ab+");
					if(arq == NULL) printf("Erro na Abertura do Arquivo!\n\n");
					else {
						do {
							do {
								printf("Informe o Código da Avaliação: ");
								scanf("%d", &avaliacoes.codigo);
								if(avaliacoes.codigo < 1 || avaliacoes.codigo > 1000) printf("O Código Deve Estar Entre 1 e 1000!\n\n");
							} while(avaliacoes.codigo < 1 || avaliacoes.codigo > 1000);

							pos = busca(arq, avaliacoes.codigo, ENT_AVALIACAO);
							if(pos != -1) printf("Código Já Cadastrado!\n\n");
						} while(pos != -1);

						do {
							printf("Informe o Código do Aluno: ");
							scanf("%d", &avaliacoes.codAluno);
							ok = existeAlunoAvaliacao(avaliacoes.codAluno);
							if(!ok) printf("Aluno Não Encontrado ou Inativo!\n\n");
						} while(!ok);

						do {
							printf("Informe o Código do Profissional: ");
							scanf("%d", &avaliacoes.codProfissional);
							ok = existeProfissionalAvaliacao(avaliacoes.codProfissional);
							if(!ok) printf("Profissional Não Encontrado ou Inativo!\n\n");
						} while(!ok);

						do {
							printf("Informe a Data da Avaliação (DD MM AAAA): ");
							scanf("%d%d%d", &avaliacoes.data.dia, &avaliacoes.data.mes, &avaliacoes.data.ano);
							if(validaData(avaliacoes.data) == 0) printf("Data Inválida!\n\n");
						} while(validaData(avaliacoes.data) != 1);

						do {
							printf("Informe o Peso (KG): ");
							scanf("%f", &avaliacoes.peso);
							if(avaliacoes.peso < 1 || avaliacoes.peso > 500) printf("O Peso Deve Estar Entre 1 e 500 KG!\n\n");
						} while(avaliacoes.peso < 1 || avaliacoes.peso > 500);

						do {
							printf("Informe a Altura (M): ");
							scanf("%f", &avaliacoes.altura);
							if(avaliacoes.altura < 0.5 || avaliacoes.altura > 2.5) printf("A Altura Deve Estar Entre 0.5 e 2.5 M!\n\n");
						} while(avaliacoes.altura < 0.5 || avaliacoes.altura > 2.5);
						avaliacoes.imc = avaliacoes.peso / (avaliacoes.altura * avaliacoes.altura);
						do {
							fflush(stdin);
							printf("Informe o Percentual de Gordura: ");
							scanf("%f", &avaliacoes.percGordura);
							if(avaliacoes.percGordura < 0 || avaliacoes.percGordura > 100) printf("Percentual Inválido! (0 a 100)\n\n");
						} while(avaliacoes.percGordura < 0 || avaliacoes.percGordura > 100);
						fflush(stdin);
						printf("Observações: ");
						fgets(avaliacoes.observacoes, 101, stdin);
						if(validaExcedeu(avaliacoes.observacoes) == 1) 
							printf("Somente os Primeiros 100 Caracteres Serão Cadastrados!\n\n");
						avaliacoes.ativo = 1;
						fseek(arq, 0, SEEK_END);
						fwrite(&avaliacoes, sizeof(Avaliacao), 1, arq);
						fclose(arq);
						printf("\n==============================================\n");
						printf("     AVALIAÇÃO CADASTRADA COM SUCESSO!\n");
						printf("==============================================\n");
						printf("IMC: %.2f\n\n", avaliacoes.imc);
					}
				}
				break;
			case 6:
				break;
			case 7:
				break;
			case 8:
				break;
			case 9:
				//Atividades
				cadastrarAtividade();
				break;

			case 0:
				printf("\n  Voltando...\n");
				break;

			default:
				printf("\n Opção Inválida!");
				break;
		}
		system("pause");
	} while(OpcoesCadastro != 0);
}

//2 - FUNÇÃO DE GERENCIAR
void Gerenciar(void) {
	int OpcoesGerenciar;

	do {
		system("cls");
		printf("==============================================\n");
		printf("                  GERENCIAR\n");
		printf("==============================================\n");
		printf("  Escolha o cadastro a ser gerenciado\n");
		printf("\n");
		printf("  PESSOAS\n");
		printf("  [1]  Alunos\n");
		printf("  [2]  Profissionais\n");
		printf("\n");
		printf("  REGISTROS\n");
		printf("  [3]  Treinos\n");
		printf("  [4]  Planos\n");
		printf("  [5]  Matrículas\n");
		printf("  [6]  Avaliações físicas\n");
		printf("  [7]  Agenda de profissionais\n");
		printf("  [8]  Equipamentos\n");
		printf("  [9]  Pacotes de modalidades\n");
		printf("  [10] Atividades\n");
		printf("\n");
		printf("  [0]  Voltar ao menu principal\n");
		printf("----------------------------------------------\n");
		printf("  Opção: ");
		OpcoesGerenciar = 0;
		scanf("%d", &OpcoesGerenciar);
		fflush(stdin);

		switch(OpcoesGerenciar)	{
			case 1:
				//Alunos
				altera(ENT_ALUNO);
				break;

		//===========================================================*/

			case 2:
			  	//Profissionais
			  	altera(ENT_PROFISSIONAL);
		   		break;

		//===========================================================*/

			case 3:
			   	//Treinos
			   	altera(ENT_TREINO);
				break;

		//===========================================================*/

			case 4:
			   	//Planos
			   	altera(ENT_PLANO);
				break;

		//===========================================================*/
		    case 5:
		    	//Matrículas
		    	altera(ENT_MATRICULA);
		        break;

		//===========================================================*/

			case 6:
			   	//Avaliações Físicas
			   	altera(ENT_AVALIACAO);
				break;

		//===========================================================*/

			case 7:
			   	//Agenda de Profissionais
			   	altera(ENT_AGENDA);
				break;

		//===========================================================*/

			case 8:
			   	//Equipamentos
			   	altera(ENT_EQUIPAMENTO);
				break;

		//===========================================================*/
			case 9:
			   	//Pacote de Modalidades
			   	altera(ENT_PACOTE);
				break;


		//===========================================================*/
			case 10:
				//Atividades
				altera(ENT_ATIVIDADE);
				break;

		//===========================================================*/

			case 0:
			  	printf("\n Voltando...");
				getch();
			   	break;

		//===========================================================*/

			default:
				printf("\n Opção Inválida!");
				getch();
				break;
		}
	} while(OpcoesGerenciar != 0);
}

//3 - FUNÇÃO LISTAMENTO DE FREQUÊNCIA
void frequencia(void) {
	int Opcoes;

	do {
		system("cls");
		printf("==============================================\n");
		printf("            CONTROLE DE FREQUÊNCIA\n");
		printf("==============================================\n");
		printf("\n");
		printf("  [1]  Listar frequência\n");
		printf("       Consulta as presenças dos alunos\n");
		printf("  [2]  Fazer chamada\n");
		printf("       Registra a presença do dia\n");
		printf("  [3]  Alterar frequência\n");
		printf("       Corrige uma presença registrada\n");
		printf("  [0]  Voltar ao menu principal\n");
		printf("----------------------------------------------\n");
		printf("  Opção: ");
		Opcoes = 0;
		scanf("%d", &Opcoes);
		fflush(stdin);

		switch(Opcoes) {
			case 1:
				break;
			case 2:
				break;
			case 3:
				break;
			case 0:
				printf("\n Voltando...");
				getch();
				break;

			default:
				printf("\n Opção Inválida!");
				getch();
				break;
		}
	} while(Opcoes != 0);
}

//4 - FUNÇÃO DE CONTROLE FINANCEIRO
void financeiro(void) {
	int Opcoes;

	do {
		system("cls");
		printf("==============================================\n");
		printf("             CONTROLE FINANCEIRO\n");
		printf("==============================================\n");
		printf("\n");
		printf("  [1]  Faturamento mensal\n");
		printf("       Total recebido em cada mês\n");
		printf("  [2]  Faturamento por plano\n");
		printf("       Receita separada por plano\n");
		printf("  [3]  Gastos mensais\n");
		printf("       Despesas lançadas no mês\n");
		printf("  [4]  Pagamentos por aluno\n");
		printf("       Histórico de pagamentos do aluno\n");
		printf("  [5]  Cadastrar gastos\n");
		printf("       Lança uma despesa da academia\n");
		printf("  [0]  Voltar ao menu principal\n");
		printf("----------------------------------------------\n");
		printf("  Opção: ");
		Opcoes = 0;
		scanf("%d", &Opcoes);
		fflush(stdin);

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
				//Cadastrar gastos
				break;

			case 0:
				printf("\n Voltando...");
				getch();
				break;

			default:
				printf("\n Opção Inválida!");
				getch();
				break;
		}
	} while(Opcoes != 0);
}

//5 - FUNÇÃO DE OPERAÇÕES
void operacoes(void) {
	int Opcoes;

	do {
		system("cls");
		printf("==============================================\n");
		printf("                  OPERAÇÕES\n");
		printf("==============================================\n");
		printf("\n");
		printf("  [1]  Efetuar matrícula\n");
		printf("       Vincula aluno, plano e pacote\n");
		printf("  [2]  Efetuar recebimento\n");
		printf("       Dá baixa e gera o boleto seguinte\n");
		printf("  [3]  Efetuar cancelamento\n");
		printf("       Calcula a retenção e a devolução\n");
		printf("  [0]  Voltar ao menu principal\n");
		printf("----------------------------------------------\n");
		printf("  Opção: ");
		Opcoes = 0;
		scanf("%d", &Opcoes);
		fflush(stdin);

		switch(Opcoes) {
			case 1:
				break;
			case 2:
				break;
			case 3:
				break;
			case 0:
				printf("\n Voltando...");
				getch();
				break;

			default:
				printf("\n Opção Inválida!");
				getch();
				break;
		}
	} while(Opcoes != 0);
}

//6 - FUNÇÃO DE RELATÓRIOS
void relatorios(void) {
	int Opcoes;

	do {
		system("cls");
		printf("==============================================\n");
		printf("                  RELATÓRIOS\n");
		printf("==============================================\n");
		printf("\n");
		printf("  [1]  Extrato de pagamentos\n");
		printf("       Por período, aluno e situação\n");
		printf("  [2]  Alunos por plano\n");
		printf("       Por plano e situação da matrícula\n");
		printf("  [3]  Alunos inadimplentes\n");
		printf("       Por mês de referência e plano\n");
		printf("  [4]  Matrículas por situação\n");
		printf("       Por situação, período e plano\n");
		printf("  [5]  Faturamento por plano e mês\n");
		printf("       Por ano e plano\n");
		printf("  [6]  Frequência\n");
		printf("       Por aluno, período e atividade\n");
		printf("  [0]  Voltar ao menu principal\n");
		printf("----------------------------------------------\n");
		printf("  Opção: ");
		Opcoes = 0;
		scanf("%d", &Opcoes);
		fflush(stdin);

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
				break;
			case 6:
				break;
			case 0:
				printf("\n Voltando...");
				getch();
				break;

			default:
				printf("\n Opção Inválida!");
				getch();
				break;
		}
	} while(Opcoes != 0);
}

int main() {
	setlocale(LC_ALL, "Portuguese");
	int Opcoes;

	do {
		Opcoes = menu();
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
				//Operações
				operacoes();
				break;

		//===========================================================*/

			case 4:
				//Controle de Frequência
				frequencia();
				break;

		//===========================================================*/

			case 5:
				//Controle Financeiro
				financeiro();
				break;

		//===========================================================*/

			case 6:
				//Relatórios
				relatorios();
				break;

		//===========================================================*/

			case 0:
				printf("\n SISTEMA ENCERRADO! ");
				break;

		//===========================================================*/

			default:
				printf("\n Opção Inválida!\n");
				getch();
				break;
		}
	} while(Opcoes != 0);

	return 0;
}
