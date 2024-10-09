/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bomb_difuse.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sauron <sauron@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 00:41:05 by sauron            #+#    #+#             */
/*   Updated: 2024/10/10 00:43:05 by sauron           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define N 8 // dimensions de la matrice (max)
#define INF 1000000

// Directions possibles (diagonales, horizontales, verticales)
int dir[8][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}, {-1, -1}, {-1, 1}, {1, -1}, {1, 1}};

// Fonction pour vérifier si une position est dans les limites de la matrice
bool isValid(int x, int y, int rows, int cols) {
    return (x >= 0 && x < rows && y >= 0 && y < cols);
}

// BFS pour trouver la distance minimale entre deux points
int bfs(char m[N][N], int startX, int startY, int targetX, int targetY, int rows, int cols) {
    int dist[N][N];
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            dist[i][j] = INF;
        }
    }
    
    dist[startX][startY] = 0;
    int queue[N * N][2];  // file pour le BFS
    int front = 0, rear = 0;
    queue[rear][0] = startX;
    queue[rear][1] = startY;
    rear++;
    
    while (front < rear) {
        int x = queue[front][0];
        int y = queue[front][1];
        front++;
        
        for (int d = 0; d < 8; d++) {
            int newX = x + dir[d][0];
            int newY = y + dir[d][1];
            if (isValid(newX, newY, rows, cols) && m[newX][newY] != 'X' && dist[newX][newY] == INF) {  // 'X' pour obstacle
                dist[newX][newY] = dist[x][y] + 1;
                queue[rear][0] = newX;
                queue[rear][1] = newY;
                rear++;
            }
        }
    }
    return dist[targetX][targetY];
}

bool canDefuse(char m[N][N], int time, int rows, int cols) {
    int ctX, ctY, bombX, bombY, kitX = -1, kitY = -1;
    
    // Trouver la position du CT, de la bombe et du kit
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (m[i][j] == 'C') {
                ctX = i;
                ctY = j;
            }
            if (m[i][j] == 'B') {
                bombX = i;
                bombY = j;
            }
            if (m[i][j] == 'K') {
                kitX = i;
                kitY = j;
            }
        }
    }
    
    // Calculer la distance entre le CT et la bombe
    int distToBomb = bfs(m, ctX, ctY, bombX, bombY, rows, cols);
    
    // Cas 1 : pas de kit
    if (distToBomb + 10 <= time) {
        return true;
    }
    
    // Cas 2 : kit présent
    if (kitX != -1 && kitY != -1) {
        int distToKit = bfs(m, ctX, ctY, kitX, kitY, rows, cols);
        int distKitToBomb = bfs(m, kitX, kitY, bombX, bombY, rows, cols);
        
        if (distToKit + distKitToBomb + 5 <= time) {
            return true;
        }
    }
    
    return false;
}

int main() {
    int time = 14;
    char m[N][N] = {
        {'0', '0', '0', '0', 'B'},
        {'0', '0', '0', '0', 'C'},
        {'0', '0', '0', '0', '0'},
        {'0', 'K', '0', '0', '0'}
    };
    
    if (canDefuse(m, time, 4, 5)) {
        printf("Bomb defused!\n");
    } else {
        printf("Bomb detonated!\n");
    }
    
    return 0;
}
