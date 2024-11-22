#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

struct Character {
    char name[50]; //character name
    int level;  //character level
    char class[50]; //character class
    char background[50];  //character background
    char race[50]; //character race
    char alignment[50];  //character alignment
    int strength; //character strength val
    int dexterity;  //character dexterity val
    int constitution;  //character constitution val
    int intelligence;  //character intelligence val
    int wisdom;  //character wisdom val
    int charisma;  //character charisma val
    int speed;  //character speed val
    int armorClass;  //character armor class
    char armor[50];  //character armor - helps determine their armor class
    int hasShield;  //characters has a shield or not - helps determine their armor class
    struct Character *next;  //points to the next character in the list
};


//calculate functions
int calculateModifier(int attribute); //takes attributes from character and converts it to their modifier
int calculateArmorClass(int dexterity, const char *armor, int hasShield);


//display functions
void addCharacter(struct Character **head); //adds new character to character list
void displayCharacter(struct Character *head); //displays your characters
void searchCharacter(struct Character *head, char *searchName); //searches for a character you have created and prints it
void updateCharacter(struct Character *head, char *updateCharacterName); //updates an existing character with new data
void deleteCharacter(struct Character **head, char *deleteCharacterName); //deletes an existing character
int calculateRollModifier(struct Character *head, char *diceCharacterName, int numChoice);
int rollD20(); //rolls a D20

int main(){

srand(time(NULL)); //seeds a random number

char searchCharacterName[50]; //Array to store name for searching
char updateCharacterName[50]; //Array to store name for updating
char deleteCharacterName[50]; //Array to store name for deleting
char diceCharacterName[50];  //Array to store name for dice rolling

int choice; //users choice
int numChoice; //users choice for dice roll
struct Character *characterList = NULL;

do{
    //menu options
    printf("\nWelcome to the DnD Character Creator!\n\n");
    printf("1. Add a new character\n");
    printf("2. Display character\n");
    printf("3. Search for character\n");
    printf("4. Update character\n");
    printf("5. Delete character\n");
    printf("6. Roll a D20 with modifiers\n");
    printf("7. Roll a D20\n");
    printf("8. Exit Dnd Character Creator\n");
    printf("Enter your choice: ");
    scanf("%d", &choice); //user's choice

    //based on user input:
    if(choice == 1){
        addCharacter(&characterList);
    }

    if(choice == 2){
        displayCharacter(characterList);
    }

    if(choice == 3){
        printf("Enter the name of the character to search for: ");
        scanf("%s", searchCharacterName);
        searchCharacter(characterList, searchCharacterName); 
    }

    if(choice == 4){
        printf("Enter the name of your character to update: ");
        scanf("%s", updateCharacterName); 
        updateCharacter(characterList, updateCharacterName);
    }
    if(choice == 5){
        printf("Enter the name of your character to delete: ");
        scanf("%s", deleteCharacterName); 
        deleteCharacter(&characterList, deleteCharacterName); 
    }
    if(choice == 6){
        printf("Enter the name of your character: ");
        scanf("%s", diceCharacterName); 
        printf("Enter which modifier to add on to your roll:\n");
        printf("1. Strength\n");
        printf("2. Dexterity\n");
        printf("3. Constitution\n");
        printf("4. Intelligence\n");
        printf("5. Wisdom\n");
        printf("6. Charisma\n");
        printf("7. None\n");
        printf("Enter your choice: ");
        scanf("%d", &numChoice); //user's choice

        printf("You rolled: %d\n\n", calculateRollModifier(characterList, diceCharacterName, numChoice));
    }
    if(choice == 7){
        printf("You rolled: %d", rollD20());
    }
    if(choice == 8){
        printf("Exiting DnD Character Creator...\n");
    }
} while(choice != 8);


struct Character *temp;
    while (characterList != NULL) {
        temp = characterList;
        characterList = characterList->next;
        free(temp);
    }

return 0;
}


//functions:

void addCharacter(struct Character **head){
    //make space for new character in memory
    struct Character *newCharacter = (struct Character *)malloc(sizeof(struct Character));

    if (newCharacter == NULL){
        printf("Failed to allocate memory :(\n");
        exit(1);
    }

    //prompt user to enter details for character
    printf("For more information regarding DnD character details visit DnD Beyond\n\n");
    printf("Enter your characters name: ");
    scanf("%s", newCharacter->name); 

    printf("Enter your characters level: ");
    scanf("%d", &newCharacter->level); 

    printf("Enter your characters class: ");
    scanf("%s", newCharacter->class); 

    printf("Enter your characters background: ");
    scanf("%s", newCharacter->background); 

    printf("Enter your characters race: ");
    scanf("%s", newCharacter->race); 

    printf("Enter your characters alignment: ");
    scanf("%s", newCharacter->alignment); 

    printf("Enter your characters strength value (8-20): ");
    scanf("%d", &newCharacter->strength); 

    printf("Enter your characters dexterity value (8-20): ");
    scanf("%d", &newCharacter->dexterity); 

    printf("Enter your characters constitution value (8-20): ");
    scanf("%d", &newCharacter->constitution); 

    printf("Enter your characters intelligence value (8-20): ");
    scanf("%d", &newCharacter->intelligence); 

    printf("Enter your characters wisdom value (8-20): ");
    scanf("%d", &newCharacter->wisdom); 

    printf("Enter your characters charisma value (8-20): ");
    scanf("%d", &newCharacter->charisma); 

    printf("Enter your characters movement speed: ");
    scanf("%d", &newCharacter->speed); 

    printf("Enter your characters armor: \n (Some examples include 'unarmored', 'plate_armor', 'hide_armor', 'half_plate_armor')");
    scanf("%s", newCharacter->armor); 

    printf("Does your character have a shield? (1 for yes 0 for no):");
    scanf("%d", &newCharacter->hasShield); 

    //inserts the new character at the beginning of the list
    newCharacter->next = *head;
    *head = newCharacter;
}


void displayCharacter(struct Character *head){

    //check to see if list is empty
    if(head == NULL){ 
        printf("No characters in the list.\n");
        printf("Please enter a character first\n");
        return;
    }

    //displays all charcters
    printf("List of characters:\n\n");
    while(head != NULL){
        printf("    ___________ Name: %s ___________\n\n", head->name);
        printf("Class: %s   Level: %d   Background: %s\n\n", head->class, head->level, head->background);                                    //displays Class, Level, Background
        printf("Race: %s    Alignment: %s\n\n", head->race, head->alignment);                                                                //displays Race, Alignment
        printf("Armor Class: %d     Speed: %dft\n\n", calculateArmorClass(head->dexterity, head->armor, head->hasShield), head->speed);   //displays Armor Class, Speed
        printf("Strength\nAbility Score: %d\nModifier: %d\n\n", head->strength, calculateModifier(head->strength));                          //displays Strength
        printf("Dexterity\nAbility Score: %d\nModifier: %d\n\n", head->dexterity, calculateModifier(head->dexterity));                       //displays Dexterity 
        printf("Constitution\nAbility Score: %d\nModifier: %d\n\n", head->constitution, calculateModifier(head->constitution));              //displays Constitution
        printf("Intelligence\nAbility Score: %d\nModifier: %d\n\n", head->intelligence, calculateModifier(head->intelligence));              //displays Intelligence
        printf("Wisdom\nAbility Score: %d\nModifier: %d\n\n", head->wisdom, calculateModifier(head->wisdom));                               //displays Wisdom
        printf("Charisma\nAbility Score: %d\nModifier: %d\n\n", head->charisma, calculateModifier(head->charisma));                          //displays Charisma
        head = head->next;
    }
}


int calculateModifier(int attribute){

//returns the modifer associated with your character attribute

if(attribute < 2){ 
    return -5; 
}

else if(attribute < 4){
    return -4;
}

else if(attribute < 6){
    return -3;
}

else if(attribute < 8){
    return -2;
}

else if(attribute < 10){
    return -1;
}

else if(attribute < 12){
    return 0;
}

else if(attribute < 14){
    return 1;
}

else if(attribute < 16){
    return 2;
}

else if(attribute < 18){
    return 3;
}

else if(attribute < 20){
    return 4;
}

else{
    return 5;
}
}


int calculateArmorClass(int dexterity, const char *armor, int hasShield){

int addArmorClass; //temporary variable

//if character has a shiled than they gain a +2 to armor class
if(hasShield == 0){
    addArmorClass = 0;
}
else{
    addArmorClass = 2;
}

//Calculates armor class based on what kind of armor, dexterity, and if they have a shield
if(strcmp(armor, "unarmored") == 0){
    return 10 + calculateModifier(dexterity) + addArmorClass;
}
else if(strcmp(armor, "plate_armor") == 0){
    return 18 + addArmorClass;
}
else if(strcmp(armor, "hide_armor") == 0){
    if(calculateModifier(dexterity) > 2){
        return 14 + addArmorClass;
    }
    else{
        return 12 + calculateModifier(dexterity) + addArmorClass;
    }
    }
    else if(strcmp(armor, "half_plate_armor") == 0){
         if(calculateModifier(dexterity) > 2){
        return 17 + addArmorClass;
    }
    else{
        return 15 + calculateModifier(dexterity) + addArmorClass;
    }
    }
return 10;
}


int calculateRollModifier(struct Character *head, char *diceCharacterName, int numChoice){

    //check to see if list is empty
    if(head == NULL){ 
        return 0;
    }
    
    if (strcmp(head->name, diceCharacterName) == 0){
        if(numChoice > 6 || numChoice < 1){
            return 0;
        }
        else if(numChoice == 1){
            return rollD20() + calculateModifier(head->strength);
        }
        else if(numChoice == 2){
                return rollD20() + calculateModifier(head->dexterity);
        }
        else if(numChoice == 3){
                return rollD20() + calculateModifier(head->constitution);
        }
         else if(numChoice == 4){
            return rollD20() + calculateModifier(head->intelligence);
        }
        else if(numChoice == 5){
            return rollD20() + calculateModifier(head->wisdom);
        }
        else if(numChoice == 6){
            return rollD20() + calculateModifier(head->charisma);
        }
        else if(numChoice == 7){
            return rollD20();
        }
        else{
        return 0;
        }
    }
    return 0;
    }


//Searches for a character by name
void searchCharacter(struct Character *head, char *searchCharacterName){
    int ifFound = 0;

    while(head != NULL){
        if (strcmp(head->name, searchCharacterName) == 0){
            printf("Your character has been found:\n\n");
            printf("Class: %s   Level: %d   Background: %s\n\n", head->class, head->level, head->background);                                    //displays Class, Level, Background
            printf("Race: %s    Alignment: %s\n\n", head->race, head->alignment);                                                                //displays Race, Alignment
            printf("Armor Class: %d     Speed: %dft\n\n", calculateArmorClass(head->dexterity, head->armor, head->hasShield), head->speed);   //displays Armor Class, Speed
            printf("Strength\nAbility Score: %d\nModifier: %d\n\n", head->strength, calculateModifier(head->strength));                          //displays Strength
            printf("Dexterity\nAbility Score: %d\nModifier: %d\n\n", head->dexterity, calculateModifier(head->dexterity));                       //displays Dexterity 
            printf("Constitution\nAbility Score: %d\nModifier: %d\n\n", head->constitution, calculateModifier(head->constitution));              //displays Constitution
            printf("Intelligence\nAbility Score: %d\nModifier: %d\n\n", head->intelligence, calculateModifier(head->intelligence));              //displays Intelligence
            printf("Wisdom\nAbility Score: %d\nModifier: %d\n\n", head->wisdom, calculateModifier(head->wisdom));                                //displays Wisdom
            printf("Charisma\nAbility Score: %d\nModifier: %d\n\n", head->charisma, calculateModifier(head->charisma));                          //displays Charisma
            ifFound = 1;
            break;
        }
        head = head->next;
    }

    if(ifFound != 1){
        printf("Your character could not found :(\n\n");
    }
}

//Updates details of your character
void updateCharacter(struct Character *head, char *updateCharacterName){

    while (head != NULL){
        if (strcmp(head->name, updateCharacterName) == 0){
            printf("Enter updated details for your character: %s:\n", updateCharacterName);
            printf("Enter your characters level: ");
            scanf("%d", &head->level); 

            printf("Enter your characters class: ");
            scanf("%s", head->class); 

            printf("Enter your characters background: ");
            scanf("%s", head->background); 

            printf("Enter your characters race: ");
            scanf("%s", head->race); 

            printf("Enter your characters alignment: ");
            scanf("%s", head->alignment); 

            printf("Enter your characters strength value (8-20): ");
            scanf("%d", &head->strength); 

            printf("Enter your characters dexterity value (8-20): ");
            scanf("%d", &head->dexterity); 

            printf("Enter your characters constitution value (8-20): ");
            scanf("%d", &head->constitution); 

            printf("Enter your characters intelligence value (8-20): ");
            scanf("%d", &head->intelligence); 

            printf("Enter your characters wisdom value (8-20): ");
            scanf("%d", &head->wisdom); 

            printf("Enter your characters charisma value (8-20): ");
            scanf("%d", &head->charisma); 

            printf("Enter your characters movement speed: ");
            scanf("%d", &head->speed); 

            printf("Enter your characters armor: \n (Some examples include 'unarmored', 'plate_armor', 'hide_armor', 'half_plate_armor')");
            scanf("%s", head->armor); 

            printf("Does your character have a shield? (1 for yes 0 for no):");
            scanf("%d", &head->hasShield); 
            printf("Character details have updated successfully.\n\n");
            return;
        }
        head = head->next;
    }

    printf("Character could not be found :( \n\n");
}

//Deletes a character from the list
void deleteCharacter(struct Character **head, char *deleteCharacterName){
    struct Character *prev = NULL;

    struct Character *temp = *head;

    while(temp != NULL && strcmp(temp->name, deleteCharacterName) != 0){
        prev = temp;
        temp = temp->next;
    }

    if(temp == NULL){
        printf("Your character could not be found :(\n\n");
        return;
    }

    if(prev == NULL){
        *head = temp->next;
    } 
    else{
        prev->next = temp->next;
    }

    free(temp);
    printf("Your character has been deleted...\n\n");
}


int rollD20(){
    return rand() % 20 + 1;
}
