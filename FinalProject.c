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
    char weapon[50];  //character weapon - determines how much damage a character can do
    int hasShield;  //characters has a shield or not - helps determine their armor class
    struct Character *next;  //points to the next character in the list
};

//calculate functions
int calculateModifier(int attribute); //takes attributes from character and converts it to their modifier
int calculateArmorClass(int dexterity, const char *armor, int hasShield); //returns armor class via certain character criteria
int calculateRollModifier(struct Character *head, char *diceCharacterName, int numChoice); //uses your characters modifiers and +/- from dice roll

//display functions
void addCharacter(struct Character **head); //adds new character to character list
void displayCharacter(struct Character *head); //displays your characters
void searchCharacter(struct Character *head, char *searchName); //searches for a character you have created and prints it
void updateCharacter(struct Character *head, char *updateCharacterName); //updates an existing character with new data
void deleteCharacter(struct Character **head, char *deleteCharacterName); //deletes an existing character

//dice rolling function
int rollD20(void); //rolls a D20
int rollD12(void); //rolls a D12
int rollD10(void); //rolls a D10
int rollD8(void); //rolls a D8
int rollD6(void); //rolls a D6
int rollD4(void); //rolls a D4

//character creater functions
char *pickClass(int userChoice); //via user input returns your characters class
char *pickBackground(int userChoice); //via user input returns your characters background
char *pickRace(int userChoice); //via user input returns your characters race
char *pickAlignment(int userChoice); //via user input returns your characters alignment
char *pickArmor(int userChoice); //via user input returns your characters armor (if any)

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


//returns the modifer associated with your character attribute
int calculateModifier(int attribute){
    return (attribute - 10) / 2;
}

//returns your characters armor class number
int calculateArmorClass(int dexterity, const char *armor, int hasShield){
    int addArmorClass; //if character has a shiled than they gain a +2 to armor class
    int maxDexterity; //some armor only allows a max of +2 from your dexterity modifier

    if(hasShield == 0){
        addArmorClass = 0;
    }
    else{
        addArmorClass = 2;
    }

    if(calculateModifier(dexterity) > 2){
    maxDexterity = 2;
    }
    else{
        maxDexterity = calculateModifier(dexterity);
    }

    //Calculates armor class based on what kind of armor, dexterity, and if they have a shield
    if(strcmp(armor, "Unarmored") == 0){
        return 10 + calculateModifier(dexterity) + addArmorClass;
    }
    //light armor
    if(strcmp(armor, "Padded Armor") == 0){
        return 11 + calculateModifier(dexterity) + addArmorClass;
    }
    if(strcmp(armor, "Leather Armor") == 0){
        return 11 + calculateModifier(dexterity) + addArmorClass;
    }
    if(strcmp(armor, "Studded Leather Armor") == 0){
        return 12 + calculateModifier(dexterity) + addArmorClass;
    }
    //medium armor
    if(strcmp(armor, "Hide Armor") == 0){
        return 12 + maxDexterity + addArmorClass;
    }
    if(strcmp(armor, "Chain Shirt Armor") == 0){
        return 13 + maxDexterity + addArmorClass;
    }
    if(strcmp(armor, "Scale Mail Armor") == 0){
        return 14 + maxDexterity + addArmorClass;
    }
    if(strcmp(armor, "Breastplate Armor") == 0){
        return 14 + maxDexterity + addArmorClass;
    }
    if(strcmp(armor, "Half Plate Armor") == 0){
        return 15 + maxDexterity + addArmorClass;
    }
    //heavy armor
    if(strcmp(armor, "Ring Mail Armor") == 0){
        return 14 +  addArmorClass;
    }
    if(strcmp(armor, "Chain Mail Armor") == 0){
        return 16 + addArmorClass;
    }
    if(strcmp(armor, "Splint Armor") == 0){
        return 17 + addArmorClass;
    }
    if(strcmp(armor, "Plate Armor") == 0){
        return 18 + addArmorClass;
    }
    printf("\nInvlaid armor type\n\n");
    return 10; //deafults to 10
}


int calculateRollModifier(struct Character *head, char *diceCharacterName, int numChoice){
    //puts all characters modifiers in an array
    int *attributes[] = {
        &head->strength, &head->dexterity,
        &head->constitution, &head->intelligence,
        &head->wisdom, &head->charisma
    };  
    //check to see if list is empty
    if(head == NULL){ 
        return -1; //indicates error
    }
    //checks to make sure the character entered exists
    if (strcmp(head->name, diceCharacterName) != 0){
        return -1; //not found -1 indicating error
    }
    //validates numChoice
    if(numChoice > 6 || numChoice < 1){
        return rollD20();
    }
    //returns the baseRoll + your character modifier of choice
    return rollD20() + calculateModifier(*attributes[numChoice - 1]);
}

void addCharacter(struct Character **head){
    //make space for new character in memory
    struct Character *newCharacter = (struct Character *)malloc(sizeof(struct Character));
    int usersClass;
    int usersBackground;
    int usersRace;
    int usersAlignment;
    int usersArmor;

    if (newCharacter == NULL){
        printf("Failed to allocate memory :(\n");
        exit(1);
    }

    //prompt user to enter details for character
    printf("For more information regarding DnD character details visit DnD Beyond\n\n");
    printf("Enter your characters name: ");
    scanf("%s", newCharacter->name); 

    printf("Enter your characters level (min: 1 max: 20): ");
    scanf("%d", &newCharacter->level); 

    printf("Enter which class your character is:\n");
    printf("1. Barbarian\n");
    printf("2. Bard\n");
    printf("3. Cleric\n");
    printf("4. Druid\n");
    printf("5. Fighter\n");
    printf("6. Monk\n");
    printf("7. Paladin\n");
    printf("8. Ranger\n");
    printf("9. Rogue\n");
    printf("10. Sorcerer\n");
    printf("11. Warlock\n");
    printf("12. Wizard\n");
    scanf("%d", &usersClass); 
    strcpy(newCharacter->class, pickClass(usersClass));
    
    printf("Enter your characters background:\n");
    printf("1. Acolyte\n");
    printf("2. Artisan\n");
    printf("3. Charlatan\n");
    printf("4. Criminal\n");
    printf("5. Entertainer\n");
    printf("6. Farmer\n");
    printf("7. Guard\n");
    printf("8. Guide\n");
    printf("9. Hermit\n");
    printf("10. Merchant\n");
    printf("11. Noble\n");
    printf("12. Sage\n");
    printf("13. Sailor\n");
    printf("14. Scribe\n");
    printf("15. Soldier\n");
    printf("16. Wayfarer\n");
    scanf("%d", &usersBackground); 
    strcpy(newCharacter->background, pickBackground(usersBackground));

    printf("Enter your characters race:\n");
    printf("1. Aasimar\n");
    printf("2. Dragonborn\n");
    printf("3. Dwarf\n");
    printf("4. Elf\n");
    printf("5. Gnome\n");
    printf("6. Goliath\n");
    printf("7. Halfling\n");
    printf("8. Human\n");
    printf("9. Orc\n");
    printf("10. Tiefling\n");
    scanf("%d", &usersRace); 
    strcpy(newCharacter->race, pickRace(usersRace));

    printf("Enter your characters alignment:\n");
    printf("1. Lawful Good\n");
    printf("2. Neutral Good\n");
    printf("3. Chaotic Good\n");
    printf("4. Lawful Neutral\n");
    printf("5. True Neutral\n");
    printf("6. Chaotic Neutral\n");
    printf("7. Lawful Evil\n");
    printf("8. Neutral Evil\n");
    printf("9. Chaotic Evil\n");
    scanf("%d", &usersAlignment); 
    strcpy(newCharacter->alignment, pickAlignment(usersAlignment));

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

    printf("Enter your characters armor: \n");
    printf("1. Unarmored\n");
    printf("___Light Armor___\n");
    printf("2. Padded Armor\n");
    printf("3. Leather Armor\n");
    printf("4. Studded Leather Armor\n");
    printf("___Medium Armor___\n");
    printf("5. Hide Armor\n");
    printf("6. Chain Shirt Armor\n");
    printf("7. Scale Mail Armor\n");
    printf("8. Breastplate Armor\n");
    printf("9. Half Plate Armor\n");
    printf("___Heavy Armor___\n");
    printf("10. Ring Mail Armor\n");
    printf("11. Chain Mail Armor\n");
    printf("12. Splint Armor\n");
    printf("13. Plate Armor\n");
    scanf("%d", &usersArmor); 
    strcpy(newCharacter->armor, pickArmor(usersArmor));

    printf("Does your character have a shield? (1 for yes 0 for no):");
    scanf("%d", &newCharacter->hasShield); 


    newCharacter->speed = 30;
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
        printf("Armor: %s   Armor Class: %d     Speed: %dft\n\n", head->armor, calculateArmorClass(head->dexterity, head->armor, head->hasShield), head->speed);   //displays Armor Class, Speed
        printf("Strength\nAbility Score: %d\nModifier: %d\n\n", head->strength, calculateModifier(head->strength));                          //displays Strength
        printf("Dexterity\nAbility Score: %d\nModifier: %d\n\n", head->dexterity, calculateModifier(head->dexterity));                       //displays Dexterity 
        printf("Constitution\nAbility Score: %d\nModifier: %d\n\n", head->constitution, calculateModifier(head->constitution));              //displays Constitution
        printf("Intelligence\nAbility Score: %d\nModifier: %d\n\n", head->intelligence, calculateModifier(head->intelligence));              //displays Intelligence
        printf("Wisdom\nAbility Score: %d\nModifier: %d\n\n", head->wisdom, calculateModifier(head->wisdom));                                //displays Wisdom
        printf("Charisma\nAbility Score: %d\nModifier: %d\n\n", head->charisma, calculateModifier(head->charisma));                          //displays Charisma
        head = head->next;
    }
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
    int usersClass;
    int usersBackground;
    int usersRace;
    int usersAlignment;
    int usersArmor;

    while (head != NULL){
        if (strcmp(head->name, updateCharacterName) == 0){
            printf("Enter updated details for your character: %s:\n", updateCharacterName);
            printf("Enter your characters level: ");
            scanf("%d", &head->level); 

            printf("Enter which class your character is:\n");
            printf("1. Barbarian\n");
            printf("2. Bard\n");
            printf("3. Cleric\n");
            printf("4. Druid\n");
            printf("5. Fighter\n");
            printf("6. Monk\n");
            printf("7. Paladin\n");
            printf("8. Ranger\n");
            printf("9. Rogue\n");
            printf("10. Sorcerer\n");
            printf("11. Warlock\n");
            printf("12. Wizard\n");
            scanf("%d", &usersClass); 
            strcpy(head->class, pickClass(usersClass));

            printf("Enter your characters background:\n");
            printf("1. Acolyte\n");
            printf("2. Artisan\n");
            printf("3. Charlatan\n");
            printf("4. Criminal\n");
            printf("5. Entertainer\n");
            printf("6. Farmer\n");
            printf("7. Guard\n");
            printf("8. Guide\n");
            printf("9. Hermit\n");
            printf("10. Merchant\n");
            printf("11. Noble\n");
            printf("12. Sage\n");
            printf("13. Sailor\n");
            printf("14. Scribe\n");
            printf("15. Soldier\n");
            printf("16. Wayfarer\n");
            scanf("%d", &usersBackground); 
            strcpy(head->background, pickBackground(usersBackground));

            printf("Enter your characters race:\n");
            printf("1. Aasimar\n");
            printf("2. Dragonborn\n");
            printf("3. Dwarf\n");
            printf("4. Elf\n");
            printf("5. Gnome\n");
            printf("6. Goliath\n");
            printf("7. Halfling\n");
            printf("8. Human\n");
            printf("9. Orc\n");
            printf("10. Tiefling\n");
            scanf("%d", &usersRace); 
            strcpy(head->race, pickRace(usersRace));

            printf("Enter your characters alignment:\n");
            printf("1. Lawful Good\n");
            printf("2. Neutral Good\n");
            printf("3. Chaotic Good\n");
            printf("4. Lawful Neutral\n");
            printf("5. True Neutral\n");
            printf("6. Chaotic Neutral\n");
            printf("7. Lawful Evil\n");
            printf("8. Neutral Evil\n");
            printf("9. Chaotic Evil\n");
            scanf("%d", &usersAlignment); 
            strcpy(head->alignment, pickAlignment(usersAlignment));

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

            printf("Enter your characters armor: \n");
            printf("1. Unarmored\n");
            printf("___Light Armor___\n");
            printf("2. Padded Armor\n");
            printf("3. Leather Armor\n");
            printf("4. Studded Leather Armor\n");
            printf("___Medium Armor___\n");
            printf("5. Hide Armor\n");
            printf("6. Chain Shirt Armor\n");
            printf("7. Scale Mail Armor\n");
            printf("8. Breastplate Armor\n");
            printf("9. Half Plate Armor\n");
            printf("___Heavy Armor___\n");
            printf("10. Ring Mail Armor\n");
            printf("11. Chain Mail Armor\n");
            printf("12. Splint Armor\n");
            printf("13. Plate Armor\n");
            scanf("%d", &usersArmor); 
            strcpy(head->armor, pickArmor(usersArmor));

            printf("Does your character have a shield?\n");
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
    printf("\nYour character has been deleted...\n\n");
}

//generates a random number between 1 & 20
int rollD20(void){
    return rand() % 20 + 1;
}
//generates a random number between 1 & 12
int rollD12(void){
    return rand() % 12 + 1;
}
//generates a random number between 1 & 10
int rollD10(void){
    return rand() % 10 + 1;
}
//generates a random number between 1 & 8
int rollD8(void){
    return rand() % 8 + 1;
}
//generates a random number between 1 & 6
int rollD6(void){
    return rand() % 6 + 1;
}
//generates a random number between 1 & 4
int rollD4(void){
    return rand() % 4 + 1;
}

char *pickClass(int userChoice){
// depending on how user responds it will return a string of whatever class is picked
    switch (userChoice) {
        case 1: return "Barbarian";
        case 2: return "Bard";
        case 3: return "Cleric";
        case 4: return "Druid";
        case 5: return "Fighter";
        case 6: return "Monk";
        case 7: return "Paladin";
        case 8: return "Ranger";
        case 9: return "Rogue";
        case 10: return "Sorcerer";
        case 11: return "Warlock";
        case 12: return "Wizard";
        default: return "Unknown"; // defaults to Unknown for invalid choice
    }
}

char *pickBackground(int userChoice){
// depending on how user responds it will return a string of whatever class is picked
    switch (userChoice) {
        case 1: return "Acolyte";
        case 2: return "Artisan";
        case 3: return "Charlatan";
        case 4: return "Criminal";
        case 5: return "Entertainer";
        case 6: return "Farmer";
        case 7: return "Guard";
        case 8: return "Guide";
        case 9: return "Hermit";
        case 10: return "Merchant";
        case 11: return "Noble";
        case 12: return "Sage";
        case 13: return "Sailor";
        case 14: return "Scribe";
        case 15: return "Soldier";
        case 16: return "Wayfarer"; 
        default: return "Unknown"; // defaults to Unknown for invalid choice
    }
}

char *pickRace(int userChoice){
// depending on how user responds it will return a string of whatever class is picked
    switch (userChoice) {
        case 1: return "Aasimar";
        case 2: return "Dragonborn";
        case 3: return "Dwarf";
        case 4: return "Elf";
        case 5: return "Gnome";
        case 6: return "Goliath";
        case 7: return "Halfling";
        case 8: return "Human";
        case 9: return "Orc";
        case 10: return "Tiefling";
        default: return "Unknown"; // defaults to Unknown for invalid choice
    }
}

char *pickAlignment(int userChoice){
// depending on how user responds it will return a string of whatever class is picked
    switch (userChoice) {
        case 1: return "Lawful Good";
        case 2: return "Neutral Good";
        case 3: return "Chaotic Good";
        case 4: return "Lawful Neutral";
        case 5: return "True Neutral";
        case 6: return "Chaotic Neutral";
        case 7: return "Lawful Evil";
        case 8: return "Neutral Evil";
        case 9: return "Chaotic Evil";
        default: return "Unknown"; // defaults to Unknown for invalid choice
    }
}

char *pickArmor(int userChoice){
// depending on how user responds it will return a string of whatever class is picked
    switch (userChoice) {
        case 1: return "Unarmored";
        case 2: return "Padded Armor";
        case 3: return "Leather Armor";
        case 4: return "Studded Leather Armor";
        case 5: return "Hide Armor";
        case 6: return "Chain Shirt Armor";
        case 7: return "Scale Mail Armor";
        case 8: return "Breastplate Armor";
        case 9: return "Half Plate Armor";
        case 10: return "Ring Mail Armor";
        case 11: return "Chain Mail Armor";
        case 12: return "Splint Armor";
        case 13: return "Plate Armor";
        default: return "Unknown"; // defaults to Unknown for invalid choice
    }
}