#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define MAX_NAME 30
#define MAX_ITEM 5

typedef struct
{
    char **items;
    int count;
    int health;
    int sanity;
    int strength, intelligence, agility;
    int key, tome, wisdom;
} Player;

typedef struct
{
    char name[MAX_NAME];
    int health;
    int attack;
} Enemy;

void clearScreen(){ system("cls || clear"); }
void pressEnter(){ printf("\n[Press ENTER]"); getchar(); }
void modSanity(Player *p,int a){
    p->sanity+=a;
    if(p->sanity>100) p->sanity=100;
    if(p->sanity<0) p->sanity=0;
}

void showIntro() 
{
    clearScreen();
    printf("************************** STORY BEGINS **************************\n\n");
    printf("Long ago, an ancient civilization known as the Arkenites lived\n");
    printf("under these mountains. They protected a mysterious being known\n");
    printf("as The Oracle, a guardian of forbidden knowledge.\n\n");
    printf("The Oracle holds secrets about life, death, reality, and the truth\n");
    printf("of the universe... knowledge so powerful that the Arkenites sealed it\n");
    printf("deep underground to keep it hidden from unworthy hands.\n\n");
    pressEnter();

    printf("You are an explorer who studied the legends your whole life.\n");
    printf("During your expedition, the ground collapses beneath you!\n");
    pressEnter();

    printf("You wake up in the darkness of the forgotten caves...\n");
    printf("Strange glowing symbols pulse around you.\n");
    printf("A distant whisper echoes:\n");
    printf("  \"Seeker! find the Three Relics and prove your worth! \"\n\n");
    printf("To escape and discover the Oracle's truth, you must collect:\n");
    printf("  1) The Ancient Key\n");
    printf("  2) The Oracle's Tome\n");
    printf("  3) The Seal of Wisdom\n\n");
    printf("Your mind and health will be tested. The caves bend reality.\n");
    printf("Stay calm, stay alert, and uncover the truth waiting at the end.\n");
    printf("\n************************** GOOD LUCK **************************\n");
    pressEnter();
}

int choice(int max)
{
    int c;
    printf("Choose: ");
    if(scanf("%d", &c) != 1){ while(getchar()!='\n'); return -1; }
    getchar();
    return (c >= 1 && c <= max) ? c : -1;
}

void modHP(Player *p, int a)
{
    p->health += a;
    if(p->health > 100) p->health = 100;
    if(p->health <= 0){
        printf("\nYou faint! waking up to find yourself back at the entrance.\n");
        p->health = 50;
        p->sanity -= 20;
        if(p->sanity < 0) p->sanity = 0;
        pressEnter();
    }
}

void addItem(Player *p, const char *item)
{
    if(p->count < MAX_ITEM){
        strcpy(p->items[p->count++], item);
        printf("\nObtained: %s\n", item);
    }
}

int combat(Player *p, Enemy *e)
{
    printf("\n--- Combat: %s ---\n", e->name);
    while(p->health>0 && e->health>0){
        printf("\nYour HP:%d  SAN:%d | %s HP:%d\n", p->health,p->sanity,e->name,e->health);
        printf("1) Attack  2) Flee\n");
        int c=choice(3);
        if(c==1)
		{
            int dmg=(rand()%10)+p->strength;
            printf("You strike for %d damage!\n",dmg);
            e->health-=dmg;
        }
         else 
		{
            if(rand()%2==0){ printf("Flee successful!\n"); return 0; }
            else printf("Cannot escape!\n");
        }
        if(e->health>0){ int dmg=e->attack+rand()%5; printf("%s attacks for %d damage!\n",e->name,dmg); modHP(p,-dmg);}
    }
    if(p->health>0) { printf("\nYou defeated %s!\n",e->name); return 1; }
    return 0;
}


char* rQ[] = 
{
    "I begin with one and end with many.I twist the truth until truth becomes me.Once I grow, I never fully die",
    "I can shatter in a second, yet I am never touched.I can blind, yet I have no eyes.I can speak without a tongue, yet everyone listens.",
    "You measure my life in hours and I serve you by expiring. I'm quick when I'm thin and slow when I'm fat. The wind is my enemy."
};

char* rA[][4] = 
{
    {"Regret","Corruption","Suspicion","Rumor"},
    {"Silence"," Dream","Reflection","Shadow"},
    {"Clock","Candle","Shadow","Ice"}
};

int rCorrect[] = {4,1,2};

int runRiddle()
{
    int id = rand() % 3;
    printf("\nRiddle: %s\n", rQ[id]);
    for(int i=0;i<4;i++) printf("%d) %s\n", i+1, rA[id][i]);
    int c = choice(4);
    return (c == rCorrect[id]);
}


void left(Player *p)
{
    clearScreen();
    printf("\n--- GLOWING CHAMBER ---\n");
    if(p->key){ printf("You already took the Ancient Key.\n"); pressEnter(); return; }
    if(rand()%2){ Enemy gob={"Cave Goblin",30,5}; combat(p,&gob); }
    if(runRiddle(p)){ printf("Correct! The runes glow...\n"); addItem(p,"Ancient Key"); p->key=1; modSanity(p,10);}
    else { printf("Wrong! Dark shock hits you!\n"); modHP(p,-20); modSanity(p,-10);}
    pressEnter();
}

void center(Player *p)
{
    clearScreen();
    printf("\n--- OLD LIBRARY ---\n");
    printf("Dusty shelves whisper as if remembering you.\n");

    if(p->tome){ printf("Already obtained the Oracle's Tome.\n"); pressEnter(); return; }

    printf("Three shelves glow...\n");
    printf("1) Red Shelf\n2) Blue Shelf\n3) Black Shelf\n");

    int c = choice(3);
    if(c==2){
        printf("You find the Oracle's Tome!\n");
        addItem(p, "Oracle's Tome");
        p->tome = 1;
        p->sanity += 15;
        if(p->sanity > 100) p->sanity = 100;
    } else if(c==1){ 
        printf("A bright flash burns your hand!\n"); 
        modHP(p,-20); 
    } else { 
        printf("Dark whispers coil around your mind...\n"); 
        p->sanity -= 20; 
        if(p->sanity < 0) p->sanity = 0;
    }

    pressEnter();
}

void right(Player *p)
{
    clearScreen();
    printf("\n--- PUZZLE ROOM ---\n");
    printf("Symbols rearrange themselves as you step inside.\n");

    if(p->wisdom){ printf("Puzzle already solved.\n"); pressEnter(); return; }

    if(runRiddle()){
        printf("Correct! A glowing seal rises from the floor.\n");
        addItem(p, "Seal of Wisdom");
        p->wisdom = 1;
        p->sanity += 20;
        if(p->sanity > 100) p->sanity = 100;
    } else {
        printf("Wrong! The room trembles violently!\n");
        modHP(p,-15);
        p->sanity -= 10;
        if(p->sanity < 0) p->sanity = 0;
    }

    pressEnter();
}

void finalDoor(Player *p)
{
    clearScreen();
    if(!(p->key && p->tome && p->wisdom))
	{
        printf("\nThe Oracle's Door stays sealed. Missing items:\n");
        if(!p->key) printf("- Ancient Key\n");
        if(!p->tome) printf("- Oracle's Tome\n");
        if(!p->wisdom) printf("- Seal of Wisdom\n");
        pressEnter();
        return;
    }

    printf("\n------------------ THE ORACLE'S CHAMBER ------------------\n");
    printf("The relics glow in your hands. The massive door shakes,\n");
    printf("then slowly opens with a thunderous rumble...\n\n");
    printf("A blinding figure rises from the darkness - THE ORACLE.\n\n");

    printf("\"Seeker... you have proven your mind, body, and spirit.\n");
    printf("   You desire the truth... so listen carefully.\"\n\n");

    printf("The cave walls shift like liquid. A warm light surrounds you.\n");
    printf("The Oracle whispers the TRUTH:\n\n");

    printf("\"THE TRUTH IS THIS:\n");
    printf("   Knowledge is the greatest power - but also the greatest burden.\n");
    printf("   Those who chase truth must be strong enough to survive it.\n");
    printf("   You faced fear, confusion, and darkness… and still moved forward.\n");
    printf("   That courage is the truth you sought.\n");
    printf("   The truth is within you, not in these caves.\"\n\n");

    printf("The Oracle fades, the chamber cracks open, and sunlight breaks in.\n");
    printf("You step out of the mountain… forever changed.\n\n");

    printf("*********************** YOU WIN! ***********************\n");
    exit(0);
}

void hub(Player *p)
{
    while(1){
        clearScreen();
        printf("HP:%d  SAN:%d\nITEMS:%d\n\n", p->health, p->sanity, p->count);
        printf("1) Left Chamber\n2) Library\n3) Puzzle Room\n4) Final Door\n5) Rest (+30 HP, +20 Sanity)\n");

        int c = choice(5);
        if(c==1) left(p);
        else if(c==2) center(p);
        else if(c==3) right(p);
        else if(c==4) finalDoor(p);
        else {
            printf("You rest safely...\n");
            modHP(p,30);
            p->sanity += 20;
            if(p->sanity>100) p->sanity = 100;
            pressEnter();
        }
    }
}

int main()
{
    srand(time(NULL));

    Player p;
    p.items = malloc(MAX_ITEM * sizeof(char*));
    for(int i=0;i<MAX_ITEM;i++) p.items[i] = malloc(30);

    p.count=0; p.health=100; p.sanity=100;
    p.key=p.tome=p.wisdom=0;

    showIntro();
    hub(&p);
    return 0;
}
