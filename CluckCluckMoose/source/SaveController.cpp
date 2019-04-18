#include <cugl/assets/CUJsonLoader.h>
#include "SaveController.h"

;

SaveController::SaveController() : Asset(),
player(nullptr),
opponent(nullptr)
{
}

SaveController::~SaveController() {
	if (player != nullptr) player = nullptr;
	if (opponent != nullptr) opponent = nullptr;
}

void SaveController::init() {
	player = nullptr;
	opponent = nullptr;
}

bool SaveController::loadSaveGame(const std::shared_ptr<JsonValue>& json) {
	bool success = false;

	auto level = json->get(LEVEL_FIELD);
	success = level->isNumber();
	int l = level->asInt();

	auto volume = json->get(VOLUME_FIELD);
	success = success && volume->isNumber();
	int vol = volume->asDouble();

	auto skin = json->get(SKIN_FIELD);
	success = success && skin->isString();
	string s = skin->asString();

	auto purchases = json->get(PURCHASES_FIELD);
	success = success && purchases->isArray();
	vector<string> p = purchases->asStringArray();

	return success;
}

bool SaveController::loadPlayerMoose(const std::shared_ptr<JsonValue>& json) { //called in onResume()
	bool success = false;

	auto health = json->get(HEALTH_FIELD);
	success = health->isNumber();
	int h = health->asInt();

	auto handArray = json->get(HAND_FIELD);
	success = success && handArray->isArray();
	vector<int> hand = handArray->asIntArray();

	auto playOrderArray = json->get(PLAYORDER_FIELD);
	success = success && playOrderArray->isArray();
	vector<int> playOrder = playOrderArray->asIntArray();

	auto coopArray = json->get(COOP_FIELD);
	success = success && coopArray->isArray();
	vector<int> coop = coopArray->asIntArray();

	auto discardArray = json->get(DISCARD_FIELD);
	success = success && discardArray->isArray();
	vector<int> discard = discardArray->asIntArray();

	auto costume = json->get(COSTUME_FIELD);
	success = success && costume->isString();
	string cost = costume->asString();

	player = Moose::alloc(3, 3);
	player->jsonInit(h, hand, playOrder, coop, discard, cost);

	return success;
}

bool SaveController::loadOpponentMoose(const std::shared_ptr<JsonValue>& json) { //called in onResume()
	bool success = false; 

	auto health = json->get(HEALTH_FIELD);
	success = health->isNumber();
	int h = health->asInt();

	auto handArray = json->get(HAND_FIELD);
	success = success && handArray->isArray();
	vector<int> hand = handArray->asIntArray();

	auto playOrderArray = json->get(PLAYORDER_FIELD);
	success = success && playOrderArray->isArray();
	vector<int> playOrder = playOrderArray->asIntArray();

	auto handPoolArray = json->get(COOP_FIELD);
	success = success && handPoolArray->isArray();
	vector<vector<int>> handPool;
	for (int i = 0; i < handPoolArray->size(); i++) {
		handPool.push_back(handPoolArray->get(i)->asIntArray());
	}

	auto discardArray = json->get(DISCARD_FIELD);
	success = success && discardArray->isArray();
	vector<int> discard = discardArray->asIntArray();

	auto costume = json->get(COSTUME_FIELD);
	success = success && costume->isString();
	string cost = costume->asString();

	opponent = Moose::alloc(3, 3);
	opponent->jsonInit(h, hand, playOrder, handPool, discard, cost);

	return success;
}

bool SaveController::loadLevelTag(const std::shared_ptr<JsonValue>& json) { //called in onResume()
	bool success = false;

	auto tag = json->get(TAG_FIELD);
	success = tag->isNumber();
	int t = tag->asInt();

	return success;
}

void SaveController::saveGame(int level) { //called in onSuspend()
	//create JSONValue
	JsonValue saveGame;
	saveGame.initObject();

	//Add Level (last level completed)
	JsonValue lastLevel;
	lastLevel.init((float)level);
	saveGame.appendChild("Level", std::make_shared<JsonValue>(lastLevel));

	//Add Volume
	JsonValue vol;
	vol.init(0.0); //@TODO Get from game state
	saveGame.appendChild("Volume", std::make_shared<JsonValue>(vol));

	//Add SkinName (current skin of player)
	JsonValue skin;
	skin.init("default"); //@TODO Get from game state
	saveGame.appendChild("SkinName", std::make_shared<JsonValue>(skin));

	//Add Purchases (cosmetics bought by player)
	JsonValue purchases;
	purchases.initArray();
	//@TODO Get from game state
	saveGame.appendChild("Purchases", std::make_shared<JsonValue>(purchases));

	//save to file
	JsonWriter file;
	std::shared_ptr<JsonWriter> writer = file.alloc("saveGame.json");
	writer->writeJson(&saveGame);
	writer->close();
}

void SaveController::saveLevel(std::shared_ptr<Moose> playerPtr, std::shared_ptr<Moose> oppPtr, int level) { //called in onSuspend()
	Moose player = *playerPtr;
	Moose opp = *oppPtr;

	//create JSONValue
	JsonValue saveFile;
	saveFile.initObject();

	//Add PlayerMoose (Health, Hand, PlayOrder, Coop, Discard, Skin)
	JsonValue playerMoose;
	playerMoose.initObject();

	JsonValue playerHealth;
	playerHealth.init((double)player.getHealth());
	playerMoose.appendChild("Health", std::make_shared<JsonValue>(playerHealth));

	JsonValue playerHand;
	playerHand.initArray();
	JsonValue cardInHand; //Make player Hand
	for (int i = 0; i < player.getHand().size(); i++) {
		cardInHand.init((float)specialToInt(player.getHandAt(i).getSpecial()));
		playerHand.appendChild(std::make_shared<JsonValue>(cardInHand));
	}
	playerMoose.appendChild("Hand", std::make_shared<JsonValue>(playerHand));

	JsonValue playerOrder;
	playerOrder.initArray();
	JsonValue cardInOrder; //Make player PlayOrder
	for (int i = 0; i < player.getPlayOrder().size(); i++) {
		cardInOrder.init((float)specialToInt(player.getPlayOrderAt(i).getSpecial()));
		playerOrder.appendChild(std::make_shared<JsonValue>(cardInOrder));
	}
	playerMoose.appendChild("PlayOrder", std::make_shared<JsonValue>(playerOrder));

	JsonValue playerCoop;
	playerCoop.initArray();
	JsonValue cardInCoop; //Make player Coop
	for (int i = 0; i < player.getDeck().getSize(); i++) {
		cardInCoop.init((float)specialToInt(player.getDeckAt(i).getSpecial()));
		playerCoop.appendChild(std::make_shared<JsonValue>(cardInCoop));
	}
	playerMoose.appendChild("Coop", std::make_shared<JsonValue>(playerCoop));

	JsonValue playerDiscard;
	playerDiscard.initArray();
	JsonValue cardInDiscard; //Make player Discard
	for (int i = 0; i < player.getDiscard().size(); i++) {
		cardInDiscard.init((float)specialToInt(player.getDiscardAt(i).getSpecial()));
		playerDiscard.appendChild(std::make_shared<JsonValue>(cardInDiscard));
	}
	playerMoose.appendChild("Discard", std::make_shared<JsonValue>(playerDiscard));

	JsonValue playerSkin;
	playerSkin.init("zoose"); //GET FROM GAME STATE
	playerMoose.appendChild("Costume", std::make_shared<JsonValue>(playerSkin));

	saveFile.appendChild("PlayerMoose", std::make_shared<JsonValue>(playerMoose));

	//Add OppMoose (Health, Hand, PlayOrder, Coop, Discard, Skin)
	JsonValue oppMoose;
	oppMoose.initObject();

	JsonValue oppHealth;
	oppHealth.init((double)opp.getHealth());
	oppMoose.appendChild("Health", std::make_shared<JsonValue>(oppHealth));

	JsonValue oppHand;
	oppHand.initArray();
	//Make opp Hand
	for (int i = 0; i < opp.getHand().size(); i++) {
		cardInHand.init((float)specialToInt(opp.getHandAt(i).getSpecial()));
		oppHand.appendChild(std::make_shared<JsonValue>(cardInHand));
	}
	oppMoose.appendChild("Hand", std::make_shared<JsonValue>(oppHand));

	JsonValue oppOrder;
	oppOrder.initArray();
	//Make opp PlayOrder
	for (int i = 0; i < opp.getPlayOrder().size(); i++) {
		cardInOrder.init((float)specialToInt(opp.getPlayOrderAt(i).getSpecial()));
		oppOrder.appendChild(std::make_shared<JsonValue>(cardInOrder));
	}
	oppMoose.appendChild("PlayOrder", std::make_shared<JsonValue>(oppOrder));

	JsonValue oppCoop;
	oppCoop.initArray();
	//Make opp Coop
	for (int h = 0; h < opp.getHandPool().size(); h++) {
		JsonValue oppCoopHand;
		for (int i = 0; i < opp.getHandPool().at(h).size(); i++) {
			cardInCoop.init((float)specialToInt(opp.getDeckAt(i).getSpecial()));
			oppCoopHand.appendChild(std::make_shared<JsonValue>(cardInCoop));
		}
		oppCoop.appendChild(std::make_shared<JsonValue>(oppCoopHand));
	}
	oppMoose.appendChild("Coop", std::make_shared<JsonValue>(oppCoop));

	JsonValue oppDiscard;
	oppDiscard.initArray();
	//Make opp Discard
	for (int i = 0; i < opp.getDiscard().size(); i++) {
		cardInDiscard.init((float)specialToInt(opp.getDiscardAt(i).getSpecial()));
		oppDiscard.appendChild(std::make_shared<JsonValue>(cardInDiscard));
	}
	oppMoose.appendChild("Discard", std::make_shared<JsonValue>(oppDiscard));

	JsonValue oppSkin;
	oppSkin.init("royal_moose"); //GET FROM GAME STATE
	oppMoose.appendChild("Costume", std::make_shared<JsonValue>(oppSkin));

	saveFile.appendChild("OpponentMoose", std::make_shared<JsonValue>(oppMoose));

	//Add Tag
	JsonValue levelTag;
	levelTag.init((float)level);
	saveFile.appendChild(std::make_shared<JsonValue>(levelTag));

	//save to file
	JsonWriter file;
	std::shared_ptr<JsonWriter> writer = file.alloc("saveLevel.json");
	writer->writeJson(&saveFile);
	writer->close();
}