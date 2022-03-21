#include "RandomEngine.h"
RandomEngineManager& RandomEngineManager::getInstance()
{
	static RandomEngineManager engine;
	return engine;
}

RandomEngine& RandomEngineManager::getEngine(int engine_id)
{
	return engines[engine_id];
}

void RandomEngineManager::createEngines(int num)
{
	engines.resize(num);
}

RandomEngineManager::RandomEngineManager():engines(1)
{}

float RandomEngine::sampleUniform(float start, float end)
{
	return std::uniform_real_distribution<float>(start, end)(engine);
}

RandomEngine::RandomEngine()
{
	std::random_device device;
	engine.seed(device());
}
