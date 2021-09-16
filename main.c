#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

typedef struct rankTab {
    int position;
    long int value;
} rankTab;

typedef enum {
    false, true
} boolean;

typedef struct distTab {
    long int dist;
    boolean check;
} distTab;

void siftDown(rankTab heap[], int k, int i) {
    int sx = i * 2 + 1;
    int dx = i * 2 + 2;
    int tmp1;
    int tmp2;
    int max = i;

    if (sx < k) {
        if (heap[max].value < heap[sx].value)
            max = sx;
    }

    if (dx < k) {
        if (heap[max].value < heap[dx].value)
            max = dx;
    }

    if (max == sx || max == dx) {
        tmp1 = heap[max].value;
        tmp2 = heap[max].position;
        heap[max].value = heap[i].value;
        heap[max].position = heap[i].position;
        heap[i].value = tmp1;
        heap[i].position = tmp2;
        siftDown(heap, k, max);
    }
}

void siftUp(rankTab heap[], int k, int i) {
    int father = (i - 1) >> 1; // (i - 1) / 2
    int tmp1;
    int tmp2;
    if (father >= 0) {
        if (heap[father].value < heap[i].value) {
            tmp1 = heap[father].value;
            tmp2 = heap[father].position;
            heap[father].value = heap[i].value;
            heap[father].position = heap[i].position;
            heap[i].value = tmp1;
            heap[i].position = tmp2;
            siftUp(heap, k, father);
        }
    }
}

void resetTable(distTab tab[], long int d) {
    for (int i = 0; i < d; i++) { // initializing table
        tab[i].dist = LONG_MAX;
        tab[i].check = false;
    }
    tab[0].dist = 0; // source vertex set to 0
}

void shortestPath(distTab tab[], long int d, long int matrix[d][d]) {
    int a = 0;
    long int distMin;
    for (int node = 0; node < d - 1; node++) {
        distMin = LONG_MAX;
        for (int q = 0; q < d; q++) {
            if (tab[q].dist <= distMin && tab[q].check == false) {
                distMin = tab[q].dist;
                a = q;
            }
        }
        // a is the nearest vertex
        tab[a].check = true;

        for (int b = 0; b < d; b++) { // a->b
            if (matrix[a][b] != 0 && tab[b].check == false && tab[a].dist != LONG_MAX &&
                tab[b].dist > tab[a].dist + matrix[a][b])
                tab[b].dist = tab[a].dist + matrix[a][b];
        }
    }
}

int sumPath(distTab *tab, int d) {
    int i;
    int val = 0;
    for (i = 0; i < d; i++) {
        if (tab[i].dist != LONG_MAX)
            val = val + tab[i].dist;
    }
    return val;
}

int main() {
    long int d, k;
    char c;
    int i;
    int val;
    int kEff;
    int index = 0;
    int elem = scanf("%ld %ld", &d, &k);

    if ((d < 1 || d > LONG_MAX) || (k < 0 || k > LONG_MAX) || elem != 2)
        exit(0);

    long int matrix[d][d];
    distTab totalDist[d];
    rankTab totalGraphs[k];
    c = getchar_unlocked();
    c = getchar_unlocked();
    while ((c == 'A' || c == 'T') && !feof(stdin)) {
        if (c == 'A') { // enters AddGraph instruction
            for (i = 0; i < 7; i++) {
                getchar_unlocked();
            }
            val = 0;
            int tmp;
            for (i = 0; i < d; i++) {
                for (int j = 0; j < d; j++) {
                    tmp = 0;
                    for (c = getchar_unlocked(); c != ',' && c != '\n'; c = getchar_unlocked()) {
                        tmp = tmp * 10 + c - '0';
                        matrix[i][j] = tmp;
                }
            }
            // RESET THE TABLE
            resetTable(totalDist, d);

            // FIND THE SHORTEST PATH
            shortestPath(totalDist, d, matrix);

            // SUM THE SHORTEST PATHS
            val = sumPath(totalDist, d);

            // UPDATE TOPK ARRAY
            if (index < k) { //altrimenti, se la tabella è da finire di riempire, aggiorno
                totalGraphs[index].value = val;
                totalGraphs[index].position = index;
                siftUp(totalGraphs, k, index);
            }
            if (index >= k && val < totalGraphs[0].value) { //se la tabella è piena devo scremare
                totalGraphs[0].value = val;
                totalGraphs[0].position = index;
                siftDown(totalGraphs, k, 0);
            }
            index++;
        }

        if (c == 'T') { // enters TopK command
            for (i = 0; i < 4; i++) {
                getchar_unlocked();
            }
            if (index < k)
                kEff = index;
            else
                kEff = k;
            for (i = 0; i < kEff - 1; i++) {
                printf("%d ", totalGraphs[i].position);
            }
            printf("%d\n", totalGraphs[kEff - 1].position);
        }
        c = getchar_unlocked();
    } // out of general while
    return 0;
}