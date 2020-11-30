#include "header.h"

struct Request RequestParser (char *text, size_t len) {

    struct Request res;

    char *ptext = (char*) calloc(len + 1, sizeof(char));
    memcpy(ptext, text, len);
    ptext[len] = '\0';

    char *token;
    char delims[] = ": \n\t\v\r";

    token = strtok(ptext, delims);

    while (token != NULL) {

        if (strcmp(token, "TYPE") == 0) {
            token = strtok(NULL, delims);

            if (strcmp(token, "TEXT") == 0)
                res.type = TEXT;
            else {
                res.valid = -1;
                return res;
            }

        } else if (strcmp(token, "TO") == 0) {
            token = strtok(NULL, delims);

            if (token == NULL) {
                res.valid = -1;
                return res;
            }

            res.To.size = strlen(token) + 1;
            res.To.text = (char*) calloc(res.To.size, sizeof(char));

            if (res.To.text == NULL) {
                res.valid = -1;
                return res;
            }

            memcpy(res.To.text, token, res.To.size);

        } else if (strcmp(token, "FROM") == 0) {
            token = strtok(NULL, delims);

            if (token == NULL) {
                res.valid = -1;
                return res;
            }

            res.From.size = strlen(token) + 1;
            res.From.text = (char*) calloc(res.From.size, sizeof(char));

            if (res.From.text == NULL) {
                res.valid = -1;
                return res;
            }

            memcpy(res.From.text, token, res.From.size);
        } else if (strcmp(token, "MESSAGE") == 0) {
            token = strtok(NULL, delims);

            if (token == NULL)
        }
        token = strtok(NULL, delims);
    }

}

struct Response ResponseParser (char *text, size_t len) {

}