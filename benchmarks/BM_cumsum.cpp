// SPDX-License-Identifier: BSD-2-Clause

// This code is part of the sfizz library and is licensed under a BSD 2-clause
// license. You should have receive a LICENSE.md file along with the code.
// If not, contact the sfizz maintainers at https://github.com/sfztools/sfizz

#include "SIMDHelpers.h"
#include <benchmark/benchmark.h>
#include <random>
#include <numeric>
#include <vector>
#include <cmath>
#include <iostream>

class CumArray : public benchmark::Fixture {
public:
  void SetUp(const ::benchmark::State& state) {
    std::random_device rd { };
    std::mt19937 gen { rd() };
    std::uniform_real_distribution<float> dist { 0.1f, 1.0f };
    input = std::vector<float>(state.range(0));
    output = std::vector<float>(state.range(0));
    std::generate(input.begin(), input.end(), [&]() { return dist(gen); });
  }

  void TearDown(const ::benchmark::State& state [[maybe_unused]]) {

  }

  std::vector<float> input;
  std::vector<float> output;
};


BENCHMARK_DEFINE_F(CumArray, Sum_Scalar)(benchmark::State& state) {
    for (auto _ : state)
    {
        sfz::cumsum<float, false>(input, absl::MakeSpan(output));
    }
}

BENCHMARK_DEFINE_F(CumArray, Sum_SIMD)(benchmark::State& state) {
    for (auto _ : state)
    {
        sfz::cumsum<float, true>(input, absl::MakeSpan(output));
    }
}

BENCHMARK_DEFINE_F(CumArray, Sum_Scalar_Unaligned)(benchmark::State& state) {
    for (auto _ : state)
    {
        sfz::cumsum<float, false>(absl::MakeSpan(input).subspan(1), absl::MakeSpan(output).subspan(1));
    }
}

BENCHMARK_DEFINE_F(CumArray, Sum_SIMD_Unaligned)(benchmark::State& state) {
    for (auto _ : state)
    {
        sfz::cumsum<float, true>(absl::MakeSpan(input).subspan(1), absl::MakeSpan(output).subspan(1));
    }
}


BENCHMARK_REGISTER_F(CumArray, Sum_Scalar)->RangeMultiplier(4)->Range(1 << 2, 1 << 12);
BENCHMARK_REGISTER_F(CumArray, Sum_SIMD)->RangeMultiplier(4)->Range(1 << 2, 1 << 12);
BENCHMARK_REGISTER_F(CumArray, Sum_Scalar_Unaligned)->RangeMultiplier(4)->Range(1 << 2, 1 << 12);
BENCHMARK_REGISTER_F(CumArray, Sum_SIMD_Unaligned)->RangeMultiplier(4)->Range(1 << 2, 1 << 12);
BENCHMARK_MAIN();
