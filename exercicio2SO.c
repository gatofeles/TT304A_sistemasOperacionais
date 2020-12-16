/*

Trabalho Final da Disciplina Sistemas Operacionais - TT304
Equipe: Cerulean
Membro: Wagner Alexandre Martins Junior RA: 148222

Programa destinado à solução do problema 2 do trabalho final.

** A compilação do programa deve ser realizada a partir da utilização
do compilador Gnu Compiler Collection (GNU), a partir da utilização do
seguinte comando:  gcc tarefa2.c -lpthread -o nomeDoArquivoDeSaida .

*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>//Biblioteca para criação de Threads.
#include <unistd.h>

#define N 20
int contPostIt = 0; 

//Semáforo utilizado para acordar o pombo.
pthread_mutex_t semaforoViagem = PTHREAD_MUTEX_INITIALIZER; 

//Semáforo utilizado para que dois usuários não colem mensagens ao mesmo tempo.
pthread_mutex_t semaforoMensagem = PTHREAD_MUTEX_INITIALIZER;

//Semáforo que sinaliza que o pombo está ausente.
pthread_mutex_t semaforoAusenciaDoPombo = PTHREAD_MUTEX_INITIALIZER;

/*Método que define por quanto tempo o usuário
permanecerá inativo.*/
void dorme_aleatorio(){

    int tempoDeSono = rand()%4;
    sleep(tempoDeSono);

}

/* Método que define o tempo de viagem do pombo.*/
void leva_mochila_ate_B_e_volta(){

    int tempoDeViagem = rand()%4;
    sleep(tempoDeViagem);

}

/* Este método foi inserido para que o usuário
   indique sua intenção de colocar o post it. */
void ColaPostIt(int id){

    printf("Usuário %d quer colar o post it.\n", id + 1);
    pthread_mutex_lock(&semaforoMensagem);
        
}


/* Método que define o comportamento do usuário. */
void* usuario(void *arg) {

    int *id = (int*)arg;
    
    while(1)
    {

        dorme_aleatorio();
        ColaPostIt(*id);
        contPostIt++;
        printf("Usuário %d colou a mensagem: %d.\n", *id + 1, contPostIt);        
        
        if (contPostIt == N)
        {   

            printf("mochila cheia\n");
            
            pthread_mutex_lock(&semaforoAusenciaDoPombo);//Indica que o pombo estará ausente.
            pthread_mutex_unlock(&semaforoViagem);//Usuário acorda o pombo.

            /* A passagem por este ponto só será permitida após o retorno do pombo
            bloqueando o acesso à bolsa.*/
            pthread_mutex_lock(&semaforoAusenciaDoPombo);
                          
        }

        /*Após o retorno do pombo, as mensagens podem ser coladas na bolsa
        novamente*/
        pthread_mutex_unlock(&semaforoMensagem);
        /* Semáforo de ausência do pombo é resetado para indicar que o mesmo está
        presente.*/
        pthread_mutex_unlock(&semaforoAusenciaDoPombo);

    }

    return NULL;
}

/* Método que define o comportamento do pombo. */
void* pombo(void *arg) {

    while(1){
        /*Após ser acordado por um usuário, o 
        pombo bloqueia o semáforo viagem e entrega as mensagens.*/
        pthread_mutex_lock(&semaforoViagem);
        printf("Pombo voando. Pruu...\n");
        leva_mochila_ate_B_e_volta();
        contPostIt = 0;
        for (int i = 0; i < N; i++)
        {
            printf("Adic. msg %d\n", i+1);
        }
        printf("Pombo Voltou.\n");
        sleep(1);
        pthread_mutex_unlock(&semaforoAusenciaDoPombo);
        
    }

    return NULL;
} 


int main(){

    pthread_t threadPombo;//Thread do pombo.
    int numeroDeUsuarios;
    /*Semáforo para acordar o pombo inicializado no estado down.*/
    pthread_mutex_lock(&semaforoViagem);
    
    printf("Escolha o numero de usuários: \n");
    scanf("%d", &numeroDeUsuarios);

    pthread_t threadUsuarios[numeroDeUsuarios];//Vetor de threads dos usuários.
    int aux[numeroDeUsuarios];//Vetor usado para armazenar o Id dos usuários.

    pthread_create(&threadPombo, NULL, pombo, NULL);//Inicialização da thead pombo.

    /*Inicialização das threads de usuário, passando a identificação como parâmetro.*/
    for(int i = 0; i < numeroDeUsuarios; i++)
    {
        aux[i] = i;
        pthread_create(&threadUsuarios[i], NULL, usuario, &aux[i]);
    }

    pthread_join(threadPombo, NULL);//Impede que o programa encerre antes de a thread encerrar.
    
    return 0;
}