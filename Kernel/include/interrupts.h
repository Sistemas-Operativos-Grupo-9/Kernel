/*
 *   interrupts.h
 *
 *  Created on: Apr 18, 2010
 *      Author: anizzomc
 */

#ifndef INTERRUPS_H_
#define INTERRUPS_H_

#include "idtLoader.h"
#include "registers.h"
#include "stdint.h"
#include <stdbool.h>

void _irq00Handler(void);
void _irq01Handler(void);
void _irq02Handler(void);
void _irq03Handler(void);
void _irq04Handler(void);
void _irq05Handler(void);
void _nextProcess(void);

void _syscallHandler(void);

void _exception0Handler(void);
void _exception6Handler(void);

bool getIF();
void _cli(void);
void _sti(void);

void _hlt(void);

void _storeRegisters(struct RegistersState *structure);

void picMasterMask(uint8_t mask);

void picSlaveMask(uint8_t mask);

// Termina la ejecución de la cpu.
void haltcpu(void);

#endif /* INTERRUPS_H_ */
