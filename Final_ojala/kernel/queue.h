#ifndef QUEUE_H
#define QUEUE_H

#include <stdint.h>
#include <stdbool.h>

/*
 * Cola circular simple para almacenar punteros a PCB
 * Usada por el scheduler para sus colas de prioridad.
 *
 * No usa memoria dinámica (ideal para kernels freestanding).
 */

#define QUEUE_MAX 32     /* tamaño fijo por simplicidad */

typedef struct {
    void *items[QUEUE_MAX];
    uint32_t head;       /* apunta al próximo elemento a sacar */
    uint32_t tail;       /* apunta al próximo espacio para insertar */
    uint32_t count;      /* número actual de elementos */
} queue_t;

/* ============================================================
   Inicializar cola
   ============================================================ */
static inline void queue_init(queue_t *q) {
    q->head = 0;
    q->tail = 0;
    q->count = 0;
}

/* ============================================================
   ¿Está vacía?
   ============================================================ */
static inline bool queue_empty(const queue_t *q) {
    return (q->count == 0);
}

/* ============================================================
   ¿Está llena?
   ============================================================ */
static inline bool queue_full(const queue_t *q) {
    return (q->count == QUEUE_MAX);
}

/* ============================================================
   Encolar (push)
   Retorna false si la cola está llena.
   ============================================================ */
static inline bool queue_push(queue_t *q, void *item) {
    if (queue_full(q))
        return false;

    q->items[q->tail] = item;
    q->tail = (q->tail + 1) % QUEUE_MAX;
    q->count++;
    return true;
}

/* ============================================================
   Desencolar (pop)
   Retorna NULL si la cola está vacía.
   ============================================================ */
static inline void* queue_pop(queue_t *q) {
    if (queue_empty(q))
        return 0;

    void *out = q->items[q->head];
    q->head = (q->head + 1) % QUEUE_MAX;
    q->count--;
    return out;
}

#endif /* QUEUE_H */
