#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

struct Character {
    char name[50];            //character name
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
    char name[50];            // Name of the armor (e.g., "Chain Mail")
    char type[20];            // Type of armor (e.g., "Light", "Medium", "Heavy")
    int baseAC;               // Base armor class provided by the armor
    int maxDexBonus;          // Maximum Dexterity modifier that can be added
    int requiresDexCap;       // 1 if the Dex modifier is capped, 0 otherwise
    int strengthRequirement;  // Minimum Strength required to wear the armor
    int stealthDisadvantage;  // 1 if the armor imposes disadvantage on Stealth checks
};


struct Weapon {
    char name[50];            // Name of the weapon (e.g., "Longsword")
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
int calculateArmorClass(int dexterity, const char *armorName, int hasShield) {
    int shieldBonus = hasShield ? 2 : 0;            // +2 AC if the character has a shield
    int maxDex = 0;                                 // Default max dex modifier is unlimited

    // Find the armor in the armors array
    struct Armor *selectedArmor = NULL;
    for (int i = 0; i < sizeof(armors) - 1; i++) {
        if (strcmp(armors[i].name, armorName) == 0) {
            selectedArmor = &armors[i];
            break;
        }
    }
    // If armor not found, return a default value
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

    selectAlignment(newCharacter);
    
    selectAttributes(newCharacter);

    selectArmor(newCharacter);

    printf("Does your character have a shield? (1 for yes 0 for no):");
    scanf("%d", &newCharacter->hasShield); 

    newCharacter->speed = 30;
    //inserts the new character at the beginning of the list
    newCharacter->next = *newChar;
    *newChar = newCharacter;
}

void displayCharacter(struct Character *character){

    //check to see if list is empty
    if(character == NULL){ 
        printf("No characters in the list.\n");
        printf("Please enter a character first\n");
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
void searchCharacter(struct Character *head, char *searchCharacterName){
    int ifFound = 0;

    while(head != NULL){
        if (strcmp(head->name, searchCharacterName) == 0){
            printf("Your character has been found:\n\n");
            printf("Class: %s   Level: %d   Background: %s\n\n", head->class, head->level, head->background);                                    //displays Class, Level, Background
            printf("Race: %s    Alignment: %s\n\n", head->race, head->alignment);                                                                //displays Race, Alignment
            printf("Armor Class: %d     Speed: %dft\n\n", calculateArmorClass(head->dexterity, head->armor->name, head->hasShield), head->speed);   //displays Armor Class, Speed
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
void updateCharacter(struct Character *updatedCharacter, char *updateCharacterName){
    int usersClass;
    int usersBackground;
    int usersRace;
    int usersAlignment;
    int usersArmor;

    while (updatedCharacter != NULL){
        if (strcmp(updatedCharacter->name, updateCharacterName) == 0){
            printf("Enter updated details for your character: %s:\n", updateCharacterName);
            printf("Enter your characters level: ");
            scanf("%d", &updatedCharacter->level); 

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
            strcpy(updatedCharacter->class, pickClass(usersClass));

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
            strcpy(updatedCharacter->background, pickBackground(usersBackground));

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
            strcpy(updatedCharacter->race, pickRace(usersRace));

            selectAlignment(updatedCharacter);

            selectAttributes(updatedCharacter); 

            selectArmor(updatedCharacter);

            printf("Does your character have a shield?\n");
            scanf("%d", &updatedCharacter->hasShield); 
            printf("Character details have been updated successfully.\n\n");
            return;
        }
        updatedCharacter = updatedCharacter->next;
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
// depending on how user responds it will return a string of whatever background is picked
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
// depending on how user responds it will return a string of whatever race is picked
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
// depending on how user responds it will return a string of whatever alignment is picked
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

void selectArmor(struct Character *character) {
    int usersArmor;
    //displays menu
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
    scanf("%d", &usersArmor);

    // Validate input
    if (usersArmor < 1 || usersArmor > sizeof(armors) / sizeof(armors[0])) {
        printf("Invalid armor choice!\n");
        return;
    }

    // Assign selected armor
    character->armor = &armors[usersArmor - 1];

    printf("Selected armor: %s\n", character->armor->name);
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
        printf("Enter your Choice: \n");
        scanf("%d", &usersAlignment);

        if (usersAlignment < 1 || usersAlignment > 9) {
            printf("Invalid number, please try again...\n\n");
        } 
        else {
            validInput = 1;
        }
    }

    strcpy(character->alignment, alignments[usersAlignment - 1]);

    printf("Selected: %s\n", character->alignment);
}
