#include "Noise.h"

engin::Noise::Noise(uint32_t seed)
{
    updateSeed(seed);
}

engin::Noise::~Noise()
{
}

float engin::Noise::noise1d(float args)
{
    unsigned int X1, X2;
    float x1, x2;

    setup(args, X1, X2, x1, x2);

    float sx = smootherstep(x1);

    float u = x1 * m_gradient1d[m_permu[X1]];
    float v = x2 * m_gradient1d[m_permu[X2]];

    return (lerp(sx, u, v) + 1.f) / 2.f;
}

float engin::Noise::noise2d(const float(&& args)[2])
{
    unsigned int X1, X2, Y1, Y2;
    float x1, x2, y1, y2;

    setup(args[0], X1, X2, x1, x2);
    setup(args[1], Y1, Y2, y1, y2);

    float sx = smootherstep(x1);
    float sy = smootherstep(y1);

    return 		(lerp(sy,
        lerp(sx, dot2d(m_gradient2d[m_permu[m_permu[X1] + Y1]], { x1,y1 }), dot2d(m_gradient2d[m_permu[m_permu[X2] + Y1]], { x2,y1 })),
        lerp(sx, dot2d(m_gradient2d[m_permu[m_permu[X1] + Y2]], { x1,y2 }), dot2d(m_gradient2d[m_permu[m_permu[X2] + Y2]], { x2,y2 }))
    ) + 1.f) / 2.f;
}

float engin::Noise::smootherstep(float x)
{
    return x * x * x * (x * (x * 6 - 15) + 10);
}

float engin::Noise::clerp(float alpha, float a, float b)
{
    alpha = (1 - (cos(alpha * (22.f / 7.f)))) * 0.5f;
    return (b - a) * alpha + a;
}

float engin::Noise::lerp(float alpha, float a, float b)
{
    return (b - a) * alpha + a;
}

float engin::Noise::dot3d(float args[3], float(&& args1)[3])
{
    return args[0] * args1[0] + args[1] * args1[1] + args[2] * args1[2];
}

float engin::Noise::dot2d(float args[2], float(&& args1)[2])
{
    return args[0] * args1[0] + args[1] * args1[1];
}

void engin::Noise::normalize2(float v[2])
{
    float s;

    s = sqrt(v[0] * v[0] + v[1] * v[1]);
    v[0] = v[0] / s;
    v[1] = v[1] / s;
}

void engin::Noise::normalize3(float v[3])
{
    float s;

    s = sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
    v[0] = v[0] / s;
    v[1] = v[1] / s;
    v[2] = v[2] / s;
}

float engin::Noise::inverseLerp(float val1, float val2, float orgVal)
{
    return (orgVal - val1) / (val2 - val1);
}

void engin::Noise::updateSeed(unsigned int seed)
{
    m_seed = seed;
    for (int i = 0, j = 0; i < 512; i++, j++)
    {
        m_permu[i] = j;
        if (i == 255) j = -1;

    }

    std::shuffle(std::begin(m_permu), std::end(m_permu), std::default_random_engine(seed));
    updateGradient();
}

void engin::Noise::updateGradient()
{
    srand(time(NULL));
    for (int i = 0; i < 256; i++)
    {
        m_gradient1d[i] = (float)((rand() & (BM + BM)) - BM) / BM;

        for (int j = 0; j < 2; j++)
            m_gradient2d[i][j] = (float)((rand() & (BM + BM)) - BM) / BM;
        normalize2(m_gradient2d[i]);

        for (int j = 0; j < 3; j++)
            m_gradient3d[i][j] = (float)((rand() & (BM + BM)) - BM) / BM;
        normalize3(m_gradient3d[i]);
    }
}

void engin::Noise::setup(const float& value, unsigned int& b0, unsigned int& b1, float& r0, float& r1) const
{
    b0 = int(value) & 0xff;
    b1 = (b0 + 1) & 0xff;
    r0 = value - int(value);
    r1 = r0 - 1.f;
}
