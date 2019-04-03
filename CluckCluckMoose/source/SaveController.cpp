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

	auto coopArray = json->get(COOP_FIELD);
	success = success && coopArray->isArray();
	vector<int> coop = coopArray->asIntArray();

	auto discardArray = json->get(DISCARD_FIELD);
	success = success && discardArray->isArray();
	vector<int> discard = discardArray->asIntArray();

	auto costume = json->get(COSTUME_FIELD);
	success = success && costume->isString();
	string cost = costume->asString();

	opponent = Moose::alloc(3, 3);
	opponent->jsonInit(h, hand, playOrder, coop, discard, cost);

	return success;
}

bool loadLevelTag(const std::shared_ptr<JsonValue>& json) { //called in onResume()
	bool success = false;

	auto tag = json->get(TAG_FIELD);
	success = tag->isNumber();
	int t = tag->asInt();

	return success;
}

void SaveController::saveGame() { //called in onSuspend()
	//create JSONValue
	JsonValue saveGame;
	saveGame.initObject();
	std::shared_ptr<JsonValue> savePtr(JsonValue saveValue);

	//Add Level (last level completed)
	JsonValue level;
	level.init(0.0); //@TODO Get from game state
	saveGame.appendChild("Level", savePtr(level));

	//Add Volume
	JsonValue vol;
	vol.init(0.0); //@TODO Get from game state
	saveGame.appendChild("Volume", savePtr(vol));

	//Add SkinName (current skin of player)
	JsonValue skin;
	skin.init("default"); //@TODO Get from game state
	saveGame.appendChild("SkinName", savePtr(skin));

	//Add Purchases (cosmetics bought by player)
	JsonValue purchases;
	purchases.initArray();
	//@TODO Get from game state
	saveGame.appendChild("Purchases", savePtr(purchases));

	//save to file
	JsonWriter file;
	std::shared_ptr<JsonWriter> writer = file.alloc("saveGame.json");
	writer->writeJson(&saveGame);
	writer->close();
}

void SaveController::saveLevel(Moose player, Moose opp) { //called in onSuspend()
	//create JSONValue
	JsonValue saveFile;
	saveFile.initObject();
	std::shared_ptr<JsonValue> savePtr(JsonValue saveValue);

	//Add PlayerMoose (Health, Hand, PlayOrder, Coop, Discard, Skin)
	JsonValue playerMoose;
	playerMoose.initObject();

	JsonValue playerHealth;
	playerHealth.init((double)player.getHealth());
	playerMoose.appendChild("Health", savePtr(playerHealth));

	JsonValue playerHand;
	playerHand.initArray();
	JsonValue cardInHand; //Make player Hand
	for (int i = 0; i < player.getHand().size(); i++) {
		cardInHand.init(player.getHandAt(i).toString());
		playerHand.appendChild(savePtr(cardInHand));
	}
	playerMoose.appendChild("Hand", savePtr(playerHand));

	JsonValue playerOrder;
	playerOrder.initArray();
	JsonValue cardInOrder; //Make player PlayOrder
	for (int i = 0; i < player.getPlayOrder().size(); i++) {
		cardInOrder.init(player.getPlayOrderAt(i).toString());
		playerOrder.appendChild(savePtr(cardInOrder));
	}
	playerMoose.appendChild("PlayOrder", savePtr(playerOrder));

	JsonValue playerCoop;
	playerCoop.initArray();
	JsonValue cardInCoop; //Make player Coop
	for (int i = 0; i < player.getDeck().getSize(); i++) {
		cardInCoop.init(player.getDeckAt(i).toString());
		playerCoop.appendChild(savePtr(cardInCoop));
	}
	playerMoose.appendChild("Coop", savePtr(playerCoop));

	JsonValue playerDiscard;
	playerDiscard.initArray();
	JsonValue cardInDiscard; //Make player Discard
	for (int i = 0; i < player.getDiscard().size(); i++) {
		cardInDiscard.init(player.getDiscardAt(i).toString());
		playerDiscard.appendChild(savePtr(cardInDiscard));
	}
	playerMoose.appendChild("Discard", savePtr(playerDiscard));

	JsonValue playerSkin;
	playerSkin.init("zoose"); //GET FROM GAME STATE
	playerMoose.appendChild("Costume", savePtr(playerSkin));

	saveFile.appendChild("PlayerMoose", savePtr(playerMoose));

	//Add OppMoose (Health, Hand, PlayOrder, Coop, Discard, Skin)
	JsonValue oppMoose;
	oppMoose.initObject();

	JsonValue oppHealth;
	oppHealth.init((double)opp.getHealth());
	oppMoose.appendChild("Health", savePtr(oppHealth));

	JsonValue oppHand;
	oppHand.initArray();
	JsonValue cardInHand; //Make opp Hand
	for (int i = 0; i < opp.getHand().size(); i++) {
		cardInHand.init(opp.getHandAt(i).toString());
		oppHand.appendChild(savePtr(cardInHand));
	}
	oppMoose.appendChild("Hand", savePtr(oppHand));

	JsonValue oppOrder;
	oppOrder.initArray();
	JsonValue cardInOrder; //Make opp PlayOrder
	for (int i = 0; i < opp.getPlayOrder.size(); i++) {
		cardInOrder.init(opp.getPlayOrderAt(i).toString());
		oppOrder.appendChild(savePtr(cardInOrder));
	}
	oppMoose.appendChild("PlayOrder", savePtr(oppOrder));

	JsonValue oppCoop;
	oppCoop.initArray();
	JsonValue cardInCoop; //Make opp Coop
	for (int i = 0; i < opp.getDeck().getSize(); i++) {
		cardInCoop.init(opp.getDeckAt(i).toString());
		oppCoop.appendChild(savePtr(cardInCoop));
	}
	oppMoose.appendChild("Coop", savePtr(oppCoop));

	JsonValue oppDiscard;
	oppDiscard.initArray();
	JsonValue cardInDiscard; //Make opp Discard
	for (int i = 0; i < opp.getDiscard.size(); i++) {
		cardInDiscard.init(opp.getDiscardAt(i).toString());
		oppDiscard.appendChild(savePtr(cardInDiscard));
	}
	oppMoose.appendChild("Discard", savePtr(oppDiscard));

	JsonValue oppSkin;
	oppSkin.init("royal_moose"); //GET FROM GAME STATE
	oppMoose.appendChild("Costume", savePtr(oppSkin));

	saveFile.appendChild("OpponentMoose", savePtr(oppMoose));

	//Add Tag
	JsonValue levelTag;
	levelTag.init(0.0); //GET FROM GAME STATE
	saveFile.appendChild(savePtr(levelTag));

	//save to file
	JsonWriter file;
	std::shared_ptr<JsonWriter> writer = file.alloc("saveLevel.json");
	writer->writeJson(&saveFile);
	writer->close();
}