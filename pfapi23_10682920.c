
// Created by Utente on 24/08/2023.
//
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


//struttura nodo dell'albero

typedef struct tStation {
    int distance;       // Distanza dalla partenza dell'autostrada chiave
    int numCars;        // Num di macchine disponibili
    int *autonomies;    // puntatore ad array di autonomie
    struct tStation *left; // puntatore a figlio sin
    struct tStation *right;   // punt a ramo destro

} tStation;


void inorderTraversal(tStation *root) {
    if (root != NULL) {
        inorderTraversal(root->left);
        printf("%d ", root->distance);
        inorderTraversal(root->right);
    }
}


//ricerca di una stazione  ( se la stazione esiste torna il puntatore alla stazione, altrimenti NULL)

tStation *searchStation(tStation *root, int distance) {
    // Caso base: l'albero è vuoto o la chiave è stata trovata nella radice
    if (root == NULL || root->distance == distance) {
        return root;
    }

    // Se la chiave è minore della chiave della radice, cerca nel sottoalbero sinistro
    if (distance < root->distance) {
        return searchStation(root->left, distance);
    }

    // Altrimenti, cerca nel sottoalbero destro
    return searchStation(root->right, distance);
}


//funzione che cerca l'autonomia e ne ritorna il puntatore a intero

int* searchCar(int *autonomies,int numCars,int autonomy){
    for (int i = 0; i < numCars; i++) {
        if (autonomies[i] == autonomy) {
            return &autonomies[i]; // Restituisce l'indice dell'elemento trovato
        }
    }
    return NULL;

}

// void



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

tStation *findMin(tStation *node) {
    while (node->left != NULL) {
        node = node->left;
    }
    return node;
}

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








void demStn(tStation* root,int distance){

    if(searchStation(root,distance)==NULL){
        printf("non demolita\n");
    } else {
        deleteStation(root, distance);
        printf("demolita");
    }

}

void addCar(tStation *root, int distance,int aut) {

    tStation * station = searchStation(root,distance);

    if (station==NULL) {
        printf("non aggiunta\n");
    }

    else {

        station->numCars++; // Incrementa la dimensione dell'array
        station->autonomies = realloc(station->autonomies, sizeof(int) * station->numCars);
        if (station->autonomies != NULL) {
            station->autonomies[station->numCars - 1] = aut; // Aggiunge l'elemento all'array
            printf("aggiunta\n");
        }

    }
}


void scrCar(tStation *root, int distance, int aut) {
    tStation * station = searchStation(root,distance);
    if(station==NULL){
        printf("non rottamata");}
    else {
         int* srcCar =  searchCar(station->autonomies,station->numCars,aut);

         if(srcCar==NULL){
        printf("non rottamata");
    }else {
        int index = srcCar - station->autonomies; // Calcola l'indice dell'elemento da eliminare

                 if (index< 0 || index >= station->numCars) {
                     printf("Indice non valido.\n");
                     return;
                 }

                 for (int i = index; i < station->numCars - 1; i++) {
                     (station->autonomies)[i] = (station->autonomies)[i + 1]; // Sposta gli elementi successivi
                 }

                 (station->numCars)--; // Riduci la lunghezza dell'array di 1

                 station->autonomies = realloc(station->autonomies, sizeof(int) * (station->numCars)); // Rialloca la memoria con la nuova dimensione

             printf("rottamata");
        }

    }



}

void plnRoute(tStation *root, int startDis, int endDis) {
    printf("nessun percorso");
}


int main() {

    tStation *root = NULL;    //inizializzo la radice a null
    root = createStation(0, 0, NULL);

    char riga[100];
    while (fgets(riga, sizeof(riga), stdin) != NULL) {
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
            } else if (strcmp(cmd, "quit") == 0) {
                exit(0);
            } else {
                printf("Comando sconosciuto: %s\n", cmd);
            }
        }
    }

    return 0;


}
