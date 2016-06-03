#pragma once

#include "mtwist.h"

namespace internal
{
    template <typename T_GENERATOR, typename T>
    class RandomGenerator
    {
    public:
        RandomGenerator(T seed) { static_assert(false, "Invalid template parameters"); }
        
        RandomGenerator(const RandomGenerator& other)
            : m_generator(other.m_generator), m_seed(other.m_seed), m_steps(other.m_steps) {}

        inline void set_seed(T seed, T steps = 0) {}
        inline T get_seed() { return m_seed; }
        inline T get_steps() { return m_steps; }

        inline T get() {}
        inline T get(T n) {}
        inline T get(T min, T max) {}

        inline float get_float() {}
        inline float get_float(float n) {}
        inline float get_float(float min, float max) {}

    private:
        T_GENERATOR m_generator;
        T m_seed;
        T m_steps;
    };

    // 32 bit
    template <>
    RandomGenerator<MTGen32, int32_t>::RandomGenerator(int32_t seed)
    {
        mt_32_seed(&m_generator, seed);
        m_seed = seed;
        m_steps = 0;
    }

    template <>
    inline void RandomGenerator<MTGen32, int32_t>::set_seed(int32_t seed, int32_t steps)
    {
        mt_32_seed(&m_generator, seed);
        m_seed = seed;
        m_steps = steps;
        for (int32_t i = 0; i < steps; ++i) {
            _mt_32_next(&m_generator);
        }
    }

    template <>
    inline int32_t RandomGenerator<MTGen32, int32_t>::get()
    {
        return _mt_32_next(&m_generator);
    }

    template <>
    inline int32_t RandomGenerator<MTGen32, int32_t>::get(int32_t n)
    {
        return mt_32_next(&m_generator, n);
    }

    template <>
    inline int32_t RandomGenerator<MTGen32, int32_t>::get(int32_t min, int32_t max)
    {
        return mt_32_range(&m_generator, min, max);
    }

    template <>
    inline float RandomGenerator<MTGen32, int32_t>::get_float()
    {
        return mt_32_nextf(&m_generator);
    }

    template <>
    inline float RandomGenerator<MTGen32, int32_t>::get_float(float n)
    {
        return mt_32_rangef(&m_generator, 0.f, n);
    }

    template <>
    inline float RandomGenerator<MTGen32, int32_t>::get_float(float min, float max)
    {
        return mt_32_rangef(&m_generator, min, max);
    }


    // 64 bit
    template <>
    RandomGenerator<MTGen64, int64_t>::RandomGenerator(int64_t seed)
    {
        mt_64_seed(&m_generator, seed);
        m_seed = seed;
        m_steps = 0;
    }

    template <>
    inline void RandomGenerator<MTGen64, int64_t>::set_seed(int64_t seed, int64_t steps)
    {
        mt_64_seed(&m_generator, seed);
        m_seed = seed;
        m_steps = steps;
        for (int64_t i = 0; i < steps; ++i) {
            _mt_64_next(&m_generator);
        }
    }

    template <>
    inline int64_t RandomGenerator<MTGen64, int64_t>::get()
    {
        return _mt_64_next(&m_generator);
    }

    template <>
    inline int64_t RandomGenerator<MTGen64, int64_t>::get(int64_t n)
    {
        return mt_64_next(&m_generator, n);
    }

    template <>
    inline int64_t RandomGenerator<MTGen64, int64_t>::get(int64_t min, int64_t max)
    {
        return mt_64_range(&m_generator, min, max);
    }

    template <>
    inline float RandomGenerator<MTGen64, int64_t>::get_float()
    {
        return mt_64_nextf(&m_generator);
    }

    template <>
    inline float RandomGenerator<MTGen64, int64_t>::get_float(float n)
    {
        return mt_64_rangef(&m_generator, 0.f, n);
    }

    template <>
    inline float RandomGenerator<MTGen64, int64_t>::get_float(float min, float max)
    {
        return mt_64_rangef(&m_generator, min, max);
    }
}

template <int N>
class MersenneTwister
{
    static_assert(N == 32 || N == 64, "Invalid precision for RandomGenerator");
};

template <>
class MersenneTwister<32> : public internal::RandomGenerator<MTGen32, int32_t>
{
public:
    inline MersenneTwister(int32_t seed) : internal::RandomGenerator<MTGen32, int32_t>(seed) {}
};

template <>
class MersenneTwister<64> : public internal::RandomGenerator<MTGen64, int64_t>
{
public:
    inline MersenneTwister(int64_t seed) : internal::RandomGenerator<MTGen64, int64_t>(seed) {}
};

typedef MersenneTwister<32> Random32;
typedef MersenneTwister<64> Random64;
