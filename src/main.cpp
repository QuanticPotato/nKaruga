#include "common.h"
#include "levels.h"

int main(int argc, char **argv) {
	KeyEvent kEv = 0;
	int levelCounter, levelTimer, enemyCounter, waveIndex;
	bool levelEnded = false;
	
	// Engine
	enable_relative_paths(argv);
	
	if(initBuffering())
		exit(0);
	
	buildGameLUTs();
	
	Player ship(32);
	Enemy **enemiesArray;
	
	enemiesArray = (Enemy**)malloc(sizeof(Enemy*) * MAX_ENEMY);
	if(!enemiesArray)
	{
		printf("Couldn't allocate %d bytes for enemiesArray.\n", sizeof(Enemy) * MAX_ENEMY);
		exit(0);
	}
	
	for(int i = 0; i < MAX_ENEMY; i++)
	{
		enemiesArray[i] = new Enemy;
		enemiesArray[i]->deactivate();
	}
	
	ship.x = itofix(160);
	ship.y = itofix(220);
	
	levelCounter = 0;
	levelTimer = 0;
	enemyCounter = 0;
	waveIndex = 0;
	
	while(!KQUIT(kEv) && !levelEnded)
	{
		if(!levelTimer)
		{
			// Load the current enemy from the level stream
			if(levelStream[levelCounter] == LVLSTR_CMD)
			{
				levelCounter++;
				if(levelStream[levelCounter] == LVLSTR_NEWWAVE)
				{
					waveIndex = 0;
					levelCounter++;
				}
				else if(levelStream[levelCounter] == LVLSTR_WAIT)
				{
					levelTimer = levelStream[levelCounter + 1];
					levelCounter += 2;
				}
				else if(levelStream[levelCounter] == LVLSTR_KILLED)
				{
					int levelCanProgress = 1;
					for(int i = 0; i < MAX_ENEMY; i++)
					{
						if(enemiesArray[i]->isActive())
						{
							levelCanProgress = 0;
						}
					}
					if(levelCanProgress) levelCounter++;
					else levelCounter--;
				}
			}
			else if(levelStream[levelCounter] == LVLSTR_END)
			{
				levelEnded = true;
			}
			else
			{
				enemiesArray[enemyCounter]->activate(levelStream[levelCounter], levelStream[levelCounter + 1], levelStream[levelCounter + 2], levelStream[levelCounter + 3],
													waveIndex, levelStream[levelCounter + 4], levelStream[levelCounter + 5]);
				levelCounter += 6;
				enemyCounter = (enemyCounter + 1) & MAX_ENEMY - 1;
				waveIndex++;
			}
		}
		else
			levelTimer--;
		
		kEv = getk();
		ship.handle(kEv, enemiesArray);
		
		for(int i = 0; i < MAX_ENEMY; i++)
		{
			if(enemiesArray[i]->isActive())
			{
				enemiesArray[i]->handle(&ship);
			}
		}
	
		if(!ship.getLives())
			levelEnded = 1;
	
		updateScreen();
		memset(BUFF_BASE_ADDRESS, 0xff, BUFF_BYTES_SIZE);
		
		if(is_cx) sleep(4);
	}
	
	for(int i = 0; i < MAX_ENEMY; i++)
		delete enemiesArray[i];
	free(enemiesArray);
	
	deinitBuffering();
	return 0;
}