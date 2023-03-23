#ifndef NOISE_H
#define NOISE_H
#pragma warning(disable : 4244)
#include <random>
#include <time.h>
#include <math.h>
#define BM 0xff //255
namespace engin
{
	class Noise
	{
	private:
		unsigned int m_permu[512];
		float m_gradient1d[256];
		float m_gradient2d[256][2];
		float m_gradient3d[256][3];
		unsigned int m_seed;

	public:
		Noise(uint32_t seed);
		~Noise();

		float noise1d(float args);
		float noise2d(const float(&& args)[2]);

		static float smootherstep(float x);
		static float clerp(float alpha, float a, float b);
		static float lerp(float alpha, float a, float b);
		static float dot3d(float args[3], float(&& args1)[3]);
		static float dot2d(float args[2], float(&& args1)[2]);
		static void normalize2(float v[2]);
		static void normalize3(float v[3]);
		static float inverseLerp(float val1, float val2, float orgVal);

	private:
		void updateSeed(unsigned int seed = 1);
		void updateGradient();
		void setup(const float& value, unsigned int& b0, unsigned int& b1, float& r0, float& r1) const;

	};
}


#endif // !NOISE_H


