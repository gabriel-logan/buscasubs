#include <stdio.h>
#include <netdb.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <ncurses.h>
#include <unistd.h>
#define MAX_LENGTH 100
int main(int argc, char *argv[]) {
	if (argc < 3) {
		printf("Criado por Gabriel Logan \n");
        printf("Modo de uso: %s <dominio_principal> <arquivo_lista_subs>\n", argv[0]);
        printf("EX: %s sitedeexemplo.com.br subdominios.txt\n", argv[0]);
        return 1;
    }
	printf("Ira iniciar as tentativas ... \n");
	sleep(1); // Atraso de 1 segundo
    char *dominioPrincipal = argv[1];
    char *listaDeSubs = argv[2]; // Nomeia os argumentos em variaveis
	FILE *file = fopen(listaDeSubs, "r"); // Abre o arquivo selecionado
	if (file == NULL) {
		printf("Falha ao abrir o arquivo, arquivo invalido \n"); // Se houver algum problema com a lista
		return 1;
	}
    int totalDePalavras = 0; // Contagem inicial do total de palavras da lista
	char command[100];
    sprintf(command, "wc -w %s", listaDeSubs); // Comando para contar palavras no arquivo
    char result[10];
    FILE *cmdOutput = popen(command, "r"); // Executa o comando e captura a saída
    if (cmdOutput == NULL) { // Cria um arquivo com os valores escritos na lista
        printf("Falha ao executar o comando de outPut do arquivo \n");
        return 1;
    }
	FILE *outputFile = fopen("subdominiosencontrados.txt", "w"); // Abre o arquivo de saída
	if (outputFile == NULL) {
		printf("Falha ao criar o arquivo de saída \n");
		fclose(file);
		return 1;
	}
    fgets(result, sizeof(result), cmdOutput); // Lê a saída do comando
    totalDePalavras = atoi(result); // Converte a saída para inteiro
    pclose(cmdOutput); // Fecha o fluxo do comando
	char word[MAX_LENGTH]; // Cria a variavel word que recebera as palavras de dentro do arquivo
	int wordCount = 0; // Inicio da contagem do contador de palavras
	// Inicializar ncurses
    initscr();
    cbreak();
    noecho();
	// Configurar a barra de carregamento
    WINDOW *progressBarWin = newwin(3, 40, 10, 10);
    box(progressBarWin, 0, 0);
    refresh();
    wrefresh(progressBarWin);
    int progressBarWidth = 30;  // Largura da barra de carregamento (considerando a borda)
    int progress = 0;  // Progresso atual da barra de carregamento
	while (fgets(word, MAX_LENGTH, file) != NULL) { // Faço um loop dentro do arquivo com a lista de palavras
		// Remover a quebra de linha no final da palavra, se houver
		int length = strlen(word); // Verifica quantidade de linhas da lista
		if (length > 0 && word[length - 1] == '\n') {
			word[length - 1] = '\0'; // remove a que ja foi lida
		}
		// Processar a palavra (nesse exemplo, apenas imprimimos)
		const char *hostname = "%s.%s"; // Cria o hostname
		// Tamanho máximo do hostname formatado
		char formatted_hostname[MAX_LENGTH + strlen(hostname) - 2]; // Hostname formatado para receber a variavel word 
		sprintf(formatted_hostname, hostname, word, dominioPrincipal); // Injeta no hostname formatado a variavel word
		struct hostent *alvo = gethostbyname(formatted_hostname); // pega o host na variavel alvo
		if (alvo != NULL) { // verifica se é nulo
			// printa na tela os hosts que resultaram em true pelo gethostbyname
			printw("%s -> %s \n", inet_ntoa(*((struct in_addr *)alvo->h_addr)), formatted_hostname);
            fprintf(outputFile, "%s -> %s \n", inet_ntoa(*((struct in_addr *)alvo->h_addr)), formatted_hostname); // Joga a saida para o arquivo
            refresh();  // Atualizar a tela do ncurses para exibir o texto imediatamente
		}
		// Processar a palavra (nesse exemplo, apenas incrementamos o contador)
        char *token = strtok(word, " "); // Separa a linha em palavras usando o espaço como delimitador
        while (token != NULL) {
            wordCount++;
            token = strtok(NULL, " "); // Continua a separação a partir do último ponto encontrado
        }
		// Atualizar a barra de carregamento
        progress = (int) (((float) wordCount / totalDePalavras) * progressBarWidth);
        wclear(progressBarWin);
        box(progressBarWin, 0, 0);
        mvwprintw(progressBarWin, 1, 1, "Progresso: [%-*.*s]", progressBarWidth, progress, "=======================");
        wrefresh(progressBarWin);
		printf("Tentativas restantes: %d\n", (totalDePalavras - wordCount));
		if(((float)(totalDePalavras - wordCount) / 60) > 90){
			printf("Tempo estimado restante: %.2f Hora(s)\n", (float)(totalDePalavras - wordCount) / 3600);
		}else if(((float)(totalDePalavras - wordCount) / 60) > 1){
			printf("Tempo estimado restante: %.2f Minuto(s)\n", (float)(totalDePalavras - wordCount) / 60);
		}else{
			printf("Tempo estimado restante: %i Segundos(s)\n", (totalDePalavras - wordCount));
		}
	}
	fclose(file);
	fclose(outputFile);
    // Encerrar ncurses
	endwin();
	printf("Criado por Gabriel Logan, obrigado por usar (: \n");
	printf("Foi criado um arquivo com nome de subdominiosencontrados.txt com a lista dos host encontrados\n");
	return 1;
}