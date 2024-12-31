#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <string.h>
#include <time.h>

struct Character {
    char name[25];            //character name
    int level;                //character level
    struct Class *class;      //character class
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
    int proficiencyModifier;
    int HP;  //characters current proficiency modifier
    struct Character *next;   //points to the next character in the list
};

struct Armor {
    char name[25];            // Name of the armor (e.g., "Chain Mail")
    char type[20];            // Type of armor (e.g., "Light", "Medium", "Heavy")
    int baseAC;               // Base armor class provided by the armor
    int maxDexBonus;          // Maximum Dexterity modifier that can be added
    int requiresDexCap;       // 1 if the Dex modifier is capped, 0 otherwise
    int stealthDisadvantage;  // 1 if the armor imposes disadvantage on Stealth checks
};


struct Weapon {
    char name[25];            // Name of the weapon (e.g., "Longsword")
    char type[25];            // Type of weapon (e.g., "Melee", "Ranged")
    char damageType[20];      // Damage type (e.g., "Slashing", "Piercing")
    char damageDice[5];       // Damage dice (e.g., "1d8", "2d6")
    char twoHandDamage[5];    // The Weapons damage with two hands if versatile
    int isFinesse;            // 1 if weapon uses Dex for attack/damage, 0 otherwise
    int isVersatile;          // 1 if the weapon can be used one- or two-handed
    int isTwoHanded;          // 1 if the weapon requires two hands
    int range[2];             // Minimum and maximum range (e.g., {0, 60} for thrown weapons)
    int isLight;              // 1 if the weapon is light (good for dual-wielding)
    int isHeavy;              // 1 if the weapon is heavy (not for small creatures)
    int isReach;
};

struct Class {
    char name[20];
    char subClass[30];
    char hitDie[10];
};

struct Armor armors[] = {
    {"Unarmored", "-----", 10, -1, 0, 0},
    //light armor
    {"Padded Armor", "Light", 11, -1, 0, 1},
    {"Leather Armor", "Light", 11, -1, 0, 0},
    {"Studded Leather Armor", "Light", 12, -1, 0, 0},
    //medium armor
    {"Hide Armor", "Medium", 12, 2, 1, 0},
    {"Chain Shirt Armor", "Medium", 13, 2, 1, 0},
    {"Scale Mail Armor", "Medium", 14, 2, 1, 1},
    {"Breastplate Armor", "Medium", 14, 2, 1, 0},
    {"Half Plate Armor", "Medium", 15, 2, 1, 1},
    //heavy armor
    {"Ring Mail Armor", "Heavy", 14, 0, 0, 1},
    {"Chain Mail Armor", "Heavy", 16, 0, 0, 1},
    {"Splint Armor", "Heavy", 17, 0, 0, 1},
    {"Plate Armor", "Heavy", 18, 0, 0, 1},
};

struct Weapon weapons[] = {
    //bludgeoning
    {"Club", "Melee", "Bludgeoning", "1d4", "1d4", 0, 0, 0, {0, 0}, 0, 1, 0},
    {"Light Hammer", "Melee/Ranged", "Bludgeoning", "1d4", "1d4", 0, 0, 0, {20, 60}, 0, 1, 0},
    {"Mace", "Melee", "Bludgeoning", "1d6", "1d6", 0, 0, 0, {0, 0}, 0, 0, 0},
    {"Quarterstaff", "Melee", "Bludgeoning", "1d6", "1d8", 0, 1, 0, {0, 0}, 0, 0, 0},
    {"Flail", "Melee", "Bludgeoning", "1d8", "1d8", 0, 0, 0, {0, 0}, 0, 0, 0},
    {"Greatclub", "Melee", "Bludgeoning", "1d8", "1d8", 0, 0, 1, {0, 0}, 0, 0, 0},
    {"Warhammer", "Melee", "Bludgeoning", "1d8", "1d10", 0, 1, 0, {0, 0}, 0, 0, 0},
    {"Maul", "Melee", "Bludgeoning", "2d6", "2d6", 0, 0, 1, {0, 0}, 0, 1, 0},
    //piercing
    {"Dagger", "Melee/Ranged", "Piercing", "1d4", "1d4", 1, 0, 0, {20, 60}, 0, 1, 0},
    {"Javelin", "Ranged", "Piercing", "1d6", "1d6", 0, 0, 0, {30, 120}, 0, 0, 0},
    {"Spear", "Melee/Ranged", "Piercing", "1d6", "1d8", 0, 1, 0, {20, 60}, 0, 0, 0},
    {"Trident", "Melee/Ranged", "Piercing", "1d6", "1d8", 0, 1, 0, {20, 60}, 0, 0, 0},
    {"Rapier", "Melee", "Piercing", "1d8", "1d8", 1, 0, 0, {0, 0}, 0, 0, 0},
    {"War Pick", "Melee", "Piercing", "1d8", "1d8", 0, 0, 0, {0, 0}, 0, 0, 0},
    {"Pike", "Melee", "Piercing", "1d10", "1d10", 0, 0, 1, {0, 0}, 0, 1, 1},
    {"Lance", "Melee", "Piercing", "1d12", "1d12", 0, 0, 0, {0, 0}, 0, 0, 1},
    //slashing
    {"Sickle", "Melee", "Slashing", "1d4", "1d4", 0, 0, 0, {0, 0}, 0, 1, 0},
    {"Handaxe", "Melee/Ranged", "Slashing", "1d6", "1d6", 0, 0, 0, {20, 60}, 0, 1, 0},
    {"Scimitar", "Melee", "Slashing", "1d6", "1d6", 1, 0, 0, {0, 0}, 1, 0, 0},
    {"Shortsword", "Melee", "Piercing", "1d6", "1d6", 1, 0, 0, {0, 0}, 1, 0, 0},
    {"Battleaxe", "Melee", "Slashing", "1d8", "1d10", 0, 1, 0, {0, 0}, 0, 0, 0},
    {"Longsword", "Melee", "Slashing", "1d8", "1d10", 0, 1, 0, {0, 0}, 0, 0, 0},
    {"Glaive", "Melee", "Slashing", "1d10", "1d10", 0, 0, 1, {0, 0}, 0, 1, 1},
    {"Halberd", "Melee", "Slashing", "1d10", "1d10", 0, 0, 1, {0, 0}, 0, 1, 1},
    {"Greataxe", "Melee", "Slashing", "1d12", "1d12", 0, 0, 1, {0, 0}, 0, 1, 0},
    {"Greatsword", "Melee", "Slashing", "2d6", "2d6", 0, 0, 1, {0, 0}, 0, 1, 0},
    //ranged
    {"Hand Crossbow", "Ranged", "Piercing", "1d6", "1d6", 0, 0, 0, {30, 120}, 1, 0, 0},
    {"Shortbow", "Ranged", "Piercing", "1d6", "1d6", 0, 0, 1, {80, 320}, 0, 0, 0},
    {"Light Crossbow", "Ranged", "Piercing", "1d8", "1d8", 0, 0, 1, {80, 320}, 0, 0, 0},
    {"Longbow", "Ranged", "Piercing", "1d8", "1d8", 0, 0, 1, {150, 600}, 0, 1, 0},
    {"Heavy Crossbow", "Ranged", "Piercing", "1d10", "1d10", 0, 0, 1, {100, 400}, 0, 1, 0}
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

const char *classes[12][5] = {
{"Barbarian", "Path of the Berserker", "Path of the Wild Heart", "Path of the World Tree", "Path of the Zealot"},
{"Bard", "College of Dance", "College of Glamour", "College of Lore", "College of Valor"},
{"Cleric", "Life Domain", "Light Domain", "Trickery Domain", "War Domain"},
{"Druid", "Circle of the Land", "Circle of the Moon", "Circle of the Sea", "Circle of the Stars"},
{"Fighter", "Battle Master", "Champion", "Eldritch Knight", "Psi Warrior"}, 
{"Monk", "Warrior of Mercy", "Warrior of Shadow", "Warrior of the Elements", "Warrior of the Open Hand"}, 
{"Paladin", "Oath of Devotion", "Oath of Glory", "Oath of the Ancients", "Oath of Vengeance"}, 
{"Ranger", "Beast Master", "Fey Wanderer", "Gloom Stalker", "Hunter"}, 
{"Rogue", "Arcane Trickster", "Assassin", "Soulknife", "Thief"},
{"Sorcerer", "Aberrant Sorcery", "Clockwork Sorcery", "Draconic Sorcery", "Wild Magic Sorcery"}, 
{"Warlock", "Archfey Patron", "Celestial Patron", "Fiend Patron", "Great Old One Patron"}, 
{"Wizard", "Abjurer", "Diviner", "Evoker", "Illusionist"}
};

//armor functions
char *armorRequirement(struct Armor *armor);
char *armorStealth(struct Armor *armor);

//weapon functions
char *weaponFinesse(struct Weapon *weapon);
char *weaponVersatile(struct Weapon *weapon);
char *weaponRange(struct Weapon *weapon);

//selecting functions (adding updating character data)
void selectName(struct Character *character);
void selectShield(struct Character *character);
int selectLevel(struct Character *character);
void selectClass(struct Character *character);
void selectSubClass(struct Character *character);
void selectBackground(struct Character *character);
void selectRace(struct Character *character);
void selectAlignment(struct Character *character);
void selectAttributes(struct Character *character);
void selectArmor(struct Character *newCharacter);
void selectWeapon(struct Character *character);

//calculate functions
int calculateModifier(int attribute); //takes attributes from character and converts it to their modifier
int calculateArmorClass(int dexterity, const char *armorName, int hasShield); //returns armor class via certain character criteria
int calculateRollModifier(struct Character *head, char *diceCharacterName, int numChoice); //uses your characters modifiers and +/- from dice roll
int calculateDamageDiceRoll(struct Weapon *weapon);
int calculateDamageRoll(struct Character *character, char *characterName);
int calculateAttackRoll(struct Character *character, char *characterName);
int calculateHealth(struct Character *character);

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

char searchCharacterName[50];   //Array to store name for searching
char updateCharacterName[50];   //Array to store name for updating
char deleteCharacterName[50];   //Array to store name for deleting
char diceCharacterName[50];      //Array to store name for dice rolling
char attackRolCharacterName[50]; //Array to store name for attack roles
char arCharacterName[50];
char dmgrCharacterName[50];

int choice, numChoice; //users choice

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
    printf("8. Make an attack role\n");
    printf("9. Roll for damage\n");
    printf("10. Exit Dnd Character Creator\n");
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
        printf("Enter the name of your character you would like to attack with: ");
        scanf("%s", arCharacterName);
        printf("You rolled: %d\n\n", calculateAttackRoll(characterList, arCharacterName));
    }
    if(choice == 9){
        printf("Enter the name of your character you would like roll for damage with: ");
        scanf("%s", dmgrCharacterName);
        printf("You rolled: %d\n\n", calculateDamageRoll(characterList, dmgrCharacterName));
    }
    if(choice == 10){
        printf("Exiting DnD Character Creator...\n");
    }
    if(choice > 10 || choice < 1){
        printf("\nInvalid choice, please try again...\n\n");
    }
} while(choice != 10);


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
    int userCurrLevel;
    //make space for new character in memory
    struct Character *newCharacter = (struct Character *)malloc(sizeof(struct Character));

    if (newCharacter == NULL){
        printf("Failed to allocate memory :(\n");
        exit(1);
    }

    //prompt user to enter details for character
    printf("For more information regarding DnD character details visit DnD Beyond\n");

    printf("Enter your characters name: ");
    scanf("%20s", newCharacter->name);
    printf("Your characters name is: %s\n", newCharacter->name);
    // Calls the select functions to gather information about your character
    userCurrLevel = selectLevel(newCharacter);
    selectClass(newCharacter); 
    if(userCurrLevel > 2){
        selectSubClass(newCharacter);
    }
    selectBackground(newCharacter);
    selectRace(newCharacter);
    selectAlignment(newCharacter);
    selectAttributes(newCharacter);
    selectArmor(newCharacter);
    selectWeapon(newCharacter);
    selectShield(newCharacter);

    newCharacter->speed = 30;
    newCharacter->proficiencyModifier = 1; ////////////////////////////////////////////////////////////////
    newCharacter->HP = calculateHealth(newCharacter);
    //inserts the new character at the beginning of the list
    newCharacter->next = *newChar;
    *newChar = newCharacter;
    if(newCharacter->class->subClass == NULL){
        strcpy(newCharacter->class->subClass, "N/A");
    }
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
        printf("Class: %s   Level: %d   Background: %s\n\n", character->class->name, character->level, character->background);                                    //displays Class, Level, Background
        printf("Sub Class: %s   Race: %s    Alignment: %s\n\n", character->class->subClass, character->race, character->alignment);                                                                //displays Race, Alignment
        printf("Armor: %s   Armor Class: %d     Weapon: %s\n\n", character->armor->name, calculateArmorClass(character->dexterity, character->armor->name, character->hasShield), character->weapon->name);   //displays Armor, Armor Class, Weapon
        printf("Total HP: %d\n\n", character->HP);
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
            printf("    ___________ Name: %s ___________\n\n", character->name);
        printf("Class: %s   Level: %d   Background: %s\n\n", character->class->name, character->level, character->background);                                    //displays Class, Level, Background
        printf("Sub Class: %s   Race: %s    Alignment: %s\n\n", character->class->subClass, character->race, character->alignment);                                                                //displays Race, Alignment
        printf("Armor: %s   Armor Class: %d     Weapon: %s\n\n", character->armor->name, calculateArmorClass(character->dexterity, character->armor->name, character->hasShield), character->weapon->name);   //displays Armor, Armor Class, Weapon
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
            selectWeapon(updatedCharacter);
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

int calculateDamageDiceRoll(struct Weapon *weapon){

    if (!weapon || !weapon->damageDice){
            return -1; // Error: Null pointer
        }

        if (strcasecmp(weapon->damageDice, "1D4") == 0){
            return rollD4();
        } 
        else if (strcasecmp(weapon->damageDice, "1D6") == 0){
            return rollD6();
        } 
        else if (strcasecmp(weapon->damageDice, "1D8") == 0){
            return rollD8();
        } 
        else if (strcasecmp(weapon->damageDice, "1D10") == 0){
            return rollD10();
        } 
        else if (strcasecmp(weapon->damageDice, "1D12") == 0){
            return rollD12();
        } 
        else if (strcasecmp(weapon->damageDice, "2D6") == 0){
            return rollD6() + rollD6();
        } 
        else{
            return -1; // Error: Unknown dice type
        }
}

int calculateDamageRoll(struct Character *character, char *characterName) {
    int dmgDice;

    // Check if character or characterName is NULL
    if (character == NULL || characterName == NULL) {
        return -1; // Error case
    }

    // Check if the character name matches
    if (strcmp(character->name, characterName) == 0) {
        // Ensure weapon is not NULL
        if (character->weapon != NULL) {
            struct Weapon *weapon = character->weapon;

            // Handle finesse property
            if (weapon->isFinesse == 1) {
                dmgDice = calculateDamageDiceRoll(weapon) + calculateModifier(character->dexterity);
            }
            // Handle versatile property
            else if (weapon->isVersatile == 1){
                if (character->hasShield == 0){
                    // Use two-handed damage dice
                    dmgDice = calculateDamageDiceRoll(weapon) + calculateModifier(character->strength);
                } 
                else {
                    // Use standard one-handed damage dice
                    dmgDice = calculateDamageDiceRoll(weapon) + calculateModifier(character->strength);
                }
            }
            // Handle standard strength-based attacks
            else {
                dmgDice = calculateDamageDiceRoll(weapon) + calculateModifier(character->strength);
            }
        } 
        else {
            return -1; // Weapon is NULL
        }
    } 
    else {
        return -1; // Character name does not match
    }

    return dmgDice; // Return damage dealt
}

int calculateAttackRoll(struct Character *character, char *characterName){

    if (character == NULL || characterName == NULL || character->weapon == NULL) {
        return -1; // Error case
    }

    // Base roll
    int baseRoll = rollD20();
    int abilityModifier;
    int isProficient = character->proficiencyModifier;

    // Determine which ability modifier to use
    if (character->weapon->isFinesse == 1 || strcmp(character->weapon->type, "Ranged") == 0 || (strcmp(character->weapon->type, "Melee/Ranged") == 0 && character->weapon->isFinesse == 1)) {
        abilityModifier = character->dexterity;
    } 
    else {
        abilityModifier = character->strength;
    }

    // Calculate and return the attack roll
    return baseRoll + calculateModifier(abilityModifier) + isProficient;
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

int calculateHealth(struct Character *character){

    int conMod = calculateModifier(character->constitution);
    int charLVL = character->level;

    if (!character || !character->class || !character->class->hitDie) {
        printf("Error: Invalid character or class data.\n");
        return -1; // Error
    }

    if(strcmp(character->class->hitDie, "1D6") == 0){
        return (6 + conMod) + (4 * charLVL) + (conMod * charLVL);
    }
    else if(strcmp(character->class->hitDie, "1D8") == 0){
        return (8 + conMod) + (5 * charLVL) + (conMod * charLVL);
    }
    else if(strcmp(character->class->hitDie, "1D10") == 0){
        return (10 + conMod) + (6 * charLVL) + (conMod * charLVL);
    }
    else if(strcmp(character->class->hitDie, "1D12") == 0){
        return (12 + conMod) + (7 * charLVL) + (conMod * charLVL);
    }
    else{
        return -1; // Error
    }
}

char *armorRequirement(struct Armor *armor) {
    // Check specific strength requirements for heavy armor types

    if (strcmp(armor->name, "Chain Mail Armor") == 0) {
        return "13 Strength";
    } 
    else if (strcmp(armor->name, "Splint Armor") == 0 || strcmp(armor->name, "Plate Armor") == 0) {
        return "15 Strength";
    } 
    else {
        return " --------- "; // No strength requirement for other armors
    }
}

char *armorStealth(struct Armor *armor) {

    if(armor->stealthDisadvantage == 1) {
        return "Disadvantage";
    } 
    else{
        return " ---------- ";
    } 

}

void selectArmor(struct Character *character) {
    int usersArmor, check;

    do{
        check = 0;
        // Display armor menu
        printf("Select Your Armor:\n");
        printf("-------------------------------------------------------------------------------------\n");
        printf("   %-22s |   %-6s |  %-3s | %-8s |    %-10s| %-15s\n", "Name", "Type", "AC", "Dex Mod.", "Stealth", "Requirements");
        printf("-------------------------------------------------------------------------------------\n");
        //prints armor from armors array
        for ( int i = 0; i < 9; i++) {
            printf("%d. %-22s |  %-7s |  %-3d |    %-5d | %-12s | %-15s\n", i + 1, armors[i].name, armors[i].type, armors[i].baseAC, calculateModifier(character->dexterity), armorStealth(&armors[i]), armorRequirement(&armors[i]));
        }
        for (int i = 9; i < 13; i++) {
            printf("%d. %-21s |  %-7s |  %-3d |    %-5d | %-12s | %-15s\n", i + 1, armors[i].name, armors[i].type, armors[i].baseAC, calculateModifier(character->dexterity), armorStealth(&armors[i]), armorRequirement(&armors[i]));
        }

        printf("-------------------------------------------------------------------------------------\n");
        printf("\nEnter your choice: ");
        scanf("%d", &usersArmor);

        // Validate input + make sure character has enough strength
        if (usersArmor < 1 || usersArmor > 13) {
            printf("\nInvalid armor choice! Please try again.\n\n");
        }
        else if(strcmp(armorRequirement(&armors[usersArmor - 1]), "13 Strength") == 0){
            if(character->strength > 12){
                check = 1;
            }
            printf("\nYou do  not have enough strength to wield this armor! Please try again.\n\n");
        }
        else if(strcmp(armorRequirement(&armors[usersArmor - 1]), "15 Strength") == 0){
            if(character->strength > 14){
                check = 1;
            }
            printf("\nYou do  not have enough strength to wield this armor! Please try again.\n\n");
        }
        else{
            check = 1;
        }

    } while(usersArmor < 1 || usersArmor > 13 || check == 0);

    // Assign selected armor
    character->armor = &armors[usersArmor - 1];
    printf("You selected: %s\n\n", character->armor->name);
}

char *weaponFinesse(struct Weapon *weapon){
    if(weapon->isFinesse == 1){
        return "Yes";
    }
    else{
        return "No";
    }
}

char *weaponVersatile(struct Weapon *weapon){
    if(weapon->isVersatile == 1){
        return "Yes";
    }
    else{
        return "No";
    }
}

char *weaponRange(struct Weapon *weapon) {
    static char rangeStr[20]; // Use a static buffer for returning the string
    if (weapon->range[0] == 0 && weapon->range[1] == 0) {
        return "Melee";
    } 
    else {
        // Format the range into a string
        snprintf(rangeStr, sizeof(rangeStr), "%d/%d", weapon->range[0], weapon->range[1]);
        return rangeStr;
    }
}


void selectWeapon(struct Character *character){
    int usersWeapon;

    do{
        // Display weapon menu
        printf("Select Your Weapon(s):\n");
        printf("-----------------------------------------------------------------------------------------------\n");
        printf(" %-19s |      %-9s |   %-10s | %-8s | %-5s | %-5s | %-10s \n", "Name", "Type", "DMG type", "DMG dice", "Finesse", "Versitile", "Range");
        printf("-----------------------------------------------------------------------------------------------\n");
        //prints weapons from the weapons array
        for ( int i = 0; i < 9; i++) {
            printf("%d.  %-16s |  %-13s |  %-11s |    %-5s |   %-5s |    %-6s | %-10s \n", i + 1, weapons[i].name, weapons[i].type, weapons[i].damageType, weapons[i].damageDice, weaponFinesse(&weapons[i]), weaponVersatile(&weapons[i]), weaponRange(&weapons[i]));
        }
        for (int i = 9; i < 31; i++) {
            printf("%d.  %-15s |  %-13s |  %-11s |    %-5s |   %-5s |    %-6s | %-10s \n", i + 1, weapons[i].name, weapons[i].type, weapons[i].damageType, weapons[i].damageDice, weaponFinesse(&weapons[i]), weaponVersatile(&weapons[i]), weaponRange(&weapons[i]));
        }

        printf("-----------------------------------------------------------------------------------------------\n");
        printf("\nEnter your choice: ");
        scanf("%d", &usersWeapon);

        // Validate input + make sure character has enough strength
        if (usersWeapon < 1 || usersWeapon > 31) {
            printf("\nInvalid weapon choice! Please try again.\n\n");
        }
   
    } while(usersWeapon < 1 || usersWeapon > 31);

    // Assign selected armor
    character->weapon = &weapons[usersWeapon - 1];
    printf("You selected: %s\n\n", character->weapon->name);
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

if (character->class == NULL) {
        character->class = malloc(sizeof(struct Class));
        if (!character->class) {
            fprintf(stderr, "Memory allocation failed!\n");
            exit(EXIT_FAILURE);
        }
    }

    // Display Class options
    printf("Enter your character's class:\n");
    for(int i = 0; i < 12; i++){
        printf("%d. %s\n", i + 1, classes[i][0]);
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

    strcpy(character->class->name, classes[usersClass - 1][0]);

    printf("You selected: %s\n\n", character->class->name);

    //adds hit die
    if(strcmp(character->class->name, "Wizard") == 0 || strcmp(character->class->name, "Sorcerer") == 0){
        strcpy(character->class->hitDie, "1D6");
    }
    else if(strcmp(character->class->name, "Bard") == 0 || strcmp(character->class->name, "Cleric") == 0 || strcmp(character->class->name, "Druid") == 0 || strcmp(character->class->name, "Monk") == 0 || strcmp(character->class->name, "Rogue") == 0 || strcmp(character->class->name, "Warlock") == 0){
        strcpy(character->class->hitDie, "1D8");
    }
    else if(strcmp(character->class->name, "Fighter") == 0 || strcmp(character->class->name, "Paladin") == 0 || strcmp(character->class->name, "Ranger") == 0){
        strcpy(character->class->hitDie, "1D10");
    }
    else if(strcmp(character->class->name, "Barbarian") == 0){
        strcpy(character->class->hitDie, "1D12");
    }
    else{
        strcpy(character->class->hitDie, "-1");
    }
}

void selectSubClass(struct Character *character){
    int usersSubClass, usersChoice = 0;
    int validInput = 0;
    int usersClass = -1;

    // Display sub class options
    printf("Enter your character's sub class:\n");
    
    for(int i = 0; i < 13; i++){
        if(strcmp(character->class->name, classes[i][0]) == 0){
            for(int j = 1; j < 5; j++){
                usersClass = i;
                printf("%d. %s\n", j, classes[usersClass][j]);
            }
            break;
        }
    }
    
    if (usersClass == -1) {
        printf("Error: Class not found. Please ensure your character's class is valid.\n");
        return;
    }

    // Input validation loop
    while(!validInput){
        printf("Enter your Choice: ");
        scanf("%d", &usersChoice);

        if(usersChoice < 1 || usersChoice > 4 || classes[usersClass][usersChoice] == NULL) {
            printf("Invalid choice, please try again...\n\n");
        } 
        else{
            validInput = 1;
        }
    }

    strcpy(character->class->subClass, classes[usersClass][usersChoice]);

    printf("You selected: %s\n\n", character->class->subClass);
}

int selectLevel(struct Character *character){
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
    return usersLevel;
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