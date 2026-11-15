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
	int  quantidade;
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
	float vTotal;
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
void cadastrarPlano(void);
float calcularValorPlano(Plano p);
void alterarPlano(void);
void excluirPlano(void);
void listarPlanos(void);
int  buscarPlano(int codigo);
int  existePlano(int codigo);
int  planoTemMatricula(int codPlano);
int  lerInteiroPlano(int *valor);
int  lerValorPlano(float *valor);
int  escolherTipoPlano(Plano *p);
int  tipoPlanoRepetido(FILE *arq, char nome[], int codIgnorar);
void mostrarPlano(Plano p);
void validarCodigo(int cod);
void cadastrarTreino(void);
void alterarTreino(void);
void excluirTreino(void);
void listarTreinos(void);
void cadastrarEquipamento(void);
void alterarEquipamento(void);
void excluirEquipamento(void);
void listarEquipamentos(void);
void relatorioAlfabetico(void);

// Função de Busca
int busca(FILE *arq, int cod, int codEnt) {
	rewind(arq);
	
	Aluno tempAluno;
	Profissional tempProfissional;
	Avaliacao tempAvaliacao;
	Atividade tempAtividade;
	Plano tempPlano;
	Treino tempTreino;
	Equipamento tempEquipamento;
	
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
		case ENT_PLANO:
			while(fread(&tempPlano, sizeof(Plano), 1, arq) == 1) {
				if(tempPlano.codigo == cod) return (int)(ftell(arq) - sizeof(Plano));
			}
			return -1;
			break;
		case ENT_TREINO:
			while(fread(&tempTreino, sizeof(Treino), 1, arq) == 1) {
				if(tempTreino.codigo == cod) return (int)(ftell(arq) - sizeof(Treino));
			}
			return -1;
			break;
		case ENT_EQUIPAMENTO:
			while(fread(&tempEquipamento, sizeof(Equipamento), 1, arq) == 1) {
				if(tempEquipamento.codigo == cod) return (int)(ftell(arq) - sizeof(Equipamento));
			}
			return -1;
			break;
	}
	return -1;
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

void validarCodigo(int cod) {
	if(cod < 1 || cod > 1000) {
		printf("O Código Deve Estar Entre 1 e 1000!\n\n");
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

// ===================== INÍCIO DO MENU PLANOS (parte da Ana) =====================
int lerInteiroPlano(int *valor) {
	char texto[32];
	int i = 0, digitos = 0, numero = 0, c;

	if(fgets(texto, 32, stdin) == NULL) {
		clearerr(stdin);
		return 0;
	}
	if(strchr(texto, '\n') == NULL) {       // digitou mais do que cabe: joga fora o resto
		while((c = getchar()) != '\n' && c != EOF);
		return 0;
	}
	while(texto[i] == ' ') i++;             // ignora espaços antes
	while(isdigit(texto[i])) {
		numero = numero * 10 + (texto[i] - '0');
		digitos++;
		i++;
		if(digitos > 6) return 0;           // número grande demais
	}
	while(texto[i] == ' ') i++;             // ignora espaços depois
	if(digitos == 0 || texto[i] != '\n') return 0;

	*valor = numero;
	return 1;
}

// Lê um valor em reais (aceita 100, 100.50 ou 100,50). Não usa scanf nem strtod,
// por isso funciona igual com o setlocale("Portuguese") do programa.
// Devolve 1 se leu um valor válido (guardado em *valor) e 0 se não.
int lerValorPlano(float *valor) {
	char texto[32];
	int i = 0, digitos = 0, temSeparador = 0, c;
	double numero = 0, casa = 1;

	if(fgets(texto, 32, stdin) == NULL) {
		clearerr(stdin);
		return 0;
	}
	if(strchr(texto, '\n') == NULL) {
		while((c = getchar()) != '\n' && c != EOF);
		return 0;
	}
	while(texto[i] == ' ') i++;
	while(texto[i] != '\n') {
		if(isdigit(texto[i])) {
			if(temSeparador == 0) numero = numero * 10 + (texto[i] - '0');
			else {
				casa = casa / 10;
				numero = numero + (texto[i] - '0') * casa;
			}
			digitos++;
		} else if((texto[i] == '.' || texto[i] == ',') && temSeparador == 0) {
			temSeparador = 1;
		} else if(texto[i] == ' ') {
			while(texto[i] == ' ') i++;      // só espaços até o Enter
			if(texto[i] != '\n') return 0;
			break;
		} else return 0;                     // letra ou símbolo
		i++;
	}
	if(digitos == 0) return 0;

	*valor = (float)numero;
	return 1;
}

// Mostra o menu de tipos e preenche nome, meses e desconto do plano recebido.
// O desconto NÃO é digitado: sai do tipo (Mensal 0%, Trimestral 5%, Semestral 10%, Anual 15%).
// Devolve 1 se escolheu um tipo válido e 0 se não.
int escolherTipoPlano(Plano *p) {
	int opcao;

	printf("\nTIPOS DE PLANO\n");
	printf("  [1] Mensal      (1 mês,   0%% de desconto)\n");
	printf("  [2] Trimestral  (3 meses, 5%% de desconto)\n");
	printf("  [3] Semestral   (6 meses, 10%% de desconto)\n");
	printf("  [4] Anual       (12 meses, 15%% de desconto)\n");
	printf("Escolha: ");
	if(lerInteiroPlano(&opcao) == 0) {
		printf("Digite apenas números!\n");
		return 0;
	}

	switch(opcao) {
		case 1:
			strcpy(p->nome, "Mensal");
			p->meses = 1;
			p->porcentagemDesconto = 0;
			return 1;
		case 2:
			strcpy(p->nome, "Trimestral");
			p->meses = 3;
			p->porcentagemDesconto = 5;
			return 1;
		case 3:
			strcpy(p->nome, "Semestral");
			p->meses = 6;
			p->porcentagemDesconto = 10;
			return 1;
		case 4:
			strcpy(p->nome, "Anual");
			p->meses = 12;
			p->porcentagemDesconto = 15;
			return 1;
	}
	printf("Opção inválida!\n");
	return 0;
}

// Confere se já existe OUTRO plano com o mesmo tipo (nome). O arquivo chega ABERTO
// (esta função não abre nem fecha). codIgnorar é o código do próprio plano que está
// sendo alterado (no cadastro, passar 0). Depois dela o ponteiro do arquivo fica no
// fim: quem chamou precisa dar fseek antes de gravar.
int tipoPlanoRepetido(FILE *arq, char nome[], int codIgnorar) {
	Plano tempPlano;

	rewind(arq);
	while(fread(&tempPlano, sizeof(Plano), 1, arq) == 1) {
		if(strcmp(tempPlano.nome, nome) == 0 && tempPlano.codigo != codIgnorar) return 1;
	}
	return 0;
}

// Mostra um plano na tela, um campo por linha e uma linha de ===== no final
// (mesmo formato da listagem de Alunos).
void mostrarPlano(Plano p) {
	printf("Código: %d\n", p.codigo);
	printf("Tipo: %s\n", p.nome);
	printf("Meses: %d\n", p.meses);
	printf("Valor mensal: R$ %.2f\n", p.valorMensal);
	printf("Desconto: %.0f%%\n", p.porcentagemDesconto);
	printf("Valor total: R$ %.2f\n", calcularValorPlano(p));
	printf("Ativo: ");
	if(p.ativo == 1) printf("Sim\n");
	else printf("Não\n");
	printf("==============================================\n");
}

// Calcula o valor total do plano já com desconto:
// valor mensal x meses x (1 - desconto/100). Ex.: 100 x 3 x 0,95 = 285,00.
float calcularValorPlano(Plano p) {
	float total;

	total = p.valorMensal * p.meses;
	total = total - (total * p.porcentagemDesconto / 100);

	return total;
}

// Procura o plano pelo código em planos.bin. Se achar, deixa o registro preenchido
// na variável global planos e devolve a posição em bytes. Se não achar (ou se o
// arquivo ainda não existir), devolve -1. Abre e fecha o arquivo sozinha.
// Uso: a Matrícula chama buscarPlano(cod) e depois calcularValorPlano(planos).
int buscarPlano(int codigo) {
	FILE *arq;
	int pos = -1;

	arq = fopen("planos.bin", "rb");
	if(arq != NULL) {
		pos = busca(arq, codigo, ENT_PLANO);
		if(pos != -1) {
			fseek(arq, pos, SEEK_SET);
			fread(&planos, sizeof(Plano), 1, arq);
		}
		fclose(arq);
	}
	return pos;
}

// Devolve 1 se existe plano com esse código e 0 se não existe. (Para Matrícula e
// Pacotes validarem vínculo.) Abre, busca, fecha.
int existePlano(int codigo) {
	FILE *arq;
	int existe = 0;

	arq = fopen("planos.bin", "rb");
	if(arq != NULL) {
		if(busca(arq, codigo, ENT_PLANO) != -1) existe = 1;
		fclose(arq);
	}
	return existe;
}

// Devolve 1 se alguma matrícula usa esse plano (lê matriculas.bin do início ao fim).
// Se matriculas.bin ainda não existe, devolve 0. Usada para bloquear a exclusão.
int planoTemMatricula(int codPlano) {
	FILE *arq;
	Matricula tempMatricula;
	int achou = 0;

	arq = fopen("matriculas.bin", "rb");
	if(arq != NULL) {
		while(achou == 0 && fread(&tempMatricula, sizeof(Matricula), 1, arq) == 1) {
			if(tempMatricula.codPlano == codPlano) achou = 1;
		}
		fclose(arq);
	}
	return achou;
}

// CADASTRAR PLANO: pede código (1 a 1000, sem repetir), tipo (o desconto e os meses
// saem do tipo, sem repetir o tipo) e valor mensal (maior que zero). Grava no fim de
// planos.bin com ativo = 1.
void cadastrarPlano(void) {
	FILE *arq;
	int codigo, pos = -1;

	system("cls");
	printf("==============================================\n");
	printf("  CADASTRAR - Planos\n");
	printf("==============================================\n\n");

	arq = fopen("planos.bin", "ab+");
	if(arq == NULL) printf("Erro na Abertura do Arquivo!\n\n");
	else {
		fflush(stdin);
		memset(&planos, 0, sizeof(Plano));     // limpa tudo: não grava lixo de memória

		do {
			printf("Informe o código do plano (1 a 1000): ");
			if(lerInteiroPlano(&codigo) == 0) {
				printf("Digite apenas números!\n\n");
				codigo = 0;
			} else if(codigo < 1 || codigo > 1000) {
				printf("O Código Deve Estar Entre 1 e 1000!\n\n");
			} else {
				pos = busca(arq, codigo, ENT_PLANO);
				if(pos != -1) printf("Código Já Cadastrado!\n\n");
			}
		} while(codigo < 1 || codigo > 1000 || pos != -1);
		planos.codigo = codigo;

		while(escolherTipoPlano(&planos) == 0);   // pergunta de novo até escolher um tipo válido

		{
			if(tipoPlanoRepetido(arq, planos.nome, 0) == 1) {
				printf("\nEste tipo de plano já está cadastrado!\n");
			} else {
				do {
					printf("\nInforme o valor mensal: R$ ");
					if(lerValorPlano(&planos.valorMensal) == 0) {
						printf("Digite apenas números (ex.: 100,50)!\n");
						planos.valorMensal = 0;
					} else if(planos.valorMensal <= 0 || planos.valorMensal > 100000) {
						printf("O valor deve ser maior que zero e menor que 100000!\n");
						planos.valorMensal = 0;
					}
				} while(planos.valorMensal <= 0);

				planos.ativo = 1;

				fseek(arq, 0, SEEK_END);       // depois de ler, volta ao fim para gravar
				fwrite(&planos, sizeof(Plano), 1, arq);

				printf("\nPlano cadastrado com sucesso!\n");
				printf("Valor total: R$ %.2f\n", calcularValorPlano(planos));
			}
		}
		fclose(arq);
	}
}

// LISTAR PLANOS: lê planos.bin do início ao fim mostrando cada plano.
// Se o arquivo não existe ou está vazio, avisa e volta ao menu.
void listarPlanos(void) {
	FILE *arq;
	int total = 0;

	system("cls");
	printf("==============================================\n");
	printf("  LISTAGEM - Planos\n");
	printf("==============================================\n");

	arq = fopen("planos.bin", "rb");
	if(arq == NULL) printf("Nenhum plano cadastrado!\n\n");
	else {
		while(fread(&planos, sizeof(Plano), 1, arq) == 1) {
			mostrarPlano(planos);
			total++;
		}
		if(total == 0) printf("Nenhum plano cadastrado!\n\n");
		else printf("Total de planos: %d\n", total);
		fclose(arq);
	}
	system("pause");
}

// ALTERAR PLANO: escolhe o que mudar (tipo, valor mensal ou ativo), pede o código,
// acha o registro e regrava NA MESMA POSIÇÃO. O código não pode ser alterado.
// Mudar o tipo muda também meses e desconto (o desconto nunca é digitado).
void alterarPlano(void) {
	FILE *arq;
	Plano novo;
	int opcao, codigo, pos, gravar, valido;

	do {
		system("cls");
		printf("==============================================\n");
		printf("  ALTERAR - Planos\n");
		printf("==============================================\n\n");
		printf("  [1]  Tipo do plano\n");
		printf("  [2]  Valor mensal\n");
		printf("  [3]  Ativo\n");
		printf("  [0]  Voltar\n\n");
		printf("----------------------------------------------\n");
		printf("  Opção: ");
		fflush(stdin);
		if(lerInteiroPlano(&opcao) == 0) opcao = -1;

		if(opcao != 0) {
			if(opcao < 1 || opcao > 3) printf("\nOpção Inválida!\n\n");
			else {
				arq = fopen("planos.bin", "rb+");
				if(arq == NULL) printf("\nNenhum plano cadastrado!\n\n");
				else {
					printf("\nInforme o código do plano: ");
					if(lerInteiroPlano(&codigo) == 0) printf("Digite apenas números!\n\n");
					else {
						pos = busca(arq, codigo, ENT_PLANO);
						if(pos != -1) {
							fseek(arq, pos, SEEK_SET);
							fread(&planos, sizeof(Plano), 1, arq);
							gravar = 0;

							switch(opcao) {
								case 1:
									novo = planos;
									if(escolherTipoPlano(&novo) == 1) {
										if(tipoPlanoRepetido(arq, novo.nome, planos.codigo) == 1) {
											printf("\nEste tipo de plano já está cadastrado!\n");
										} else {
											strcpy(planos.nome, novo.nome);
											planos.meses = novo.meses;
											planos.porcentagemDesconto = novo.porcentagemDesconto;
											gravar = 1;
										}
									}
									break;
								case 2:
									do {
										printf("Informe o Novo Valor Mensal: R$ ");
										valido = lerValorPlano(&planos.valorMensal);
										if(valido == 0) printf("Digite apenas números (ex.: 100,50)!\n\n");
										else if(planos.valorMensal <= 0 || planos.valorMensal > 100000) {
											printf("O valor deve ser maior que zero e menor que 100000!\n\n");
											valido = 0;
										}
									} while(valido == 0);
									gravar = 1;
									break;
								case 3:
									do {
										printf("Informe a Situação (1- Ativo / 0- Inativo): ");
										valido = lerInteiroPlano(&planos.ativo);
										if(valido == 0 || (planos.ativo != 1 && planos.ativo != 0)) {
											printf("Informe Uma Situação Válida!\n\n");
											valido = 0;
										}
									} while(valido == 0);
									gravar = 1;
									break;
							}

							if(gravar == 1) {
								fseek(arq, pos, SEEK_SET);     // volta para o início do registro
								fwrite(&planos, sizeof(Plano), 1, arq);
								printf("\nAlteração Realizada com Sucesso!\n");
							}
						} else printf("\nPlano Não Encontrado!\n\n");
					}
					fclose(arq);
				}
			}
			system("pause");
		}
	} while(opcao != 0);
}

// EXCLUIR PLANO (exclusão física, como na Base): mostra o plano, bloqueia se existe
// matrícula com ele, pede S/N e só depois mexe nos arquivos: copia todos os planos,
// menos o excluído, para auxiliar.bin; apaga planos.bin e renomeia o auxiliar.
void excluirPlano(void) {
	FILE *arq, *temp;
	Plano tempPlano;
	int codigo, pos;

	system("cls");
	printf("==============================================\n");
	printf("  EXCLUIR - Planos\n");
	printf("==============================================\n\n");

	arq = fopen("planos.bin", "rb");
	if(arq == NULL) printf("Nenhum plano cadastrado!\n\n");
	else {
		fflush(stdin);
		printf("Informe o código do plano: ");
		if(lerInteiroPlano(&codigo) == 0) printf("Digite apenas números!\n\n");
		else {
			pos = busca(arq, codigo, ENT_PLANO);
			if(pos != -1) {
				fseek(arq, pos, SEEK_SET);
				fread(&planos, sizeof(Plano), 1, arq);
				printf("==============================================\n");
				mostrarPlano(planos);

				if(planoTemMatricula(codigo) == 1) {
					printf("Não é possível excluir: existe matrícula com este plano!\n");
				} else {
					printf("Deseja excluir? S/N: ");
					if(toupper(getche()) == 'S') {
						temp = fopen("auxiliar.bin", "wb");
						if(temp == NULL) printf("\nErro ao criar o arquivo auxiliar!\n");
						else {
							rewind(arq);
							while(fread(&tempPlano, sizeof(Plano), 1, arq) == 1) {
								if(tempPlano.codigo != codigo) fwrite(&tempPlano, sizeof(Plano), 1, temp);
							}
							fclose(temp);
							fclose(arq);
							arq = NULL;                    // já fechado: não fechar de novo
							remove("planos.bin");
							rename("auxiliar.bin", "planos.bin");
							printf("\nO Plano Foi Excluído com Sucesso!\n");
						}
					} else printf("\nExclusão Cancelada!\n");
				}
			} else printf("\nPlano Não Encontrado!\n");
		}
		if(arq != NULL) fclose(arq);
	}
	system("pause");
}
// ===================== FIM DO MENU PLANOS =====================

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

// ===== MENU TREINOS E EQUIPAMENTOS (parte do Igor) =====
void alterarTreino() {
	FILE *arq;
	FILE *arqAlunos;
	FILE *arqProfissionais;
	
	int i, opcao, pos, posProfissional;
	int numExerc, opExerc, indice, duplicado, exibir;

	do {
		system("cls");
		printf("==============================================\n");
		printf("  ALTERAR - Treinos\n");
		printf("==============================================\n\n");
		printf("  [1]  Código do Profissional\n");
		printf("  [2]  Nome do Treino\n");
		printf("  [3]  Exercícios\n");
		printf("  [4]  Objetivos\n");
		printf("  [0]  Voltar\n\n");
		printf("----------------------------------------------\n");
		printf("  Opção: ");
		scanf("%d", &opcao);
	
		if(opcao !=0) {
			arq = fopen("treinos.bin", "rb+");
		
			if(arq == NULL) {
				printf("Erro na Abertura do Arquivo!\n\n");
				system("pause");
				system("cls");
				return;
			} else {
				system("cls");	
				switch(opcao) {
					case 1:
						// verifica se o codigo do treino existe.
						do {	
							printf("Informe o Código do Treino: ");
							scanf("%d", &treinos.codigo);
							validarCodigo(treinos.codigo);
								
						} while(treinos.codigo < 1 || treinos.codigo > 1000);
							
						pos = busca(arq, treinos.codigo, ENT_TREINO);
						if(pos == -1) {
							printf("[ERRO] Código não encontrado!\n\n");
							fclose(arq);
							
							system("pause");
							system("cls");
							return;
						} 
						
						fseek(arq, pos, SEEK_SET);
						fread(&treinos, sizeof(Treino), 1, arq);
							
						arqProfissionais = fopen("profissionais.bin", "rb");
						if(arqProfissionais == NULL) {
							printf("Erro na Abertura do Arquivo!\n\n");
							
							fclose(arq);
							system("pause");
							system("cls");
							return;
						}
						
						// exibe as informações do profissional.
						posProfissional = busca(arqProfissionais, treinos.codProfissional, ENT_PROFISSIONAL);
						if(posProfissional != -1) {
							fseek(arqProfissionais, posProfissional, SEEK_SET);
							fread(&profissionais, sizeof(Profissional), 1, arqProfissionais);
							printf("  Profissional Atual: %d | %s\n\n", profissionais.codigo, profissionais.nome);
						}
						
						do {
							printf("Novo Código do Profissional: ");
							scanf("%d", &treinos.codProfissional);
							validarCodigo(treinos.codProfissional);
							
						} while(treinos.codProfissional < 1 || treinos.codProfissional > 1000);
					
						// verifica se o codigo do profissional existe.
						posProfissional = busca(arqProfissionais, treinos.codProfissional, ENT_PROFISSIONAL);
						if(posProfissional == -1) {
							printf("[ERRO] Profissional não Encontrado!\n\n");
							fclose(arq);
							fclose(arqProfissionais);
							system("pause");
							system("cls");
							return;
						}
						
						fseek(arq, pos, SEEK_SET);
						fwrite(&treinos, sizeof(Treino), 1, arq);
						printf("\n[SUCESSO] Alteração Realizada com Sucesso!\n");
					
						fclose(arqProfissionais);
						system("pause");
						system("cls");	
					break;
					case 2:
						// verifica se o codigo do treino existe.
						do {	
							printf("Informe o Código do Treino: ");
							scanf("%d", &treinos.codigo);
							validarCodigo(treinos.codigo);
								
						} while(treinos.codigo < 1 || treinos.codigo > 1000);
							
						pos = busca(arq, treinos.codigo, ENT_TREINO);
						if(pos == -1) {
							printf("[ERRO] Código não encontrado!\n\n");
							
							fclose(arq);
							system("pause");
							system("cls");
							return;
						} 
						
						// lê as informações especificas do respectivo treino.
						fseek(arq, pos, SEEK_SET);
						fread(&treinos, sizeof(Treino), 1, arq);
						
						printf("  Nome do Treino: %s\n", treinos.nome);
				
						getchar();
						printf("Informe o Novo Nome do Treino: ");
						fgets(treinos.nome, 31, stdin);
						
						fseek(arq, pos, SEEK_SET);
						fwrite(&treinos, sizeof(Treino), 1, arq);
						printf("\n[SUCESSO] Alteração Realizada com Sucesso!\n");
					
						system("pause");
						system("cls");	
					break;
					case 3:
						system("cls");
						printf("==============================================\n");
						printf("  ALTERAR - Treinos\n");
						printf("==============================================\n\n");
						printf("  [1]  Alterar Todos os Exercícios\n");
						printf("  [2]  Alterar Exercício Específico\n");
						printf("  [3]  Adicionar Exercício\n");
						printf("  [4]  Remover Exercício\n");
						printf("  [0]  Voltar\n\n");
						printf("----------------------------------------------\n");
						printf("  Opção: ");
						scanf("%d", &opExerc);
						
						switch(opExerc) {
							case 1:
								// verifica se o codigo do treino existe.
								system("cls");
								do {	
									printf("Informe o Código do Treino: ");
									scanf("%d", &treinos.codigo);
									getchar();
									validarCodigo(treinos.codigo);
										
								} while(treinos.codigo < 1 || treinos.codigo > 1000);
									
								pos = busca(arq, treinos.codigo, ENT_TREINO);
								if(pos == -1) {
									printf("[ERRO] Código não encontrado!\n\n");
									
									fclose(arq);
									system("pause");
									system("cls");
									return;
								} 
								
								// lê as informações especificas do respectivo treino.
								fseek(arq, pos, SEEK_SET);
								fread(&treinos, sizeof(Treino), 1, arq);
								
								printf("\nExercícios:\n");
								for(i=0; i<treinos.quantidade; i++) {
									printf("  %d) Exercício: %s", i+1, treinos.exercicios[i]);
								}
								
								printf("\nNovos Exercícios:\n");
								for(i=0; i<treinos.quantidade; i++) {
									printf("  %d) Exercício: ", i+1);
								    fgets(treinos.exercicios[i], 41, stdin);
								}
								
								fseek(arq, pos, SEEK_SET);
								fwrite(&treinos, sizeof(Treino), 1, arq);
								printf("\n[SUCESSO] Alteração Realizada com Sucesso!\n");
							
								system("pause");
								system("cls");	
							break;
							case 2:
								// verifica se o codigo do treino existe.
								system("cls");
								do {	
									printf("Informe o Código do Treino: ");
									scanf("%d", &treinos.codigo);
									getchar();
									validarCodigo(treinos.codigo);
										
								} while(treinos.codigo < 1 || treinos.codigo > 1000);
									
								pos = busca(arq, treinos.codigo, ENT_TREINO);
								if(pos == -1) {
									printf("[ERRO] Código não encontrado!\n\n");
									
									fclose(arq);
									system("pause");
									system("cls");
									return;
								} 
								
								// lê as informações especificas do respectivo treino.
								fseek(arq, pos, SEEK_SET);
								fread(&treinos, sizeof(Treino), 1, arq);
								
								printf("\nExercícios:\n");
								for(i=0; i<treinos.quantidade; i++) {
									printf("  %d) Exercício: %s", i+1, treinos.exercicios[i]);
								}
								
								printf("\nInforme o Número do Exercício a Alterar: ");
								scanf("%d", &numExerc);
								getchar();
								
								if(numExerc>=1 && numExerc<=treinos.quantidade) {
									indice = numExerc - 1;
									printf("  %d) Exercício: %s", indice+1, treinos.exercicios[indice]);
											
									printf("\nNovo Exercício: ");
									fgets(treinos.exercicios[indice], 41, stdin);
									
									fseek(arq, pos, SEEK_SET);
									fwrite(&treinos, sizeof(Treino), 1, arq);
									printf("\n[SUCESSO] Alteração Realizada com Sucesso!\n");
								
									system("pause");
									system("cls");	
								} else {
									printf("\n[AVISO] Número Inválido!\n\n");
									
									fclose(arq);
									system("pause");
									system("cls");
									return;
								}
								
							break;
							case 3:
								// verifica se o codigo do treino existe.
								system("cls");
								
								do {	
									printf("Informe o Código do Treino: ");
									scanf("%d", &treinos.codigo);
									getchar();
									validarCodigo(treinos.codigo);
										
								} while(treinos.codigo < 1 || treinos.codigo > 1000);
									
								pos = busca(arq, treinos.codigo, ENT_TREINO);
								if(pos == -1) {
									printf("[ERRO] Código não encontrado!\n\n");
									
									fclose(arq);
									system("pause");
									system("cls");
									return;
								} 
								
								// lê as informações especificas do respectivo treino.
								fseek(arq, pos, SEEK_SET);
								fread(&treinos, sizeof(Treino), 1, arq);
								
								if(treinos.quantidade >= 10) {
									printf("[AVISO] Quantidade Máxima de Exercícios Atingida!\n\n");
									fclose(arq); 
									
									system("pause");
									system("cls");
									return;
								} 
								
								printf("\nExercícios:\n");
								for(i=0; i<treinos.quantidade; i++) {
									printf("  %d) Exercício: %s", i+1, treinos.exercicios[i]);
								}
								
								do {
									if(exibir == 1) {
										printf("Informe o Código do Treino: %d\n", treinos.codigo);
									
										printf("\nExercícios:\n");
										for(i=0; i<treinos.quantidade; i++) {
											printf("  %d) Exercício: %s", i+1, treinos.exercicios[i]);
										}
									}
									
									duplicado = 0;
									exibir = 0;
									
									printf("\nNovo Exercício: ");
									fgets(treinos.exercicios[treinos.quantidade], 41, stdin);
								
									for(i=0; i<treinos.quantidade; i++) {
										if(stricmp(treinos.exercicios[treinos.quantidade], treinos.exercicios[i]) == 0) {
											printf("\n[AVISO] Exercício já cadastrado!\n\n");
											duplicado = 1;
											exibir = 1;
											
											system("pause");
											system("cls");
										}
									}
								} while(duplicado == 1);
								treinos.quantidade++;
								
								fseek(arq, pos, SEEK_SET);
								fwrite(&treinos, sizeof(Treino), 1, arq);
								printf("\n[SUCESSO] Alteração Realizada com Sucesso!\n");
							
								system("pause");
								system("cls");
							break;
							case 4:
								// verifica se o codigo do treino existe.
								system("cls");
								do {	
									printf("Informe o Código do Treino: ");
									scanf("%d", &treinos.codigo);
									getchar();
									validarCodigo(treinos.codigo);
										
								} while(treinos.codigo < 1 || treinos.codigo > 1000);
									
								pos = busca(arq, treinos.codigo, ENT_TREINO);
								if(pos == -1) {
									printf("[ERRO] Código não encontrado!\n\n");
									
									fclose(arq);
									system("pause");
									system("cls");
									return;
								} 
								
								// lê as informações especificas do respectivo treino.
								fseek(arq, pos, SEEK_SET);
								fread(&treinos, sizeof(Treino), 1, arq);
								
								if(treinos.quantidade == 1) {
									printf("[AVISO] O Treino deve Possuir pelo menos 1 Exercício Cadastrado!\n\n");
									fclose(arq); 
									
									system("pause");
									system("cls");
									return;
								} 
								
								printf("\nExercícios:\n");
								for(i=0; i<treinos.quantidade; i++) {
									printf("  %d) Exercício: %s", i+1, treinos.exercicios[i]);
								}
								
								printf("\nInforme o Número do Exercício a Remover: ");
								scanf("%d", &numExerc);
								getchar();
									
								if(numExerc>=1 && numExerc<=treinos.quantidade) {
									indice = numExerc - 1;
									printf("  %d) Exercício: %s", indice+1, treinos.exercicios[indice]);
												
									printf("\nDeseja Remover o Exercício? (S/N): ");
									if(toupper(getche()) == 'S') {
										for(i=indice; i<=treinos.quantidade-2; i++) {
											strcpy(treinos.exercicios[i], treinos.exercicios[i+1]);
										}
										treinos.quantidade --;
											
										fseek(arq, pos, SEEK_SET);
										fwrite(&treinos, sizeof(Treino), 1, arq);
										printf("\n\n[SUCESSO] Alteração Realizada com Sucesso!\n");
										
										fclose(arq);
										system("pause");
										system("cls");
										return;	
									} else {
										printf("\n[AVISO] Exclusão Cancelada!\n");
										
										fclose(arq);
										system("pause");
										system("cls");
										return;
									}
									
								} else {
									printf("\n[AVISO] Número Inválido!\n\n");
									
									fclose(arq);
									system("pause");
									system("cls");
									return;
								}
							break;
						}
					break;
					case 4:
						// verifica se o codigo do treino existe.
						do {
							do {	
							printf("Informe o Código do Treino: ");
							scanf("%d", &treinos.codigo);
							validarCodigo(treinos.codigo);
								
							} while(treinos.codigo < 1 || treinos.codigo > 1000);
								
							pos = busca(arq, treinos.codigo, ENT_TREINO);
							if(pos == -1) {
								printf("[ERRO] Código não encontrado!\n\n");
								
								fclose(arq);
								system("pause");
								system("cls");
							} 
						} while(pos == -1);
						
						// lê as informações especificas do respectivo treino.
						fseek(arq, pos, SEEK_SET);
						fread(&treinos, sizeof(Treino), 1, arq);
						
						printf("  Objetivo: %s\n", treinos.objetivo);
				
						getchar();
						printf("Informe o Novo Objetivo: ");
						fgets(treinos.objetivo, 61, stdin);
						
						fseek(arq, pos, SEEK_SET);
						fwrite(&treinos, sizeof(Treino), 1, arq);
						printf("\n[SUCESSO] Alteração Realizada com Sucesso!\n");
					
						system("pause");
						system("cls");	
					break;
					default:
						printf("\n Opção Inválida!");
					break;
				}
				fclose(arq);
			}	
		}
	} while(opcao !=0);	
}

// Função Alterar Equipamento
void alterarEquipamento() {
	FILE *arq;
	Equipamento equipamentos;
	
	int pos, opcao;
	
	do {
		system("cls");
		printf("==============================================\n");
		printf("  ALTERAR - Equipamentos\n");
		printf("==============================================\n\n");
		printf("  [1]  Nome do Equipamento\n"); // Ok
		printf("  [2]  Tipo do Equipamento\n"); // Ok
		printf("  [3]  Data da Aquisição\n"); // Ok
		printf("  [4]  Data da Última Manutenção\n"); // Ok
		printf("  [5]  Situação\n"); // Ok
		printf("  [6]  Valor\n");
		printf("  [7]  Quantidade\n");
		printf("  [0]  Voltar\n\n");
		printf("----------------------------------------------\n");
		printf("  Opção: ");
		scanf("%d", &opcao);
		
		if(opcao !=0) {
			arq = fopen("equipamentos.bin", "rb+");
		
			if(arq == NULL) {
				printf("Erro na Abertura do Arquivo!\n\n");
				system("pause");
				system("cls");
				return;
			} else {
				system("cls");	
				switch(opcao) {
					
					case 1:
						// verifica se o codigo do treino existe.
						do {	
							printf("Informe o Código do Equipamento: ");
							scanf("%d", &equipamentos.codigo);
							validarCodigo(equipamentos.codigo);
								
						} while(equipamentos.codigo < 1 || equipamentos.codigo > 1000);
							
						pos = busca(arq, equipamentos.codigo, ENT_EQUIPAMENTO);
						if(pos == -1) {
							printf("[ERRO] Código não encontrado!\n\n");
							
							fclose(arq);
							system("pause");
							system("cls");
							return;
						} 
						
						// lê as informações especificas do respectivo treino.
						fseek(arq, pos, SEEK_SET);
						fread(&equipamentos, sizeof(Equipamento), 1, arq);
						
						printf("  Nome do Equipamento: %s\n", equipamentos.nome);
				
						getchar();
						printf("Informe o Novo Nome do Equipamento: ");
						fgets(equipamentos.nome, 31, stdin);
						
						fseek(arq, pos, SEEK_SET);
						fwrite(&equipamentos, sizeof(Equipamento), 1, arq);
						printf("\n[SUCESSO] Alteração Realizada com Sucesso!\n");
					
						system("pause");
						system("cls");	
					break;
					case 2:
						// verifica se o codigo do treino existe.
						do {	
							printf("Informe o Código do Equipamento: ");
							scanf("%d", &equipamentos.codigo);
							validarCodigo(equipamentos.codigo);
								
						} while(equipamentos.codigo < 1 || equipamentos.codigo > 1000);
							
						pos = busca(arq, equipamentos.codigo, ENT_EQUIPAMENTO);
						if(pos == -1) {
							printf("[ERRO] Código não encontrado!\n\n");
							
							fclose(arq);
							system("pause");
							system("cls");
							return;
						} 
						
						// lê as informações especificas do respectivo treino.
						fseek(arq, pos, SEEK_SET);
						fread(&equipamentos, sizeof(Equipamento), 1, arq);
						
						printf("  Tipo do Equipamento: %s\n", equipamentos.tipo);
				
						getchar();
						printf("Informe o Novo Tipo do Equipamento: ");
						fgets(equipamentos.tipo, 31, stdin);
						
						fseek(arq, pos, SEEK_SET);
						fwrite(&equipamentos, sizeof(Equipamento), 1, arq);
						printf("\n[SUCESSO] Alteração Realizada com Sucesso!\n");
					
						system("pause");
						system("cls");
					break;
					case 3:
						// verifica se o codigo do treino existe.
						do {	
							printf("Informe o Código do Equipamento: ");
							scanf("%d", &equipamentos.codigo);
							validarCodigo(equipamentos.codigo);
								
						} while(equipamentos.codigo < 1 || equipamentos.codigo > 1000);
							
						pos = busca(arq, equipamentos.codigo, ENT_EQUIPAMENTO);
						if(pos == -1) {
							printf("[ERRO] Código não encontrado!\n\n");
							
							fclose(arq);
							system("pause");
							system("cls");
							return;
						} 
						
						// lê as informações especificas do respectivo treino.
						fseek(arq, pos, SEEK_SET);
						fread(&equipamentos, sizeof(Equipamento), 1, arq);
						
						printf("  Data de Aquisição: %d/%d/%d\n\n", equipamentos.dataAquisicao.dia, equipamentos.dataAquisicao.mes, equipamentos.dataAquisicao.ano);
				
						printf("Informe a Nova Data de Aquisição (DD MM AAAA): ");
						scanf("%d%d%d", &equipamentos.dataAquisicao.dia, &equipamentos.dataAquisicao.mes, &equipamentos.dataAquisicao.ano);
						
						fseek(arq, pos, SEEK_SET);
						fwrite(&equipamentos, sizeof(Equipamento), 1, arq);
						printf("\n[SUCESSO] Alteração Realizada com Sucesso!\n");
					
						system("pause");
						system("cls");
					break;
					case 4:
						// verifica se o codigo do treino existe.
						do {	
							printf("Informe o Código do Equipamento: ");
							scanf("%d", &equipamentos.codigo);
							validarCodigo(equipamentos.codigo);
								
						} while(equipamentos.codigo < 1 || equipamentos.codigo > 1000);
							
						pos = busca(arq, equipamentos.codigo, ENT_EQUIPAMENTO);
						if(pos == -1) {
							printf("[ERRO] Código não encontrado!\n\n");
							
							fclose(arq);
							system("pause");
							system("cls");
							return;
						} 
						
						// lê as informações especificas do respectivo treino.
						fseek(arq, pos, SEEK_SET);
						fread(&equipamentos, sizeof(Equipamento), 1, arq);
						
						printf("  Data da Última Manutenção: %d/%d/%d\n\n", equipamentos.dataUltimaManutencao.dia, equipamentos.dataUltimaManutencao.mes, equipamentos.dataUltimaManutencao.ano);
				
						printf("Informe a Nova Data de Manutenção (DD MM AAAA): ");
						scanf("%d%d%d", &equipamentos.dataUltimaManutencao.dia, &equipamentos.dataUltimaManutencao.mes, &equipamentos.dataUltimaManutencao.ano);
						
						fseek(arq, pos, SEEK_SET);
						fwrite(&equipamentos, sizeof(Equipamento), 1, arq);
						printf("\n[SUCESSO] Alteração Realizada com Sucesso!\n");
					
						system("pause");
						system("cls");
					break;
					case 5:
						// verifica se o codigo do treino existe.
						do {	
							printf("Informe o Código do Equipamento: ");
							scanf("%d", &equipamentos.codigo);
							validarCodigo(equipamentos.codigo);
								
						} while(equipamentos.codigo < 1 || equipamentos.codigo > 1000);
							
						pos = busca(arq, equipamentos.codigo, ENT_EQUIPAMENTO);
						if(pos == -1) {
							printf("[ERRO] Código não encontrado!\n\n");
							
							fclose(arq);
							system("pause");
							system("cls");
							return;
						} 
						
						// lê as informações especificas do respectivo treino.
						fseek(arq, pos, SEEK_SET);
						fread(&equipamentos, sizeof(Equipamento), 1, arq);
						
						printf("\nSituação: ");
						printf("\n  Opção: %d", equipamentos.situacao);
				
						printf("\n\nNova Situação: 1 = Bom | 2 = Manutenção | 3 = Inativo");
						printf("\n  Opção: ");
						scanf("%d", &equipamentos.situacao);
						
						fseek(arq, pos, SEEK_SET);
						fwrite(&equipamentos, sizeof(Equipamento), 1, arq);
						printf("\n[SUCESSO] Alteração Realizada com Sucesso!\n");
					
						system("pause");
						system("cls");
					break;
					case 6:
						// verifica se o codigo do treino existe.
						do {	
							printf("Informe o Código do Equipamento: ");
							scanf("%d", &equipamentos.codigo);
							validarCodigo(equipamentos.codigo);
								
						} while(equipamentos.codigo < 1 || equipamentos.codigo > 1000);
							
						pos = busca(arq, equipamentos.codigo, ENT_EQUIPAMENTO);
						if(pos == -1) {
							printf("[ERRO] Código não encontrado!\n\n");
							
							fclose(arq);
							system("pause");
							system("cls");
							return;
						} 
						
						// lê as informações especificas do respectivo treino.
						fseek(arq, pos, SEEK_SET);
						fread(&equipamentos, sizeof(Equipamento), 1, arq);
						
						printf("  Valor: %.2f\n\n", equipamentos.valor);
				
				        equipamentos.vTotal = equipamentos.valor * equipamentos.quantidade;
				
						printf("Informe o Novo Valor: ");
						scanf("%f", &equipamentos.valor);
						
						fseek(arq, pos, SEEK_SET);
						fwrite(&equipamentos, sizeof(Equipamento), 1, arq);
						printf("\n[SUCESSO] Alteração Realizada com Sucesso!\n");
					
						system("pause");
						system("cls");
					break;
					case 7:
						// verifica se o codigo do treino existe.
						do {	
							printf("Informe o Código do Equipamento: ");
							scanf("%d", &equipamentos.codigo);
							validarCodigo(equipamentos.codigo);
								
						} while(equipamentos.codigo < 1 || equipamentos.codigo > 1000);
							
						pos = busca(arq, equipamentos.codigo, ENT_EQUIPAMENTO);
						if(pos == -1) {
							printf("[ERRO] Código não encontrado!\n\n");
							
							fclose(arq);
							system("pause");
							system("cls");
							return;
						} 
						
						// lê as informações especificas do respectivo treino.
						fseek(arq, pos, SEEK_SET);
						fread(&equipamentos, sizeof(Equipamento), 1, arq);
						
						printf("  Quantidade: %d\n\n", equipamentos.quantidade);
				
						printf("Informe a Nova Quantidade: ");
						scanf("%d", &equipamentos.quantidade);
						
						equipamentos.vTotal = equipamentos.valor * equipamentos.quantidade;
						
						fseek(arq, pos, SEEK_SET);
						fwrite(&equipamentos, sizeof(Equipamento), 1, arq);
						printf("\n[SUCESSO] Alteração Realizada com Sucesso!\n");
					
						system("pause");
						system("cls");
					break;
					default:
						printf("\n Opção Inválida!");
					break;
				}
				
				fclose(arq);
			}	
		}
	} while(opcao !=0);	
}

// Funções de Exclusão
void excluirTreino() {
	FILE *arq, *temp;
	FILE *arqProfissional, *arqAlunos;
	
	int i, pos, codigo=0;
	
	system("cls");
	printf("==============================================\n");
	printf("  EXCLUIR - Treinos\n");
	printf("==============================================\n\n");
	
	arq = fopen("treinos.bin", "rb");
	if(arq == NULL) {
		printf("Erro na Abertura do Arquivo!\n\n");
	} else {
		// verifica se o codigo do treino existe.
		do {	
			printf("Informe o Código do Treino: ");
			scanf("%d", &treinos.codigo);
		    validarCodigo(treinos.codigo);
						
		} while(treinos.codigo < 1 || treinos.codigo > 1000);
							
		pos = busca(arq, treinos.codigo, ENT_TREINO);
		if(pos == -1) {
			printf("[ERRO] Código não encontrado!\n\n");
							
			fclose(arq);
			system("pause");
			system("cls");
			return;
		} 
		
		// lê as informações especificas do respectivo treino.
		fseek(arq, pos, SEEK_SET);
		fread(&treinos, sizeof(Treino), 1, arq);
		
		codigo = treinos.codigo;
		
		printf("\n==============================================\n");
		printf("Código do Treino: %d\n\n", treinos.codigo);
		
		arqAlunos = fopen("alunos.bin", "rb");
		printf("Código do Aluno: %d\n", treinos.codAluno);
		if(arqAlunos == NULL) {
			printf("Erro na Abertura do Arquivo!\n\n");
		} else {
			rewind(arqAlunos);
			while(fread (&alunos, sizeof(Aluno), 1, arqAlunos) == 1) {
				if(alunos.codigo == treinos.codAluno) {
				    printf("  Nome do Aluno: %s\n\n", alunos.nome);
				}
			}
		}
		
		arqProfissional = fopen("profissionais.bin", "rb");	
		printf("Código do Profissional: %d\n", treinos.codProfissional);
		if(arqProfissional == NULL) {
			printf("Erro na Abertura do Arquivo!\n\n");
		} else {
			rewind(arqProfissional);
			while(fread (&profissionais, sizeof(Profissional), 1, arqProfissional) == 1) {
				if(profissionais.codigo == treinos.codProfissional) {
					printf("  Nome do Profissional: %s\n\n", profissionais.nome);
				}
			}
		}
				
		printf("Nome do Treino: %s", treinos.nome);
		printf("Objetivo do Treino: %s\n", treinos.objetivo);
				
		printf("Exercícios:\n");
		for(i=0; i<treinos.quantidade; i++) {
			printf("  %d) Exercício: %s", i+1, treinos.exercicios[i]);
		}
					
		printf("\nCriado em: %d/%d/%d\n", treinos.dataCriacao.dia, treinos.dataCriacao.mes, treinos.dataCriacao.ano);
		printf("Ativo: ");
		if(treinos.ativo == 1) {
			printf("Sim\n");
		} else {
			printf("Não\n");
		}
		printf("==============================================\n");
		
		printf("\nDeseja Excluir o Treino? (S/N): ");
		if(toupper(getche()) == 'S') {
			temp = fopen("auxiliar.bin", "wb");
			if(temp == NULL) {
				printf("Erro na Abertura do Arquivo!\n\n");
			} else {
				rewind(arq);
			
				while(fread(&treinos, sizeof(Treino), 1, arq) == 1) {
					if(treinos.codigo != codigo) {
						fwrite(&treinos, sizeof(Treino), 1, temp);
					}
				}
				fclose(arq);
				fclose(temp);
				if(arqAlunos != NULL) fclose(arqAlunos);
				if(arqProfissional != NULL) fclose(arqProfissional);
				remove("treinos.bin");
				rename("auxiliar.bin", "treinos.bin");
				printf("\n\n[SUCESSO]Treino Excluído com Sucesso!\n");
			} 						
		} else {
			printf("\n\n[AVISO] Exclusão Cancelada!\n");
				
			if(arqAlunos != NULL) fclose(arqAlunos);
			if(arqProfissional != NULL) fclose(arqProfissional);					
			fclose(arq);
			system("pause");
			system("cls");
			return;
		}
		system("pause");
	}
}

void excluirEquipamento() {
	FILE *arq, *temp;
	Equipamento equipamentos;
	
	int pos, codigo;
	
	system("cls");
	printf("==============================================\n");
	printf("  EXCLUIR - Equipamentos\n");
	printf("==============================================\n\n");
	
	arq = fopen("equipamentos.bin", "rb");
	if(arq == NULL) {
		printf("Erro na Abertura do Arquivo!\n\n");
	} else {
		// verifica se o codigo do treino existe.
		do {	
			printf("Informe o Código do Equipamento: ");
			scanf("%d", &equipamentos.codigo);
		    validarCodigo(equipamentos.codigo);
						
		} while(equipamentos.codigo < 1 || equipamentos.codigo > 1000);
							
		pos = busca(arq, equipamentos.codigo, ENT_EQUIPAMENTO);
		if(pos == -1) {
			printf("[ERRO] Código não encontrado!\n\n");
							
			fclose(arq);
			system("pause");
			system("cls");
			return;
		} 
		codigo = equipamentos.codigo;
		
		// lê as informações especificas do respectivo treino.
		fseek(arq, pos, SEEK_SET);
		fread(&equipamentos, sizeof(Equipamento), 1, arq);
		
		printf("\n==============================================\n");
		printf("Código do Equipamento: %d\n\n", equipamentos.codigo);
				
		printf("Nome do Equipamento: %s", equipamentos.nome);
		printf("Tipo do Equipamento: %s", equipamentos.tipo);
		printf("Data de Aquisição: %d/%d/%d\n\n", equipamentos.dataAquisicao.dia, equipamentos.dataAquisicao.mes, equipamentos.dataAquisicao.ano);
				
		printf("Quantidade: %d\n", equipamentos.quantidade);
		printf("Valor do Equipamento: %.2f\n\n", equipamentos.valor);
				
		printf("Situação: \n");
		printf("  Opção: ");
		if(equipamentos.situacao == 1) {
			printf("Bom\n\n");
		} else if(equipamentos.situacao == 2) {
			printf("Manutenção\n\n");
		} else if(equipamentos.situacao == 3) {
			printf("Inativo\n\n");
		}
		
		printf("Data da Última Manutenção: %d/%d/%d\n", equipamentos.dataUltimaManutencao.dia, equipamentos.dataUltimaManutencao.mes, equipamentos.dataUltimaManutencao.ano);
				
		printf("Ativo: ");
		if(equipamentos.ativo == 1) {
			printf("Sim\n");
		} else {
			printf("Não\n");
		}
		printf("\n==============================================\n");
		
		printf("\nDeseja Excluir o Equipamento? (S/N): ");
		if(toupper(getche()) == 'S') {
			temp = fopen("auxiliar.bin", "wb");
			if(temp == NULL) {
				printf("Erro na Abertura do Arquivo!\n\n");
			} else {
				rewind(arq);
			
				while(fread(&equipamentos, sizeof(Equipamento), 1, arq) == 1) {
					if(equipamentos.codigo != codigo) {
						fwrite(&equipamentos, sizeof(Equipamento), 1, temp);
					}
				}
				fclose(arq);
				fclose(temp);
	
				remove("equipamentos.bin");
				rename("auxiliar.bin", "equipamentos.bin");
				printf("\n\n[SUCESSO] Equipamento Excluído com Sucesso!\n");
			} 						
		} else {
			printf("\n\n[AVISO] Exclusão Cancelada!\n");
									
			fclose(arq);
			system("pause");
			system("cls");
			return;
		}
		system("pause");
	}
}

// Funções de Listagem
void listarTreinos() {
	FILE *arq;
	arq = fopen("treinos.bin", "rb");
	
	FILE *arqAluno;
	arqAluno = fopen("alunos.bin", "rb");
	
	FILE *arqProfissional;
	arqProfissional = fopen("profissionais.bin", "rb");
	
	int i;
	system("cls");
	printf("==============================================\n");
	printf("  LISTAGEM - Treinos\n");
	printf("==============================================\n");
	
	if(arq == NULL) {
		printf("Erro na Abertura do Arquivo!\n\n");
	} else {
		
		fseek(arq, 0, SEEK_END);
		if(ftell(arq) == 0) {
		    printf("\n[ERRO] Nenhum Dado foi Encontrado!\n\n");
			system("pause");
			system("cls");
			return;
		} else {
			
			fseek(arq, 0, SEEK_SET); // Retorna ao inicio do arquivo
			while(fread(&treinos, sizeof(Treino), 1, arq) == 1) {
				printf("Código do Treino: %d\n\n", treinos.codigo);
				printf("Código do Aluno: %d\n", treinos.codAluno);
				
				if(arqAluno == NULL) {
					printf("Erro na Abertura do Arquivo!\n\n");
				} else {
					rewind(arqAluno);
					while(fread (&alunos, sizeof(Aluno), 1, arqAluno) == 1) {
						if(alunos.codigo == treinos.codAluno) {
							printf("  Nome do Aluno: %s\n\n", alunos.nome);
						}
					}
				}
				
				printf("Código do Profissional: %d\n", treinos.codProfissional);
				
				if(arqProfissional == NULL) {
					printf("Erro na Abertura do Arquivo!\n\n");
				} else {
					rewind(arqProfissional);
					while(fread (&profissionais, sizeof(Profissional), 1, arqProfissional) == 1) {
						if(profissionais.codigo == treinos.codProfissional) {
							printf("  Nome do Profissional: %s\n\n", profissionais.nome);
						}
					}
				}
				
				printf("Nome do Treino: %s", treinos.nome);
				printf("Objetivo do Treino: %s\n", treinos.objetivo);
				
				printf("Exercícios:\n");
				for(i=0; i<treinos.quantidade; i++) {
					printf("  %d) Exercício: %s", i+1, treinos.exercicios[i]);
				}
					
				printf("\nCriado em: %d/%d/%d\n", treinos.dataCriacao.dia, treinos.dataCriacao.mes, treinos.dataCriacao.ano);
	
				printf("Ativo: ");
				if(treinos.ativo == 1) {
					printf("Sim\n");
				} else {
				    printf("Não\n");
				}
				printf("==============================================\n");
			}
	    }
	}
	
	fclose(arq);
	system("pause");
}

// Listar Equipamentos
void listarEquipamentos() {
	FILE *arq;
	Equipamento equipamentos;
	
	int i, encontrados, opcao;
	
	system("cls");
	arq = fopen("equipamentos.bin", "rb");
	if(arq == NULL) {
		printf("Erro na Abertura do Arquivo!\n\n");
	} else  {
		fseek(arq, 0, SEEK_END);
		if(ftell(arq) == 0) {
		    printf("\n[ERRO] Nenhum Dado foi Encontrado!\n\n");
			system("pause");
			system("cls");
			return;
		} else {
			do {
			    system("cls");
			    printf("==============================================\n");
			    printf("  LISTAGEM - Equipamentos\n");
			    printf("==============================================\n\n");
			    printf("  [1]  Listar Equipamentos Bons\n");
			    printf("  [2]  Listar Equipamentos em Manutenção\n");
			    printf("  [3]  Listar Equipamentos Inativos\n");
			    printf("  [4]  Listar Todos os Equipamentos\n");
			    printf("  [0]  Voltar\n\n");
			    printf("----------------------------------------------\n");
			    printf("  Opção: ");
			    scanf("%d", &opcao);
			
			    system("cls");
			    switch (opcao) {
			        case 1:
			            encontrados = 0;
			            fseek(arq, 0, SEEK_SET);
			            while (fread(&equipamentos, sizeof(Equipamento), 1, arq) == 1) {
			                if (equipamentos.situacao == 1) {
			                    printf("Código do Equipamento: %d\n\n", equipamentos.codigo);
			
			                    printf("Nome do Equipamento: %s", equipamentos.nome);
			                    printf("Tipo do Equipamento: %s", equipamentos.tipo);
			                    printf("Data de Aquisição: %d/%d/%d\n\n", equipamentos.dataAquisicao.dia, equipamentos.dataAquisicao.mes, equipamentos.dataAquisicao.ano);
			
			                    printf("Quantidade: %d\n", equipamentos.quantidade);
			                    printf("Valor do Equipamento: %.2f\n", equipamentos.valor);
			                    printf("Valor Total dos Equipamentos: R$%.2f\n\n", equipamentos.vTotal);
			
			                    printf("Situação: Bom\n\n");
			
			                    printf("Data da Última Manutenção: %d/%d/%d\n", equipamentos.dataUltimaManutencao.dia, equipamentos.dataUltimaManutencao.mes, equipamentos.dataUltimaManutencao.ano);
			
			                    printf("Ativo: %s\n", equipamentos.ativo == 1 ? "Sim" : "Não");
			                    printf("==============================================\n");
			
			                    encontrados++;
			                }
			            }
			            if (encontrados == 0) {
			                printf("[ERRO] Nenhum equipamento cadastrado.\n\n");
			            }
			            system("pause");
			            break;
			
			        case 2:
			            encontrados = 0;
			            fseek(arq, 0, SEEK_SET);
			            while (fread(&equipamentos, sizeof(Equipamento), 1, arq) == 1) {
			                if (equipamentos.situacao == 2) {
			                    printf("Código do Equipamento: %d\n\n", equipamentos.codigo);
			
			                    printf("Nome do Equipamento: %s", equipamentos.nome);
			                    printf("Tipo do Equipamento: %s", equipamentos.tipo);
			                    printf("Data de Aquisição: %d/%d/%d\n\n", equipamentos.dataAquisicao.dia, equipamentos.dataAquisicao.mes, equipamentos.dataAquisicao.ano);
			
			                    printf("Quantidade: %d\n", equipamentos.quantidade);
			                    printf("Valor do Equipamento: %.2f\n", equipamentos.valor);
			                    printf("Valor Total dos Equipamentos: R$%.2f\n\n", equipamentos.vTotal);
			
			                    printf("Situação: Manutenção\n\n");
			
			                    printf("Data da Última Manutenção: %d/%d/%d\n", equipamentos.dataUltimaManutencao.dia, equipamentos.dataUltimaManutencao.mes, equipamentos.dataUltimaManutencao.ano);
			
			                    printf("Ativo: %s\n", equipamentos.ativo == 1 ? "Sim" : "Não");
			                    printf("==============================================\n");
			
			                    encontrados++;
			                }
			            }
			            if (encontrados == 0) {
			                printf("[ERRO] Nenhum equipamento cadastrado.\n\n");
			            }
			            system("pause");
			            break;
			
			        case 3:
			            encontrados = 0;
			            fseek(arq, 0, SEEK_SET);
			            while (fread(&equipamentos, sizeof(Equipamento), 1, arq) == 1) {
			                if (equipamentos.situacao == 3) {
			                    printf("Código do Equipamento: %d\n\n", equipamentos.codigo);
			
			                    printf("Nome do Equipamento: %s", equipamentos.nome);
			                    printf("Tipo do Equipamento: %s", equipamentos.tipo);
			                    printf("Data de Aquisição: %d/%d/%d\n\n", equipamentos.dataAquisicao.dia, equipamentos.dataAquisicao.mes, equipamentos.dataAquisicao.ano);
			
			                    printf("Quantidade: %d\n", equipamentos.quantidade);
			                    printf("Valor do Equipamento: %.2f\n", equipamentos.valor);
			                    printf("Valor Total dos Equipamentos: R$%.2f\n\n", equipamentos.vTotal);
			
			                    printf("Situação: Inativo\n\n");
			
			                    printf("Data da Última Manutenção: %d/%d/%d\n", equipamentos.dataUltimaManutencao.dia, equipamentos.dataUltimaManutencao.mes, equipamentos.dataUltimaManutencao.ano);
			
			                    printf("Ativo: %s\n", equipamentos.ativo == 1 ? "Sim" : "Não");
			                    printf("==============================================\n");
			
			                    encontrados++;
			                }
			            }
			            if (encontrados == 0) {
			                printf("[ERRO] Nenhum equipamento cadastrado.\n\n");
			            }
			            system("pause");
			            break;
			
			        case 4:
			            encontrados = 0;
			            fseek(arq, 0, SEEK_SET);
			            while (fread(&equipamentos, sizeof(Equipamento), 1, arq) == 1) {
			                printf("Código do Equipamento: %d\n\n", equipamentos.codigo);
			
			                printf("Nome do Equipamento: %s", equipamentos.nome);
			                printf("Tipo do Equipamento: %s", equipamentos.tipo);
			                printf("Data de Aquisição: %d/%d/%d\n\n", equipamentos.dataAquisicao.dia, equipamentos.dataAquisicao.mes, equipamentos.dataAquisicao.ano);
			
			                printf("Quantidade: %d\n", equipamentos.quantidade);
			                printf("Valor do Equipamento: %.2f\n", equipamentos.valor);
			                printf("Valor Total dos Equipamentos: R$%.2f\n\n", equipamentos.vTotal);
			
			                printf("Situação: ");
			                if (equipamentos.situacao == 1) {
			                    printf("Bom\n\n");
			                } else if (equipamentos.situacao == 2) {
			                    printf("Manutenção\n\n");
			                } else if (equipamentos.situacao == 3) {
			                    printf("Inativo\n\n");
			                }
			
			                printf("Data da Última Manutenção: %d/%d/%d\n", equipamentos.dataUltimaManutencao.dia, equipamentos.dataUltimaManutencao.mes, equipamentos.dataUltimaManutencao.ano);
			
			                printf("Ativo: %s\n", equipamentos.ativo == 1 ? "Sim" : "Não");
			                printf("==============================================\n");
			
			                encontrados++;
			            }
			            if (encontrados == 0) {
			                printf("[ERRO] Nenhum equipamento cadastrado.\n\n");
			            }
			            system("pause");
			            break;
			
			        case 0:
			            break;
			
			        default:
			            printf("Opção inválida!\n");
			            system("pause");
			    }
			} while (opcao != 0);
		}
		fclose(arq);
		system("pause");
		system("cls");
	}
}

// Cadastrar Treinos
void cadastrarTreino() {
	int i, quant, pos, ok;
	
	FILE *arq;
	
	system("cls");
	if(!temAlunoAtivo()) {
		printf("\nNão Existem Alunos Ativos Cadastrados!\n\n");
		system("pause");
		return;
	}
	if(!temProfissionalAtivo()) {
		printf("\nNão Existem Profissionais Ativos Cadastrados!\n\n");
		system("pause");
		return;
	}
	arq = fopen("treinos.bin", "ab+");
	if(arq == NULL) {
		printf("Erro na Abertura do Arquivo!\n\n");
	} else {
		
		// verifica se o codigo do treino existe.
		do {
			do {	
				printf("Informe o Código do Treino: ");
				scanf("%d", &treinos.codigo);
			    validarCodigo(treinos.codigo);
							
			} while(treinos.codigo < 1 || treinos.codigo > 1000);
								
			pos = busca(arq, treinos.codigo, ENT_TREINO);
			if(pos != -1) printf("Código já Cadastrado!\n\n");
		} while(pos != -1);
		
		// Verifica se o aluno existe ou não
		do {
			printf("Informe o Código do Aluno: ");
			scanf("%d", &treinos.codAluno);
			ok = existeAlunoAvaliacao(treinos.codAluno);
			if(!ok) printf("Aluno não Encontrado ou Inativo!\n\n");
		} while(!ok);
		
		// Verifica se o profissional existe ou não
		do {
			printf("Informe o Código do Profissional: ");
			scanf("%d", &treinos.codProfissional);
			ok = existeProfissionalAvaliacao(treinos.codProfissional);
			if(!ok) printf("Profissional não Encontrado ou Inativo!\n\n");
		} while(!ok);
		
		fflush(stdin);
		printf("Informe o Nome do Treino: ");
		fgets(treinos.nome, 31, stdin);
		
		fflush(stdin);
		printf("Informe o Objetivo do Treino: ");
		fgets(treinos.objetivo, 61, stdin);
		
		do {
			printf("\nInforme Quantos Exercícios Deseja Cadastrar: ");
			scanf("%d", &treinos.quantidade);
			getchar();
			
			if(treinos.quantidade>0 && treinos.quantidade <=10) {
				for(i=0; i<treinos.quantidade; i++) {
					printf("  %d) Exercício: ", i+1);
				    fgets(treinos.exercicios[i], 41, stdin);
				}
			} else {
				printf("[AVISO] A Quantidade Máxima de Exercícios por Treino é 10!!\n\n");
			}
		} while(treinos.quantidade<=0 || treinos.quantidade>10);
		
		do {
			printf("\nInforme a Data de Criação (DD MM AAAA): ");
			scanf("%d%d%d", &treinos.dataCriacao.dia, &treinos.dataCriacao.mes, &treinos.dataCriacao.ano);
			if(validaData(treinos.dataCriacao) == 0) printf("Insira Uma Data Valida!\n");
		} while(validaData(treinos.dataCriacao) != 1);
		
		treinos.ativo = 1;
	
		fwrite(&treinos, sizeof(Treino), 1, arq);
		printf("\n[SUCESSO] Treino cadastrado!\n");
		system("pause");
		system("cls");
		fclose(arq);
	}
}

// Cadastrar Equipamentos
void cadastrarEquipamento() {
	FILE *arq;
	Equipamento equipamentos;
	
	int pos;
	
	system("cls");
	arq = fopen("equipamentos.bin", "ab+");
	if(arq == NULL) {
		printf("Erro na Abertura do Arquivo!\n\n");
	} else  {
		// verifica se o codigo do treino existe.
		do {	
			printf("Informe o Código do Equipamento: ");
			scanf("%d", &equipamentos.codigo);
		    validarCodigo(equipamentos.codigo);
						
		} while(equipamentos.codigo < 1 || equipamentos.codigo > 1000);
							
		pos = busca(arq, equipamentos.codigo, ENT_EQUIPAMENTO);
		if(pos != -1) {
			printf("[ERRO] Código já cadastrado!\n\n");
							
			fclose(arq);
			system("pause");
			system("cls");
			return;
		}
		
		fflush(stdin);
		printf("\nInforme o Nome do Equipamento: ");
		fgets(equipamentos.nome, 61, stdin);
		
		fflush(stdin);
		printf("Informe o Tipo do Equipamento: ");
		fgets(equipamentos.tipo, 31, stdin);
		
		printf("Informe a Data de Aquisição (DD MM AAAA): ");
		scanf("%d%d%d", &equipamentos.dataAquisicao.dia, &equipamentos.dataAquisicao.mes, &equipamentos.dataAquisicao.ano);
		
		do {
			printf("\nInforme a Quantidade: ");
		    scanf("%d", &equipamentos.quantidade);
		    
		    if(equipamentos.quantidade <= 0) {
		    	printf("[ERRO] A Quantidade deve ser Superior à 0!\n\n");
		    }
		} while(equipamentos.quantidade <= 0);
		
		do {
			printf("Informe o Valor do Equipamento: ");
			scanf("%f", &equipamentos.valor);
		    
		    if(equipamentos.valor <= 0) {
		    	printf("[ERRO] O valor deve ser Superior à 0!\n\n");
		    }
		} while(equipamentos.valor <= 0);
		
		printf("\nSituação: 1 = Bom | 2 = Manutenção | 3 = Inativo\n");
		printf("  Opção: ");
		scanf("%d", &equipamentos.situacao);
		
		printf("\nInforme da Última Manutenção (DD MM AAAA): ");
		scanf("%d%d%d", &equipamentos.dataUltimaManutencao.dia, &equipamentos.dataUltimaManutencao.mes, &equipamentos.dataUltimaManutencao.ano);
		
		equipamentos.vTotal = equipamentos.valor * equipamentos.quantidade;
		equipamentos.ativo = 1;
		
		fwrite(&equipamentos, sizeof(Equipamento), 1, arq);
		printf("\n[SUCESSO] Equipamento cadastrado!\n");
		system("pause");
		system("cls");
		fclose(arq);
	}
}

// ===== FIM DO MENU TREINOS E EQUIPAMENTOS =====

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
						alterarTreino();
						break;
					case ENT_PLANO:
						alterarPlano();
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
						alterarEquipamento();
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
						excluirTreino();
						break;
					case ENT_PLANO:
						excluirPlano();
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
						excluirEquipamento();
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
						listarTreinos();
						break;
					case ENT_PLANO:
						listarPlanos();
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
						listarEquipamentos();
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
				//Treinos
				cadastrarTreino();
				break;
			case 4:
				//Planos
				cadastrarPlano();
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
				//Equipamentos
				cadastrarEquipamento();
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
// ===== RELATÓRIO EM ORDEM ALFABÉTICA =====
// Ordena uma CÓPIA do arquivo (ordenado.bin) com a lógica do professor
// (troca de registros vizinhos usando fseek/fread/fwrite), assim o arquivo original não muda de ordem.
typedef union {
	Aluno        a;
	Profissional p;
	Treino       t;
	Plano        pl;
	Atividade    at;
	Equipamento  e;
} Registro;

int tamanhoRegistro(int codEnt) {
	switch(codEnt) {
		case ENT_ALUNO:        return sizeof(Aluno);
		case ENT_PROFISSIONAL: return sizeof(Profissional);
		case ENT_TREINO:       return sizeof(Treino);
		case ENT_PLANO:        return sizeof(Plano);
		case ENT_ATIVIDADE:    return sizeof(Atividade);
		case ENT_EQUIPAMENTO:  return sizeof(Equipamento);
	}
	return 0;
}

void nomeRegistro(Registro *r, int codEnt, char destino[]) {
	switch(codEnt) {
		case ENT_ALUNO:        strcpy(destino, r->a.nome);  break;
		case ENT_PROFISSIONAL: strcpy(destino, r->p.nome);  break;
		case ENT_TREINO:       strcpy(destino, r->t.nome);  break;
		case ENT_PLANO:        strcpy(destino, r->pl.nome); break;
		case ENT_ATIVIDADE:    strcpy(destino, r->at.nome); break;
		case ENT_EQUIPAMENTO:  strcpy(destino, r->e.nome);  break;
	}
	if(strlen(destino) > 0 && destino[strlen(destino)-1] == '\n') destino[strlen(destino)-1] = '\0';
}

int codigoRegistro(Registro *r, int codEnt) {
	switch(codEnt) {
		case ENT_ALUNO:        return r->a.codigo;
		case ENT_PROFISSIONAL: return r->p.codigo;
		case ENT_TREINO:       return r->t.codigo;
		case ENT_PLANO:        return r->pl.codigo;
		case ENT_ATIVIDADE:    return r->at.codigo;
		case ENT_EQUIPAMENTO:  return r->e.codigo;
	}
	return 0;
}

int ativoRegistro(Registro *r, int codEnt) {
	switch(codEnt) {
		case ENT_ALUNO:        return r->a.ativo;
		case ENT_PROFISSIONAL: return r->p.ativo;
		case ENT_TREINO:       return r->t.ativo;
		case ENT_PLANO:        return r->pl.ativo;
		case ENT_ATIVIDADE:    return r->at.ativo;
		case ENT_EQUIPAMENTO:  return r->e.ativo;
	}
	return 0;
}

void relatorioAlfabetico(void) {
	FILE *arq, *ordenado;
	Registro reg, regi;
	char nome1[61], nome2[61], arquivo[31], titulo[31];
	int opcao, codEnt, tam, qtde, i, total;

	system("cls");
	printf("==============================================\n");
	printf("  RELATÓRIO - Ordem Alfabética\n");
	printf("==============================================\n\n");
	printf("  [1]  Alunos\n");
	printf("  [2]  Profissionais\n");
	printf("  [3]  Treinos\n");
	printf("  [4]  Planos\n");
	printf("  [5]  Atividades\n");
	printf("  [6]  Equipamentos\n");
	printf("  [0]  Voltar\n\n");
	printf("----------------------------------------------\n");
	printf("  Opção: ");
	opcao = -1;
	scanf("%d", &opcao);
	fflush(stdin);

	switch(opcao) {
		case 1: codEnt = ENT_ALUNO;        strcpy(arquivo, "alunos.bin");        strcpy(titulo, "Alunos");        break;
		case 2: codEnt = ENT_PROFISSIONAL; strcpy(arquivo, "profissionais.bin"); strcpy(titulo, "Profissionais"); break;
		case 3: codEnt = ENT_TREINO;       strcpy(arquivo, "treinos.bin");       strcpy(titulo, "Treinos");       break;
		case 4: codEnt = ENT_PLANO;        strcpy(arquivo, "planos.bin");        strcpy(titulo, "Planos");        break;
		case 5: codEnt = ENT_ATIVIDADE;    strcpy(arquivo, "atividades.bin");    strcpy(titulo, "Atividades");    break;
		case 6: codEnt = ENT_EQUIPAMENTO;  strcpy(arquivo, "equipamentos.bin");  strcpy(titulo, "Equipamentos");  break;
		case 0: return;
		default:
			printf("\n Opção Inválida!\n");
			system("pause");
			return;
	}

	tam = tamanhoRegistro(codEnt);

	arq = fopen(arquivo, "rb");
	if(arq == NULL) {
		printf("\nNenhum Registro Cadastrado!\n\n");
		system("pause");
		return;
	}

	// copia os registros para o arquivo auxiliar
	ordenado = fopen("ordenado.bin", "wb");
	if(ordenado == NULL) {
		printf("Erro na Abertura do Arquivo!\n\n");
		fclose(arq);
		system("pause");
		return;
	}
	while(fread(&reg, tam, 1, arq) == 1) {
		fwrite(&reg, tam, 1, ordenado);
	}
	fclose(arq);
	fclose(ordenado);

	// ordena o arquivo auxiliar (bubble sort direto no arquivo)
	ordenado = fopen("ordenado.bin", "rb+");
	if(ordenado == NULL) {
		printf("Erro na Abertura do Arquivo!\n\n");
		system("pause");
		return;
	}
	fseek(ordenado, 0, SEEK_END);
	qtde = ftell(ordenado) / tam;
	total = qtde;
	while(qtde > 1) {
		for(i = 0; i < qtde - 1; i++) {
			fseek(ordenado, i * tam, SEEK_SET);
			fread(&reg, tam, 1, ordenado);
			fseek(ordenado, (i + 1) * tam, SEEK_SET);
			fread(&regi, tam, 1, ordenado);
			nomeRegistro(&reg, codEnt, nome1);
			nomeRegistro(&regi, codEnt, nome2);
			if(stricmp(nome1, nome2) > 0) {
				fseek(ordenado, i * tam, SEEK_SET);
				fwrite(&regi, tam, 1, ordenado);
				fseek(ordenado, (i + 1) * tam, SEEK_SET);
				fwrite(&reg, tam, 1, ordenado);
			}
		}
		qtde--;
	}

	// exibe
	system("cls");
	printf("==============================================\n");
	printf("  %s em Ordem Alfabética\n", titulo);
	printf("==============================================\n\n");
	rewind(ordenado);
	while(fread(&reg, tam, 1, ordenado) == 1) {
		nomeRegistro(&reg, codEnt, nome1);
		printf("  Código: %-5d Nome: %-30s %s\n", codigoRegistro(&reg, codEnt), nome1, ativoRegistro(&reg, codEnt) == 1 ? "Ativo" : "Inativo");
	}
	printf("\n  Total: %d registro(s)\n\n", total);
	fclose(ordenado);
	remove("ordenado.bin");
	system("pause");
}

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
		printf("  [7]  Cadastros em ordem alfabética\n");
		printf("       Alunos, profissionais, treinos, planos e mais\n");
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
			case 7:
				relatorioAlfabetico();
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
