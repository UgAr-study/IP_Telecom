#include "lexer_header.h"

int is_delim (char c) {
    char delims[] = ": \n\t\v\r";
    for (int i = 0; i < strlen(delims); ++i)
        if (c == delims[i])
            return 1;
    return 0;
}

int is_command (const char * word) {
    if (strcmp(word, "TYPE")    == 0 ||
        strcmp(word, "TO")      == 0 ||
        strcmp(word, "FROM")    == 0 ||
        strcmp(word, "MESSAGE") == 0)
        return 1;

    return 0;
}

char * getWord (char *text, int *len) {
    int i = 0;
    *len = 0;

    while (is_delim(text[i])) {
        if (text[i] == '\0') {
            *len += i;
            return NULL;
        }
        i++;
    }

    *len += i;
    char *begin = text + i;

    if (*begin == '\"') {
        begin++;
        len++;

        int size = 0;
        while (begin[size] != '\"') {
            if (begin[size] == '\0')
                return NULL;
            size++;
        }
        size++;

        *len += size;

        char *phrase = (char*) calloc (size, sizeof(char));
        strncpy(phrase, begin, size - 1);

        return phrase;
    }

    int size = 0;
    while (!is_delim(begin[size++]));

    *len += size;

    char *word = (char*) calloc(size, sizeof(char));
    strncpy(word, begin, size - 1);

    return word;
}

struct Lexem getNextLexem (char *text) {

    static char *cur_pos = NULL;

    if (cur_pos == NULL)
        cur_pos = text;

    printf ("cur_pos: [%s]\n", cur_pos);
    struct Lexem res;
    res.command = ERR;
    res.value.type = TEXT;

    int len = 0;

    //TODO: free calloc!
    char *token = getWord(cur_pos, &len);
    cur_pos += len;

    if (token == NULL) {
        printf ("NULL\n");
        if (*cur_pos == '\0') {
            res.command = END;
        }
        return res;
    }


    if (strcmp(token, "TYPE") == 0) {
        free(token);

        //TODO: free calloc!
        token = getWord(cur_pos, &len);

        if (token == NULL)
            return res;

        if (strcmp(token, "TEXT") == 0) {
            res.command = TYPE;
            res.value.type = TEXT;
            cur_pos += len;
        }

    }
    else if (strcmp(token, "TO") == 0) {
        free(token);

        //TODO: free calloc!
        token = getWord(cur_pos, &len);

        if (token == NULL)
            return res;

        if (!is_command(token)){
            res.command = TO;
            res.value.buffer.text = token;
            res.value.buffer.size = strlen(token) + 1;
            printf ("TO: cur_pos: [%s]\n", cur_pos);
            cur_pos += len;
            return res;
        }

    }
    else if (strcmp(token, "FROM") == 0) {
        free(token);

        //TODO: free calloc!
        token = getWord(cur_pos, &len);

        if (token == NULL)
            return res;

        if (!is_command(token)){
            res.command = FROM;
            res.value.buffer.text = token;
            res.value.buffer.size = strlen(token) + 1;
            cur_pos += len;
            return res;
        }

    }
    else if (strcmp(token, "MESSAGE") == 0) {
        free(token);

        //TODO: free calloc!
        token = getWord(cur_pos, &len);

        if (token == NULL)
            return res;

        if (!is_command(token)){
            res.command = MESSAGE;
            res.value.buffer.text = token;
            res.value.buffer.size = strlen(token) + 1;
            cur_pos += len;
            return res;
        }

    }

    free(token);
    return res;
}

void print_lex (struct Lexem lex) {
    switch (lex.command) {

        case TYPE :{
            printf("TYPE: ");

            switch (lex.value.type) {

                case TEXT:
                    printf("TEXT\n");
                    break;
            }

            break;
        }

        case TO:
            printf ("TO: %s\n", lex.value.buffer.text);
            break;

        case FROM:
            printf ("FROM: %s\n", lex.value.buffer.text);
            break;

        case MESSAGE:
            printf ("MESSAGE: %s\n", lex.value.buffer.text);
            break;

        case ERR:
            printf("Error\n");
            break;

        case END:
            printf ("END\n");
            break;
    }
}


int main () {
    char text[] = "TO: Ignat ";
    struct Lexem lex = getNextLexem(text);
    print_lex(lex);
    lex = getNextLexem(text);
    print_lex(lex);
    return 0;
}