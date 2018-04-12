/*
 * MIT License
 *
 * Copyright (c) 2018 Kalate Hexanome, 4IF, INSA Lyon
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
*/
#include <stdint.h>
#include <stdio.h>


void displayNumber(int32_t number) {
    int32_t tab[50] = {0};
    int32_t index = 0;
    int32_t i;

    // Negative numbers
    if (number < 0) {
        putchar('-');
        number *= -1;
    }

    while (number >= 1) {
        tab[index] = number % 10;
        number /= 10;
        index++;
    }
    if (index == 0) { // special case for displayNumber(0)
        index = 1;
    }

    for (i = index; i > 0; i--) {
        putchar('0' + tab[i - 1]);
    }
}

void displayTab(int32_t tab[], int32_t size) {
    int32_t i;
    for (i = 0; i < size; i++) {
        displayNumber(tab[i]);
        putchar(' ');
    }
    putchar('\n');
}

void echanger(int32_t tableau[], int32_t a, int32_t b, int32_t size) {
    int32_t temp = tableau[a];
    tableau[a] = tableau[b];
    tableau[b] = temp;

    displayTab(tableau, size);
}

void quickSort(int32_t tableau[], int32_t debut, int32_t fin, int32_t size) {
    int32_t gauche = debut - 1;
    int32_t droite = fin + 1;
    int32_t pivot = tableau[debut];

    /* Si le tableau est de longueur nulle, il n'y a rien à faire. */
    if (debut >= fin) {
        return;
    }

    /* Sinon, on parcourt le tableau, une fois de droite à gauche, et une
       autre de gauche à droite, à la recherche d'éléments mal placés,
       que l'on permute. Si les deux parcours se croisent, on arrête. */
    while (1) {
        droite--;
        while (tableau[droite] > pivot) {
            droite--;
        }

        gauche++;
        while (tableau[gauche] < pivot) {
            gauche++;
        }

        if (gauche < droite) {
            echanger(tableau, gauche, droite, size);
        } else {
            break;
        }
    }

    /* Maintenant, tous les éléments inférieurs au pivot sont avant ceux
       supérieurs au pivot. On a donc deux groupes de cases à trier. On utilise
       pour cela... la méthode quickSort elle-même ! */
    quickSort(tableau, debut, droite, size);
    quickSort(tableau, droite + 1, fin, size);
}

int32_t main() {
    int32_t a[] = {4, 65, 2, -31, 0, 99, 2, 83, 782, 1};
    int32_t n = 10;

    displayTab(a, n);

    quickSort(a, 0, n - 1, n);

    displayTab(a, n);

    return 0;
}
