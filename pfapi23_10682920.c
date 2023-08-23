#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>



/*

#define MAX_CMD_LEN 32
static const char *addStnCmd    = "aggingi-stazione ";
static const char *demStnCmd = "demolisci-stazione";
static const char *addCarCmd   = "aggiungi-auto";
static const char *scrCarCmd    = "rottama-auto";
static const char *plnRuteCmd = "pianifica-percorso";  */

typedef struct{
    int aut;  //autonomia auto
}Car;

typedef struct{
    int distance;   // distanza dall'inizio univoca
    int nCar;       // numero di macchine disponibili
    Car car[512];    // massimo numero di macchine per stazione
}Station;

/*typedef struct {
    int distance;
    int numStations;
    Station *stations;
} Highway;                              // insieme delle stazioni   */


// tree   + creazione di una coda per la ricerca in ampiezza

typedef struct tNode {
    int distance; // Distanza dalla partenza dell'autostrada
    int numCars; // Num di macchine disponibili
    int *autonomies; // puntatore ad array di autonomie
    struct tNode *left;
    struct tNode *right;
    struct tNode *parent;
} tNode;


typedef struct qNode {
    tNode *node;
    int numSteps;
    struct qNode *next;
} qNode;    //quequenode


// inserimento

tNode *insert(tNode *root, int distance, int numCars, int *autonomy, tNode *parent) {
    if (root == NULL) {
        tNode *newNode = (tNode *)malloc(sizeof(tNode));
        newNode->distance = distance;
        newNode->numCars = numCars;
        newNode->autonomies = (int *)malloc(sizeof(int) * numCars);
        for (int i = 0; i < numCars; i++) {
            newNode->autonomies[i] = autonomy[i];
        }
        newNode->left = newNode->right = NULL;
        newNode->parent = parent; // Imposta il riferimento al nodo padre
        return newNode;
    }

    if (distance < root->distance) {
        root->left = insert(root->left, distance, numCars, autonomy, root);
    } else if (distance > root->distance) {
        root->right = insert(root->right, distance, numCars, autonomy, root);
    }

    return root;
}


// ricerca valore minimo


tNode *findMin(tNode *node) {
    while (node->left != NULL) {
        node = node->left;
    }
    return node;
};


// eliminazione del nodo

tNode *deleteNode(tNode *root, int distance) {
    if (root == NULL) {
        return root;
    }

    // Esegui la ricerca del nodo da eliminare
    if (distance < root->distance) {
        root->left = deleteNode(root->left, distance);
    } else if (distance > root->distance) {
        root->right = deleteNode(root->right, distance);
    } else {

        // Il nodo con il valore specifico è stato trovato, procedi all'eliminazione
        if (root->left == NULL) {
            tNode *temp = root->right;
            if (temp != NULL) {
                temp->parent = root->parent;
            }
            free(root);
            return temp;
        } else if (root->right == NULL) {
            tNode *temp = root->left;
            if (temp != NULL) {
                temp->parent = root->parent;
            }
            free(root);
            return temp;
        }

        tNode *temp = findMin(root->right);
        root->distance = temp->distance;
        root->numCars = temp->numCars;
        root->autonomies = temp->autonomies;

        root->right = deleteNode(root->right, temp->distance);
    }
    return root;
}



// ricerca del nodo

tNode *search(tNode *root, int disVal) {
    if (root == NULL || root->distance == disVal) {
        return root;
    }

    if (disVal < root->distance) {
        return search(root->left, disVal);
    } else {
        return search(root->right, disVal);
    }
}


// gestione coda


qNode *createQueue() {
    return NULL;
}

// Verifica se la coda è vuota
bool isEmpty(qNode *queue) {
    return queue == NULL;
}

// Inserisce un nodo in coda alla coda
void enqueue(qNode **queue, tNode *node, int numSteps) {
    qNode *newNode = malloc(sizeof(qNode));
    newNode->node = node;
    newNode->numSteps = numSteps;
    newNode->next = NULL;

    if (*queue == NULL) {
        *queue = newNode;
    } else {
        qNode *current = *queue;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newNode;
    }
}

// Rimuove e restituisce il nodo in testa alla coda
qNode *dequeue(qNode **queue) {
    if (*queue == NULL) {
        return NULL;
    }
    qNode *frontNode = *queue;
    *queue = (*queue)->next;
    return frontNode;
}


 //funzione che trova il percorso più breve

void findShortestRoute(tNode *root, tNode *startNode, tNode *endNode) {

    qNode *queue = NULL;
    enqueue(&queue, startNode, 0);

    int *srtstRoute = NULL;
    int srtstRouteLngth = INT_MAX;
    tNode *nrstStnToZero = NULL;

    while (!isEmpty(queue)) {
        qNode *curr = dequeue(&queue);
        tNode *node = curr->node;
        int numSteps = curr->numSteps;

        if (node == endNode) {
            int *route = malloc(sizeof(int) * (numSteps + 1));
            int routeLngth = 0;

            tNode *currentRouteNode = endNode;
            while (currentRouteNode != startNode) {
                route[routeLngth++] = currentRouteNode->distance;
                currentRouteNode = currentRouteNode->parent;
            }
            route[routeLngth++] = startNode->distance;

            if (routeLngth < srtstRouteLngth ||
                (routeLngth == srtstRouteLngth && route[1] < nrstStnToZero->distance)) {
                free(srtstRoute);
                srtstRoute = route;
                srtstRouteLngth = routeLngth;
                nrstStnToZero = search(root, route[1]);
            } else {
                free(route);
            }

            continue;
        }

        for (int i = 0; i < node->numCars; i++) {
            int nextDistance = node->distance + node->autonomies[i];
            tNode *nextNode = search(root, nextDistance);
            if (nextNode != NULL) {
                enqueue(&queue, nextNode, numSteps + 1);
                nextNode->parent = node; // Aggiungi il collegamento al padre
            }
        }

        free(curr);
    }

    if (srtstRoute != NULL) {
        for (int i = srtstRouteLngth - 1; i >= 0; i--) {
            printf("%d ", srtstRoute[i]);
        }
        printf("\n");
        free(srtstRoute);
    } else {
        printf("nessun percorso");
    }
}



void addStn(tNode *root,int distance, int numCars, int *aut ){
   if( search(root, distance) == NULL) {
       *insert(root, distance, numCars, aut,root);
       printf("aggiunta");
   }
   else printf("non aggiunta");

};

void demStn(tNode *root , int distance){
    if( search(root, distance) == NULL) {
        printf("non demolita");
    }
        else {
            *deleteNode(root, distance);
            printf("demolita");
        }


};

// creare la funzione che aggiunge le macchine




void addCar(tNode *root,int distance,int aut){

    tNode *station= search(root,distance);
    int numCars= station->numCars;

    if( search(root, distance) == NULL || numCars==512){
        printf("non aggiunta");
    }
    else{
            station->numCars++; // Incrementa la dimensione dell'array
        station->autonomies = realloc(station->autonomies, sizeof(int) * station->numCars);
        if (station->autonomies!= NULL) {
            station->autonomies[station->numCars - 1] = aut;                        // Aggiunge l'elemento all'array
        }
    }
        printf("non aggiunta");
};




// funzione che cerca un valore autonomia nell'array

bool searchAut(int array[], int size, int aut) {
    for (int i = 0; i < size; i++) {
        if (array[i] == aut) {
            return true;
        }
    }
    return false;
}

void removeAut(int *array, int size, int autToRemove) {
    int found = 0;
    for (int i = 0; i < size; i++) {
        if (!found && array[i] == autToRemove) {
            found = 1;
        }
        if (found && i < size - 1) {
            array[i] = array[i + 1]; // Sposta gli elementi successivi a sinistra
        }
    }
    if (found) {
        (size)--;
    }
}


void scrCar(tNode *root, int distance, int aut){

    tNode *station= search(root,distance);

    if(search(root, distance) == NULL || searchAut(station->autonomies,station->numCars,aut)==false){
        printf("non rottamata");
        }
    else if (station->distance== distance && searchAut(station->autonomies,station->numCars,aut)==true) {

        removeAut(station->autonomies,station->numCars,aut);

        printf("rottamata");
     }

};

void pltRoute(tNode *root, int startDist,int endDist) {

    tNode *startNode = search(root, startDist);
    tNode *endNode = search(root, endDist);
    if (startNode == NULL || endNode == NULL) {
        printf("nessun percorso");
    } else

    {
        findShortestRoute(root, startNode, endNode);   //funzione che trova la rotta e la stampa

    }
}



int main(int argc, char *argv[]){

    tNode *root = NULL;

    // Creazione della radice con valore 0 e nessuna macchina
    int initDist = 0;
    int initNumCars = 0;
    int *initAutonomy = NULL;

    root = insert(root, initDist, initNumCars, initAutonomy,NULL);



    FILE *file_input = fopen("comandi.txt", "r");

        if (file_input == NULL) {
            printf("Impossibile aprire il file\n");
            return 1;
        }

        char riga[100];
        while (fgets(riga, sizeof(riga), file_input) != NULL) {
            char cmd[30];   //dim max cmd
            int par[520]; // Supponiamo un massimo di 10 parametri
            int num_par = 0;

            //lettura comandi e parametri

            if (sscanf(riga, "%s", cmd) == 1) {
                char *token = strtok(riga + strlen(cmd) + 1, " \t\n"); // Ignora il comando e leggi il resto

                while (token != NULL && num_par < 520) {
                    if (sscanf(token, "%d", &par[num_par]) == 1) {
                        num_par++;
                    }
                    token = strtok(NULL, " \t\n");
                }

                // Esegui operazioni basate sul comando
                if (strcmp(cmd, "aggiungi-stazione") == 0) {
                    addStn(root,par[0], par[1], &par[2]);
                } else if (strcmp(cmd, "demolisci-stazione") == 0) {
                    demStn(root,par[0]);
                } else if (strcmp(cmd, "pianifica-percorso") == 0) {
                    pltRoute(root,par[0], par[1]);
                } else if (strcmp(cmd, "rottama-auto") == 0) {
                    scrCar(root,par[0], par[1]);
                } else if (strcmp(cmd, "aggiungi-auto") == 0) {
                    addCar(root,par[0], par[1]);
                } else {
                    printf("Comando sconosciuto: %s\n", cmd);
                }
            }
        }

        fclose(file_input);


        return 0;

}
