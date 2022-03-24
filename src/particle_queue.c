// adapted from https://www.programiz.com/dsa/circular-queue

#include "particle_common.h"
#include "particle_queue.h"

uint16_t items[PARTICLE_COUNT];

// Check if the queue is full
int is_full() {
  if ((front == rear + 1) || (front == 0 && rear == PARTICLE_COUNT - 1)) return 1;
  return 0;
}

// Check if the queue is empty
int is_empty() {
  if (front == -1) return 1;
  return 0;
}

// mark a particle as being available
void particle_queue_return_index(uint16_t particle_index) {
    if (is_full()) {
        // more particles returned than available - should never happen
        while (1) {}
    }

    if (front == -1) {
        front = 0;
    }
    rear = (rear + 1) % PARTICLE_COUNT;
    items[rear] = particle_index;
}

// get the index of the next available particle
uint16_t particle_queue_request_index() {
    int particle_index;
    if (is_empty()) {
        // fail
        while (1) {}
    }

    particle_index = items[front];
    if (front == rear) {
        front = -1;
        rear = -1;
    } else {
        front = (front + 1) % PARTICLE_COUNT;
    }

    return particle_index;
}

void particle_queue_init()
{
    front = -1;
    rear = -1;
    for (int index = 0; index < PARTICLE_COUNT; index++) {
        particle_queue_return_index(index);
    }
}
