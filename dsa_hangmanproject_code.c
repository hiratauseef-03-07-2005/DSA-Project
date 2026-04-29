#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#define MAX 100
struct wordnode {
    char word[20];
    char clue[200];
    struct wordnode* left;
    struct wordnode* right;
};

struct GuessStack {
    char letter;
    struct GuessStack* next;
};
struct node{
char name[20];
int score;
struct node* next;
};
struct nodeforgraph{//-------------FOR WORD RELATIONSHIP GRAPH------------------
char word[50];
struct nodeforgraph* next;
};

struct nodeforgraph* adjlist[MAX];
int nodecount=0;
char namesnode[MAX][50];
struct node* leaderboard_details=NULL;
struct GuessStack* top = NULL;
char currentWord[20];
int wordLength=0;
int wrongGuesses = 0;
int attemptsLeft = 6;
int score=0;

//-----------------reset game--------------------

void resetgame(){
top=NULL;
wordLength=0;
wrongGuesses=0;
attemptsLeft=6;
score=0;
memset(currentWord,0,sizeof(currentWord));
}

//---------------word relationship graph----------
int getindex(char word[]){
for(int i=0;i<nodecount;i++){
    if(strcmp(namesnode[i],word)==0)
        return i;
}

    return -1;

}
int addnode(char word[]){
int index=getindex(word);
if(index==-1){
    strcpy(namesnode[nodecount],word);
    index=nodecount;
    nodecount++;
}
return index;
}
void addedges(char u[],char v[]){
    int ind_u=addnode(u);
    int ind_v=addnode(v);
struct nodeforgraph* node1=(struct nodeforgraph*)malloc(sizeof(struct nodeforgraph));
strcpy(node1->word,v);
node1->next=adjlist[ind_u];
adjlist[ind_u]=node1;

struct nodeforgraph* node2=(struct nodeforgraph*)malloc(sizeof(struct nodeforgraph));
strcpy(node2->word,u);
node2->next=adjlist[ind_v];
adjlist[ind_v]=node2;

}

//---------additional function----------

struct wordnode* searchInBST(struct wordnode* root,char word[]) {
    if (root == NULL)
        return NULL;
    int cmp = strcmp(word, root->word);
    if (cmp == 0)
        return root;
    else if (cmp < 0)
        return searchInBST(root->left, word);
    else
        return searchInBST(root->right, word);
}

struct wordnode* getrelatedword(struct wordnode* root,char baseword[]){
int index=getindex(baseword);
if(index==-1 || adjlist[index]==NULL){
    return NULL;
}
int count =0;
struct nodeforgraph* temp=adjlist[index];
while(temp!=NULL){
    if(strcmp(temp->word,baseword)!=0){
        count++;
        temp=temp->next;
    }
}
if(count==0){
    return NULL;
}
int pick=rand()%count;
temp=adjlist[index];
int current=0;
while(temp!=NULL){
    if(strcmp(temp->word,baseword)!=0){
        if(current==pick){
            return searchInBST(root,temp->word);

        }
        current++;
    }
    temp=temp->next;
}
return NULL;
};



void printgraph(char word[]){
int index=getindex(word);
if(index==-1){
    printf("node not found....\n");
}
else{
    printf("\n========== WORD RELATIONSHIP GRAPH ==========\n");
printf("Base Word: %s\n", word);
printf("---------------------------------------------\n");
printf("%s", namesnode[index]);

struct nodeforgraph* temp = adjlist[index];
while (temp != NULL) {
    printf(" -> %s", temp->word);
    temp = temp->next;
}
printf(" -> NULL\n");
printf("=============================================\n");

}
}
// ---------------- BST Functions ----------------

struct wordnode* addword(struct wordnode* root, char word[],const char* clue) {
    if (root == NULL) {
        struct wordnode* t = (struct wordnode*)malloc(sizeof(struct wordnode));
        strcpy(t->word, word);
        strcpy(t->clue,clue);
        t->left = NULL;
        t->right = NULL;
        return t;
    }
    if (strcmp(word, root->word) < 0) {
        root->left = addword(root->left, word,clue);
    }
    if (strcmp(word, root->word) > 0) {
        root->right = addword(root->right, word,clue);
    }
    return root;
}

int countnodes(struct wordnode* root) {
    if (root == NULL) return 0;
    return 1 + countnodes(root->left) + countnodes(root->right);
}

struct wordnode* randomnode(struct wordnode* root, int ran, int* count) {
    if (root == NULL) return NULL;

    struct wordnode* left = randomnode(root->left, ran, count);
    if (left != NULL) return left;

    if (*count == ran) return root;
    (*count)++;

    return randomnode(root->right, ran, count);
}

// ---------------- Stack Functions ----------------

void addGuess(char letter) {
    struct GuessStack* newGuess = (struct GuessStack*)malloc(sizeof(struct GuessStack));
    newGuess->letter = toupper(letter);
    newGuess->next = top;
    top = newGuess;
}

int wasGuessed(char letter) {
    struct GuessStack* current = top;
    while (current != NULL) {
        if (current->letter == toupper(letter))
            return 1;
        current = current->next;
    }
    return 0;
}

// ---------------- Hangman Display ----------------

void drawHangman(int wrong) {
        printf("\n");
    printf("=========================================================\n");
    printf("                  THE FINAL CURVE                    \n");
    printf("=========================================================\n");
    printf("      A life dangles by a thread... every letter counts. \n");
    printf("=========================================================\n\n");
    const char *space = "                          "; // 26 spaces (80-char console width)

    printf("\n\n");
    printf("%s +---+\n", space);
    printf("%s |   |\n", space);

    switch (wrong) {
        case 0:
            printf("%s |    \n", space);
            printf("%s |    \n", space);
            printf("%s |    \n", space);
            printf("%s |    \n", space);
            break;
        case 1:
            printf("%s |   O\n", space);
            printf("%s |    \n", space);
            printf("%s |    \n", space);
            printf("%s |    \n", space);
            break;
        case 2:
            printf("%s |   O\n", space);
            printf("%s |   |\n", space);
            printf("%s |    \n", space);
            printf("%s |    \n", space);
            break;
        case 3:
            printf("%s |   O\n", space);
            printf("%s |  /|\n", space);
            printf("%s |    \n", space);
            printf("%s |    \n", space);
            break;
        case 4:
            printf("%s |   O\n", space);
            printf("%s |  /|\\\n", space);
            printf("%s |    \n", space);
            printf("%s |    \n", space);
            break;
        case 5:
            printf("%s |   O\n", space);
            printf("%s |  /|\\\n", space);
            printf("%s |  / \n", space);
            printf("%s |    \n", space);
            break;
        case 6:
            printf("%s |   O\n", space);
            printf("%s |  /|\\\n", space);
            printf("%s |  / \\\n", space);
            printf("%s |  DEAD\n", space);
            break;
    }

    printf("%s |\n", space);
    printf("%s=========\n", space);
}


int validate_name(char name[]){
for(int i=0;name[i]!='\0';i++){
    if(!isalpha(name[i])){
        return 0;
    }
}
return 1;
}

//-----------leaderboard work------------------

struct node* insertdetails_leader(struct node* root,char name[],int score){//leaderboard linklist
    struct node* p=(struct node*)malloc(sizeof(struct node));
    strcpy(p->name,name);
    p->score=score;
    p->next=NULL;
    if(root==NULL || score>root->score){
        p->next=root;
        return p;
    }
    else{
        struct node* current=root;
        while(current->next!=NULL && current->next->score>=score){
            current=current->next;
        }
        p->next=current->next;
        current->next=p;
    }
    return root;
    };

void display_leaderboardvianode(struct node* leaderboard_details){
struct node* p=leaderboard_details;
printf("\n\n");
    printf("==================================================================\n");
    printf("                      LEADERBOARD                           \n");
    printf("==================================================================\n");
    printf("| %-25s | %-15s |\n", "PLAYER NAME", "SCORE");
    printf("------------------------------------------------------------------\n");

    if (p == NULL) {
        printf("| %-41s |\n", "No players in leaderboard yet.");
    } else {
        while (p != NULL) {
            printf("| %-25s | %-15d |\n", p->name, p->score);
            p = p->next;
        }
    }

    printf("==================================================================\n\n");
}

//----------file to save leaderboard details--------------

void saveScore(char playerName[], int score) {
    FILE *file = fopen("leaderboard.txt", "a"); // "a" for append mode
    if (file == NULL) {
        printf("Error opening file!\n");
        return;
    }
    fprintf(file, "%s %d\n", playerName, score);
    fclose(file);
}

void retrievedata_fromfile(){
    FILE* file=fopen("leaderboard.txt","r");
    if(file==NULL){
leaderboard_details=NULL;
    }
    else{
            char name[20];
            int score;
    while(fscanf(file,"%s %d\n",name,&score)== 2){
    leaderboard_details=insertdetails_leader(leaderboard_details,name,score);
    }

    fclose(file);
    }
}

//-------------displaying clue function-----------

void display_clue(struct wordnode* currentselectedword){
    if(currentselectedword!=NULL){
printf("\nclue is %s",currentselectedword->clue);
    }
    else{
        printf("\nno clue");
    }
    printf("\npress any key to continue: ");
 getchar(); getchar();
}

//----------displaying game function-----------

void displayGame() {
    system("cls"); // Use "clear" for Linux/Mac
    drawHangman(wrongGuesses);

        printf("\n\nWord: ");
    for (int i = 0; i < strlen(currentWord); i++) {
        int found = 0;
        struct GuessStack* current = top;
        while (current != NULL) {
            if (tolower(current->letter) == tolower(currentWord[i])) {
                printf("%c ", current
                       ->letter);  // print as uppercase as guessed
                found = 1;
                break;
            }
            current = current->next;
        }
        if (!found) {
            printf("_ ");
        }
    }


         printf("\n\nWrong guesses: ");
    struct GuessStack* current = top;
    while (current != NULL) {
        int isWrong = 1;
        for (int i = 0; i < strlen(currentWord); i++) {
            if (tolower(current->letter) == tolower(currentWord[i])) {
                isWrong = 0;
                break;
            }
        }
        if (isWrong) {
            printf("%c ", current->letter);
        }
        current = current->next;
    }

    printf("\nAttempts left: %d\n", attemptsLeft);
    printf("Your Score: %d\n",score);
}

// ---------------- Load and Run Game ----------------
void loadgame() {
    resetgame();
    for(int i=0;i<MAX;i++){
    adjlist[i]=NULL;
}

    srand(time(NULL));
    char player_name[20];
    char relatedchoice;
    int gameendedbywin=0;
    struct wordnode* newone = NULL;
    newone= addword(newone, "apple","snowwhite fell in sweet lumber\n");
    newone = addword(newone, "wizard","Casts spells\nLong beard, robe, and staff\nThink Gandalf or Dumbledore");
    newone = addword(newone, "mango","Mirza Ghalib ko kia marghub tha\n");
    newone = addword(newone, "zebra","donkey painted");
    newone = addword(newone, "sherlock","Solves mysteries\nLives on Baker Street\nBrilliant detective skills");
    newone = addword(newone, "batman","Fights crime at night\nWears a mask\nGotham city");
    newone = addword(newone, "firewall","Security barrier\nBlocks threats\nNetwork protection");
    newone = addword(newone, "portal","Dimensional gate\nMagic door\nTime travel");
    newone = addword(newone, "hourglass","Timekeeper\nSand trickle\nAncient clocka");
    newone = addword(newone, "pinkpanthar","Cool and pink\nSneaky and clever cat\nSilent comedy star");
    newone = addword(newone, "lemon","sour");
    // Fruits Category  difficult wale
    newone = addword(newone, "dragonfruit", "Exotic pink fruit\nLooks like fire\nFrom cactus family");
    newone = addword(newone, "kiwi", "Fuzzy brown outside\nGreen inside\nShares name with a bird");
    newone = addword(newone, "pomegranate", "Many ruby seeds\nSymbol of prosperity\nDifficult to eat");

// Technology Category  difficult
   newone = addword(newone, "blockchain", "Digital ledger\nDecentralized\nBitcoin technology");
   newone = addword(newone, "algorithm", "Step-by-step procedure\nUsed in computing\nLike a recipe for computers");
   newone = addword(newone, "metaverse", "Virtual reality world\nFuture internet\nFacebook's new name");

// Mythology Category
  newone = addword(newone, "phoenix", "Reborn from ashes\nFire bird\nMythical creature");
  newone = addword(newone, "minotaur", "Half man half bull\nLabyrinth dweller\nGreek mythology");
  newone = addword(newone, "valkyrie", "Chooses who dies in battle\nNorse mythology\nRides winged horses");

// Animals Category
  newone = addword(newone, "narwhal", "Arctic whale\nUnicorn horn\nReal life sea unicorn");
  newone = addword(newone, "axolotl", "Mexican walking fish\nCan regenerate\nPink aquatic salamander");
  newone = addword(newone, "quokka", "World's happiest animal\nAustralian marsupial\nAlways smiling");

// Science Category
  newone = addword(newone, "quantum", "Smallest discrete unit\nPhysics theory\nSchrödinger's cat");
  newone = addword(newone, "neutrino", "Ghost particle\nAlmost massless\nPasses through matter");
  newone = addword(newone, "photosynthesis", "Plant energy process\nUses sunlight\nProduces oxygen");

    addedges("mango","apple");
    addedges("mango","dragonfruit");
    addedges("mango","lemon");
    addedges("apple","pomegranate");
    addedges("apple","lemon");
    addedges("wizard","portal");
    addedges("wizard","hourglass" );
    addedges("batman","sherlock");
    addedges("batman","firewall");
    addedges("sherlock","wizard");
    addedges("pinkpanthar","batman");
    // Cross-Category Links
   addedges("wizard","quantum"); // Magic and physics
   addedges("portal","metaverse");
   addedges("sherlock","algorithm");

   addedges("blockchain","phoenix");
   addedges("minotaur","zebra");
   addedges("quokka","narwhal");
    int total_nodes = countnodes(newone);
    int ran = rand() % total_nodes;
    int count = 0;
    struct wordnode* selected = randomnode(newone, ran, &count);
    strcpy(currentWord, selected->word);
    wordLength = strlen(currentWord);

    printf("Random word selected! Let's play.\n");

    // ----------- Start Game Loop ------------

    while (attemptsLeft > 0) {
        displayGame();
       char guess;
       printf("Enter your guess (a-z) or '?' to get a clue: ");
scanf(" %c", &guess);

if (guess == '?') {
    display_clue(selected);
    continue;
}

     if (!isalpha(guess)) {
     printf("Invalid input! Please enter a letter (a-z).\n");
     getchar();getchar();
     continue;
}

  guess = tolower(guess);


        if (wasGuessed(guess)) {
            printf("You already guessed that letter.\n");
            continue;
        }

        addGuess(guess);

        if (!strchr(currentWord, guess)) {
            wrongGuesses++;
            attemptsLeft--;
            score-=1;
        }
        else{
            score+=2;
        }

        // Check if all letters are guessed
        int allGuessed = 1;
        for (int i = 0; i < wordLength; i++) {
            if (!wasGuessed(currentWord[i])) {
                allGuessed = 0;

                break;
            }
        }

        if (allGuessed) {
            displayGame();
            printf("\n\n?? Congratulations! You guessed the word: %s\n", currentWord);
            printgraph(currentWord);
           // fflush(stdin);
           getchar();
            printf("do you want a related word of the guessed word (yes(y)/any key to exit game): ");
            scanf(" %c",&relatedchoice);
            // while (getchar() != '\n' && getchar() != EOF);

            if(tolower(relatedchoice)=='y'){
                struct wordnode* relatedword=getrelatedword(newone,currentWord);
                if(relatedword==NULL){
                    printf("no word has been found....\n");
                    break;
                }
                strcpy(currentWord,relatedword->word);
                top=NULL;
                wordLength=strlen(currentWord);
                wrongGuesses=0;
                attemptsLeft=6;
                score+=5;
                selected=relatedword;
                printf("a new word has been selected of same category...\n");
                fflush(stdin);
                continue;
            }
            else{
                 do{
                    printf("enter your name: ");
            scanf("%s",player_name);
            if(!validate_name(player_name)){
                printf("invalid name.....it should only contain alphabets only\n");
            }
            }while(!validate_name(player_name));
            printf("Your Score: %d\n",score);
            saveScore(player_name,score);
           // fflush(stdin);
            // while (getchar() != '\n' && getchar() != EOF);
            printf("name added to leaderboard file\n");
                gameendedbywin=1;
                break;
            }

        }
    }
if(attemptsLeft<=0){
        displayGame();
   //      printf("\n\n?? Game Over! The correct word was: %s\n", currentWord);
 do{
                    printf("enter your name: ");
            scanf("%s",player_name);
            if(!validate_name(player_name)){
                printf("invalid name.....it should only contain alphabets only\n");
            }
            }while(!validate_name(player_name));
            printf("Your Score: %d\n",score);
            saveScore(player_name,score);
           // fflush(stdin);
            // while (getchar() != '\n' && getchar() != EOF);
            printf("name added to leaderboard file\n");
             if(!gameendedbywin){
    //displayGame();
    printf("\n\n?? Game Over! The correct word was: %s\n", currentWord);
    }

}
    // If user runs out of attempts

}

// ---------------- Main ----------------
int main() {
    int choice;
    char replay;
    do{
            system("cls");
    printf("\n");
    printf("\t\t========================================================\n");
    printf("                   \t\t\t?  THE GALLOWS  ?                 \n");
    printf("\t\t========================================================\n");
    printf("        \t\t\t A life hangs in the balance...              \n");
    printf("      \t\t\tOne word holds the key to their fate.          \n");
    printf("\t\t--------------------------------------------------------\n");
    printf("    \t\t\t You have limited guesses. Choose wisely.        \n");
    printf("    \t\t\t Each wrong letter tightens the noose.           \n");
    printf("\t\t--------------------------------------------------------\n");
    printf("     \t\t\t Will you uncover the word in time...           \n");
    printf("           \t\t\tor witness the final drop?                \n");
    printf("\t\t=======================================================\n");
    printf("              \t\t\t Press any key to begin.               \n");
    printf("\t\t=======================================================\n");
    printf("\n");

    printf("1. START GAME\n");
    printf("2. SHOW LEADERBOARD\n");
    printf("3. EXIT GAME\n");

    printf("Enter your choice: ");
    scanf("%d", &choice);
while(choice<=0 || choice>3){
    printf("invalid.......\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);
}
    switch (choice) {
        case 1:
            system("cls");
            printf("Game starting...\n");
            loadgame();
            break;

        case 2:
            system("cls");
            retrievedata_fromfile();
            display_leaderboardvianode(leaderboard_details);
            break;

        case 3:
            return 0;

        default:
            printf("Invalid option entered.\n");
            break;
    }


    printf("enter 1 to return to main menu or any key to exit: ");
    scanf(" %c",&replay);
    }while(replay=='1');

    return 0;
}

