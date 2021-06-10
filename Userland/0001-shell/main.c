#include <stdio.h>
#include <time.h>
#include <print.h>
#include <syscall.h>
#include <string.h>

char commandHistory[10][256];
const uint64_t historyLength = sizeof(commandHistory) / sizeof(*commandHistory);
int commandsIssued = 0;
char command[256];

void replaceComand(char *buffer, char *newCommand, int *length, int *position) {
    for (int i = *position; i < *length; i++) {
        moveCursorRight();
    }
    for (int i = 0; i < *length; i++) {
        putchar('\b');
    }
    strcpy(buffer, newCommand);
    *length = strlen(buffer);
    for (int i = 0; i < *length; i++) {
        putchar(buffer[i]);
    }
    *position = *length;
}

void inputCommand(char *buffer, int maxLength) {
    buffer[0] = 0;
    int length = 0;
    int position = 0;
    int historyIndex = -1;
    while (true) {
        KeyStroke key = readKeyStroke();
        if (!key.isPrintable && key.data == '\n') {
            break;
        }
        if (key.isPrintable) {
            length++;
            for (int i = length; i > position; i--) {
                buffer[i] = buffer[i - 1];
            }
            buffer[position++] = key.data;
            putchar(key.data);
        } else if (key.data == '\b') {
            if (position > 0) {
                for (int i = position - 1; i < length; i++) {
                    buffer[i] = buffer[i + 1];
                }
                length--;
                position--;
                putchar('\b');
            }
        } else if (key.data == '\n') {
            return;
        } else if (key.arrow != NO_ARROW) {
            if (key.arrow == ARROW_LEFT && position > 0) {
                moveCursorLeft();
                position--;
            } else if (key.arrow == ARROW_RIGHT && position < length) {
                moveCursorRight();
                position++;
            }

            if (key.arrow == ARROW_UP && historyIndex + 1 < historyLength) {
                historyIndex++;
                replaceComand(buffer, commandHistory[historyIndex], &length, &position);
            }

            if (key.arrow == ARROW_DOWN) {
                if (historyIndex + 1 > 0) {
                    historyIndex--;
                    replaceComand(buffer, commandHistory[historyIndex], &length, &position);
                } else if (historyIndex == 0) {
                    historyIndex--;
                    replaceComand(buffer, "", &length, &position);
                }
            }
        }
    }
}

bool validateCommand(char *command) {
    if (strlen(command) > 0) {
        return true;
    }
    return false;
}

// Returns true if exit
bool execCommand(char *command) {
    if (strcmp(command, "quit") == 0) {
        return true;
    }
    return false;
}

int main() {
    while (true) {
        puts("~");
        inputCommand(command, sizeof(command));
        if (validateCommand(command)) {
            commandsIssued++;
            for (int i = historyLength - 1; i > 0; i--)
                strcpy(commandHistory[i], commandHistory[i - 1]);
            strcpy(commandHistory[0], command);
        }
        if (execCommand(command)) {
            return 0;
        }
        putchar('\n');
        puts(command);
        putchar('\n');
    }

    return 0;
}