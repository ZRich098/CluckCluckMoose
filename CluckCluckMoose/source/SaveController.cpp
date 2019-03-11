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


bool SaveController::loadPlayerMoose(const std::shared_ptr<JsonValue>& json) {
	bool success = false;

	auto health = json->get(HEALTH_FIELD);
	success = health->isNumber();
	int h = health->asInt();

	auto handArray = json->get(HAND_FIELD);
	success = success && handArray->isArray();
	vector<int> hand = handArray->asIntArray();

	auto stackArray = json->get(STACK_FIELD);
	success = success && stackArray->isArray();
	vector<int> stack = stackArray->asIntArray();

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
	player->jsonInit(h, hand, stack, coop, discard, cost);

	return success;
}

bool SaveController::loadOpponentMoose(const std::shared_ptr<JsonValue>& json) {
	bool success = false; 

	auto health = json->get(HEALTH_FIELD);
	success = health->isNumber();
	int h = health->asInt();

	auto handArray = json->get(HAND_FIELD);
	success = success && handArray->isArray();
	vector<int> hand = handArray->asIntArray();

	auto stackArray = json->get(STACK_FIELD);
	success = success && stackArray->isArray();
	vector<int> stack = stackArray->asIntArray();

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
	opponent->jsonInit(h, hand, stack, coop, discard, cost);

	return success;
}