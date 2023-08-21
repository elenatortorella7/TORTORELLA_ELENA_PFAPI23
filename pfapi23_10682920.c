#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* #ifndef IO_H
#define IO_H

#define API_INLINE static inline __attribute__((always_inline)) __attribute__((unused))

static void io_init(void);
static void io_printLine(const char *str);
static void io_printInt(int n);
static void io_readLine(char *str, size_t strSz);
static int io_readInt(void);

#if defined(IO_FAST)

API_INLINE void io_init(void)
{
}

API_INLINE void io_printLine(const char *str)
{
  puts(str);
}

API_INLINE void io_printInt(int n)
{
  printf("%d\n", n);
}

API_INLINE void io_readLine(char *str, size_t strSz)
{
  fgets(str, strSz, stdin);
}

API_INLINE int io_readInt(void)
{
  char tmp[32];
  fgets(tmp, 32, stdin);
  return atoi(tmp);
}

#else // defined(IO_PRINTF)

API_INLINE void io_init(void)
{
}

API_INLINE void io_printLine(const char *str)
{
    printf("%s\n", str);
}

API_INLINE void io_printInt(int n)
{
    printf("%d\n", n);
}

API_INLINE void io_readLine(char *str, size_t strSz)
{
    int n = scanf("%s", str);
    (void)n;
}

API_INLINE int io_readInt(void)
{
    int res;
    int n = scanf("%d", &res);
    (void)n;
    return res;
}

#endif

#endif
 */




#define MAX_CMD_LEN 32
static const char *addStnCmd    = "aggingi-stazione";
static const char *demStnCmd = "demolisci-stazione";
static const char *addCarCmd   = "aggiungi-auto";
static const char *scrCarCmd    = "rottama-auto";
static const char *plnRuteCmd = "pianifica-percorso";

typedef struct{
    int aut;  //autonomia auto
}Car;

typedef struct{
    int distance;   // distanza dall'inizio univoca
    int nCar;       // numero di macchine disponibili
    Car car[512];    // massimo numero di macchine per stazione
}Station;

typedef struct {
    int distance;
    int numStations;
    Station *stations;
} Highway;


// tree

typedef struct tNode {
    int distance; // Distanza dalla partenza dell'autostrada
    int numCars; // Numero di veicoli disponibili
    int *autonomies; // Array di autonomie dei veicoli
    struct tNode *left;
    struct tNode *right;
} tNode;




int addStn(Highway *highway, int distance, int numCar, int *aut ){};
int demStn(Highway *highway, int distance){};
int addCar(Highway *highway, int distance,int aut){};
int scrCar(Highway *highway, int distance, int aut){};
int pltRute(Highway *highway, int distance){};


int main() {
    printf("Hello, World!\n");
    return 0;
}
