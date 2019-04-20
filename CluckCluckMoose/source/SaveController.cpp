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

void SaveController::saveLevel(std::shared_ptr<Moose> playerPtr, std::shared_ptr<Moose> oppPtr, std::shared_ptr<AI> ai, int level) { //called in onSuspend()
	Moose player = *playerPtr;
	Moose opp = *oppPtr;

	//create JSONValue
	std::shared_ptr<JsonValue> saveFile = JsonValue::allocObject();
	//saveFile.allocObject();

	//Add PlayerMoose (Health, Hand, PlayOrder, Coop, Discard, Skin)
	std::shared_ptr<JsonValue> playerMoose = JsonValue::allocObject();
	//playerMoose.allocObject();

	//JsonValue playerHealth;
	//playerHealth.alloc((double)player.getHealth());
	playerMoose->appendValue("Health", (double)player.getHealth());

	std::shared_ptr<JsonValue> playerHand = JsonValue::allocArray();
	//playerHand.allocArray();
	//JsonValue cardInHand; //Make player Hand
	for (int i = 0; i < player.getHand().size(); i++) {
		//cardInHand.init((float)specialToInt(player.getHandAt(i).getSpecial()));
		playerHand->appendValue((float)specialToInt(player.getHandAt(i).getSpecial()));
	}
	playerMoose->appendChild("Hand", std::make_shared<JsonValue>(playerHand));

	/*JsonValue playerOrder;
	playerOrder.allocArray();
	//JsonValue cardInOrder; //Make player PlayOrder
	for (int i = 0; i < player.getPlayOrder().size(); i++) {
		//cardInOrder.init((float)specialToInt(player.getPlayOrderAt(i).getSpecial()));
		playerOrder.appendValue((float)specialToInt(player.getPlayOrderAt(i).getSpecial()));
	}
	playerMoose->appendChild("PlayOrder", std::make_shared<JsonValue>(playerOrder));

	JsonValue playerCoop;
	playerCoop.allocArray();
	//JsonValue cardInCoop; //Make player Coop
	for (int i = 0; i < player.getDeck().getSize(); i++) {
		//cardInCoop.init((float)specialToInt(player.getDeckAt(i).getSpecial()));
		playerCoop.appendValue((float)specialToInt(player.getDeckAt(i).getSpecial()));
	}
	playerMoose->appendChild("Coop", std::make_shared<JsonValue>(playerCoop)); */

	/* JsonValue playerDiscard;
	playerDiscard.initArray();
	JsonValue cardInDiscard; //Make player Discard
	for (int i = 0; i < player.getDiscard().size(); i++) {
		cardInDiscard.init((float)specialToInt(player.getDiscardAt(i).getSpecial()));
		playerDiscard.appendChild(std::make_shared<JsonValue>(cardInDiscard));
	}
	playerMoose.appendChild("Discard", std::make_shared<JsonValue>(playerDiscard)); */

	/*JsonValue playerSkin;
	playerSkin.alloc("zoose"); //GET FROM GAME STATE
	playerMoose->appendChild("Costume", std::make_shared<JsonValue>(playerSkin));

	saveFile->appendChild("PlayerMoose", std::make_shared<JsonValue>(playerMoose));

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
		//cardInHand.init((float)specialToInt(opp.getHandAt(i).getSpecial()));
		oppHand.appendValue((float)specialToInt(opp.getHandAt(i).getSpecial()));
	}
	oppMoose.appendChild("Hand", std::make_shared<JsonValue>(oppHand));

	JsonValue oppOrder;
	oppOrder.initArray();
	//Make opp PlayOrder
	for (int i = 0; i < opp.getPlayOrder().size(); i++) {
		//cardInOrder.init((float)specialToInt(opp.getPlayOrderAt(i).getSpecial()));
		oppOrder.appendValue((float)specialToInt(opp.getPlayOrderAt(i).getSpecial()));
	}
	oppMoose.appendChild("PlayOrder", std::make_shared<JsonValue>(oppOrder));

	JsonValue oppCoop;
	oppCoop.initArray();
	//Make opp Coop
	for (int h = 0; h < opp.getHandPool().size(); h++) {
		JsonValue oppCoopHand;
		for (int i = 0; i < opp.getHandPool().at(h).size(); i++) {
			//cardInCoop.init((float)specialToInt(opp.getDeckAt(i).getSpecial()));
			oppCoopHand.appendValue((float)specialToInt(opp.getDeckAt(i).getSpecial()));
		}
		oppCoop.appendChild(std::make_shared<JsonValue>(oppCoopHand));
	}
	oppMoose.appendChild("Coop", std::make_shared<JsonValue>(oppCoop)); */

	/* JsonValue oppDiscard;
	oppDiscard.initArray();
	//Make opp Discard
	for (int i = 0; i < opp.getDiscard().size(); i++) {
		cardInDiscard.init((float)specialToInt(opp.getDiscardAt(i).getSpecial()));
		oppDiscard.appendChild(std::make_shared<JsonValue>(cardInDiscard));
	}
	oppMoose.appendChild("Discard", std::make_shared<JsonValue>(oppDiscard)); */

	/*JsonValue oppSkin;
	oppSkin.init("royal_moose"); //GET FROM GAME STATE
	oppMoose.appendChild("Costume", std::make_shared<JsonValue>(oppSkin));

	saveFile->appendChild("OpponentMoose", std::make_shared<JsonValue>(oppMoose));

	//Add AI
	JsonValue aiType;
	AIType a = ai->getType();
	if (a == AIType::Basic) {
		aiType.init("Basic");
	}
	else if (a == AIType::Dumb) {
		aiType.init("Dumb");
	}
	else if (a == AIType::Expert) {
		aiType.init("Expert");
	}
	else if (a == AIType::Intro) {
		aiType.init("Intro");
	}
	else if (a == AIType::Smart) {
		aiType.init("Smart");
	}
	else if (a == AIType::Starter) {
		aiType.init("Starter");
	}
	else {
		CULog("AI Type not found");
		aiType.init("Basic");
	}
	saveFile->appendChild("AI", std::make_shared<JsonValue>(aiType));

	//Add Tag
	JsonValue levelTag;
	levelTag.init((float)level);
	saveFile->appendChild("Level", std::make_shared<JsonValue>(levelTag));

	//save to file
	JsonWriter file;
	std::shared_ptr<JsonWriter> writer = file.alloc("saveLevel.json");
	writer->writeJson(saveFile);
	writer->close();*/
}