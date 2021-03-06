// SPDX-License-Identifier: BSD-2-Clause

// This code is part of the sfizz library and is licensed under a BSD 2-clause
// license. You should have receive a LICENSE.md file along with the code.
// If not, contact the sfizz maintainers at https://github.com/sfztools/sfizz

#include "SIMDHelpers.h"
#include "absl/types/span.h"
#include <benchmark/benchmark.h>
#include <cmath>
#include <iostream>
#include <numeric>
#include <random>
#include <vector>

class MyFixture : public benchmark::Fixture {
public:
    void SetUp(const ::benchmark::State& state)
    {
        std::random_device rd {};
        std::mt19937 gen { rd() };
        std::uniform_real_distribution<float> dist { 0.1f, 1.0f };
        source = std::vector<float>(state.range(0));
        result = std::vector<float>(state.range(0));
        intResult = std::vector<int>(state.range(0));
        std::generate(source.begin(), source.end(), [&]() { return dist(gen); });
    }

    void TearDown(const ::benchmark::State& state [[maybe_unused]])
    {
    }

    std::vector<float> source;
    std::vector<float> result;
    std::vector<int> intResult;
};

BENCHMARK_DEFINE_F(MyFixture, Dummy)
(benchmark::State& state)
{
    for (auto _ : state) {
        for (int i = 0; i < state.range(0); ++i)
            result[i] = source[i];
        benchmark::DoNotOptimize(result);
    }
}

BENCHMARK_DEFINE_F(MyFixture, ScalarExp)
(benchmark::State& state)
{
    for (auto _ : state) {
        sfz::exp<float, false>(source, absl::MakeSpan(result));
        benchmark::DoNotOptimize(result);
    }
}

BENCHMARK_DEFINE_F(MyFixture, SIMDExp)
(benchmark::State& state)
{
    for (auto _ : state) {
        sfz::exp<float, true>(source, absl::MakeSpan(result));
        benchmark::DoNotOptimize(result);
    }
}

BENCHMARK_DEFINE_F(MyFixture, ScalarExp_Unaligned)
(benchmark::State& state)
{
    for (auto _ : state) {
        sfz::exp<float, false>(absl::MakeSpan(source).subspan(1), absl::MakeSpan(result).subspan(1));
        benchmark::DoNotOptimize(result);
    }
}

BENCHMARK_DEFINE_F(MyFixture, SIMDExp_Unaligned)
(benchmark::State& state)
{
    for (auto _ : state) {
        sfz::exp<float, true>(absl::MakeSpan(source).subspan(1), absl::MakeSpan(result).subspan(1));
        benchmark::DoNotOptimize(result);
    }
}

BENCHMARK_DEFINE_F(MyFixture, ScalarLog)
(benchmark::State& state)
{
    for (auto _ : state) {
        sfz::log<float, false>(source, absl::MakeSpan(result));
        benchmark::DoNotOptimize(result);
    }
}

BENCHMARK_DEFINE_F(MyFixture, SIMDLog)
(benchmark::State& state)
{
    for (auto _ : state) {
        sfz::log<float, true>(source, absl::MakeSpan(result));
        benchmark::DoNotOptimize(result);
    }
}

BENCHMARK_DEFINE_F(MyFixture, ScalarSin)
(benchmark::State& state)
{
    for (auto _ : state) {
        sfz::sin<float, false>(source, absl::MakeSpan(result));
        benchmark::DoNotOptimize(result);
    }
}

BENCHMARK_DEFINE_F(MyFixture, SIMDSin)
(benchmark::State& state)
{
    for (auto _ : state) {
        sfz::sin<float, true>(source, absl::MakeSpan(result));
        benchmark::DoNotOptimize(result);
    }
}

BENCHMARK_DEFINE_F(MyFixture, ScalarCos)
(benchmark::State& state)
{
    for (auto _ : state) {
        sfz::cos<float, false>(source, absl::MakeSpan(result));
        benchmark::DoNotOptimize(result);
    }
}

BENCHMARK_DEFINE_F(MyFixture, SIMDCos)
(benchmark::State& state)
{
    for (auto _ : state) {
        sfz::cos<float, true>(source, absl::MakeSpan(result));
        benchmark::DoNotOptimize(result);
    }
}

BENCHMARK_DEFINE_F(MyFixture, ScalarLibmFloorLog2)
(benchmark::State& state)
{
    for (auto _ : state) {
        for (size_t i = 0, n = source.size(); i < n; ++i) {
            intResult[i] = static_cast<int>(
                std::floor(std::log2(std::fabs(source[i]))));
        }
        benchmark::DoNotOptimize(intResult);
    }
}

BENCHMARK_DEFINE_F(MyFixture, ScalarFastFloorLog2)
(benchmark::State& state)
{
    for (auto _ : state) {
        for (size_t i = 0, n = source.size(); i < n; ++i) {
            intResult[i] = fp_exponent(source[i]);
        }
        benchmark::DoNotOptimize(intResult);
    }
}

BENCHMARK_REGISTER_F(MyFixture, Dummy)->RangeMultiplier(4)->Range(1 << 6, 1 << 10);
BENCHMARK_REGISTER_F(MyFixture, ScalarExp)->RangeMultiplier(4)->Range(1 << 6, 1 << 10);
BENCHMARK_REGISTER_F(MyFixture, SIMDExp)->RangeMultiplier(4)->Range(1 << 6, 1 << 10);
BENCHMARK_REGISTER_F(MyFixture, ScalarExp_Unaligned)->RangeMultiplier(4)->Range(1 << 6, 1 << 10);
BENCHMARK_REGISTER_F(MyFixture, SIMDExp_Unaligned)->RangeMultiplier(4)->Range(1 << 6, 1 << 10);
BENCHMARK_REGISTER_F(MyFixture, ScalarLog)->RangeMultiplier(4)->Range(1 << 6, 1 << 10);
BENCHMARK_REGISTER_F(MyFixture, SIMDLog)->RangeMultiplier(4)->Range(1 << 6, 1 << 10);
BENCHMARK_REGISTER_F(MyFixture, ScalarSin)->RangeMultiplier(4)->Range(1 << 6, 1 << 10);
BENCHMARK_REGISTER_F(MyFixture, SIMDSin)->RangeMultiplier(4)->Range(1 << 6, 1 << 10);
BENCHMARK_REGISTER_F(MyFixture, ScalarCos)->RangeMultiplier(4)->Range(1 << 6, 1 << 10);
BENCHMARK_REGISTER_F(MyFixture, SIMDCos)->RangeMultiplier(4)->Range(1 << 6, 1 << 10);
BENCHMARK_REGISTER_F(MyFixture, ScalarLibmFloorLog2)->RangeMultiplier(4)->Range(1 << 6, 1 << 10);
BENCHMARK_REGISTER_F(MyFixture, ScalarFastFloorLog2)->RangeMultiplier(4)->Range(1 << 6, 1 << 10);

BENCHMARK_MAIN();
