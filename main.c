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

#define MIN_STACK_POINTER_VALUE 1900

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

// Simple task 1_3
void simple_task1_4(void *parameters)
{
	while(1) {
		PORTB = ~seg_7[4];
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

void* x_malloc(size_t size) {
	uint16_t stored_sp = eeprom_read_word((uint16_t*)EEPROM_ADDR_SP);
	void* new_memory;

	// OS Is not yet running, So SP has not changed
	if(stored_sp == 0) {
		return malloc(size);
	} else {
		uint16_t tmp_sp = SP;
		SP = stored_sp;
		new_memory = malloc(size);
		SP = tmp_sp;
		return new_memory;
	}
}

void* x_realloc(void* ptr, size_t size) {
	uint16_t stored_sp = eeprom_read_word((uint16_t*)EEPROM_ADDR_SP);
	void* new_memory;

	// OS Is not yet running, So SP has not changed
	if(stored_sp == 0) {
		return realloc(ptr, size);
	} else {
		uint16_t tmp_sp = SP;
		SP = stored_sp;
    	new_memory = realloc(ptr, size);
		SP = tmp_sp;
		return new_memory;
	}
}

// Dynamic task 1_4
void dynamic_task1_4(void *parameters)
{
	while(1) {
		PORTB = ~seg_7[4];
	}
}

// Dynamic task 1_1
void dynamic_task1_1(void *parameters)
{
	DISABLE_INTERRUPTS
	osCreateTask(dynamic_task1_4, NULL, 64, 4);
	ENABLE_INTERRUPTS

	while(1) {
		PORTB = ~seg_7[1];
	}
}

// 2 Bytes for next function call, 2 Bytes for ISR
#define IS_SAFE_CALL(value) ((SP - value) > 4 ? 1 : 0)

// Dynamic task 1_2
void dynamic_task1_2(void *parameters)
{
	while(1) {
		PORTB = ~seg_7[2];
	}
}

void recursion(int i) {
	if(i == 0)
		return;
	
	if(IS_SAFE_CALL(osCurrentTaskPtr->sct))
		return recursion(i-1);
	else
		return;
}

// Dynamic task 1_3
void dynamic_task1_3(void *parameters)
{
	if(IS_SAFE_CALL(osCurrentTaskPtr->sct)){
		DISABLE_INTERRUPTS
		osCreateTask(dynamic_task1_1, NULL, 64, 1);
		ENABLE_INTERRUPTS
	}

	if(IS_SAFE_CALL(osCurrentTaskPtr->sct))
		recursion(100);

	while(1) {
		PORTB = ~seg_7[3];
	}
}


int main(int argc, char* argv[])
{
	eeprom_write_word((uint16_t*)EEPROM_ADDR_SP, 0x0000);

	DDRA = 0xFF;
	DDRB = 0xFF;
	DDRC = 0xFF;

    // initialize operating system
    osInit();

    // create lockable object
    mutex = osMutexCreate();

	// Simple multi-tasking
    osCreateTask(dynamic_task1_3, NULL, 90, 3);
    osCreateTask(dynamic_task1_2, NULL, 128, 2);

    // run
    osRun();
}





// 
// uint16_t *t = (uint16_t*)malloc(sizeof(uint16_t));
// *t = 10;
// uint16_t *t2 = (uint16_t*)malloc(sizeof(uint16_t));
// *t2 = 12;
// uint16_t *t3 = (uint16_t*)malloc(sizeof(uint16_t));
// *t3 = 13;
// uint16_t *t4 = (uint16_t*)malloc(sizeof(uint16_t));
// *t4 = 14;
// uint16_t *t5 = (uint16_t*)malloc(sizeof(uint16_t));
// *t5 = 15;
// uint16_t *t6 = (uint16_t*)malloc(sizeof(uint16_t));
// *t6 = 16;
// 
// uint16_t ff = t;
// uint16_t t7 = 17;
// ff = ff + t2;
// ff = ff + t3;
// ff = ff + t4;
// ff = ff + t5;
// ff = ff + t6;
// ff = ff + t7;
// 
// printf("%d", ff);