/*


                Birzeit University
            Department of Computer Science
                    COMP2421
            Data Structures And Algoritims 

            Project #3

            Student ID: 1201180

            Contact: contact@ibraheemalyan.dev
            Portfolio: https://www.ibraheemalyan.dev/

By
  ___ _               _                              _    _
 |_ _| |__  _ __ __ _| |__   ___  ___ _ __ ___      / \  | |_   _  __ _ _ __
  | || '_ \| '__/ _` | '_ \ / _ \/ _ \ '_ ` _ \    / _ \ | | | | |/ _` | '_ \
  | || |_) | | | (_| | | | |  __/  __/ | | | | |  / ___ \| | |_| | (_| | | | |
 |___|_.__/|_|  \__,_|_| |_|\___|\___|_| |_| |_| /_/   \_\_|\__, |\__,_|_| |_|
                                                            |___/


*/

/*

        Run the code in ANSI-compatible Terminal 
        e.g : cmd, powershell,  bash, zsh, macos terminal, iterm, iterm2

        Color Codes

Black: \033[0;30m
Red: \033[0;31m
Green: \033[0;32m
Yellow: \033[0;33m
Blue: \033[0;34m
Magenta: \033[0;35m
Cyan: \033[0;36m
White: \033[0;37m

        Style

Normal: \033[0;3?m
Bold:   \033[1;3?m

Reset: \033[0m

*/

// Author: Ibraheem Alyan

// /////////////////////////////////////////////////
// ////////////////////// Preprocessors
// /////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// /////////////////////////////////////////////////
// ////////////////////// Define Structs
// /////////////////////////////////////////////////

struct Word_obj
{
    char key[50];
    char meaning[200];
};

typedef struct Word_obj *Word;

enum State
{
    EMPTY,
    OCCUPIED,
    DELETED
};

enum HashAlg
{
    ASCII_SUM,
    DJB2_HASH
};

enum Probing
{
    Linear,
    Quadratic
};

typedef enum State State;

typedef struct
{
    Word w;
    State state;
} Slot;

Slot Table[487];

int size = 239;

int occupied = 0;
int collisions = 0;

enum HashAlg hash_algorithim = DJB2_HASH;
enum Probing probing_technique = Quadratic;

// /////////////////////////////////////////////////
// ////////////////////// Prototypes
// /////////////////////////////////////////////////

Word Create_Word_Obj(char word[50], char meaning[200]);
void read_file();
void write_word(Word word, FILE *fptr);
void show_menu();
int ascii_sum(char key[50]);

int DJB2(char key[50]);
void cmd_1();
void cmd_2();
void cmd_3();
void cmd_6();
int quadratic_probe_hash(int djb2, int i);

int linear_probe_hash(int djb2, int i);
int get_initial_hash(char key[50]);
void print_word(Word w);
int probe(int hash, int i);
void print_err(char msg[50]);
void print_heading(char heading[]);
int insert(Word w);
float load_factor();
void print_all_words();
void print_table();

// void print_starts_with(BST Tree, char k);
// BST delete_starts_with(BST Tree, char k);
// void write_to_file(BST root);

// /////////////////////////////////////////////////
// ////////////////////// Struct Functions
// /////////////////////////////////////////////////

Word Create_Word_Obj(char word[50], char meaning[200])
{
    Word new_word = (Word)malloc(sizeof(struct Word_obj));

    strcpy(new_word->key, word);

    strcpy(new_word->meaning, meaning);

    return new_word;
}

// /////////////////////////////////////////////////
// ////////////////////// Hash Functions
// /////////////////////////////////////////////////

void rehash_if_needed(){

    print_heading("Checking Load Factor ");

    printf("%f\n", load_factor());
    
    if ( load_factor() < 0.63 ){
        print_heading("No need to rehash");
        return;
    }

    print_heading("Rehashing");

    Slot Backup[239];

    for (size_t i = 0; i < size; i++)
    {
        Backup[i] = Table[i];
        Table[i].state = EMPTY;
        Table[i].w = NULL;
    }

    size = 487;
    occupied = 0;

    for (int i = 0; i < 239; i++)
    {
        printf("Inserting index %d in new array, state is %d\n", i, Backup[i].state);
        if ( Backup[i].w != NULL ){
            insert(Backup[i].w);
        }
        
    }

    print_heading("Rehashing DONE");

}

float load_factor()
{
    return ( (float) occupied ) / size;
}

// DJB2 Hash
int DJB2(char key[50])
{

    unsigned int hashAddress = 5381;
    for (int counter = 0; key[counter] != '\0'; counter++)
    {
        hashAddress = ((hashAddress << 5) + hashAddress) + key[counter];
    }
    return hashAddress % size;
}

// ASCII Sum Hash
int ascii_sum(char key[50])
{

    unsigned int hashAddress = 5381;
    for (int counter = 0; key[counter] != '\0'; counter++)
    {
        hashAddress += key[counter];
    }
    return hashAddress % size;
}

int linear_probe_hash(int djb2, int i)
{

    // the DJB2(key) hash of the key is calculated outside this function so if there was collisions it won't be calculated again
    // so the result of DJB2 is passed as integer

    return (djb2 + i) % size;
}

int quadratic_probe_hash(int djb2, int i)
{

    // the DJB2(key) hash of the key is calculated outside this function so if there was collisions it won't be calculated again
    // so the result of DJB2 is passed as integer

    return (djb2 + i * i) % size;
}

int get_initial_hash(char key[50])
{

    int initial_hash;

    if (hash_algorithim == DJB2_HASH)
    {

        return DJB2(key);
    }

    return ascii_sum(key);
}

int probe(int hash, int i)
{

    int initial_hash;

    if (probing_technique == Linear)
    {

        return linear_probe_hash(hash, i);
    }

    return quadratic_probe_hash(hash, i);
}

int insert(Word w)
{
    if ( w == NULL ){
        return -1;
    }

    int initial_hash = get_initial_hash(w->key);

    int i = 0;

    int index = probe(initial_hash, i);

    while (Table[index].state == OCCUPIED)
    {

        if (i > size)
        {
            print_err("Hash table is full !");
            exit(0);
        }

        if (Table[index].w != NULL && strcmp(Table[index].w->key, w->key) == 0){
            print_err("Duplicated Word !");
            return -1;
        }

        collisions += 1;

        printf("\033[0;31mCollision at index %d, probe sequence %d !, this is collision number %d\033[0m\n", index, i, collisions);

        i += 1;
        index = probe(initial_hash, i);
    }

    Table[index].w = w;
    Table[index].state = OCCUPIED;

    occupied += 1;

    return index;
}

int index_of(char key[50])
{

    int initial_hash = get_initial_hash(key);

    int i = 0;

    int index = probe(initial_hash, i);

    if (Table[index].state == OCCUPIED && strcmp(Table[index].w->key, key) == 0)
    {
        return index;
    }

    while (Table[index].state != EMPTY && i < size)
    {

        if (Table[index].state == OCCUPIED && strcmp(Table[index].w->key, key) == 0)
        {
            return index;
        }

        i += 1;
        index = probe(initial_hash, i);
    }

    return -1;
}

/////////////////////////////////////////
// ////////////////////// File functions
// /////////////////////////////////////////////////

void read_file()
{

    char filename[20] = "dictionary.txt";

    FILE *fptr = fopen(filename, "r");

    char word[50], meaning[200], line[250];

    int read_args = fscanf(fptr, "%[^:]: %[^\t\n]\n", word, meaning);

    int words_count = 0;

    while (read_args > 0)
    {

        Word w = Create_Word_Obj(word, meaning);

        words_count++;

        int index = insert(w);

        rehash_if_needed();

        printf("Inserting <%s>\n", w->key);
        printf("Index %d\n", index);
        // print_word(w);

        read_args = fscanf(fptr, "%[^:]: %[^\t\n]\n", word, meaning);
    }

    printf("\n\nWord count > %d\n", words_count);

    fclose(fptr);
}

void write_to_file(){

    FILE * fptr  = fopen("dictionary_modified.txt", "w");

    for (int i = 0; i < size; i++)
    {

        if (Table[i].state == OCCUPIED)
        {
            fprintf(fptr, "%s: %s\n", Table[i].w->key, Table[i].w->meaning);
        }
    }

    fclose(fptr);

}

// /////////////////////////////////////////////////
// ////////////////////// Menu
// /////////////////////////////////////////////////

void show_menu()
{

    while (1)
    {

        printf("\n\n\n\033[1;36m----------------------------------------------------------------------");
        printf("\n----------------------------------------------------------------------\033[0m \n\n\n");

        printf("\nAvailable Commands:\n\n");

        printf("\n\033[0;34m --- Operations --- \033[0m\n");

        printf("\033[0;31m1 \033[0m: Search for a word and display its meaning\n");

        printf("\033[0;31m2 \033[0m: Update the meaning of an existing word\n");

        printf("\033[0;31m3 \033[0m: Insert a new word with its meaning\n");

        printf("\033[0;31m4 \033[0m: Print all words in the dictionary with their associated meanings\n");
        printf("\033[0;31m5 \033[0m: Print all the hash table\n");

        printf("\033[0;31m6 \033[0m: Delete a word from the dictionary \n");
        printf("\033[0;31m7 \033[0m: Print the number of occupied slots, hash table size and the load factor \n");

        printf("\033[0;31m8 \033[0m: Save all words back in file dictionary_modified.txt \n");

        printf("\n\033[0;31mq \033[0m: Quit\n");

        printf("\n > \033[0;36mEnter command number to excute : \033[0m");

        int command = 0;
        scanf("%d", &command);

        printf("\n\n");

        switch (command)
        {

        case 1:
            print_heading("Search for a word and display its meaning");

            cmd_1();

            break;

        case 2:
            print_heading("Update the meaning of an existing word");
            cmd_2();
            break;

        case 3:
            print_heading("Insert a new word with its meaning");
            cmd_3();
            break;

        case 4:
            print_heading("Print all words in the dictionary with their associated meanings");

            print_all_words();

            break;

        case 5:
            print_heading("Print all the hash table");
            print_table();
            break;

        case 6:
            print_heading("Delete a word from the dictionary");
            cmd_6();
            break;


        case 7:
            print_heading("Print the number of occupied slots, hash table size and the load factor");

            printf("\n\nOccupied: %d\n", occupied);
            printf("Table Size: %d\n", size);
            printf("\n\nLoad Factor: %.2f\n", load_factor());

            break;

        case 8:
            print_heading("Save all words back in file dictionary_modified.txt");
            write_to_file();
            break;

        
        default:
            printf("\033[1;31minvalid command number, quitting ...\033[0m\n");
            printf("\n\n\n");
            exit(0);
        }
    }
}

// /////////////////////////////////////////////////
// ////////////////////// Menu Functions
// /////////////////////////////////////////////////

void cmd_1()
{
    printf("Enter Word : ");

    char key[50];
    scanf("%s", key);

    int index = index_of(key);

    if (index == -1)
    {
        print_err("word was not found");
        return;
    }

    print_word(Table[index].w);
}

void cmd_2(){

    printf("Enter Word : ");

    char key[50];
    scanf("%s", key);

    int index = index_of(key);

    if (index == -1)
    {
        print_err("word was not found");
        return;
    }

    char trash;
    scanf("%c", &trash);

    printf("Enter the new meaning : ");

    char meaning[200];

    fgets(meaning, sizeof(char) * 200, stdin);
    sscanf(meaning, "%[^\n]", meaning);

    strcpy( Table[index].w -> meaning,  meaning);

}

void cmd_3(){

    printf("Enter Word : ");

    char key[50];
    scanf("%s", key);

    char trash;
    scanf("%c", &trash);

    int index = index_of(key);

    if (index != -1)
    {
        print_err("Duplicate");
        return;
    }

    printf("Enter the meaning : ");

    char meaning[200];

    fgets(meaning, sizeof(char) * 200, stdin);
    sscanf(meaning, "%[^\n]", meaning);

    insert(Create_Word_Obj(key, meaning));

    rehash_if_needed();

}


void cmd_6(){

    printf("Enter Word : ");

    char key[50];
    scanf("%s", key);

    int index = index_of(key);

    if (index == -1)
    {
        print_err("word was not found");
        return;
    }

    Table[index].state = DELETED;

}

// /////////////////////////////////////////////////
// ////////////////////// Print Functions
// /////////////////////////////////////////////////

void print_word(Word w)
{

    if (w == NULL)
    {
        return;
    }

    printf("(%s): %s\n", w->key, w->meaning);
}

void print_table()
{

    print_heading("Table");

    for (int i = 0; i < size; i++)
    {

        if (Table[i].w != NULL)
        {

            printf("    %3d | ", i);

            if (Table[i].state == OCCUPIED)
            {
                printf("\033[0;31mO\033[0m");
                printf(" | %s : \033[0;33m%s\033[0m\n", Table[i].w->key, Table[i].w->meaning);
            }
            else if (Table[i].state == EMPTY)
            {
                printf("\033[0;34mE\033[0m");
                printf(" | \n");
            }
            else
            {
                printf("\033[0;32mD\033[0m");
                printf(" | %s : \033[0;33m%s\033[0m\n", Table[i].w->key, Table[i].w->meaning);
            }
        }
        else
        {
            printf("    %3d | \033[0;34mE\033[0m | \n", i);
        }
    }
}

void print_all_words()
{

    print_heading("All Words");

    for (int i = 0; i < size; i++)
    {

        if (Table[i].state == OCCUPIED)
        {

            printf("    %3d ", i);
            printf(" | %s : \033[0;33m%s\033[0m\n", Table[i].w->key, Table[i].w->meaning);
        }
    }
}

void print_err(char msg[50])
{

    printf("\n\033[0;31m%s\033[0m\n", msg);
}

void print_heading(char heading[])
{
    printf("\n\n-----------------------------------\n> \033[0;31m%s\033[0m \n-----------------------------------\n\n", heading);
}

// /////////////////////////////////////////////////
// ////////////////////// MAIN
// /////////////////////////////////////////////////

int main()
{

    printf("Running ...\n\n\n");
    printf("\033[1;032mBy\n  ___ _               _                              _    _                   \n |_ _| |__  _ __ __ _| |__   ___  ___ _ __ ___      / \\  | |_   _  __ _ _ __  \n  | || '_ \\| '__/ _` | '_ \\ / _ \\/ _ \\ '_ ` _ \\    / _ \\ | | | | |/ _` | '_ \\ \n  | || |_) | | | (_| | | | |  __/  __/ | | | | |  / ___ \\| | |_| | (_| | | | |\n |___|_.__/|_|  \\__,_|_| |_|\\___|\\___|_| |_| |_| /_/   \\_\\_|\\__, |\\__,_|_| |_|\n                                                            |___/              \033[0m");

    printf("\n\n\n\n\n\033[1;032mHIT ENTER\033[0m to continue  \n\n\n ");

    char trash;
    scanf("%c", &trash);

    printf("\nChoose a String Hash Functions:\n\n");

    printf("\033[0;31m1 \033[0m: DJB2 \n");
    printf("\033[0;31m2 \033[0m: ASCII Sum \n");
    printf("\n > \033[0;36mEnter the number of function : \033[0m");

    int hash_func = 0;
    scanf("%d", &hash_func);

    if (hash_func == 1)
    {
        hash_algorithim = DJB2_HASH;
    }
    else
    {
        hash_algorithim = ASCII_SUM;
    }

    printf("\n\n");

    printf("\nChoose a Probing Technique:\n\n");

    printf("\033[0;31m1 \033[0m: Linear \n");
    printf("\033[0;31m2 \033[0m: Quadratic \n");

    printf("\n > \033[0;36mEnter the number of technique : \033[0m");

    int probing_teq = 0;
    scanf("%d", &probing_teq);

    if (probing_teq == 1)
    {
        probing_technique = Linear;
    }
    else
    {
        probing_technique = Quadratic;
    }

    read_file();

    print_table();

    printf("\033[0;31m\n\nCollisions: %d\033[0m\n\n", collisions);

    show_menu();
}