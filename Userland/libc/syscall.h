#pragma once

#include "color.h"
#include "processes.h"
#include "semaphores.h"
#include <datetime.h>
#include <stdbool.h>
#include <stdint.h>
#include <window.h>

int read(uint64_t fd, char *buf, uint64_t count, uint64_t timeout);
int write(uint64_t fd, char *buf, uint64_t count);
uint8_t getpid();
int execve(char *moduleName, char **argv, int argc);
int fork();
uint64_t proccount();
Time gettime();
struct RegistersState getRegisters();
bool kill(int pid);
void microsleep(uint64_t micros);
void millisleep(uint64_t millis);
uint64_t millis();
uint8_t getProcesses(struct Process processes[256]);
void setGraphic(bool value);
void drawCircle(uint16_t x, uint16_t y, uint16_t radius);
void drawRectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height);
void setForeground(Color color);
WindowInfo getWindowInfo();
void drawBitmap(uint16_t x, uint16_t y, uint16_t width, uint16_t height,
                Color bitmap[][width]);
void drawText(char *text, uint16_t x, uint16_t y, bool center);
void drawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
void flip();
void switchToDesktop(uint8_t desktop);

SID semInit(const char *name, semValue value);
bool semClose(SID sem);
bool semWait(SID sem);
bool semPost(SID sem);
SID semOpen(const char *name);
void semPrintList();
