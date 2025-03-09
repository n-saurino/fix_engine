#include <benchmark/benchmark.h>
#include <cstring>
#include <vector>
#include "FIXMessage.hpp"
#include "MemoryPool.hpp"
#include "NewOrderSingle.hpp"

MemoryPool pool;

static void Benchmark_FIXMessageBuild(benchmark::State& state){
	for(auto _ : state){
		FIXBuffer buffer(pool.Allocate());
		FIXNewOrderSingle new_order_single(buffer);
		benchmark::DoNotOptimize(buffer.buffer_);
	}
}
BENCHMARK(Benchmark_FIXMessageBuild);

BENCHMARK_MAIN();
