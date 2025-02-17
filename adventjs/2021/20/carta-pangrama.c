#include <stdio.h>
#include <stdlib.h>
#include "../../../common/hashmap.h"
#include "../../../common/str.h"
#define size(arr) sizeof(arr)/sizeof(arr[0])
#define assert(assertion, msg, ...) if (!assertion) {\
    printf("[ASSERTION FAILED] (%s:%d): " #msg, __FILE_NAME__, __LINE__, __VA_ARGS__); \
    exit(1); \
}
typedef enum{
    false,
    true
} boolean;

typedef struct {
    char** letter;
    boolean expected_output;
} Test;

typedef struct {
    HashMap remaing;
    boolean pangram;
} Result;

const boolean SETTED_TRUE = true;

const size_t ALPHABET_LEN = 27;
const char* N_WITH_GNOCCHI = "Ñ";
//const size_t N_WITH_GNOCCHI_LEN = size(N_WITH_GNOCCHI);
const size_t N_WITH_GNOCCHI_LEN = 2;
// 'Ñ' is the largest char on my dataset, that could change on the future and
// make this program fail.

HashMap alphabet_map = NULL;
HashMap specials_map = NULL;

void set_special_character_map() {
    char* spe_def[] = {"A", "E", "I", "O", "U", "?", "!"};
    size_t spe_def_len = size(spe_def);
    char** current_special = malloc(spe_def_len);
    for (size_t i = 0; i < spe_def_len; i++)
        str_set(current_special+i, spe_def[i]);
    // This is enought for my test cases, but more characters could be added
    set(&specials_map, "Ä", current_special);
    set(&specials_map, "Á", current_special);
    set(&specials_map, "ä", current_special);
    set(&specials_map, "á", current_special);
    current_special++;
    set(&specials_map, "Ë", current_special);
    set(&specials_map, "É", current_special);
    set(&specials_map, "ë", current_special);
    set(&specials_map, "é", current_special);
    current_special++;
    set(&specials_map, "Ï", current_special);
    set(&specials_map, "Í", current_special);
    set(&specials_map, "ï", current_special);
    set(&specials_map, "í", current_special);
    current_special++;
    set(&specials_map, "Ó", current_special);
    set(&specials_map, "Ö", current_special);
    set(&specials_map, "ó", current_special);
    set(&specials_map, "ö", current_special);
    current_special++;
    set(&specials_map, "Ü", current_special);
    set(&specials_map, "Ú", current_special);
    set(&specials_map, "ü", current_special);
    set(&specials_map, "ú", current_special);
    current_special++;
    set(&specials_map, "¿", current_special);
    current_special++;
    set(&specials_map, "¡", current_special);
}

void print(HashMap hmap)
{
    if (!hmap || hmap->occupied == 0) {
        printf("[]\n");
        return;
    }
    Linklst hmap_keys = (Linklst)keys(hmap);
    Node *node = hmap_keys->first;
    printf("[ ");
    while (node->next) {
        char *key = (char*)node->ref;
        printf("'%s', ", key);
        node = node->next;
    }
    char *key = (char*)node->ref;
    printf("'%s']\n", key);
}

void set_alphabet_map()
{

    const size_t ALPHABET_LEN = 27;
    char** alphabet = malloc(ALPHABET_LEN*sizeof(char*));
    for (size_t i = 0; i < ALPHABET_LEN-1; i++) {
        alphabet[i] = malloc(N_WITH_GNOCCHI_LEN);
        alphabet[i][0] = 'A'+i;
        alphabet[i][1] = 0;
        set(&alphabet_map, alphabet[i], &SETTED_TRUE);
    }
    str_set(alphabet+(ALPHABET_LEN-1), N_WITH_GNOCCHI);
    set(&alphabet_map, alphabet[ALPHABET_LEN-1], &SETTED_TRUE);
}

void to_upper(char* str)
{
    *str = char_to_upper(*str);
}

Result is_pangram(char* letter)
{
    HashMap cloned_alphabet;
    hmap_clone(alphabet_map, &cloned_alphabet);
    char* key = malloc(sizeof(char)*N_WITH_GNOCCHI_LEN + 1);
    for (size_t i = 0; i < N_WITH_GNOCCHI_LEN; i++)
        key[i] = 0;
    while (cloned_alphabet->occupied > 0 && *letter != 0) {
        unsigned char byte = (unsigned char)*letter;
        if ((*letter & 0b010000000) == 0) { 
            key[0] = char_to_upper(*letter);
            key[1] = 0;
            letter++; 
        } else if ((byte >> 5) == 0b110) { 
            if (str_starts_with(letter, "ñ") || str_starts_with(letter, "Ñ")) {
                str_set((char**)&key, "Ñ");
                letter += N_WITH_GNOCCHI_LEN;
            } else {
                key[0] = letter[0];
                key[1] = letter[1];
                key[2] = 0;
                char *special = get(specials_map, key);
                if (!special)
                    assert(0, "Special character '%s' is not defined on the special map", key);
                str_set((char**)&key, *(char**)special);
                letter += 2;
            }
        } else
            assert(0, "Character with more than 2 bytes", NULL);
        remove_by_key(cloned_alphabet, key);
    }
    return (Result){
        cloned_alphabet,
        cloned_alphabet->occupied == 0
    };
}

char* pan_01 = "Extraño pan de col y kiwi se quemó bajo fugaz vaho";
char* pan_02 = "Jovencillo emponzoñado y con walkman: ¡qué figurota exhibes!";
char* pan_03 = "Esto es una frase larga pero no tiene todas las letras del abecedario";
char* pan_04 = "De la a a la z, nos faltan letras";
Test tests[] = {
    { &pan_01, true },
    { &pan_02, true },
    { &pan_03, false },
    { &pan_04, false }
};
size_t tests_len = size(tests);

int main()
{
    set_special_character_map();
    set_alphabet_map();
    printf("[Revisando las cartas]:\n");
    for (size_t i = 0; i < tests_len; i++) {
        Result result = is_pangram(tests[i].letter[0]);
        printf("  - %-2s: ", result.pangram == tests[i].expected_output ? "ok" : "err");
        print(result.remaing);
    }
    return 0;
}
