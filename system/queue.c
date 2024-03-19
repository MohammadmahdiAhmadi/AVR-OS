/*
 * Operating system for Atmel AVR microcontrollers
 * Copyright (c) 2015 Konrad Kusnierz <iryont@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <stdlib.h>
#include "queue.h"

Queue* osQueueCreate()
{
    Queue* queue = (Queue*)malloc(sizeof(Queue));

    queue->size = 0;
    queue->length = 0;
    queue->tasks = NULL;

    return queue;
}

void osQueueDestroy(Queue *queue)
{
    if(queue->length)
        free(queue->tasks);

    free(queue);
}

void osQueueRemove(Queue *queue, TaskControlBlock *task)
{
    int8_t at = -1;
    for(int8_t i = 0; i < queue->size; i++) {
        if(queue->tasks[i] == task) {
            at = i;
            break;
        }
    }

    if(at != -1) {
        if(at != queue->size - 1) 
            queue->tasks[at] = queue->tasks[queue->size - 1];

        queue->tasks[queue->size - 1] = NULL;
        queue->size--;
    }
}

bool osQueueInsert(Queue *queue, TaskControlBlock *task)
{
    if(queue->size + 1 >= queue->length) {
        queue->length++;
        queue->tasks = (TaskControlBlock**)x_realloc(queue->tasks, sizeof(TaskControlBlock*) * queue->length);
        if(!queue->tasks) {
            queue->length--;
            return false;
        }
    }

    queue->tasks[queue->size] = task;
    queue->size++;

    return true;
}
