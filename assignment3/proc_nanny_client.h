/**
 * Copyright 2015 Kyle O'Shaughnessy
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef PROC_NANNY_CLIENT_H
#define PROC_NANNY_CLIENT_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <stdbool.h>

#define REFRESH_RATE 5
#define MAX_PROCESSES 1024
#define CONFIG_FILE_LINES 256
#define LOG_MESSAGE_LENGTH 512
#define TIME_BUFFER_SIZE 40
#define PROGRAM_NAME_LENGTH 128

#define READ_PIPE 0
#define WRITE_PIPE 1

struct timeval;

typedef struct _Pipe {
    int readWrite[2]; // read READ_PIPE, write WRITE_PIPE
} Pipe;

typedef struct _LogMessage {
    char message[LOG_MESSAGE_LENGTH];
} LogMessage;

typedef struct _ProgramConfig {
    char programName[PROGRAM_NAME_LENGTH];
    unsigned int runtime;
} ProgramConfig;

typedef struct _ChildProcess {
    pid_t childPid; // may or may not be required... not sure yet
    Pipe toParent;
    Pipe toChild;
    bool isAvailable;
    pid_t processPid;
    char processName[PROGRAM_NAME_LENGTH];
    unsigned int runtime;
} ChildProcess;

typedef struct _MonitoredProcess {
    pid_t processPid;
    char processName[PROGRAM_NAME_LENGTH];
    unsigned int runtime;
    bool beingMonitored;
} MonitoredProcess;



void beginProcNanny();
void connectToServer();
void checkInputs(int args, char* argv[]);
void cleanUp();
void checkForNewMonitoredProcesses(bool logNoProcessesFound);
void checkChild(void *childProcess);
void exitError(const char* errorMessage);
void getCurrentTime(char* buffer);
void getPids(const char* processName, pid_t pids[MAX_PROCESSES]);
void initializeChild(ChildProcess* childWorker, MonitoredProcess* processToBeMonitored);
void killChild(void* childProcess);
void killPid(pid_t pid);
void killAllProcNannys();
void logToServer(const char *type, const char *msg);
void monitorNewProcesses(void *monitoredProcess);
void readConfigurationFromServer(struct timeval * tv);
void trimWhitespace(char* str);

bool monitoredProcessComparator(void *mp1, void *mp2);
bool getChildPredicate(void* childProcess);

ChildProcess* spawnNewChildWorker();

#endif //PROC_NANNY_CLIENT_H
