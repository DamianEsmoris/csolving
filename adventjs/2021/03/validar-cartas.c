#include <stdio.h>
#include <stdlib.h>
#include "../../../common/linklst.h"

#define assert(assertion, msg, ...) if (!assertion) {\
    printf("[ASSERTION FAILED] (%s:%d): " #msg, __FILE_NAME__, __LINE__, __VA_ARGS__); \
    exit(1); \
}

#define last_token(l) (LToken*)last(l)

#define _2nd_last_token(l) (LToken*)prev(l, last_token(l))

#define push_token(tkl, pl, tk) do { \
    push(&tkl, tk);                  \
    if (*tk == OPARENTHESES)         \
        push(&pl, tk);               \
    else if (*tk == CPARENTHESES) {  \
        if (pl && *last_token(pl) == OPARENTHESES)\
            nonfree_pop(&pl);        \
        else                         \
            push(&pl, tk);           \
    }                                \
} while(0)\


typedef enum {
    false,
    true
} Boolean;

typedef struct {
    char* letter;
    Boolean result;
} Test;


typedef enum {
    SPACE,
    WORD,
    CHAR,
    OPARENTHESES,
    CPARENTHESES,
    INVALID_OTHER,
    OTHER,
} LToken;

LToken get_next_token(char **letter)
{
    switch (**letter) {
        case 'a'...'z':
        case 'A'...'Z':
            (*letter)++;
            LToken token = CHAR;
            while (**letter != 0 &&
                   (**letter >= 'a' && **letter <= 'z' ||
                   **letter >= 'A' && **letter <= 'Z')) {
                token = WORD;
                (*letter)++;
            }
            return token;
        case '(':
            (*letter)++;
            return OPARENTHESES;
            break;
        case ')':
            (*letter)++;
            return CPARENTHESES;
            break;
        case ' ':
            (*letter)++;
            return SPACE;
        case '{':
        case '}':
        case '[':
        case ']':
            (*letter)++;
            return INVALID_OTHER;
            break;
        case 0:
            assert(false, "This should never happen", NULL);
        default: 
            assert(false, "Unexpected character on the letter %c", **letter);
            return OTHER;
    }
}

void print_token(LToken tk)
{
    switch (tk) {
    case SPACE:
        printf("space");
        break;
    case WORD:
        printf("word");
        break;;
    case CHAR:
        printf("char");
        break;
    case OPARENTHESES:
        printf("(");
        break;
    case CPARENTHESES:
        printf(")");
        break;
    case INVALID_OTHER:
        printf("}|]");
        break;
    case OTHER:
        printf("?");
        break;
    }
}
void print_tokens(Linklst l)
{
    if (!l) {
        printf("[]\n");
        return;
    }
    Node *lnode = l->first;
    printf("[");
    while (lnode->next) {
        print_token(*(LToken*)lnode->ref);
        printf(", ");
        lnode = lnode->next;
    }
    print_token(*(LToken*)lnode->ref);
    printf("]\n");
}

LToken* two_last_token(Linklst l)
{
    if (!l || !l->first) 
        return 0;
    Node* node = l->first;
    int count = 0;
    while (node->next) {
        count++;
        node = node->next;
    }
    return node->ref;
}

Boolean validate(char* letter)
{
    Linklst tokens = NULL;
    Linklst parentheses = NULL;
    LToken *token;

    // at least should be 2 tokens (!= SPACES) to start the verification...
    for (int i = 0; i < 2; i++) {
        token = malloc(sizeof(LToken));
        do {
            *token = get_next_token(&letter);
        } while (*token == SPACE);
        push_token(tokens, parentheses, token);
    }
    
    while (
        *letter != 0 &&

        (*_2nd_last_token(tokens) != OPARENTHESES
            || *last_token(tokens) != CPARENTHESES) &&

        ((!parentheses) ||
         (parentheses && *last_token(tokens) != INVALID_OTHER
          && *last_token(parentheses) == OPARENTHESES))
    ) { 
        token = malloc(sizeof(LToken));
        *token = get_next_token(&letter);
        if (*token != SPACE)
            push_token(tokens, parentheses, token);
    }

    if (*letter == 0)
        printf("    * letter ended\n");

    if (parentheses && *last_token(parentheses) == OPARENTHESES
            && *last_token(tokens) == INVALID_OTHER)
        printf("    * invalid token inside of parentesis\n");

    if (*_2nd_last_token(tokens) == OPARENTHESES
         && *token == CPARENTHESES)
        printf("    * empty set of parentheses\n");

    if (length(parentheses) > 0)
        printf("    * unclosed parentesis\n");

    Boolean result = *letter == 0 && length(parentheses) == 0;
    free_lst(&tokens);
    return result;
}

int main()
{
    Test tests[] = {
        { "bici coche (balon) bici coche peluche", true },
        { "(muneca) consola bici", true },
        { "bici coche (balon bici coche", false },
        { "peluche (bici [coche) bici coche balon", false },
        { "(peluche {) bici", false },
        { "() bici", false }
    };
    size_t test_len = sizeof(tests)/sizeof(Test);
    printf("[Validating letters]:\n");
    for (size_t i = 0; i < test_len; i++) {
        Boolean result = validate(tests[i].letter);
        printf(" - letter %ld: %-3s %s\n\n", i+1, result == tests[i].result ? "ok" : "err", result ? "is valid" : "is invalid");
    }

    return 0;
}
