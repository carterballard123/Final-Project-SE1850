#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <string.h>
#include <time.h>

struct Character {
    char name[25];            //character name
    int level;                //character level
    char class[20];           //character class
    char background[20];      //character background
    char race[20];            //character race
    char alignment[30];       //character alignment
    int strength;             //character strength val
    int dexterity;            //character dexterity val
    int constitution;         //character constitution val
    int intelligence;         //character intelligence val
    int wisdom;               //character wisdom val
    int charisma;             //character charisma val
    int speed;                //character speed val
    int armorClass;           //character armor class
    struct Armor *armor;      //character armor - helps determine their armor class
    struct Weapon *weapon;    //character weapon - determines how much damage a character can do
    int hasShield;            //characters has a shield or not - helps determine their armor class
    struct Character *next;   //points to the next character in the list
};

struct Armor {
    char name[25];            // Name of the armor (e.g., "Chain Mail")
    char type[20];            // Type of armor (e.g., "Light", "Medium", "Heavy")
    int baseAC;               // Base armor class provided by the armor
    int maxDexBonus;          // Maximum Dexterity modifier that can be added
    int requiresDexCap;       // 1 if the Dex modifier is capped, 0 otherwise
    int strengthRequirement;  // Minimum Strength required to wear the armor
    int stealthDisadvantage;  // 1 if the armor imposes disadvantage on Stealth checks
};


struct Weapon {
    char name[25];            // Name of the weapon (e.g., "Longsword")
    char type[25];            // Type of weapon (e.g., "Melee", "Ranged")
    char damageType[20];      // Damage type (e.g., "Slashing", "Piercing")
    char damageDice[10];      // Damage dice (e.g., "1d8", "2d6")
    char twoHandDamage[10];
    int isFinesse;            // 1 if weapon uses Dex for attack/damage, 0 otherwise
    int isVersatile;          // 1 if the weapon can be used one- or two-handed
    int isTwoHanded;          // 1 if the weapon requires two hands
    int range[2];             // Minimum and maximum range (e.g., {0, 60} for thrown weapons)
    int isLight;              // 1 if the weapon is light (good for dual-wielding)
    int isHeavy;              // 1 if the weapon is heavy (not for small creatures)
    int isReach;
};

struct Armor armors[] = {
    {"Unarmored", "N/A", 10, -1, 0, 0, 0},
    //light armor
    {"Padded Armor", "Light", 11, -1, 0, 0, 1},
    {"Leather Armor", "Light", 11, -1, 0, 0, 0},
    {"Studded Leather Armor", "Light", 12, -1, 0, 0, 0},
    //medium armor
    {"Hide Armor", "Medium", 12, 2, 1, 0, 0},
    {"Chain Shirt Armor", "Medium", 13, 2, 1, 0, 0},
    {"Scale Mail Armor", "Medium", 14, 2, 1, 0, 1},
    {"Breastplate Armor", "Medium", 14, 2, 1, 0, 0},
    {"Half Plate Armor", "Medium", 15, 2, 1, 0, 1},
    //heavy armor
    {"Ring Mail Armor", "Heavy", 14, 0, 0, 0, 1},
    {"Chain Mail Armor", "Heavy", 16, 0, 0, 13, 1},
    {"Splint Armor", "Heavy", 17, 0, 0, 15, 1},
    {"Plate Armor", "Heavy", 18, 0, 0, 15, 1},
};

struct Weapon weapons[] = {
    {"Club", "Melee", "Bludgeoning", "1d4", "1d4", 0, 0, 0, {0, 0}, 0, 1, 0},
    {"Dagger", "Melee/Ranged", "Piercing", "1d4", "1d4", 1, 0, 0, {20, 60}, 0, 1, 0},
    {"Greatclub", "Melee", "Bludgeoning", "1d8", "1d8", 0, 0, 1, {0, 0}, 0, 0, 0},
    {"Handaxe", "Melee/Ranged", "Slashing", "1d6", "1d6", 0, 0, 0, {20, 60}, 0, 1, 0},
    {"Javelin", "Ranged", "Piercing", "1d6", "1d6", 0, 0, 0, {30, 120}, 0, 0, 0},
    {"Light Hammer", "Melee/Ranged", "Bludgeoning", "1d4", "1d4", 0, 0, 0, {20, 60}, 0, 1, 0},
    {"Mace", "Melee", "Bludgeoning", "1d6", "1d6", 0, 0, 0, {0, 0}, 0, 0, 0},
    {"Quarterstaff", "Melee", "Bludgeoning", "1d6", "1d8", 0, 1, 0, {0, 0}, 0, 0, 0},
    {"Sickle", "Melee", "Slashing", "1d4", "1d4", 0, 0, 0, {0, 0}, 0, 1, 0},
    {"Spear", "Melee/Ranged", "Piercing", "1d6", "1d8", 0, 1, 0, {20, 60}, 0, 0, 0},
    {"Light Crossbow", "Ranged", "Piercing", "1d8", "1d8", 0, 0, 1, {80, 320}, 0, 0, 0},
    {"Heavy Crossbow", "Ranged", "Piercing", "1d10", "1d10", 0, 0, 1, {100, 400}, 0, 1, 0},
    {"Hand Crossbow", "Ranged", "Piercing", "1d6", "1d6", 0, 0, 0, {30, 120}, 1, 0, 0},
    {"Shortbow", "Ranged", "Piercing", "1d6", "1d6", 0, 0, 1, {80, 320}, 0, 0, 0},
    {"Longbow", "Ranged", "Piercing", "1d8", "1d8", 0, 0, 1, {150, 600}, 0, 1, 0},
    {"Battleaxe", "Melee", "Slashing", "1d8", "1d10", 0, 1, 0, {0, 0}, 0, 0, 0},
    {"Flail", "Melee", "Bludgeoning", "1d8", "1d8", 0, 0, 0, {0, 0}, 0, 0, 0},
    {"Glaive", "Melee", "Slashing", "1d10", "1d10", 0, 0, 1, {0, 0}, 0, 1, 1},
    {"Greataxe", "Melee", "Slashing", "1d12", "1d12", 0, 0, 1, {0, 0}, 0, 1, 0},
    {"Greatsword", "Melee", "Slashing", "2d6", "2d6", 0, 0, 1, {0, 0}, 0, 1, 0},
    {"Halberd", "Melee", "Slashing", "1d10", "1d10", 0, 0, 1, {0, 0}, 0, 1, 1},
    {"Lance", "Melee", "Piercing", "1d12", "1d12", 0, 0, 0, {0, 0}, 0, 0, 1},
    {"Longsword", "Melee", "Slashing", "1d8", "1d10", 0, 1, 0, {0, 0}, 0, 0, 0},
    {"Maul", "Melee", "Bludgeoning", "2d6", "2d6", 0, 0, 1, {0, 0}, 0, 1, 0},
    {"Pike", "Melee", "Piercing", "1d10", "1d10", 0, 0, 1, {0, 0}, 0, 1, 1},
    {"Rapier", "Melee", "Piercing", "1d8", "1d8", 1, 0, 0, {0, 0}, 0, 0, 0},
    {"Scimitar", "Melee", "Slashing", "1d6", "1d6", 1, 0, 0, {0, 0}, 1, 0, 0},
    {"Shortsword", "Melee", "Piercing", "1d6", "1d6", 1, 0, 0, {0, 0}, 1, 0, 0},
    {"Trident", "Melee/Ranged", "Piercing", "1d6", "1d8", 0, 1, 0, {20, 60}, 0, 0, 0},
    {"War Pick", "Melee", "Piercing", "1d8", "1d8", 0, 0, 0, {0, 0}, 0, 0, 0},
    {"Warhammer", "Melee", "Bludgeoning", "1d8", "1d10", 0, 1, 0, {0, 0}, 0, 0, 0}
};
//all attributes
const char *attributes[] = {
"Strength", "Dexterity", "Constitution", "Intelligence", "Wisdom", "Charisma"
};
//all alignments
const char *alignments[] = {
"Lawful Good", "Neutral Good", "Chaotic Good", "Lawful Neutral", "True Neutral", "Chaotic Neutral", "Lawful Evil", "Neutral Evil", "Chaotic Evil"
};

const char *races[] = {
"Aasimar", "Dragonborn", "Dwarf", "Elf", "Gnome", "Goliath", "Halfling", "Human", "Orc", "Tiefling"
};

const char *backgrounds[] = {
"Acolyte", "Artisan", "Charlatan", "Criminal", "Entertainer", "Farmer", "Guard", "Guide", "Hermit", "Merchant", "Noble", "Sage", "Sailor", "Scribe", "Soldier", "Wayfarer"
};

const char *classes[] = {
"Barbarian", "Bard", "Cleric", "Druid", "Fighter", "Monk", "Paladin", "Ranger", "Rogue", "Sorcerer", "Warlock", "Wizard"
};

//selecting functions (adding updating character data)
void selectName(struct Character *character);
void selectShield(struct Character *character);
void selectLevel(struct Character *character);
void selectClass(struct Character *character);
void selectBackground(struct Character *character);
void selectRace(struct Character *character);
void selectAlignment(struct Character *character);
void selectAttributes(struct Character *character);
void selectArmor(struct Character *newCharacter);

//calculate functions
int calculateModifier(int attribute); //takes attributes from character and converts it to their modifier
int calculateArmorClass(int dexterity, const char *armorName, int hasShield); //returns armor class via certain character criteria
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

int main(){

srand(time(NULL)); //seeds a random number

char searchCharacterName[50]; //Array to store name for searching
char updateCharacterName[50]; //Array to store name for updating
char deleteCharacterName[50]; //Array to store name for deleting
char diceCharacterName[50];   //Array to store name for dice rolling

int choice; //users choice
int numChoice; //users choice for dice roll
struct Character *characterList = NULL;
printf("\nWelcome to the DnD Character Creator!\n\n");

do{
    //menu options
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
        printf("You rolled: %d\n\n", rollD20());
    }
    if(choice == 8){
        printf("Exiting DnD Character Creator...\n");
    }
    if(choice > 8 || choice < 1){
        printf("\nInvalid choice, please try again...\n\n");
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
int calculateArmorClass(int dexterity, const char *armorName, int hasShield) {
    int shieldBonus = hasShield ? 2 : 0;            // +2 AC if the character has a shield
    int maxDex = 0;                                 // Default max dex modifier

    // Find the armor in the armors array
    struct Armor *selectedArmor = NULL;
    for (int i = 0; i < sizeof(armors) - 1; i++) {
        if (strcmp(armors[i].name, armorName) == 0) {
            selectedArmor = &armors[i];
            break;
        }
    }
    // If armor is not found, return a default value
    if (!selectedArmor) {
        printf("Invalid armor type: %s\n", armorName);
        return 10; // Default unarmored AC
    }

    // Apply max dexterity bonus if required
    if (selectedArmor->requiresDexCap == 1 && calculateModifier(dexterity) > selectedArmor->maxDexBonus) {
        maxDex = selectedArmor->maxDexBonus;
    }
    if(selectedArmor->maxDexBonus == -1){
        maxDex = calculateModifier(dexterity);
    }
    //if the armor is of "Heavy" type then AC gets 0 dexterity bonus
    if(strcmp(selectedArmor->type, "Heavy") == 0){
        maxDex = 0;
    }

    // Calculate the final AC
    int armorClass = selectedArmor->baseAC + maxDex + shieldBonus;
    return armorClass;
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

void addCharacter(struct Character **newChar){
    //make space for new character in memory
    struct Character *newCharacter = (struct Character *)malloc(sizeof(struct Character));

    if (newCharacter == NULL){
        printf("Failed to allocate memory :(\n");
        exit(1);
    }

    //prompt user to enter details for character
    printf("For more information regarding DnD character details visit DnD Beyond\n");

    printf("Enter your characters name: ");
    scanf("%24s", newCharacter->name);
    printf("Your characters name is: %s\n", newCharacter->name);
    // Calls the select functions to gather information about your character
    selectLevel(newCharacter);
    selectClass(newCharacter);  
    selectBackground(newCharacter);
    selectRace(newCharacter);
    selectAlignment(newCharacter);
    selectAttributes(newCharacter);
    selectArmor(newCharacter);
    selectShield(newCharacter);

    newCharacter->speed = 30;
    //inserts the new character at the beginning of the list
    newCharacter->next = *newChar;
    *newChar = newCharacter;
}

void displayCharacter(struct Character *character){

    //check to see if list is empty
    if(character == NULL){ 
        printf("\nNo characters in the list...\n");
        printf("Please enter a character first\n\n");
        return;
    }

    //displays all charcters
    printf("List of characters:\n\n");
    while(character != NULL){
        printf("    ___________ Name: %s ___________\n\n", character->name);
        printf("Class: %s   Level: %d   Background: %s\n\n", character->class, character->level, character->background);                                    //displays Class, Level, Background
        printf("Race: %s    Alignment: %s\n\n", character->race, character->alignment);                                                                //displays Race, Alignment
        printf("Armor: %s   Armor Class: %d     Speed: %dft\n\n", character->armor->name, calculateArmorClass(character->dexterity, character->armor->name, character->hasShield), character->speed);   //displays Armor Class, Speed
        printf("Strength\nAbility Score: %d\nModifier: %d\n\n", character->strength, calculateModifier(character->strength));                          //displays Strength
        printf("Dexterity\nAbility Score: %d\nModifier: %d\n\n", character->dexterity, calculateModifier(character->dexterity));                       //displays Dexterity 
        printf("Constitution\nAbility Score: %d\nModifier: %d\n\n", character->constitution, calculateModifier(character->constitution));              //displays Constitution
        printf("Intelligence\nAbility Score: %d\nModifier: %d\n\n", character->intelligence, calculateModifier(character->intelligence));              //displays Intelligence
        printf("Wisdom\nAbility Score: %d\nModifier: %d\n\n", character->wisdom, calculateModifier(character->wisdom));                                //displays Wisdom
        printf("Charisma\nAbility Score: %d\nModifier: %d\n\n", character->charisma, calculateModifier(character->charisma));                          //displays Charisma
        character = character->next;
    }
}

//Searches for a character by name
void searchCharacter(struct Character *character, char *searchCharacterName){
    int ifFound = 0;

    while(character != NULL){
        if (strcmp(character->name, searchCharacterName) == 0){
            printf("\n    ___________ Name: %s ___________\n\n", character->name);
            printf("Class: %s   Level: %d   Background: %s\n\n", character->class, character->level, character->background);                                    //displays Class, Level, Background
            printf("Race: %s    Alignment: %s\n\n", character->race, character->alignment);                                                                //displays Race, Alignment
            printf("Armor: %s   Armor Class: %d     Speed: %dft\n\n", character->armor->name, calculateArmorClass(character->dexterity, character->armor->name, character->hasShield), character->speed);   //displays Armor Class, Speed
            printf("Strength\nAbility Score: %d\nModifier: %d\n\n", character->strength, calculateModifier(character->strength));                          //displays Strength
            printf("Dexterity\nAbility Score: %d\nModifier: %d\n\n", character->dexterity, calculateModifier(character->dexterity));                       //displays Dexterity 
            printf("Constitution\nAbility Score: %d\nModifier: %d\n\n", character->constitution, calculateModifier(character->constitution));              //displays Constitution
            printf("Intelligence\nAbility Score: %d\nModifier: %d\n\n", character->intelligence, calculateModifier(character->intelligence));              //displays Intelligence
            printf("Wisdom\nAbility Score: %d\nModifier: %d\n\n", character->wisdom, calculateModifier(character->wisdom));                                //displays Wisdom
            printf("Charisma\nAbility Score: %d\nModifier: %d\n\n", character->charisma, calculateModifier(character->charisma));
            ifFound = 1;
            break;
        }
        character = character->next;
    }

    if(ifFound != 1){
        printf("\nYour character could not found :(\n\n");
    }
}

//Updates details of your character
void updateCharacter(struct Character *updatedCharacter, char *updateCharacterName){

    while (updatedCharacter != NULL){
        if (strcmp(updatedCharacter->name, updateCharacterName) == 0){
            printf("Enter updated details for your character: %s:\n", updateCharacterName);

            selectLevel(updatedCharacter);
            selectClass(updatedCharacter);
            selectBackground(updatedCharacter);
            selectRace(updatedCharacter);
            selectAlignment(updatedCharacter);
            selectAttributes(updatedCharacter); 
            selectArmor(updatedCharacter);
            selectShield(updatedCharacter);

            printf("Character details have been updated successfully.\n\n");
            return;
        }
        updatedCharacter = updatedCharacter->next;
    }

    printf("Character could not be found :( \n\n");
}

//Deletes a character from the list
void deleteCharacter(struct Character **character, char *deleteCharacterName){
    struct Character *prev = NULL;

    struct Character *temp = *character;

    while(temp != NULL && strcmp(temp->name, deleteCharacterName) != 0){
        prev = temp;
        temp = temp->next;
    }
    if(temp == NULL){
        printf("Your character could not be found :(\n\n");
        return;
    }

    if(prev == NULL){
        *character = temp->next;
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

void selectArmor(struct Character *character) {
    int usersArmor;

    do {
        // Display armor menu
        printf("Enter your character's armor: \n");
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
        printf("Enter your choice: ");
        scanf("%d", &usersArmor);

        // Validate input
        if (usersArmor < 1 || usersArmor > 13) {
            printf("\nInvalid armor choice! Please try again.\n\n");
        }
    } while (usersArmor < 1 || usersArmor > 13); // Repeat until valid input

    // Assign selected armor
    character->armor = &armors[usersArmor - 1];
    printf("You selected: %s\n", character->armor->name);
}

void selectAttributes(struct Character *character){
    int userChoice;

    for(int i = 0; i < 6; i++){ 
        int validInput = 0;

        while(!validInput){
            printf("Enter your character's %s value (8-20): ", attributes[i]);
            scanf("%d", &userChoice);

            if(userChoice < 8 || userChoice > 20){
                printf("Invalid number, please try again...\n\n");
            } 
            else{
                switch(i){
                    case 0: 
                        character->strength = userChoice;
                        break;
                    case 1: 
                        character->dexterity = userChoice;
                        break;
                    case 2: 
                        character->constitution = userChoice;
                        break;
                    case 3: 
                        character->intelligence = userChoice;
                        break;
                    case 4: 
                        character->wisdom = userChoice;
                        break;
                    case 5: 
                        character->charisma = userChoice;
                        break;
                    default: 
                        printf("Error in attribute selection.\n");
                        return;
                }
                printf("Your character has %d %s!\n", userChoice, attributes[i]);
                validInput = 1;
            }
        }
    }
}

void selectAlignment(struct Character *character){
    int usersAlignment;
    int validInput = 0;

    // Display alignment options
    printf("Enter your character's alignment:\n");
    for (int i = 0; i < 9; i++) {
        printf("%d. %s\n", i + 1, alignments[i]);
    }

    // Input validation loop
    while (!validInput) {
        printf("Enter your Choice: ");
        scanf("%d", &usersAlignment);

        if (usersAlignment < 1 || usersAlignment > 9) {
            printf("Invalid number, please try again...\n\n");
        } 
        else {
            validInput = 1;
        }
    }

    strcpy(character->alignment, alignments[usersAlignment - 1]);

    printf("You selected: %s\n\n", character->alignment);
}

void selectRace(struct Character *character){
    int usersRace;
    int validInput = 0;

    // Display race options
    printf("Enter your character's race:\n");
    for(int i = 0; i < 10; i++){
        printf("%d. %s\n", i + 1, races[i]);
    }

    // Input validation loop
    while(!validInput){
        printf("Enter your Choice: ");
        scanf("%d", &usersRace);

        if(usersRace < 1 || usersRace > 10) {
            printf("Invalid choice, please try again...\n\n");
        } 
        else{
            validInput = 1;
        }
    }

    strcpy(character->race, races[usersRace - 1]);

    printf("You selected: %s\n\n", character->race);
}

void selectBackground(struct Character *character){
    int usersBackground;
    int validInput = 0;

    // Display background options
    printf("Enter your character's background:\n");
    for(int i = 0; i < 16; i++){
        printf("%d. %s\n", i + 1, backgrounds[i]);
    }

    // Input validation loop
    while(!validInput){
        printf("Enter your Choice: ");
        scanf("%d", &usersBackground);

        if(usersBackground < 1 || usersBackground > 16) {
            printf("Invalid choice, please try again...\n\n");
        } 
        else{
            validInput = 1;
        }
    }

    strcpy(character->background, backgrounds[usersBackground - 1]);

    printf("You selected: %s\n\n", character->background);
}

void selectClass(struct Character *character){
    int usersClass;
    int validInput = 0;

    // Display Class options
    printf("Enter your character's class:\n");
    for(int i = 0; i < 12; i++){
        printf("%d. %s\n", i + 1, classes[i]);
    }

    // Input validation loop
    while(!validInput){
        printf("Enter your Choice: ");
        scanf("%d", &usersClass);

        if(usersClass < 1 || usersClass > 12) {
            printf("Invalid choice, please try again...\n\n");
        } 
        else{
            validInput = 1;
        }
    }

    strcpy(character->class, classes[usersClass - 1]);

    printf("You selected: %s\n\n", character->class);
}

void selectLevel(struct Character *character){
    int usersLevel, validInput = 0; // Initialize validInput to 0

    while(!validInput){
        printf("Enter your character's level (1-20): ");
        scanf("%d", &usersLevel);

        if(usersLevel < 1 || usersLevel > 20) {
            printf("Invalid choice, please try again...\n\n");
        } 
        else{
            validInput = 1;
        }
    }
    character->level = usersLevel;

    printf("Your character is level: %d\n\n", character->level);
}

void selectShield(struct Character *character){
    int usersShield, validInput = 0; 

    while(!validInput){
        printf("Enter if your character has a shield (1 for yes, 0 for no): ");
        scanf("%d", &usersShield);

        // Validate input to ensure it's either 1 or 0
        if(usersShield != 1 && usersShield != 0){
            printf("Invalid choice, please try again...\n\n");
        } 
        else{
            validInput = 1;
        }
    }

    character->hasShield = usersShield;

    if(usersShield == 1){
        printf("Your character is now holding a shield!\n\n");
    }
    else{
        printf("Your character is not holding a shield!\n\n");
    }
}
