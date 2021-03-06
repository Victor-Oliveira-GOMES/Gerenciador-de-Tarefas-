#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <windows.h>
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN

void ClearScreen()
{
	HANDLE                     hStdOut;
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	DWORD                      count;
	DWORD                      cellCount;
	COORD                      homeCoords = { 0, 0 };

	hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hStdOut == INVALID_HANDLE_VALUE) return;

	/* Get the number of cells in the current buffer */
	if (!GetConsoleScreenBufferInfo(hStdOut, &csbi)) return;
	cellCount = csbi.dwSize.X * csbi.dwSize.Y;

	/* Fill the entire buffer with spaces */
	if (!FillConsoleOutputCharacter(
		hStdOut,
		(TCHAR)' ',
		cellCount,
		homeCoords,
		&count
	)) return;

	/* Fill the entire buffer with the current colors and attributes */
	if (!FillConsoleOutputAttribute(
		hStdOut,
		csbi.wAttributes,
		cellCount,
		homeCoords,
		&count
	)) return;

	/* Move the cursor home */
	SetConsoleCursorPosition(hStdOut, homeCoords);
}

#else // !_WIN32
#include <unistd.h>
#include <term.h>

void ClearScreen()
{
	if (!cur_term)
	{
		int result;
		setupterm(NULL, STDOUT_FILENO, &result);
		if (result <= 0) return;
	}

	putp(tigetstr("clear"));
}
#endif

struct Tarefa  // struct armazenando os dados que forem obtidos através da função (tarefas)
{
	char Titulo[40];
	int Repeticao;
	char Data[10];
	int Hora;
	int Minuto;
};
struct Tarefa Criar_Tarefa;

void Tarefas(void)  // função para criação de tarefas
{
	ClearScreen();
	char sem_data[] = { "XX/YY/ZZ"};
	int numero;
	char c;
	numero = 0; //variável para verificar se o número que o usuário digitou esta correto ou não 
   do {
	   printf_s("Escreva o Título da tarefa:");
	   fgets(Criar_Tarefa.Titulo, 40, stdin);    // Vai armazenar o nome/título que o usuário digitou da tarefa na struct

	   printf_s("Digite a opção contendo o que o usuário deseja configurar.\n");
	   printf_s("Digite 1 Caso o usuário deseja que a tarefa se repita constantemente.\nDigite 2 Caso o usuário deseja adicionar uma data e/ou um horário para a realização desta tarefa.\n");
	   scanf_s("%d", &numero);
	   //while ((c = getchar()) != '\n' && c != EOF) {}  // código para a eliminação de problemas relacionados a buffer de teclado
	   switch (numero) // seleção entre qual das opções vai ser configurada
	   {
	   case 1:
		   printf_s("Escolha com que frequência a tarefa vai se repetir:\n");
		   printf_s("Digite 1 Caso deseja que a tarefa ocorra Diariamente.\n"
					"Digite 2 Caso deseja que a tarefa ocorra Semanalmente.\n"
					"Digite 3 Caso deseja que a tarefa ocorra Mensalmente.\n"
					"Digite 4 Caso deseja que a tarefa ocorra Anualmente.\n");
		   scanf_s("%d", &Criar_Tarefa.Repeticao);   // Vai armazenar a frequencia que o usuário deseja que oc da tarefa na struct
		   while ((c = getchar()) != '\n' && c != EOF) {}  // código para a eliminação de problemas relacionados a buffer de teclado
           // vai ler o que o usuário escrever e vai armazenar na struct 
		   printf_s("Defina a Hora:");							
		   scanf_s("%d",&Criar_Tarefa.Hora);
		   while ((c = getchar()) != '\n' && c != EOF) {}  // código para a eliminação de problemas relacionados a buffer de teclado
		   // vai ler o que o usuário escrever e vai armazenar na struct
		   printf_s("Defina o(s) Minuto(s):");             
		   scanf_s("%d", &Criar_Tarefa.Minuto);
		   while ((c = getchar()) != '\n' && c != EOF) {}  // código para a eliminação de problemas relacionados a buffer de teclado
		   strcpy_s(Criar_Tarefa.Data, _countof(Criar_Tarefa.Data), "Sem_Data!");
		   break;
	   case 2:
		   // vai ler o que o usuário escrever e vai armazenar na struct
		   while ((c = getchar()) != '\n' && c != EOF) {}  // código para a eliminação de problemas relacionados a buffer de teclado
		   printf_s("Digite a Data para a realização da tarefa:\n");   
		   fgets(Criar_Tarefa.Data, 10, stdin);
		   // vai ler o que o usuário escrever e vai armazenar na struct
		   while ((c = getchar()) != '\n' && c != EOF) {}  // código para a eliminação de problemas relacionados a buffer de teclado
		   printf_s("Digite o horario para a realização da tarefa:\n");
		   printf_s("Defina a Hora:");
		   scanf_s("%d", &Criar_Tarefa.Hora);
		   printf_s("Defina o(s) Minuto(s):");
		   scanf_s("%d", &Criar_Tarefa.Minuto);
		   while ((c = getchar()) != '\n' && c != EOF) {}  // código para a eliminação de problemas relacionados a buffer de teclado
		   Criar_Tarefa.Repeticao = 5;
		   break;
	   default:
		   printf_s("Número inválido, Por favor digite novamente a opção desejada entre as anteriores");
		   numero = 0;
		   break;
	   }
   } while (numero == 0); //loop para impedir que o usuário digite um número errado
}

void Salvar_dados(void) 
{
	FILE* arquivo;
	errno_t teste;
	teste = fopen_s(&arquivo, "lista_de_Tarefas.txt", "a+");
	if (arquivo == NULL)
	{
		printf("ERRO , Não foi possível ler o Arquivo\n");
		exit(1);
	};
	if (teste == 0)
	{
		printf("Arquivo Aberto\n");
		fprintf(arquivo, "%s", &Criar_Tarefa.Titulo);
		fprintf(arquivo, "%d\n", Criar_Tarefa.Repeticao);
		fprintf(arquivo, "%s\n", &Criar_Tarefa.Data);
		fprintf(arquivo, "%d\n", Criar_Tarefa.Hora);
		fprintf(arquivo, "%d\n", Criar_Tarefa.Minuto);
		fprintf(arquivo, "%d\n", 0);  // para configurar o fim e o início de cada tarefa 
	};
	int fclose_s(FILE * arquivo);
	printf("Arquivo Fechado\n");
}

void Ler_dados(void)
{
	FILE* arquivo;
	errno_t teste;
	teste = fopen_s(&arquivo, "lista_de_Tarefas.txt", "r+");
	if (arquivo == NULL)
	{
		printf("ERRO , Não foi possível ler o Arquivo\n");
		system("pause");
		exit(1);
	};
	if (teste == 0)
	{
		printf("Arquivo Aberto\n");
		char titulo[20];          //vai armazenar os dados do arquivo referentes ao nome da variável
		char data [10];                  //vai armazenar os dados do arquivo referentes ao nome da variável
		int repeticao, hora, minuto,configuracao;    //vai armazenar os dados do arquivo referentes ao nome da variável
		int referencia; // vai converter o número armazenado no arquivo uma para imprimir corretamente a frequência na tela do usuário.
		char matriz[5][15] =
		{
			"Diáriamente",
			"Semanalmente",
			"Mensalmente",
			"Anualmente",
			"Sem_repeticao"
		};
		// loop para ler todas as tarefas e imprimi-las
		do 
		{
			referencia = 0;
			fscanf_s(arquivo, "%s", &titulo, sizeof(titulo));
			fscanf_s(arquivo, "%d", &repeticao);				// vai  receber o dado referente á frequência, para poder converter na hora da impressão na tela do usuário
			fscanf_s(arquivo, "%s", &data, sizeof(data));
			fscanf_s(arquivo, "%d", &hora);
			fscanf_s(arquivo, "%d", &minuto);
			fscanf_s(arquivo, "%d", &configuracao);				// para configurar o fim e o início de cada tarefa 
			referencia = repeticao;								//  vai receber o dado  no arquivo, referente à frequência
			printf_s("\n\n\t\t____________________________________________________________________\t\t");
			printf_s("\n\t\t\t\t\tTarefa\n");
			printf_s("\t\t\t\tTítulo da tarefa:%s\n", titulo);
			printf_s("\t\t\t\tFrequência da Tarefa:%s\n", matriz[referencia - 1]);
			printf_s("\t\t\t\tData da Tarefa:%s\n", data);
			printf_s("\t\t\t\tHorário para a Tarefa:%d:%d\n", hora, minuto);
			printf_s("\t\t____________________________________________________________________\t\t\n\n");
		} while (fscanf_s(arquivo, "%d", &configuracao) != -1);  // faz a verificação de quantas tarefas estão salvas utilizando o retorno da função fscanf_s como referência.
	};
	int fclose_s(FILE * arquivo);
	printf("Arquivo Fechado\n");
}

void imprimir_struct(void) 
{
	ClearScreen();
	int referencia;								// vai converter o número armazenado na struct para uma para imprimir corretamente a frequência na tela do usuário.
	char matriz[5][15] =
	{
		"Diáriamente",
		"Semanalmente",
		"Mensalmente",
		"Anualmente",
		"Sem_repeticao"
	};
	referencia = Criar_Tarefa.Repeticao;						// vai receber o dado referente a struct
	printf_s("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
	printf_s("Título da tarefa:");
	printf_s("%s\n", Criar_Tarefa.Titulo);
	if (Criar_Tarefa.Repeticao != 0)							//verifica se tem uma repetição na struct, e caso tenha imprime na tela a frequência referente a repetição
	{
		printf_s("Frequência da Tarefa:");
		printf_s("%s\n", matriz[referencia - 1]);				// como a matriz inicia em 0, e a frequencia em 1, simplismente se subtraí 1 da referencia para os dados se corresponderem.
	}					
	if(Criar_Tarefa.Data != "Sem_Data!")						//verifica se tem uma data na struct, e caso tenha imprime na tela a data
	{
		printf_s("Data da Tarefa:");
		printf_s("%s\n", Criar_Tarefa.Data);
	}
	printf_s("Horário para a Tarefa:");
	printf_s("%d:%d\n", Criar_Tarefa.Hora, Criar_Tarefa.Minuto);
	system("pause");
}

void lista_de_tarefas(void)
{
	printf_s("*************************************************************************************************" 
		"\n");
	Ler_dados();
	printf_s("*************************************************************************************************"
			 "\n");
	system("pause");
};

int main()
{
	void ClearScreen();
	void lista_de_tarefas(void);
	void imprimir_struct(void);
	void Salvar_dados(void);
	void Ler_dados(void);
	void Tarefas(void);
	int numero,numero2, num2, opcao = 1;
	char c;
	numero = 0;
	printf_s("Gerenciador de tarefas\n\n");

	do
	{
		do
		{
			printf_s("Digite 1 se o usuário deseja Criar uma nova tarefa.\nDigite 2 se o usuario deseja abrir a lista de tarefas\n");
			scanf_s("%d", &numero);
			while ((c = getchar()) != '\n' && c != EOF) {}  // código para a eliminação de problemas relacionados a buffer de teclado
			switch (numero)
			{
			case 1: //Criar uma nova tarefa
				num2 = 1;
				printf_s("Criando nova tarefa!\n");
				Tarefas();
				break;
			case 2: //Abrir lista com todas as tarefas salvas
				num2 = 0;
				printf_s("Abrindo arquivo com a Lista de todas as tarefas\n");
				lista_de_tarefas();
				break;
			default:
				printf_s("Número incorreto\n\nDigite a sua opção novamente\n");
				numero = 0;
				break;
			}
		} while (numero == 0); //loop para impedir que o usuário digite um número errado

			if (num2 == 1) // vai verificar se foi criado uma nova tarefa e caso tenha, ele vai imprimir na tela as informações desta nova tarefa.
			{
				imprimir_struct();
				Salvar_dados(); // chamada da função que salva os dados da struct em um Arquivo
			};
			printf_s("O usuário deseja sair do programa ?\n\n\t\tDigite: 0 para sair.\n\t\tDigite: 1 para continuar.\n:");
			scanf_s("%d", &opcao);
	} while (opcao == 1);
	system("pause");
	return 0;
}