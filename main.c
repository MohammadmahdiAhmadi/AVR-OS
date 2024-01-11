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

#include <stdio.h>
#include "system/system.h"
#include <util/delay.h>

char seg_7[10] = {
	0xbf, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f
};

volatile int a = 0;
volatile int b = 0;
volatile int sharedVariable = 0;
Mutex *mutex = NULL;


// Simple task 1_1
void simple_task1_1(void *parameters)
{
	while(1) {
		PORTB = ~seg_7[1];
	}
}

// Simple task 1_2
void simple_task1_2(void *parameters)
{
	while(1) {
		PORTB = ~seg_7[2];
	}
}

// Simple task 1_3
void simple_task1_3(void *parameters)
{
	while(1) {
		PORTB = ~seg_7[3];
	}
}

// Simple task 2_1
void simple_task2_1(void *parameters)
{
	while(1) {
		a = (a + 1) % 256;
		PORTA = a;
		_delay_ms(250);
	}
}

// Simple task 2_2
void simple_task2_2(void *parameters)
{
	while(1) {
		b = (b + 1) % 256;
		PORTB = b;
		_delay_ms(250);
	}
}

// Mutex task 1_1
void mutex_task1_1(void* parameters)
{
	PORTA = ~seg_7[1];
	osMutexLock(mutex);
	PORTA = ~seg_7[2];
	while (1);
}

// Mutex task 1_2
void mutex_task1_2(void *parameters)
{
	PORTA = ~seg_7[3];
	osMutexLock(mutex);
	PORTA = ~seg_7[4];
	while (1);
}


// Mutex task 2_1
void mutex_task2_1(void* parameters)
{
    while(1) {
		// Set task number to PORTB
		PORTB = ~seg_7[1];
		
		osMutexLock(mutex);
		sharedVariable = (sharedVariable + 2) % 10;
		PORTA = ~seg_7[sharedVariable];
		osMutexUnlock(mutex);

		_delay_ms(200);
	}
}

// Mutex task 2_2
void mutex_task2_2(void* parameters)
{
	while(1) {
		// Set task number to PORTB
		PORTB = ~seg_7[2];
		
		osMutexLock(mutex);
		sharedVariable = (sharedVariable - 1) % 10;
		PORTA = ~seg_7[sharedVariable];
		osMutexUnlock(mutex);
		
		_delay_ms(200);
	}
}


int main(int argc, char* argv[])
{
	DDRA = 0xFF;
	DDRB = 0xFF;
	DDRC = 0xFF;
	
    // initialize operating system
    osInit();

    // create lockable object
    mutex = osMutexCreate();

	// Simple multi-tasking
    osCreateTask(simple_task1_1, NULL, 64, 2);
    osCreateTask(simple_task1_2, NULL, 64, 4);
    osCreateTask(simple_task1_3, NULL, 64, 8);

    // run
    osRun();
}