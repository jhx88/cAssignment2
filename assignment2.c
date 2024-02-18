#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_PARTICLES 10 // Maximum number of particles

typedef struct {
    int x, y;
    int vx, vy;
    bool exists;
} Particle;

// Function to move a particle within the grid
void moveParticle(Particle *p, int maxX, int maxY) {
    if (!p->exists) return;

    // Move particle
    p->x += p->vx;
    p->y += p->vy;

    // Boundary collision detection for x-axis
    if (p->x < 0) {
        p->x = 0;
        p->vx = -p->vx;
    } else if (p->x >= maxX) {
        p->x = maxX - 1;
        p->vx = -p->vx;
    }

    // Boundary collision detection for y-axis
    if (p->y < 0) {
        p->y = 0;
        p->vy = -p->vy;
    } else if (p->y >= maxY) {
        p->y = maxY - 1;
        p->vy = -p->vy;
    }
}

// Function to check for collisions between particles
void checkCollisions(Particle particles[], int numParticles) {
    for (int i = 0; i < numParticles; i++) {
        if (!particles[i].exists) continue;
        for (int j = i + 1; j < numParticles; j++) {
            if (particles[j].exists && particles[i].x == particles[j].x && particles[i].y == particles[j].y) {
                particles[i].exists = false;
                particles[j].exists = false;
            }
        }
    }
}

// Function to print the grid with particles
void printGrid(Particle particles[], int numParticles, int maxX, int maxY) {
    // Print the top border
    for (int x = 0; x < maxX + 2; x++) {
        printf("*");
    }
    printf("\n");

    // Print the grid interior and left/right borders
    for (int y = 0; y < maxY; y++) {
        printf("*"); // Left border
        for (int x = 0; x < maxX; x++) {
            bool isParticle = false;
            for (int i = 0; i < numParticles; i++) {
                if (particles[i].exists && particles[i].x == x && particles[i].y == y) {
                    printf("+");
                    isParticle = true;
                    break;
                }
            }
            if (!isParticle) {
                printf(" ");
            }
        }
        printf("*\n"); // Right border
    }

    // Print the bottom border
    for (int x = 0; x < maxX + 2; x++) {
        printf("*");
    }
    printf("\n");
}


int main() {
    FILE *file = fopen("input.txt", "r");
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }

    int maxX, maxY, time;
    fscanf(file, "%d %d %d", &maxX, &maxY, &time);
    // Adjusting the grid size for 0-based indexing and including the border
    maxX -= 0;
    maxY -= 0;

    Particle particles[MAX_PARTICLES];
    int numParticles = 0;
    char endOfFileCheck;

    while (true) {
        Particle p;
        if (fscanf(file, "%d,%d,%d,%d%c", &p.x, &p.y, &p.vx, &p.vy, &endOfFileCheck) < 4) break;
        if (endOfFileCheck == 'E') break; // Check for the end of file marker
        p.exists = true;
        particles[numParticles++] = p;
    }

    fclose(file);

    // Run the simulation for the specified time
    for (int t = 0; t < time; t++) {
        for (int i = 0; i < numParticles; i++) {
            moveParticle(&particles[i], maxX, maxY);
        }
        checkCollisions(particles, numParticles);
    }

    printGrid(particles, numParticles, maxX, maxY);

    return 0;
}
