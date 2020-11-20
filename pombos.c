/*
Trabalho Final da Disciplina Sistemas Operacionais - TT304


Lembre: $gcc arquivo.c -lpthread -o arqexe
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define N 20
int contPostIt = 0; 
pthread_mutex_t semaforoViagem = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t semaforoMensagem = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t semaforoAusenciaDoPombo = PTHREAD_MUTEX_INITIALIZER;

void dorme_aleatorio(){

    int tempoDeSono = rand()%10;
    sleep(tempoDeSono);

}

void leva_mochila_ate_B_e_volta(){

    int tempoDeViagem = rand()%4;
    sleep(tempoDeViagem);

}

void ColaPostIt(int id){

    printf("Usuário %d quer escrever.\n", id + 1);
    pthread_mutex_lock(&semaforoMensagem);
    
    
}

void* usuario(void *arg) {

    int *id = (int*)arg;
    
    while(1)
    {

        dorme_aleatorio();
        ColaPostIt(*id);
        contPostIt++;
        printf("Id %d Escreveu a mensagem: %d.\n", *id + 1, contPostIt);
        
        //Teste
        if(contPostIt > 20)
        {
            printf("Algo saiu errado.\n");
            exit(-1);
        }
        

        if (contPostIt == N)
        {   
            
            printf("mochila cheia\n");
            
            //pthread_mutex_lock(&semaforoMensagem);
            pthread_mutex_lock(&semaforoAusenciaDoPombo);
            pthread_mutex_unlock(&semaforoViagem);
            pthread_mutex_lock(&semaforoAusenciaDoPombo);
                          
        }

        pthread_mutex_unlock(&semaforoMensagem);
        pthread_mutex_unlock(&semaforoAusenciaDoPombo);
    }

    return NULL;
}


void* pombo(void *arg) {

    while(1){
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
        //pthread_mutex_unlock(&semaforoMensagem);
    }

    return NULL;
} 


int main(){

    pthread_t threadPombo;
    int numeroDeUsuarios;
    pthread_mutex_lock(&semaforoViagem);
    //pthread_mutex_lock(&semaforoAusenciaDoPombo)
   

    printf("Escolha o numero de usuários: \n");
    scanf("%d", &numeroDeUsuarios);

    pthread_t threadUsuarios[numeroDeUsuarios];
    int aux[numeroDeUsuarios];

    pthread_create(&threadPombo, NULL, pombo, NULL);

    for(int i = 0; i < numeroDeUsuarios; i++)
    {
        aux[i] = i;
        pthread_create(&threadUsuarios[i], NULL, usuario, &aux[i]);
    }

    pthread_join(threadPombo, NULL);
    

    return 0;
}