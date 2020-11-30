#pragma once

#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <arpa/inet.h>

#define PORT 8010

enum command_t { TYPE, TO, FROM, MESSAGE, ERR, END };
enum type_t { TEXT };

struct Text {
    char *text;
    size_t size;
};

struct Lexem {
    enum command_t command;
    union {
        struct Text buffer;
        enum type_t type;
    } value;
};

struct Request {
    int valid;
    enum type_t type;
    struct Text To, From, Message;
};

struct Response {
    int valid;
    enum type_t type;
    struct Text From, Message;
};