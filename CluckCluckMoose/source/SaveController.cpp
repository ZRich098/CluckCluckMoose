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

int SaveController::loadSaveGame(const std::shared_ptr<JsonValue>& json) {
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

	return l;
}

std::shared_ptr<Moose> SaveController::loadPlayerMoose(const std::shared_ptr<JsonValue>& json) { //called in onResume()
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

	/* auto discardArray = json->get(DISCARD_FIELD);
	success = success && discardArray->isArray();
	vector<int> discard = discardArray->asIntArray(); */

	auto costume = json->get(COSTUME_FIELD);
	success = success && costume->isString();
	string cost = costume->asString();

	player = Moose::alloc(5, 6);
	CULog("Hand size: %d, Order size: %d",hand.size(),playOrder.size());
	player->jsonInit(h, hand, playOrder, coop, cost);

	return player;
}

std::shared_ptr<Moose> SaveController::loadOpponentMoose(const std::shared_ptr<JsonValue>& json) { //called in onResume()
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

	/* auto discardArray = json->get(DISCARD_FIELD);
	success = success && discardArray->isArray();
	vector<int> discard = discardArray->asIntArray(); */

	auto costume = json->get(COSTUME_FIELD);
	success = success && costume->isString();
	string cost = costume->asString();

	opponent = Moose::alloc(5, 6);
	opponent->jsonInit(h, hand, playOrder, handPool, cost);

	return opponent;
}

AIType SaveController::loadAI(const std::shared_ptr<JsonValue>& json) {
	bool success = false;

	auto ai = json;
	success = ai->isString();
	string a = ai->asString();
	if (a == "Basic") {
		//CULog("loading Basic");
		return AIType::Basic;
	}
	else if (a == "Dumb") {
		//CULog("loading Dumb");
		return AIType::Dumb;
	}
	else if (a == "Expert") {
		//CULog("loading Expert");
		return AIType::Expert;
	}
	else if (a == "Intro") {
		//CULog("loading Intro");
		return AIType::Intro;
	}
	else if (a == "Smart") {
		//CULog("loading Smart");
		return AIType::Smart;
	}
	else if (a == "Starter") {
		//CULog("loading Starter");
		return AIType::Starter;
	}
	else {
		CULog("AI Type not found");
		return AIType::Basic;
	}
}

int SaveController::loadLevelTag(const std::shared_ptr<JsonValue>& json) { //called in onResume()
	bool success = false;

	auto tag = json;
	success = tag->isNumber();
	int t = tag->asInt();

	return t;
}

void SaveController::saveGame(int level) { //called in onSuspend()
	//create JSONValue
	std::shared_ptr<JsonValue> saveGame = JsonValue::allocObject();

	//Add Level (last level completed)
	saveGame->appendValue("Level", (double)level);

	//Add Volume
	saveGame->appendValue("Volume", 0.25); //@TODO Get from game state

	//Add SkinName (current skin of player)
	saveGame->appendValue("SkinName", "default"); //@TODO Get from game state

	//Add Purchases (cosmetics bought by player)
	saveGame->appendArray("Purchases"); //@TODO Get from game state

	//save to file
	std::shared_ptr<JsonWriter> writer = JsonWriter::alloc("saveGame.json");
	if (writer == nullptr) {
		CULogError("writer not initialized");
	}
	else {
		writer->writeJson(saveGame);
		writer->close();
	}
}

void SaveController::saveLevel(std::shared_ptr<Moose> player, std::shared_ptr<Moose> opp, std::shared_ptr<AI> ai, int level) { //called in onSuspend()
	//create JSONValue
	std::shared_ptr<JsonValue> saveFile = JsonValue::allocObject();

	//Add PlayerMoose (Health, Hand, PlayOrder, Coop, Discard, Skin)
	saveFile->appendObject("PlayerMoose");

	saveFile->get("PlayerMoose")->appendValue("Health", (double)player->getHealth());

	saveFile->get("PlayerMoose")->appendArray("Hand"); //Make player Hand
	for (int i = 0; i < player->getHand().size(); i++) {
		saveFile->get("PlayerMoose")->get("Hand")->appendValue((double)specialToInt(player->getHandAt(i).getSpecial()));
	}

	saveFile->get("PlayerMoose")->appendArray("PlayOrder"); //Make player PlayOrder
	for (int i = 0; i < player->getPlayOrder().size(); i++) {
		saveFile->get("PlayerMoose")->get("PlayOrder")->appendValue((double)specialToInt(player->getPlayOrderAt(i).getSpecial()));
	}

	saveFile->get("PlayerMoose")->appendArray("Coop"); //Make player Coop
	for (int i = 0; i < player->getDeck().getSize(); i++) {
		saveFile->get("PlayerMoose")->get("Coop")->appendValue((double)specialToInt(player->getDeckAt(i).getSpecial()));
	}

	saveFile->get("PlayerMoose")->appendValue("Costume", "zoose");

	//Add OppMoose (Health, Hand, PlayOrder, Coop, Discard, Skin)
	saveFile->appendObject("OpponentMoose");

	saveFile->get("OpponentMoose")->appendValue("Health", (double)opp->getHealth());

	saveFile->get("OpponentMoose")->appendArray("Hand"); //Make opponent Hand
	for (int i = 0; i < opp->getHand().size(); i++) {
		saveFile->get("OpponentMoose")->get("Hand")->appendValue((double)specialToInt(opp->getHandAt(i).getSpecial()));
	}

	saveFile->get("OpponentMoose")->appendArray("PlayOrder"); //Make opponent PlayOrder
	for (int i = 0; i < opp->getPlayOrder().size(); i++) {
		saveFile->get("OpponentMoose")->get("PlayOrder")->appendValue((double)specialToInt(opp->getPlayOrderAt(i).getSpecial()));
	}

	saveFile->get("OpponentMoose")->appendArray("Coop"); //Make opponent Coop
	for (int i = 0; i < opp->getHandPool().size(); i++) {
		saveFile->get("OpponentMoose")->get("Coop")->appendArray();
		saveFile->get("OpponentMoose")->get("Coop")->get(saveFile->get("OpponentMoose")->get("Coop")->size()-1)->appendValue((double)specialToInt(opp->getDeckAt(i).getSpecial()));
	}

	saveFile->get("OpponentMoose")->appendValue("Costume", "zoose");

	//Add AI
	AIType a = ai->getType();
	if (a == AIType::Basic) {
		saveFile->appendValue("AI", "Basic");
	}
	else if (a == AIType::Dumb) {
		saveFile->appendValue("AI", "Dumb");
	}
	else if (a == AIType::Expert) {
		saveFile->appendValue("AI", "Expert");
	}
	else if (a == AIType::Intro) {
		saveFile->appendValue("AI", "Intro");
	}
	else if (a == AIType::Smart) {
		saveFile->appendValue("AI", "Smart");
	}
	else if (a == AIType::Starter) {
		saveFile->appendValue("AI", "Starter");
	}
	else {
		CULog("AI Type not found");
		saveFile->appendValue("AI", "Basic");
	}

	//Add Tag
	saveFile->appendValue("Level", (double)level);

	//save to file
	std::shared_ptr<JsonWriter> writer = JsonWriter::alloc("saveLevel.json");
	if (writer == nullptr) {
		CULogError("writer not initialized");
	}
	else {
		writer->writeJson(saveFile);
		writer->close();
	}
}