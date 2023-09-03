
// Created by Utente on 24/08/2023.
//
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>


//struttura nodo dell'albero

typedef struct tStation {
    int distance;       // Distanza dalla partenza dell'autostrada chiave
    int numCars;        // Num di macchine disponibili
    int *autonomies;    // puntatore ad array di autonomie
    struct tStation *left; // puntatore a figlio sin
    struct tStation *right;   // punt a ramo destro

} tStation;




//ricerca di una stazione  ( se la stazione esiste torna il puntatore alla stazione, altrimenti NULL)

tStation *searchStation(tStation *root, int distance) {

    if (root == NULL || root->distance == distance) {
        return root;
    }

    if (distance < root->distance) {
        return searchStation(root->left, distance);
    }

    return searchStation(root->right, distance);
}




//funzione che cerca l'autonomia e ne ritorna il puntatore a intero

int* searchCar(int *autonomies,int numCars,int autonomy){
    for (int i = 0; i < numCars; i++) {
        if (autonomies[i] == autonomy) {
            return &autonomies[i];
        }
    }
    return NULL;

}

// creazione della Stazione

tStation *createStation(int distance, int numCars, const int autonomy[]) {

    tStation *newStation = (tStation *)malloc(sizeof(tStation));
    newStation->distance = distance;
    newStation->numCars = numCars;
    if(numCars==0) {
        newStation->autonomies = NULL;

    }else{
        newStation->autonomies = (int *) malloc(sizeof(int) * numCars);
        for (int i = 0; i < numCars; i++) {
            newStation->autonomies[i] = autonomy[i];
        }
    }
    newStation->left = newStation->right = NULL;
    return newStation;

}

//inserimento della Stazione nell'autostrada

tStation *insert(tStation *root, int distance,int numCars,int aut[]) {

    if (root == NULL) {
        return createStation(distance, numCars, aut);
    }

    if (distance < root->distance) {
        root->left = insert(root->left, distance,numCars,aut);
    } else if (distance > root->distance) {
        root->right = insert(root->right, distance,numCars,aut);
    }

    return root;


}

//funzione ausiliaria trova minimo

tStation *findMin(tStation *node) {
    while (node->left != NULL) {
        node = node->left;
    }
    return node;
}

//funzione ausiliaria per eliminare la stazione dall'autostrada

tStation *deleteStation(tStation *root, int distance) {
    if (root == NULL) {
        return root;
    }

    if (distance < root->distance) {
        root->left = deleteStation(root->left, distance);
    } else if (distance > root->distance) {
        root->right = deleteStation(root->right, distance);
    } else {

        if (root->left == NULL) {
            tStation *temp = root->right;
            free(root);
            return temp;
        } else if (root->right == NULL) {
            tStation *temp = root->left;

            free(root);
            return temp;
        }

        tStation *temp = findMin(root->right);
        root->distance = temp->distance;
        root->numCars = temp->numCars;
        root->autonomies = temp->autonomies;

        root->right = deleteStation(root->right, temp->distance);
    }
    return root;
}


//aggiungi stazione


void addStn(tStation* root, int distance,int numCars,int aut[]) {

    if(distance==0){
        root->numCars=numCars;
        root->autonomies = (int *)malloc(sizeof(int) * numCars);
        for (int i = 0; i < numCars; i++) {
            root->autonomies[i] = aut[i];
        }
        printf("aggiunta\n");
    }else if(searchStation(root,distance)==NULL) {

        tStation *newStation = NULL;
        newStation = createStation(distance, numCars, aut);
        insert(root, newStation->distance,numCars,aut);
        printf("aggiunta\n");
    } else printf("non aggiunta\n");
}


// demolisci stazione (successivamente incorporare delete in questa funzione)

void demStn(tStation* root,int distance){

    if(searchStation(root,distance)==NULL){
        printf("non demolita\n");
    } else {
        deleteStation(root, distance);
        printf("demolita\n");
    }

}



//funzione aggiungi macchina

void addCar(tStation *root, int distance,int aut) {

    tStation * station = searchStation(root,distance);

    if (station==NULL) {
        printf("non aggiunta\n");
    }

    else {
        station->numCars++;
        station->autonomies = realloc(station->autonomies, sizeof(int) * station->numCars);
        if (station->autonomies != NULL) {
            station->autonomies[station->numCars - 1] = aut;
            printf("aggiunta\n");
        }else{
            station->autonomies = (int *)malloc(sizeof(int) * station->numCars);
            station->autonomies[0]=aut;
            }

        }
    }


//funzione rottama auto

void scrCar(tStation *root, int distance, int aut) {
    tStation * station = searchStation(root,distance);
    if(station==NULL){
        printf("non rottamata\n");}
    else {
         int* srcCar =  searchCar(station->autonomies,station->numCars,aut);

         if(srcCar==NULL){
        printf("non rottamata\n");
    }else {
        int index = srcCar - station->autonomies;

            if (index< 0 || index >= station->numCars) {
                     printf("Indice non valido.\n");
                     return;
                 }
                 for (int i = index; i < station->numCars - 1; i++) {
                     (station->autonomies)[i] = (station->autonomies)[i + 1];
                 }
                 (station->numCars)--;

                 if(station->numCars==0){
                     free(station->autonomies);
                 }else {

                     // Rialloca la memoria con la nuova dimensione
                     station->autonomies = realloc(station->autonomies, sizeof(int) * (station->numCars));
                 }

             printf("rottamata\n");
        }
    }
}


//funzione che controlla la raggiungibilità

bool reachable(tStation *root, int src,int dst) {

    tStation *srcStation = searchStation(root, src);
    tStation *dstStation = searchStation(root, dst);

    if(srcStation==NULL|| dstStation==NULL){
        return false;
    }

    int numCars = srcStation->numCars;
    int effDist = abs(dst - src);
    int *autonomies = srcStation->autonomies;

    if (numCars <= 0) {

        return false;
    }

    int max = 0;
    for (int i = 0; i < numCars; i++) {

        if (autonomies[i] > max) {
            max = autonomies[i];
        }
        if(max>=effDist){
            return true;
        }

    }
    return false;
}

// ricerca altri nodi raggiungibili e riempe l'array di int per il confronto

void searchAndFill(struct tStation *root, struct tStation *currNode, int lastStop, int dst, int reachableArray[],int* i) {
    if (currNode == NULL) {
        return;
    }

    // Controlla se il nodo corrente soddisfa la condizione  lastStop è la penultima fermata
    if ((currNode->distance < lastStop) && reachable(currNode, currNode->distance,dst) ){
        reachableArray[*i] = currNode->distance;
        (*i)++;
    }

    // Visita il sottoalbero sinistro e destro
    searchAndFill(root,currNode->left,lastStop,dst, reachableArray, i);
    searchAndFill(root,currNode->right, lastStop,dst, reachableArray, i);
}

// ricerca percorso migliore avanti

struct tStation* findNextStop(struct tStation *root,struct tStation *currNode, struct tStation* nextNodeMin, int start, int dst) {

        if (currNode != NULL) {
            nextNodeMin = findNextStop(root,currNode->right, nextNodeMin, start, dst);

            if ((currNode->distance < dst) && (currNode->distance >= start) && (reachable(root,currNode->distance, dst) == true)
                                            && (nextNodeMin == NULL || currNode->distance < nextNodeMin->distance)) {
                nextNodeMin = currNode;
              //  printf("%d ",nextNodeMin->distance);
            }

            nextNodeMin = findNextStop(root,currNode->left, nextNodeMin, start, dst);
        }

        return nextNodeMin;
    }



//implementazione liste per perccorso all'indietro

struct ListNode {
    int key;
    int stepToDest;
    int reachable;
         //puntatore alla stazione
    struct ListNode* prev;
    struct ListNode* next;
};


struct ListNode* createListNode(int key,struct tStation *station) {
    struct ListNode* newNode = (struct ListNode*)malloc(sizeof(struct ListNode));
    newNode->key = key;
    newNode->stepToDest=0;
    newNode->reachable=0;
    newNode->prev = newNode->next = NULL;
    return newNode;
}

// crea la lista ordinata a partire dallìalbero a con testa l'arrivo e coda la partenza

void buildListFromTree(struct tStation* root, struct ListNode** listHead, int minVal, int maxVal) {
    if (root == NULL) {
        return;
    }

    buildListFromTree(root->left, listHead, minVal, maxVal);

    if (root->distance >= minVal && root->distance <= maxVal) {
        struct ListNode* newNode = createListNode(root->distance, root);

        // Inizializza prev e next a NULL per il nuovo nodo
        newNode->prev = newNode->next = NULL;

        if (*listHead == NULL) {
            *listHead = newNode;
        } else {
            // Inserisci il nuovo nodo in ordine crescente rispetto a root->distance
            struct ListNode* current = *listHead;
            struct ListNode* prev = NULL;

            while (current != NULL && current->key < root->distance) {
                prev = current;
                current = current->next;
            }

            // Inserisci il nuovo nodo tra prev e current
            if (prev != NULL) {
                prev->next = newNode;
                newNode->prev = prev;
            } else {
                *listHead = newNode; // Se il nuovo nodo è la nuova testa
            }
            if (current != NULL) {
                current->prev = newNode;
                newNode->next = current;
            }
        }
    }

    buildListFromTree(root->right, listHead, minVal, maxVal);
}


// conta passi necessari alla destinzione r

void countSteps(struct tStation *root, struct ListNode* destination) {
    if (destination == NULL) {
        return;  // Lista vuota, niente da fare
    }

   struct ListNode* last = destination;


    while (last->next != NULL) {
        last = last->next;
    }
  //   adesso last è l'ultimo elemento


     struct ListNode *current = destination->next;
    while (current != NULL) {
        if (reachable(root, current->key, destination->key) == true) {
            current->stepToDest = 1;
        }
        current = current->next;

    }


    int count = 1;

    for(int i = destination->key;i<last->key;i++) {

        if(last->stepToDest!=0){
            break;
        }

        current = destination;


        while (current != NULL ) {

            if (current->stepToDest == count) {
                // è next non può essere prev e dovrei partire dalla fine (più piccola)
                struct ListNode *currBigger = current->next;

                while (currBigger !=NULL) {

                    if (reachable(root, currBigger->key, current->key) == true && currBigger->stepToDest == 0) {
                        currBigger->stepToDest = count + 1;
                    }
                    currBigger = currBigger->next;
                }
            }
            current = current->next;
        }

        count = count + 1;
    }
}

// ricava il max passi per la destinazione

int getNumStepsFromSrc(struct ListNode* head) {
    if (head == NULL) {

        return 0;
    }


    struct ListNode* current = head;


    while (current->next != NULL) {
        current = current->next;
    }


    return current->stepToDest;
}


void brandsReachable(struct tStation *root, struct ListNode* destination,struct ListNode* last){

    struct ListNode* current = last;   //partenza da piu grande e nodo di partenza

    while (current != NULL) {
        if(current==destination){
            return;
        }
        if(current->reachable==1){

        struct ListNode *nextNode = current->prev;  //prossimo nodo nel percorso
        while (nextNode != NULL) {
            if(reachable(root,current->key,nextNode->key)==true
            && nextNode->stepToDest==(current->stepToDest-1)){
                nextNode->reachable=1;

            } nextNode = nextNode->prev;
        }
    }  // se non + raggiungibile vado al prossimo
    current = current->prev;
    }

}



void buildRoute(struct tStation *root, struct ListNode* destination, int maxSteps) {


    struct ListNode* node = destination;
    struct ListNode* last = NULL;

    // Trova l'ultimo elemento della lista
    while (node != NULL) {
        last = node;
        node = node->next;
    }

    last->reachable=1;



    brandsReachable(root, destination,last);

    if(destination->reachable==0){
        printf("nessun percorso\n");
        return;
    }


    int array[maxSteps];
    int currSize = 0;


    struct ListNode* nodeToAdd = destination;

    while(nodeToAdd!=NULL){

        int added=0;
        if(nodeToAdd==last){
            array[currSize]=nodeToAdd->key;

            break;
        }else
         {
             struct ListNode* nodeToEval = nodeToAdd->next;

             while(nodeToEval!=NULL){

                 if(nodeToEval->stepToDest==(nodeToAdd->stepToDest +1)){

                     if(nodeToEval->reachable==1 &&(reachable(root,nodeToEval->key,nodeToAdd->key))){
                         array[currSize]=nodeToEval->key;
                         currSize++;
                         nodeToAdd= nodeToEval;
                         added=1;
                         break;

                     }
                 } nodeToEval= nodeToEval->next;
             }
         }
        if (added== 0){
            nodeToAdd=nodeToAdd->next;}
    }


        for (int i = currSize-1; i >= 0; i--) {
            printf("%d ", array[i]);
        }
        printf("%d\n", destination->key);



}



// libera la lista

void freeList(struct ListNode* head) {
    struct ListNode* current = head;
    while (current != NULL) {
        struct ListNode* next = current->next;
        free(current); // Dealloca il nodo corrente
        current = next;
    }
}



//riempi array percorso
    void fillpath(int* route,int index,int station) {
    route[index] =station;
    }

// pianifica percorso


void plnRoute(tStation *root, int startDist, int endDist) {

    tStation *startStation = searchStation(root, startDist);
    tStation *endStation = searchStation(root, endDist);

    int max;
    max = abs(endDist - startDist);
    int index = 0;
    int found = 1;


    if (startStation == NULL || endStation == NULL) {
        printf("nessun percorso\n");
    } else if(startStation ==endStation ){printf("%d\n",startDist);

    }else{

        if (startDist < endDist) {
            int stationStop = endDist;


            int *route = (int *) malloc(max * sizeof(int));

            struct tStation *stop = endStation;


            while (stop->distance >= startDist) {
                stop = findNextStop(root, root, NULL, startDist, stationStop);
                if (stop == NULL) {
                    printf("nessun percorso\n");
                    found = 0;
                    break;         // se non trovo nodi stampo nessun percorso ed esco

                }
                if (stop->distance == startDist) {
                    stationStop = stop->distance;                    //la fermata è uguale al nodo di ritorno
                    fillpath(route, index, stationStop);            //riempo l'array
                    break;
                }
                stationStop = stop->distance;                    //la fermata è uguale al nodo di ritorno
                fillpath(route, index, stationStop);            //riempo l'array
                index = index + 1;
            }

            if (found == 1) {

                for (int i = index; i >= 0; i--) {
                    printf("%d ", route[i]);
                }
                printf("%d\n", endDist);
            }

            free(route);

        } else {

            //max è la max distanza fra src e dest
            int maxSteps =0;
            struct ListNode* destination = NULL;         //destinazione del percorso (elemento più piccolo della lista)
            buildListFromTree(root, &destination, endDist, startDist);
            countSteps(root, destination);

            //ricava il n max di passi
            maxSteps = getNumStepsFromSrc(destination);

            if (maxSteps==0){
                printf("nessun percorso\n");
                freeList(destination);

            }else{

                buildRoute(root,destination, maxSteps);

                freeList(destination);

        }
    }

}
}
    int main() {

        tStation *root = createStation(0, 0, NULL);
       // root = createStation(0, 0, NULL);

        char riga[10000];
        while (fgets(riga, sizeof(riga), stdin) != NULL) {
            char cmd[30];
            int par[520];
            int numPar = 0;



            //lettura comandi e parametri

            if (sscanf(riga, "%s", cmd) == 1) {
                char *token = strtok(riga + strlen(cmd) + 1, " \t\n"); // Ignora il comando e leggi il resto

                while (token != NULL && numPar < 520) {
                    if (sscanf(token, "%d", &par[numPar]) == 1) {
                        numPar++;
                    }
                    token = strtok(NULL, " \t\n");
                }



                //Esegui operazioni basate sul comando
                if (strcmp(cmd, "aggiungi-stazione") == 0) {

                    int aut[512]; // Supponiamo che l'array di autonomie abbia al massimo 512
                    for (int i = 0; i < par[1]; i++) {
                        aut[i] = par[i + 2];
                    }
                    addStn(root, par[0], par[1], aut);

                } else if (strcmp(cmd, "demolisci-stazione") == 0) {
                    demStn(root, par[0]);

                } else if (strcmp(cmd, "pianifica-percorso") == 0) {
                    plnRoute(root, par[0], par[1]);

                } else if (strcmp(cmd, "rottama-auto") == 0) {
                    scrCar(root, par[0], par[1]);
                } else if (strcmp(cmd, "aggiungi-auto") == 0) {
                    addCar(root, par[0], par[1]);
                }  else if (strcmp(cmd, "quit") == 0) {
                    exit(0);
                } else {
                    printf("Comando sconosciuto: %s\n", cmd);
                }
            }
        }


        return 0;


    }

