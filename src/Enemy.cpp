#include "common.h"

inline Fixed angleToPlayer(Enemy *e, Player *p)
{
	return (int)(atan2((double)(p->y - e->gety()), (double)(p->x - e->getx())) * 128. / M_PI);
}

Enemy::Enemy()
{
	active = false;
	diedThisFrame = false;
	isJointed = false;
}

Enemy::~Enemy()
{
}

void Enemy::handle(Player *p, BulletArray *bArray)
{
	Rect er;
	Fixed angle;
	
	if(active)
	{
		// handle bullets and pattern first
		switch(callback)
		{
			// Intro 1
			case Pattern_1_1:
				if(!(internal[0] % 2))
				{
					internal[1] = internal[0] / 2;
					x = itofix(320) - fixmul(sq(internal[1]), 768);
					y = itofix(internal[1]);
				}
				internal[0]++;
				rotationAngle = internal[0] / 2;
				break;	
			case Pattern_1_2:
				if(!(internal[0] % 2))
				{
					internal[1] = internal[0] / 2;
					x = fixmul(sq(internal[1]), 768);
					y = itofix(internal[1]);
				}
				internal[0]++;
				rotationAngle = internal[0] / 2;
				break;
			case Pattern_1_3:
				if(!(internal[0] % 3))
				{
					internal[1] = internal[0] / 3;
					x = fixdiv(sq(internal[0] - 240) - itofix(240), waveIndex * 128 + 256) + itofix(320);
					y = itofix(internal[1]);
				}
				internal[0]++;
				rotationAngle = internal[0] / 2;
				break;
			case Pattern_1_4:
				if(!(internal[0] % 3))
				{
					internal[1] = internal[0] / 3;
					x = -fixdiv(sq(internal[0] - 240) - itofix(240), waveIndex * 128 + 256);
					y += itofix(1);
				}
				internal[0]++;
				rotationAngle = internal[0] / 2;
				break;
			case Pattern_1_5:
				if(!(G_waveTimer % 3))
				{
					if(G_waveTimer < 500)
						y += itofix(1);
					else
						x += (waveIndex % 6) > 2 ? 192 : -192;
				}
				break;
			case Pattern_1_6:
				angle = angleToPlayer(this, p);
				if(!(internal[0] % 25))
				{
					bArray->add(x, y, fixcos(angle), fixsin(angle), image_LUT_enemy_bullet_1_light, polarity, true);
				}
				internal[0]++;
				rotationAngle = ~angle + 64;
				break;
			// Chapter 1
			case Pattern_1_7:
				if(!(G_waveTimer % 4))
				{
					y += abs(fixcos(internal[0])) * 3;
					internal[0]++;
					if(internal[0] < 100)
						x += x > itofix(160) ? -128 : 128;
					else
						x += x > itofix(160) ? itofix(1) : itofix(-1);
				}
				break;
			case Pattern_1_8:
				if(!(G_waveTimer % 4))
				{
					angle = angleToPlayer(this, p);
					if(internal[0] < 35)
						y += itofix(2);
					else
						rotationAngle = ~angle + 64;
					if(internal[0] > 65 && internal[0] < 100) 
					{
						if(!(G_waveTimer % 16))
							bArray->add(x, y, fixcos(angle), fixsin(angle), image_LUT_enemy_bullet_1_light, polarity, true);
					}
					else if(internal[0] > 164)
					{
						x += x < itofix(160) ? itofix(-1) : itofix(1);
						y -= itofix(1);
					}
					internal[0]++;
				}
				break;
			case Pattern_1_9:
				x += waveIndex % 2 ? 192 : -192;
				y += 128;
				
				if(!(G_waveTimer % 8))
					bArray->add(x, y, 0, itofix(2), image_LUT_enemy_bullet_1_light, polarity, true);
				break;
			case Pattern_1_10:
				if(!(G_waveTimer % 4))
				{
					int temp = fixsin(internal[0]) * 80;
					x = (waveIndex % 2 ? -temp : temp) + itofix(160);
					if(abs(temp) > itofix(70))
					{
						if(!(G_waveTimer % 8))
							bArray->add(x, y, 0, itofix(2), image_LUT_enemy_bullet_1_light, polarity, true);
					}
					y += 192;
					internal[0]++;
				}
				break;
			case Pattern_1_11:
				y += itofix(1);
				break;
			case Pattern_1_12:
				if(!(G_waveTimer % 4))
				{
					if(internal[0] < 45)
						y += itofix(2);
					else if(internal[0] == 45)
					{
						internal[1] = waveIndex % 2 ? 0 : 128;
						internal[2] = x + (waveIndex % 2 ? itofix(-20) : itofix(20));
						internal[3] = y;
					}
					else if(internal[0] - 45 < 416)
					{
						x = fixcos(internal[1]) * 20 + internal[2];
						y = fixsin(internal[1]) * 20 + internal[3];
						internal[4] = (internal[0] - 45) % 64;
						if(internal[4] >= 32)
						{
							if(!(internal[1] % 256))
							{
								angle = angleToPlayer(this, p);
								bArray->add(x, y, fixcos(angle), fixsin(angle), image_LUT_enemy_bullet_0_light, polarity, true);
							}
							internal[1] += 4;
						}
					}
					else
						x += x < itofix(160) ? itofix(-2) : itofix(2);
					
					internal[0]++;
					rotationAngle = internal[0];
				}
				break;
			case Pattern_1_13:
				rotationAngle = 16 + fixcos(G_waveTimer) / 32;
				if(!internal[1])
				{
					internal[1] = 1;
					internal[0] = -1;
				}
				if(y < itofix(30))
				{
					y += 128;
				}
				else
				{
					if(internal[0] > 2 && internal[0] < 12)
					{
						if(!(G_waveTimer % 4))
						{
							for(int i = 0; i < 4; i++)
							{
								int k = i * 8 - 20 + 48;
								bArray->add(fixcos(k + (internal[0] - 3) * 8) * img[0] / 2 + x, fixsin(k + (internal[0] - 3) * 8) * img[1] / 2 + y,
											fixmul(fixcos(k), (internal[0] - 2) * 32 + 128),
											fixmul(fixsin(k), (internal[0] - 2) * 32 + 128), image_LUT_enemy_bullet_0_light, polarity, true);
							}
							internal[0]++;
						}
					}
					else if(internal[0] == 12)
					{
						internal[0] = 0;
					}
					else
					{
						if(!(G_waveTimer % 128))
						{
							if(internal[0] < 2 && internal[0] >= 0)
							{
								for(int i = 0; i < 5; i++)
								{
									int k = (i - 2) * 16 + 48;
									for(int j = 0; j < 3; j++)
									{
										
										bArray->add(fixcos(k) * img[0] / 2 + x - fixsin(k) * (j - 1) * 6, fixsin(k) * img[1] / 2 + y + fixcos(k) * (j - 1) * 6,
													fixcos(k), fixsin(k), image_LUT_enemy_bullet_2_light, polarity, true);
									}
								}
							}
							internal[0]++;
						}
					}
				}
				break;
			case Pattern_1_14:
				rotationAngle = -16 + fixcos(G_waveTimer) / 32;
				if(y < itofix(30))
				{
					y += 128;
				}
				else
				{
					if(internal[0] > 2 && internal[0] < 12)
					{
						if(!(G_waveTimer % 4))
						{
							for(int i = 0; i < 4; i++)
							{
								int k = i * 8 - 20 + 80;
								bArray->add(fixcos(k - (internal[0] - 3) * 8) * img[0] / 2 + x, fixsin(k - (internal[0] - 3) * 8) * img[1] / 2 + y,
											fixmul(fixcos(k), (internal[0] - 2) * 32 + 128),
											fixmul(fixsin(k), (internal[0] - 2) * 32 + 128), image_LUT_enemy_bullet_0_light, polarity, true);
							}
							internal[0]++;
						}
					}
					else if(internal[0] == 12)
					{
						internal[0] = 0;
					}
					else
					{
						if(!(G_waveTimer % 128))
						{
							if(internal[0] < 2)
							{
								for(int i = 0; i < 5; i++)
								{
									int k = (i - 2) * 16 + 80;
									for(int j = 0; j < 3; j++)
									{
										
										bArray->add(fixcos(k) * img[0] / 2 + x - fixsin(k) * (j - 1) * 6, fixsin(k) * img[1] / 2 + y + fixcos(k) * (j - 1) * 6,
													fixcos(k), fixsin(k), image_LUT_enemy_bullet_2_light, polarity, true);
									}
								}
							}
							internal[0]++;
						}
					}
				}
				break;
			case Pattern_1_15:
				x = fixcos(internal[0] + (waveIndex % 2 ? 128 : 0)) * 30;
				y = fixsin(internal[0] + (waveIndex % 2 ? 128 : 0)) * 30;
				if(!(G_waveTimer % 4))
				{
					if(!(internal[0] % 128))
					{
						angle = angleToPlayer(this, p);
						bArray->add(getx(), gety(), fixcos(angle) / 2, fixsin(angle) / 2, image_LUT_enemy_bullet_0_light, polarity, true);
					}
					internal[0]++;
					rotationAngle = internal[0];
				}
				break;
		}
		
		er.x = fixtoi(x);
		er.y = fixtoi(y);
		
		if(isJointed)
		{
			er.x += fixtoi(G_enemiesArray[jointedTo]->x + jointX);
			er.y += fixtoi(G_enemiesArray[jointedTo]->y + jointY);
		}
		
		// Have a relatively big threshold for off-screen animations
		if(er.x + img[0] / 2 < -80 || er.x - img[0] / 2 > 399 ||
			er.y + img[1] / 2 < -80 || er.y - img[1] / 2 > 319)
			deactivate();		
		else
		{
			// then the enemy image
			if(!G_skipFrame)
			{
				if(hasRotation)
				{
					drawSpriteRotated(img, &er, rotationAngle);
				}
				else
				{
					er.x -= img[0] / 2;
					er.y -= img[1] / 2;
					drawSprite(img, er.x, er.y);
				}
			}
		}
	}
	else
		diedThisFrame = false;
}

bool Enemy::isActive()
{
	return active;
}

void Enemy::activate(int _x, int _y, int _HP, int shipImgID, int callbackID, int _waveIndex, bool _polarity, bool _hasRotation)
{
	HP = _HP;
	x = _x;
	y = _y;
	
	img = image_entries[shipImgID];
	
	polarity = _polarity;
	hasRotation = _hasRotation;
	rotationAngle = 0;
	callback = callbackID;
	waveIndex = _waveIndex;
	for(int i = 0; i < 6; i++)
		internal[i] = 0;
	active = true;
}

void Enemy::deactivate()
{
	active = false;
}

void Enemy::damage(Player *_p, bool _pol, BulletArray *bArray)
{
	HP--;
	if(_pol != polarity)
		HP--;
	
	if(HP <= 0)
	{
		deactivate();
		diedThisFrame = true;
		
		if(_pol == polarity)
		{
			Fixed angle = angleToPlayer(this, _p);
			for(int i = 0; i < 16; i++)
				bArray->add(x, y, fixcos(angle + (rand() % 16) - 8) << 1, fixsin(angle + (rand() % 16) - 8) + (rand() % 256), image_LUT_enemy_bullet_0_light, polarity, true);
		}
	}
}

void Enemy::joint(int _n, Fixed _x, Fixed _y)
{
	isJointed = true;
	jointedTo = _n;
	jointX = _x;
	jointY = _y;
}

Fixed Enemy::getRotation()
{
	return rotationAngle;
}

void Enemy::setRotation(Fixed r)
{
	rotationAngle = r;
}

bool Enemy::getPolarity()
{
	return polarity;
}

int Enemy::getWaveIndex()
{
	return waveIndex;
}

int Enemy::getInternal(int w)
{
	return internal[w];
}

void Enemy::setInternal(int w, int v)
{
	internal[w] = v;
}

Fixed Enemy::getx()
{
	return isJointed ? x + G_enemiesArray[jointedTo]->getx() + jointX : x;
}

Fixed Enemy::gety()
{
	return isJointed ? y + G_enemiesArray[jointedTo]->gety() + jointY : y;
}