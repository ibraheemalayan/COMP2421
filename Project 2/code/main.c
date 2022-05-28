/*


                Birzeit University
            Department of Computer Science
                    COMP2421
            Data Structures And Algoritims 

            Project #1

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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// /////////////////////////////////////////////////
// ////////////////////// Define Structs
// /////////////////////////////////////////////////

struct Node {
	char key[50];
    char meaning[200];
	struct Node *left, *right;
} Node;

typedef struct Node * BST;


// /////////////////////////////////////////////////
// ////////////////////// Prototypes
// /////////////////////////////////////////////////


BST Create_Node(char word[50], char meaning[200]);
BST insert(BST Tree, char key[50], char meaning[200]);
BST leftMost(BST root);
BST delete_node(BST Tree, char key[50]);
void show_menu(BST root);
void cmd_2(BST root);
void cmd_3(BST root);
BST cmd_4(BST root);
void cmd_6(BST root);
BST cmd_7(BST root);
BST cmd_8(BST root);
void traverse_inorder(BST root);
void print_word(BST node);
void print_tree(BST root, int level);
void print_err(char msg[50]);
void print_heading(char heading[]);
BST read_file(BST root);
void print_starts_with(BST Tree, char k);
BST delete_starts_with(BST Tree, char k);
void write_to_file(BST root);
int write_word(BST node, FILE * fptr, int index);


// /////////////////////////////////////////////////
// ////////////////////// BST Functions
// /////////////////////////////////////////////////

// creates new nodes
BST Create_Node(char word[50], char meaning[200])
{
	BST new_node
		= (struct Node*)malloc(sizeof(struct Node));

    strcpy(new_node->key, word);

    strcpy(new_node->meaning, meaning);

	new_node->left = new_node->right = NULL;
	return new_node;
}

// insert node to BST
BST insert(BST Tree, char key[50], char meaning[200])
{
	// If tree is empty (or reached a leaf in recursion)
	if (Tree == NULL){
        return Create_Node(key, meaning);
    }

    int cmp = strcmp(key, Tree -> key);

	if (cmp < 0){
        Tree->left = insert(Tree->left, key, meaning);
    }
	else if (cmp > 0){
		Tree->right = insert(Tree->right, key, meaning);
    }

	return Tree;
}


BST leftMost(BST root)
{
	BST current = root;

	// keep going left until the leaf
	while (current != NULL && current->left != NULL){
		current = current->left;
    }

	return current;
}

BST search(BST Tree, char key[50]){

    // If tree is empty (or reached a leaf in recursion)
	if (Tree == NULL){
        return Tree;
    }

    int cmp = strcmp(key, Tree -> key);

	if (cmp < 0){ // key is smaller than current tree root

        return search(Tree->left, key);

    }
	else if (cmp > 0){ // key is greater than current tree root

		return search(Tree->right, key);

    }
    
    return Tree;


}

void print_starts_with(BST Tree, char k){

    // If tree is empty (or reached a leaf in recursion)
	if (Tree == NULL){
        return;
    }

	if (k < Tree -> key[0]){ // k is smaller than first char of current tree root

        print_starts_with(Tree->left, k);

    }
	else if (k > Tree -> key[0]){ // k is greater than first char of current tree root

		print_starts_with(Tree->right, k);

    }else{ // equal

        print_starts_with(Tree->left, k);
        print_word(Tree);
        print_starts_with(Tree->right, k);

    }
}

BST delete_starts_with(BST Tree, char k){

    // If tree is empty (or reached a leaf in recursion)
	if (Tree == NULL){
        return Tree;
    }

	if (k < Tree -> key[0]){ // k is smaller than first char of current tree root

        Tree->left = delete_starts_with(Tree->left, k);

    }
	else if (k > Tree -> key[0]){ // k is greater than first char of current tree root

		Tree->right = delete_starts_with(Tree->right, k);

    }else{ // equal

        Tree->left = delete_starts_with(Tree->left, k);
        Tree->right = delete_starts_with(Tree->right, k);

        printf("\ndeleting word %s\n", Tree -> key);

        return delete_node(Tree, Tree->key);

    }

    return Tree;
}

BST delete_node(BST Tree, char key[50])
{

    // If tree is empty (or reached a leaf in recursion)
	if (Tree == NULL){
        return Tree;
    }

    int cmp = strcmp(key, Tree -> key);

	if (cmp < 0){ // key is smaller than current tree root

        Tree->left = delete_node(Tree->left, key);

    }
	else if (cmp > 0){ // key is greater than current tree root

		Tree->right = delete_node(Tree->right, key);

    } else { // key is the current tree root

		// Node with no child on the right
        if (Tree->right == NULL) {
			BST temp = Tree->left;
			free(Tree);
			return temp;
		}

        // Node with no child on the left
        if (Tree->left == NULL) {
			BST temp = Tree->right;
			free(Tree);
			return temp;
		}


		// Node with two children:
		BST temp = leftMost(Tree->right);
		strcpy(Tree->key, temp->key);
        strcpy(Tree->meaning, temp->meaning);
		Tree->right = delete_node(Tree->right, temp->key);

	}
	return Tree;
}

int main()
{

	BST root = NULL;

    printf("Running ...\n\n\n");
    printf("\033[1;032mBy\n  ___ _               _                              _    _                   \n |_ _| |__  _ __ __ _| |__   ___  ___ _ __ ___      / \\  | |_   _  __ _ _ __  \n  | || '_ \\| '__/ _` | '_ \\ / _ \\/ _ \\ '_ ` _ \\    / _ \\ | | | | |/ _` | '_ \\ \n  | || |_) | | | (_| | | | |  __/  __/ | | | | |  / ___ \\| | |_| | (_| | | | |\n |___|_.__/|_|  \\__,_|_| |_|\\___|\\___|_| |_| |_| /_/   \\_\\_|\\__, |\\__,_|_| |_|\n                                                            |___/              \033[0m");

    printf("\n\n\n\n\n\033[1;032mHIT ENTER\033[0m to read the data from dictionary.txt  \n\n\n ");

    char trash;
    scanf("%c", &trash);

    root = read_file(root);

    /////// sample testing data

	// root = insert(root, "hh", "H");
    // root = insert(root, "cc", "C");
    // root = insert(root, "dd", "D");
    // root = insert(root, "jb", "JA");
    // root = insert(root, "aa", "A");
    // root = insert(root, "jj", "J");
    // root = insert(root, "kk", "K");
    // root = insert(root, "ii", "I");
    // root = insert(root, "ab", "AB");
    // root = insert(root, "ac", "AC");
    // root = insert(root, "jm", "JM");
    // root = insert(root, "jk", "JK");
    // root = insert(root, "ja", "JA");
    // root = insert(root, "jj", "JA");
    // root = insert(root, "ad", "AD");

    show_menu(root);

	return 0;
}

// /////////////////////////////////////////////////
// ////////////////////// File functions
// /////////////////////////////////////////////////

BST read_file(BST root){


    char filename[20] = "dictionary.txt";

    FILE* fptr = fopen(filename, "r");

    char word[50], meaning[200], line[600];

    while(fgets(line, sizeof(char) * 600, fptr)){

        int chars_now;
        int chars_consumed = 0;
    
        int args_read = sscanf( line + chars_consumed, "%*d. %[^:]: %[^\t\n0123456789]%n", word, meaning, &chars_now);

        
     
        while ( args_read > 0 ) {
    
            chars_consumed += chars_now;
            
            printf("inserting word (%s)\n", word);
    
            root = insert(root, word, meaning);

            args_read = sscanf( line + chars_consumed, "%*d. %[^:]: %[^\t\n0123456789]%n", word, meaning, &chars_now);
        }

        // if (!fgets(line, sizeof(char) * 600, fptr)){
        //     break;
        // };
    
    }

    fclose(fptr);

    return root;
    
}

void write_to_file(BST root){

    FILE * fptr  = fopen("dictionary_out.txt", "w");

    write_word(root, fptr, 1);

    fclose(fptr);

}

int write_word(BST node, FILE * fptr, int index){

    if (node == NULL){
        return index;
    }

    char line[300];

    sprintf(line, "%d. %s: %s\t", index, node->key, node->meaning);

    index += 1;

    fputs(line, fptr);

    if ( index % 4 == 0 ){ // print each 4 on a line
        fputs("\n", fptr);
    }

    index = write_word(node -> left, fptr, index);

    return write_word(node -> right, fptr, index);
    
}

// /////////////////////////////////////////////////
// ////////////////////// Menu
// /////////////////////////////////////////////////

void show_menu(BST root)
{

    while (1)
    {

        printf("\n\n\n\033[1;36m----------------------------------------------------------------------");
        printf("\n----------------------------------------------------------------------\033[0m \n\n\n");

        printf("\nAvailable Commands:\n\n");

        printf("\n\033[0;34m --- Debug --- \033[0m\n");
        printf("\033[0;31m1 \033[0m: Draw Tree\n");

        printf("\n\033[0;34m --- Operations --- \033[0m\n");

        printf("\033[0;31m2 \033[0m: Search for a word and display its meaning\n");

        printf("\033[0;31m3 \033[0m: Update the meaning of an existing word\n");

        printf("\033[0;31m4 \033[0m: Insert a new word with its meaning\n");

        printf("\033[0;31m5 \033[0m: Print all words in the dictionary in an alphabetic order with their associated meanings\n");
        printf("\033[0;31m6 \033[0m: Print all words that start with a specific character in an alphabetic order\n");

        printf("\033[0;31m7 \033[0m: Delete a word from the dictionary \n");
        printf("\033[0;31m8 \033[0m: Delete all words that start with a specific letter \n");

        printf("\033[0;31m9 \033[0m: Save all words back in file dictionary_out.txt \n");

        printf("\n\033[0;31mq \033[0m: Quit\n");

        printf("\n > \033[0;36mEnter command number to excute : \033[0m");

        int command = 0;
        scanf("%d", &command);

        printf("\n\n");

        switch (command)
        {
        case 1:
            print_heading("Draw Tree");
            print_tree(root, 0);
            break;

        case 2:
            print_heading("Search for a word and display its meaning");

            cmd_2(root);

            break;

        case 3:
            print_heading("Update the meaning of an existing word");

            cmd_3(root);

            break;

        case 4:
            print_heading("Insert a new word with its meaning");

            root = cmd_4(root);

            break;

        case 5:
            print_heading("Print all words in the dictionary in an alphabetic order with their associated meanings");
            traverse_inorder(root);
            break;

        case 6:
            print_heading("Print all words that start with a specific character in an alphabetic order");

            cmd_6(root);

            break;

        case 7:
            print_heading("Delete a word from the dictionary");
            
            cmd_7(root);

            break;

        case 8:
            print_heading("Delete all words that start with a specific letter");

            root = cmd_8(root);
            
            break;

        case 9:
            print_heading("Save all words back in file dictionary_out.txt");

            write_to_file(root);
            
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

void cmd_2(BST root){
    printf("Enter Word : ");

    char key[50];
    scanf("%s", key);

    BST node = search(root, key);

    if ( node == NULL ){
        print_err("word was not found");
        return;
    }

    print_word(node);
}

void cmd_3(BST root){

    printf("Enter Word : ");

    char key[50];
    scanf("%s", key);



    BST node = search(root, key);

    if ( node == NULL ){
        print_err("word was not found");
        return;
    }

    char trash;
    scanf("%c", &trash);


    printf("Enter the new meaning : ");

    char meaning[200];

    fgets(meaning, sizeof(char) * 200, stdin);
    sscanf(meaning, "%[^\n]", meaning);

    strcpy( node -> meaning,  meaning);

}

BST cmd_4(BST root){

    printf("Enter Word : ");

    char key[50];
    scanf("%s", key);

    BST node = search(root, key);

    if ( node != NULL ){
        print_err("word already exists !");
        return root;
    }

    char trash;
    scanf("%c", &trash);

    printf("Enter the meaning : ");

    char meaning[200];

    fgets(meaning, sizeof(char) * 200, stdin);
    sscanf(meaning, "%[^\n]", meaning);

    return insert(root, key, meaning);
}

void cmd_6(BST root){

    printf("Enter character : ");

    char trash;
    scanf("%c", &trash);

    char k;
    scanf("%c", &k);

    print_starts_with(root, k);
    
}

BST cmd_7(BST root){

    printf("Enter Word : ");

    char key[50];
    scanf("%s", key);

    return delete_node(root, key);
    
}

BST cmd_8(BST root){

    printf("Enter character : ");

    char trash;
    scanf("%c", &trash);

    char k;
    scanf("%c", &k);

    return delete_starts_with(root, k);
    
}

// /////////////////////////////////////////////////
// ////////////////////// Print Functions
// /////////////////////////////////////////////////

// InOrder traversal of BST
void traverse_inorder(BST root)
{
	if (root != NULL) {
		traverse_inorder(root->left);
		printf("(%s): %s\n", root->key, root->meaning);
		traverse_inorder(root->right);
	}
}

void print_word(BST node){

    if (node == NULL){
        return;
    }

    printf("(%s): %s\n", node->key, node->meaning);

}

// for debugging Print BST (keys only)
void print_tree(BST root, int level)
{
	    

        for (int i = 0; i < level; i++){
            printf(i == level - 1 ? " |--- " : "      ");
        }

        if (root == NULL){
            printf("- \n\n");
            return;
        }

        // printf("(%s): %s] \n\n", root->key, root->meaning);
        printf("(%s) \n\n", root->key);



        print_tree(root->left, level + 1);
        print_tree(root->right, level + 1);

}

void print_err(char msg[50]){

    printf("\n\033[0;31m%s\033[0m\n", msg);

}

void print_heading(char heading[])
{
    printf("\n\n-----------------------------------\n> \033[0;31m%s\033[0m \n-----------------------------------\n\n", heading);
}