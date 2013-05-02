#include "enemy.h"

int enemyArray[MAXIMUM_NUMBER_ENEMIES] = {0};
int enemyCount = 0;

//Checks if the enemy is in the list.
//If it's in the list then it returns the enemy number.
//Otherwise it returns zero.
int enemy_checkExist(int number){
    int i;
    for(i=0; i<enemyCount; i++){
        if(enemyArray[i]==number){
            return enemyArray[i];
        }
    }
    return 0;
}

//Adds an enemy to the list of possible enemies.
//If the number of enemies reaches the limit, the number won't be added.
void enemy_addEnemy(int number){
    if(enemyCount<MAXIMUM_NUMBER_ENEMIES){
        enemyArray[enemyCount] = number;
    }
    enemyCount++;
}

//Resets the array of valid enemy numbers.
void enemy_start(void){
    int i;
    for(i=0; i<MAXIMUM_NUMBER_ENEMIES; i++){
        enemyArray[i] = 0;
    }
    enemyCount = 0;
}

void enemy_reset(void){
    enemy_start();
}
