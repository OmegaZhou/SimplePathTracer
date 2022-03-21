#pragma once
#include <random>
#include <vector>
class RandomEngine
{
public:
	float sampleUniform(float start = 0, float end = 1);
	RandomEngine();
private:
	std::default_random_engine engine;
};
class RandomEngineManager
{
public:
	static RandomEngineManager& getInstance();
	
	RandomEngine& getEngine(int engine_id);
	void createEngines(int num);
private:
	RandomEngineManager();
	std::vector<RandomEngine> engines;
};
