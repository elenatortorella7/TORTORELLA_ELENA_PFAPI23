
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

//strct nodo del grafo

struct routeNode {
    int distance;   //univoco
   // struct Edge* edges;
  //  int weight;
    struct routeNode* next;   //puntatore al prossimo nodo
};

/*struct Edge {
    struct Node* target;
    struct Edge* next;
};

 */


/*
struct Route{
    int length;
    struct routeNode** stations;
};
*/



// stampa l'autostrada,funzione per il check
void inorderTraversal(tStation *root) {
    if (root != NULL) {
        inorderTraversal(root->left);
        printf("il nodo è: %d il suo precedente é %p, il suo successivo è: %p\n ", root->distance,(void*)root->left,(void*) root->right);
        inorderTraversal(root->right);
    }
}

void inorderTraversalFromNodeDescending(tStation *node) {
    if (node != NULL) {
        inorderTraversalFromNodeDescending(node->right);
        printf("%d ", node->distance);
        inorderTraversalFromNodeDescending(node->left);
    }
}



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

void provaStampaContrario(tStation *root,int distance){
    tStation *node =searchStation(root,distance);
    inorderTraversalFromNodeDescending(node);

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
    newStation->autonomies = (int *)malloc(sizeof(int) * numCars);
    for (int i = 0; i < numCars; i++) {
        newStation->autonomies[i] = autonomy[i];
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

        // Il nodo con il valore specifico è stato trovato, procedi all'eliminazione
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





void addStn(tStation* root, int distance,int numCars,int aut[]) {

    if(searchStation(root,distance)==NULL) {

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

             // Rialloca la memoria con la nuova dimensione
                 station->autonomies = realloc(station->autonomies, sizeof(int) * (station->numCars));

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

    int max = autonomies[0];
    for (int i = 1; i < numCars; i++) {

        if (autonomies[i] > max) {
            max = autonomies[i];
        }
    }
    if (max >= effDist) {

        return true;
    }

        return false;

}



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


struct tStation* findNextStopReverse(struct tStation *root,struct tStation *currNode, struct tStation* nextNodeMin, int start, int dst) {


    if (currNode != NULL) {
        nextNodeMin = findNextStop(root,currNode->right, nextNodeMin, start, dst);  //li devo invertire alla chiamata

        if ((currNode->distance < dst) && (currNode->distance >= start) && (reachable(root, dst,currNode->distance) == true)
            && (nextNodeMin == NULL || currNode->distance < nextNodeMin->distance)) {
            nextNodeMin = currNode;
            //  printf("%d ",nextNodeMin->distance);
        }

        nextNodeMin = findNextStop(root,currNode->left, nextNodeMin, start, dst);
    }

    return nextNodeMin;
}


void fillpath(int* route,int index,int station) {

    route[index] =station;

}


void plnRoute(tStation *root, int startDist, int endDist) {

    tStation *startStation = searchStation(root, startDist);
    tStation *endStation = searchStation(root, endDist);

    int max;
    max = abs(endDist - startDist);
    int index = 0;
    int found = 1;


    if (startStation == NULL || endStation == NULL) {
        printf("nessun percorso\n");
    } else {

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
               // printf("%d ", startDist);
                for (int i = index; i >= 0; i--) {
                    printf("%d ", route[i]);
                }
                printf("%d\n", endDist);
            }

            free(route);

        }else{
            int *route = (int *) malloc(max * sizeof(int));

            int stationStop = startDist;

            struct tStation *stop = startStation;


            while (stop->distance >= endDist) {
                stop = findNextStopReverse(root, root, NULL, endDist, stationStop);
                if (stop == NULL) {
                    printf("nessun percorso\n");
                    found = 0;
                    break;         // se non trovo nodi stampo nessun percorso ed esco

                }
                if (stop->distance == endDist) {
                    stationStop = stop->distance;                    //la fermata è uguale al nodo di ritorno
                    fillpath(route, index, stationStop);            //riempo l'array
                    break;
                }
                stationStop = stop->distance;                    //la fermata è uguale al nodo di ritorno
                fillpath(route, index, stationStop);            //riempo l'array
                index = index + 1;
            }

            if (found == 1) {
                printf("%d ", startDist);
                for (int i = 0; i <= index; i++) {
                    printf("%d ", route[i]);
                }
                //printf("%d\n", endDist);
            }

            free(route);

        }
    }
}


    int main() {

        tStation *root = NULL;    //inizializzo la radice a null
        root = createStation(0, 0, NULL);

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
                } else if (strcmp(cmd, "stampa-albero") == 0) {
                    inorderTraversal(root);
                } else if (strcmp(cmd, "stampa-albero-contrario") == 0) {

                    provaStampaContrario(root,par[0]);
                }else if (strcmp(cmd, "quit") == 0) {
                    exit(0);
                } else {
                    printf("Comando sconosciuto: %s\n", cmd);
                }
            }
        }

        return 0;


    }

