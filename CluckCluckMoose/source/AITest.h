#ifndef __AITEST_H__
#define __AITEST_H__

#include "CCMGameScene.h"
#include "SceneBuilder1.h"
#include "Moose.h"
#include "AI.h"

using namespace cugl;
using namespace std;

void testAll();
int playGames(std::shared_ptr<Moose> p1, std::shared_ptr<AI> ai1, string str1, std::shared_ptr<Moose> p2, std::shared_ptr<AI> ai2, string str2, int num_games);

#endif /* __AITEST_H__ */