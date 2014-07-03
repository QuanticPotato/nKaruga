#include "common.h"
#include "levels.h"

//~ #define DEBUG_NKARUGA

bool skipFrame = false;

int main(int argc, char **argv) {
	KeyEvent kEv = 0;
	int levelCounter, levelTimer, enemyCounter, waveIndex, scrollOffset = 0, pxScrollStart, pxScrollEnd;
	bool levelEnded = false, displayBg = true;
	int readKeys = 0;
	
	enable_relative_paths(argv);
	
	buildGameLUTs();
	
	BulletArray* bArray = new BulletArray;
	Player ship;
	Enemy *enemiesArray[MAX_ENEMY];
	
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
	
	// Init display
	initBuffering();
	clearBufferW();
	
	// Save background so it is converted to the right format depending on the screen
	drawSprite(image_entries[image_LUT_background], 0, 0);
	memcpy(image_entries[image_LUT_background] + 3, BUFF_BASE_ADDRESS, BUFF_BYTES_SIZE);
	// Also, transpose it
	c_image_entries[image_LUT_background] += 6;
	// for both spritesets
	d_image_entries[image_LUT_background] += 6;
	
	while(!KQUIT(kEv) && !levelEnded)
	{
		if(!levelTimer)
		{
			// Load the current enemy from the level stream
			int currentLevelByte = levelStream[levelCounter];
			if(currentLevelByte == LVLSTR_CMD)
			{
				levelCounter++;
				currentLevelByte = levelStream[levelCounter];
				if(currentLevelByte == LVLSTR_NEWWAVE)
				{
					waveIndex = 0;
					levelCounter++;
				}
				else if(currentLevelByte == LVLSTR_WAIT)
				{
					levelTimer = levelStream[levelCounter + 1];
					levelCounter += 2;
				}
				else if(currentLevelByte == LVLSTR_KILLED)
				{
					int levelCanProgress = 1;
					for(int i = 0; i < MAX_ENEMY; i++)
					{
						if(enemiesArray[i]->isActive())
						{
							levelCanProgress = 0;
							break;
						}
					}
					if(levelCanProgress) levelCounter++;
					else levelCounter--;
				}
			}
			else if(currentLevelByte == LVLSTR_END)
			{
				levelEnded = true;
			}
			else
			{
				enemiesArray[enemyCounter]->activate(itofix(levelStream[levelCounter]), itofix(levelStream[levelCounter + 1]), levelStream[levelCounter + 2], levelStream[levelCounter + 3],
													levelStream[levelCounter + 4], waveIndex, levelStream[levelCounter + 5], levelStream[levelCounter + 6]);
				levelCounter += 7;
				enemyCounter = (enemyCounter + 1) % MAX_ENEMY;
				waveIndex++;
			}
		}
		else
			levelTimer--;
		
		if(!(readKeys % 4))
			kEv = getk();
		readKeys++;
		
		ship.handle(kEv, bArray);
		
		for(int i = 0; i < MAX_ENEMY; i++)
			enemiesArray[i]->handle(&ship, bArray);
		
		bArray->handle(&ship, enemiesArray);
		
		if(!ship.getLives())
			levelEnded = 1;
		
		if(!skipFrame)
		{
			updateScreen();
		
			if(displayBg)
			{
				// Display a scrolling background
				pxScrollStart = scrollOffset * 160;
				pxScrollEnd = 160*240 - pxScrollStart;
				// cheat using cast to copy faster since the ARM9 registers are 32-bits
				for(int i = pxScrollStart, j = 0; i < 160 * 240; i++, j++)
					((unsigned int*)BUFF_BASE_ADDRESS)[i] = ((unsigned int *)image_entries[image_LUT_background])[j];
				for(int i = 0, j = pxScrollEnd; j < 160 * 240; i++, j++)
					((unsigned int*)BUFF_BASE_ADDRESS)[i] = ((unsigned int *)image_entries[image_LUT_background])[j];
				
				scrollOffset++;
				if(scrollOffset > 239) scrollOffset = 0;
			}
			else
				clearBufferW();
		}
		skipFrame = !skipFrame;
		
		if(K7(kEv)) displayBg = true;
		if(K8(kEv)) displayBg = false;
		if(K4(kEv)) image_entries = c_image_entries;
		if(K5(kEv)) image_entries = d_image_entries;
		
		#ifdef DEBUG_NKARUGA
		sleep(3);
		#endif
	}
	
	for(int i = 0; i < MAX_ENEMY; i++)
		delete enemiesArray[i];
	
	delete bArray;
	deinitBuffering();
	return 0;
}
