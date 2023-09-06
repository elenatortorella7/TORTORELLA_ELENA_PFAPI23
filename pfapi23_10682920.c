
// Created by Utente on 24/08/2023.
//
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX 100




//struttura nodo dell'albero

typedef struct tStation {
    int distance;       // Distanza dalla partenza dell'autostrada chiave
    int numCars;        // Num di macchine disponibili
    int autonomies[512];    //  array di autonomie
    int maxAutonomy;         //autonomia massima
    struct tStation *left; // puntatore a figlio sin
    struct tStation *right;   // punt a ramo destro

} tStation;



/*tStation *findDad(tStation *node, int distance){

        if (node->distance == distance) {
            return NULL;
        } else if (node->right!=NULL && node->right->distance == distance) {
            return node;
        } else if (node->left!=NULL && node->left->distance == distance ) {
            return node;
        } else if (node->distance > distance) {
            node = node->left;
            return findDad(node,distance);
        } else if (node->distance < distance) {
            node = node->right;
            return findDad(node,distance);
        }else {
            printf("errore in find dad\n");
            return NULL;

        }
} */

tStation *findDad(tStation *root, int distance){
    tStation *parent = NULL;
    tStation *current = root;

    while (current != NULL && current->distance != distance) {
        parent = current;
        if (distance < current->distance) {
            current = current->left;
        } else {
            current = current->right;
        }
    }

    return parent;
}



//ricerca di una stazione( se la stazione esiste torna il puntatore alla stazione, altrimenti NULL)


 tStation *searchStation(tStation *root, int distance) {
    while (root != NULL) {
        if (root->distance == distance) {
            return root;
        } else if (root->distance > distance) {
            root = root->left;
        } else {
            root = root->right;
        }
    }

    return NULL;  // Nodo non trovato
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
    newStation->maxAutonomy=0;

    //setta il massimo
    if(numCars!=0) {
        // newStation->autonomies = NULL;
        // newStation->autonomies = (int *) malloc(sizeof(int) * numCars);
        for (int i = 0; i < numCars; i++) {
            newStation->autonomies[i] = autonomy[i];
            if(newStation->autonomies[i]>newStation->maxAutonomy){
                newStation->maxAutonomy=newStation->autonomies[i];
            }
        }


    }
    newStation->left = newStation->right = NULL;
    return newStation;

}

//inserimento della Stazione nell'autostrada

tStation *insert(tStation *root, int distance, int numCars, int aut[]) {
    if (root == NULL) {
        return createStation(distance, numCars, aut); // Crea e restituisci un nuovo nodo
    }

    if (distance < root->distance) {
        root->left = insert(root->left, distance, numCars, aut);
    } else if (distance > root->distance) {
        root->right = insert(root->right, distance, numCars, aut);
    }

    return root;
}

//funzione ausiliaria trova successore

tStation *findMin(tStation *node) {
    while (node->left != NULL) {
        node = node->left;
    }
    return node;
}



//funzione ausiliaria per eliminare la stazione dall'autostrada



tStation *deleteStation(tStation *root, tStation *stationToDel) {
    if (root == NULL) {
        return NULL;
    }

    if (stationToDel->distance < root->distance) {
        root->left = deleteStation(root->left, stationToDel);
    } else if (stationToDel->distance > root->distance) {
        root->right = deleteStation(root->right, stationToDel);
    } else {
        // Nodo da eliminare trovato
        tStation *dad = findDad(root, stationToDel->distance);

        if (stationToDel->left == NULL && stationToDel->right == NULL) {
            // Nodo da eliminare non ha figli
            if (dad != NULL) {
                if (stationToDel == dad->left) {
                    dad->left = NULL;
                } else {
                    dad->right = NULL;
                }
            } else {
                // Nodo da eliminare è la radice
                free(stationToDel);
                return NULL;
            }
            free(stationToDel);
        } else if (stationToDel->left == NULL) {
            // Nodo da eliminare ha solo un figlio destro
            tStation *temporary = stationToDel->right;
            if (dad == NULL) {
                // Nodo da eliminare è la radice
                free(stationToDel);
                return temporary;
            } else {
                if (stationToDel == dad->left) {
                    dad->left = temporary;
                } else {
                    dad->right = temporary;
                }
                free(stationToDel);
            }
        } else if (stationToDel->right == NULL) {
            // Nodo da eliminare ha solo un figlio sinistro
            tStation *temporary = stationToDel->left;
            if (dad == NULL) {
                // Nodo da eliminare è la radice
                free(stationToDel);
                return temporary;
            } else {
                if (stationToDel == dad->left) {
                    dad->left = temporary;
                } else {
                    dad->right = temporary;
                }
                free(stationToDel);
            }
        } else {
            // Nodo da eliminare ha due figli
            tStation *temporary = findMin(stationToDel->right);
            tStation *temporaryDad = findDad(root, temporary->distance);

            // Copia i dati del successore nel nodo corrente
            stationToDel->distance = temporary->distance;
            stationToDel->numCars = temporary->numCars;
            stationToDel->maxAutonomy = temporary->maxAutonomy;
            memcpy(stationToDel->autonomies, temporary->autonomies, sizeof(stationToDel->autonomies));

            if (temporary == temporaryDad->left) {
                // Il successore è il figlio sinistro del padre temporaneo
                if (temporary->right != NULL) {
                    temporaryDad->left = temporary->right;
                } else {
                    temporaryDad->left = NULL;
                }
                free(temporary);
            } else {
                // Il successore è il figlio destro del padre temporaneo
                if (temporary->right != NULL) {
                    temporaryDad->right = temporary->right;
                } else {
                    temporaryDad->right = NULL;
                }
                free(temporary);
            }
        }
    }
    return root;
}



tStation *addStn(tStation* root, int distance,int numCars,int aut[]) {


    if(root==NULL){

        root = createStation(distance, numCars, aut);
        insert(root, root->distance,numCars,aut);
        printf("aggiunta\n");
        return root;                             //chiama insert, che se la radice è nullla crea


    }else if(searchStation(root,distance)==NULL) {

        tStation *newStation = NULL;
        newStation = createStation(distance, numCars, aut);
        insert(root, newStation->distance,numCars,aut);
        printf("aggiunta\n");
    }
    else printf("non aggiunta\n");

    return root;
}


// demolisci stazione (successivamente incorpora delete in questa funzione)

void demStn(tStation* root, int distance){
    int changeRoot =0;
    tStation *srcStn = searchStation(root, distance);
   tStation *resultStn ;
    if( srcStn == NULL){
        printf("non demolita\n");

    } else

    {  if(root->distance==distance){
            changeRoot =1;

        }

       resultStn = deleteStation(root,srcStn);
        if(changeRoot==1) {
            root=resultStn;
            changeRoot=0;
        }

        printf("demolita\n");
    }


}



//funzione aggiungi macchina

void addCar(tStation *root, int distance,int aut) {

    tStation * station = searchStation(root,distance);

    if (station==NULL) {
        printf("non aggiunta\n");
        return;
    }

    else if(station->numCars<512){
        station->numCars++;
        //  station->autonomies = realloc(station->autonomies, sizeof(int) * station->numCars);
        station->autonomies[station->numCars - 1] = aut;
        if(aut>station->maxAutonomy) {
            station->maxAutonomy = aut;
        }
        printf("aggiunta\n");

    }else{printf("non aggiunta\n");}
}


//funzione rottama auto

void scrCar(tStation *root, int distance, int aut) {
    tStation *station = searchStation(root, distance);
    if (station == NULL) {
        printf("non rottamata\n");
        return;
    } else {
        int *srcCar = searchCar(station->autonomies, station->numCars, aut);

        if (srcCar == NULL) {
            printf("non rottamata\n");
            return;
        } else {
            int index = srcCar - station->autonomies;

            // Sposta gli elementi successivi a sinistra per riempire il vuoto
            for (int j = index; j < station->numCars ; j++) {
                station->autonomies[j] = station->autonomies[j + 1];
            }
            station->numCars--;

            //find max
            station->maxAutonomy = station->autonomies[0];
            for (int i = 1; i < station->numCars; i++) {

                if (station->autonomies[i] > station->maxAutonomy) {
                    station->maxAutonomy = station->autonomies[i];
                }
            }
            printf("rottamata\n");

        }
    }
}


//funzione che controlla la raggiungibilità



// ricerca altri nodi raggiungibili e riempe l'array di int per il confronto



// ricerca percorso migliore avanti
//all'inizio c'era root

struct tStation* findNextStop(struct tStation *currNode, struct tStation* nextNodeMin,tStation* destNode,int start) {

    if (currNode != NULL) {
        if ((currNode->distance >= start) && (currNode->distance < destNode->distance) && (currNode->maxAutonomy>=(destNode->distance - currNode->distance))
            && (nextNodeMin == NULL || currNode->distance < nextNodeMin->distance)) {
            nextNodeMin = currNode;
        }

        if (currNode->distance >= start) {
            nextNodeMin = findNextStop(currNode->left, nextNodeMin, destNode,start );
        }

        if (currNode->distance < destNode->distance) {
            nextNodeMin = findNextStop(currNode->right, nextNodeMin, destNode,start);
        }
    }

    return nextNodeMin;
}



struct tStation* findNextStopReverse(struct tStation *currNode, struct tStation* nextNodeMax, tStation* destNode,int start) {
    // è grande start è grande dst è piccola

    if (currNode != NULL) {

        if ((currNode->distance <= start) && (currNode->distance > destNode->distance) && (currNode->maxAutonomy>=(currNode->distance-destNode->distance))
            && (nextNodeMax == NULL|| currNode->distance > nextNodeMax->distance)) {
            nextNodeMax = currNode;

            //  printf("%d ",nextNodeMax->distance);
        }
        if(currNode->distance>destNode->distance) {
            nextNodeMax = findNextStopReverse(currNode->left, nextNodeMax, destNode, start);
        }

        if(currNode->distance<=start){

            nextNodeMax = findNextStopReverse( currNode->right, nextNodeMax, destNode,start);
        }
    }

    return nextNodeMax;
}





//riempi array percorso
void fillPath(int* route, int index, int station) {
    route[index] =station;
}

void fillPathStation(tStation*route[],int index,tStation *station){
    route[index]=station;
}




struct tStation* findPredecessor(struct tStation* root, int valMagg) {
    struct tStation* predecessor = NULL;

    while (root != NULL) {
        if (valMagg > root->distance) {
            predecessor = root;
            root = root->right;
        } else if (valMagg < root->distance) {
            root = root->left;
        } else {

            if (root->left != NULL) {

                struct tStation* temp = root->left;
                while (temp->right != NULL) {
                    temp = temp->right;
                }
                predecessor = temp;
            }
            break;
        }
    }

    return predecessor;
}




struct tStation* findSuccessor(struct tStation* root, int minValue) {
    struct tStation* successor = NULL;

    while (root != NULL) {
        if (minValue < root->distance) {
            successor = root;
            root = root->left;
        } else if (minValue > root->distance) {
            root = root->right;
        } else {

            if (root->right != NULL) {

                root = root->right;
                while (root->left != NULL) {
                    root = root->left;
                }
                successor = root;
            }
            break;
        }
    }

    return successor;
}



// pianifica percorso

void plnRoute(tStation *root, int startDist, int endDist) {

    tStation *startStation = searchStation(root, startDist);
    tStation *endStation = searchStation(root, endDist);


    int max;//forse dopo porre=0
    max = abs(endDist - startDist);
    int index = 0;
    int found = 1;

    if(startStation->numCars==0){
        printf("nessun percorso\n");
        return;
    }
    if (startStation == NULL || endStation == NULL) {
        printf("nessun percorso\n");
        return;
    } else if (startStation == endStation) {
        printf("%d\n", startDist);
        return;

    } else {

        if (startDist < endDist) {
            int stationStop = endDist;


            int *route = (int *) malloc(max * sizeof(int));
            struct tStation *stop = endStation;


            while (stop->distance >= startDist) {
                stop = findNextStop( root, NULL,stop, startDist);
                if (stop == NULL) {
                    printf("nessun percorso\n");
                    found = 0;
                    break;         // se non trovo nodi stampo nessun percorso ed esco

                }
                if (stop->distance == startDist) {
                    stationStop = stop->distance;                    //la fermata è uguale al nodo di ritorno
                    fillPath(route, index, stationStop);            //riempo l'array
                    break;
                }
                stationStop = stop->distance;                    //la fermata è uguale al nodo di ritorno
                fillPath(route, index, stationStop);            //riempo l'array
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

            //primo percorso


            tStation **route= (struct tStation**)malloc(MAX*sizeof(struct tStation*));

            index=0;

            struct tStation *stop = endStation;


            while (stop->distance >=endDist) {
                stop = findNextStopReverse(root, NULL, stop,startDist);//va sistemata  questa f
                if (stop == NULL) {
                    printf("nessun percorso\n");
                    found = 0;
                    break;         // se non trovo nodi stampo nessun percorso ed esco
                }
                if (stop->distance == startDist) {
                    fillPathStation(route, index, startStation);            //riempo l'array
                    break;
                }
                //   stationStop = stop->distance;
                fillPathStation(route, index, stop);            //riempo l'array
                index = index + 1;
            }

            if (found == 1) {
                // passo 2

                int steps= index+1;

                struct  tStation *imprRoute[steps+1];  //prima ero solo steps

                // fillo imprRoute dopo i risultati di algoritmo 1

                for(int j=steps;j>0;j--){

                    imprRoute[j]=route[j-1];
                }

                imprRoute[0]=endStation;
                tStation *stationToEval = findPredecessor(root,  route[index]->distance);
                tStation *currentStation = imprRoute[steps];//è il nuovo array current è lontana
                tStation *nextStop = imprRoute[0];


                int newIndex = steps-1;

                while (currentStation != nextStop) {

                    int distMax = 0;

                    while (stationToEval != nextStop) {

                        //prima cond è reachable
                        if (currentStation->maxAutonomy>=(currentStation->distance - stationToEval->distance)
                            && ((stationToEval->maxAutonomy + currentStation->distance - stationToEval->distance) >
                                distMax)) {

                            distMax = stationToEval->maxAutonomy + currentStation->distance - stationToEval->distance;
                            imprRoute[newIndex] = stationToEval;

                        }
                        stationToEval = findPredecessor(root,stationToEval->distance);
                    }

                    currentStation = imprRoute[newIndex];
                    stationToEval = findPredecessor(root,currentStation->distance);
                    newIndex--;

                    if(newIndex==0){
                        imprRoute[0]=nextStop;
                        break;
                    }

                }

                // passo 3

                tStation *betweenStation= findSuccessor(root, imprRoute[0]->distance);
                for (int k = 0; k<=(steps-2); k++) {


                    while (betweenStation->distance < imprRoute[k +1]->distance) {

                        //la prima condizione è la raggiungibilità
                        if (betweenStation->maxAutonomy>=(betweenStation->distance-imprRoute[k]->distance) &&
                            imprRoute[k+2]->maxAutonomy>= (imprRoute[k+2]->distance-betweenStation->distance)) {
                            imprRoute[k + 1] = betweenStation;
                            break;
                        }
                        betweenStation = findSuccessor(root, betweenStation->distance);
                    }
                }


                for(int k= steps;k>0;k--){
                    printf("%d ",imprRoute[k]->distance);
                }
                printf("%d\n",imprRoute[0]->distance);

            }
        }
    }
}


int main() {

    tStation *root = NULL;                                           //createStation(0, 0, NULL);
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

                int aut[512];
                for (int i = 0; i < par[1]; i++) {
                    aut[i] = par[i + 2];
                }
                if(root==NULL){
                    root=addStn(root, par[0], par[1], aut);
                }else {
                    addStn(root, par[0], par[1], aut);
                }


            } else if (strcmp(cmd, "demolisci-stazione") == 0) {
                demStn(root, par[0]);

            } else if (strcmp(cmd, "pianifica-percorso") == 0) {
                plnRoute(root, par[0], par[1]);

            } else if (strcmp(cmd, "rottama-auto") == 0) {
                scrCar(root, par[0], par[1]);
            } else if (strcmp(cmd, "aggiungi-auto") == 0) {
                addCar(root, par[0], par[1]);
            } else if (strcmp(cmd, "quit") == 0) {
                exit(0);
            } else {
                printf("Comando sconosciuto: %s\n", cmd);
            }
        }
    }


    return 0;


}