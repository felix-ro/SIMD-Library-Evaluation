CC = g++
STANDARD_FLAGS = -std=c++17

# ------ OPTIMIZATION FLAGS ------
OPTIMIZATION_FLAGS = -fopenmp -O2 -ftree-vectorize -march=haswell -mtune=haswell -maes

# ------ INCLUDE PATHS -------
GOOGLE_BENCHMARK_INCLUDE = -isystem ../benchmark/include \
  -L../benchmark/build/src -lbenchmark -lpthread
GOOGLE_HIGHWAY_INCLUDE = -I ../highway/ -L../highway/build -lhwy
VCDEVEL_VC_INCLUDE = -I ../Vc/include -L../Vc/lib -lVc
LIBSIMDPP_INCLUDE = -I../libsimdpp -DSIMDPP_ARCH_X86_AVX2 -mavx2 -DSIMDPP_ARCH_X86_FMA3 -mfma
PURE_SIMD_INCLUDE = -I../pure_simd/include
NSIMD_INCLUDE = -DAVX2 -mavx2 -L../nsimd/build -lnsimd_AVX2 -I../nsimd/include

# -------- Target Configuration -------- 
# -------- AVX512 ---------
# Choose correct optimization flags for system here
# AVX512: OPTIMIZATION_FLAGS = -fopenmp -O2 -ftree-vectorize -march=skylake-avx512 
AVX512: OPTIMIZATION_FLAGS = -fopenmp -O2 -ftree-vectorize -march=icelake-client -mavx512vpopcntdq
AVX512: LIBSIMDPP_INCLUDE = -I../libsimdpp -DSIMDPP_ARCH_X86_AVX512F -mavx512f -DSIMDPP_ARCH_X86_FMA3 -mfma
AVX512: NSIMD_INCLUDE = -mavx512f -DNSIMD_AVX512_KNL -L../nsimd/build -lnsimd_AVX512_KNL -I../nsimd/include
AVX512: CFLAGS = -DAVX512

# -------- SVE ---------
SVE: OPTIMIZATION_FLAGS = -fopenmp -O2 -ftree-vectorize  -march=armv8.2-a+sve -mcpu=a64fx+sve
SVE: NSIMD_INCLUDE = -DNSIMD_SVE -L../nsimd/build -lnsimd_SVE -I../nsimd/include
SVE: VCDEVEL_VC_INCLUDE = 
SVE: LIBSIMDPP_INCLUDE = 
SVE: CFLAGS = -DSVE

popcntReduceBenchSVE: CC = clang++

# -------- NEON ---------
NEON: OPTIMIZATION_FLAGS = -fopenmp -O2 -ftree-vectorize -march=armv8-a+simd -mtune=thunderx2t99
NEON: VCDEVEL_VC_INCLUDE =
NEON: LIBSIMDPP_INCLUDE = -I../libsimdpp -DSIMDPP_ARCH_ARM_NEON 
NEON: NSIMD_INCLUDE = -DNSIMD_AARCH64 -L../nsimd/build -lnsimd_neon128 -I../nsimd/include
NEON: CFLAGS = -DNEON

# -------- Main Targets ---------
AVX2: mandelBench mandelTest dotBench dotProductTest popcntReduceBench popcntBench logicalFunctionsBench 
AVX512: mandelBench mandelTest dotBench dotProductTest  # popcntReduceBench popcntBench
SVE: mandelBench mandelTest popcntBench popcntReduceBenchSVE
NEON: mandelBench mandelTest

# --------- Executables ---------
mandelBench: mandelbrot/mandelbrotBenchmark.cpp mandelbrot.o nsimdMandelbrot.o nsimdBaseMandelbrot.o simdeMandelbrot.o mandelbrotScalar.o utils.o 
	$(CC) $(STANDARD_FLAGS) $(OPTIMIZATION_FLAGS) -ffast-math mandelbrot/mandelbrotBenchmark.cpp mandelbrot.o nsimdMandelbrot.o nsimdBaseMandelbrot.o simdeMandelbrot.o mandelbrotScalar.o utils.o -o mandelBench $(GOOGLE_HIGHWAY_INCLUDE) $(VCDEVEL_VC_INCLUDE) $(LIBSIMDPP_INCLUDE) $(GOOGLE_BENCHMARK_INCLUDE) $(CFLAGS)

mandelTest: test/mandelTest.cpp mandelbrot.o nsimdMandelbrot.o nsimdBaseMandelbrot.o simdeMandelbrot.o utils.o
	$(CC) $(STANDARD_FLAGS) $(OPTIMIZATION_FLAGS) test/mandelTest.cpp mandelbrot.o nsimdMandelbrot.o nsimdBaseMandelbrot.o simdeMandelbrot.o utils.o -o mandelTest $(GOOGLE_HIGHWAY_INCLUDE) $(VCDEVEL_VC_INCLUDE) $(LIBSIMDPP_INCLUDE) $(CFLAGS)

dotBench: dotProduct/dotProductBenchmark.cpp dotProduct.o dotProductHighway.o utils.o
	$(CC) $(STANDARD_FLAGS) $(OPTIMIZATION_FLAGS) -ffast-math dotProduct/dotProductBenchmark.cpp dotProduct.o dotProductHighway.o utils.o -o dotBench $(GOOGLE_HIGHWAY_INCLUDE) $(VCDEVEL_VC_INCLUDE) $(LIBSIMDPP_INCLUDE) $(GOOGLE_BENCHMARK_INCLUDE) $(CFLAGS) 

dotProductTest: test/dotProductTest.cpp dotProduct.o dotProductHighway.o utils.o
		$(CC) $(STANDARD_FLAGS) $(OPTIMIZATION_FLAGS) test/dotProductTest.cpp dotProduct.o dotProductHighway.o utils.o -o dotTest $(GOOGLE_HIGHWAY_INCLUDE) $(VCDEVEL_VC_INCLUDE) $(CFLAGS)

popcntReduceBench: functionBench/popcntReduceBenchmark.cpp populationCount.o 
	$(CC) $(STANDARD_FLAGS) $(OPTIMIZATION_FLAGS) $(CFLAGS) functionBench/popcntReduceBenchmark.cpp populationCount.o -o popcntReduceBench $(GOOGLE_HIGHWAY_INCLUDE) $(GOOGLE_BENCHMARK_INCLUDE)

popcntReduceBenchSVE: functionBench/popcntReduceBenchmark.cpp populationCount.o
	        $(CC) $(STANDARD_FLAGS) $(OPTIMIZATION_FLAGS) $(CFLAGS) functionBench/popcntReduceBenchmark.cpp populationCount.o -o popcntReduceBench $(GOOGLE_HIGHWAY_INCLUDE) $(GOOGLE_BENCHMARK_INCLUDE)

popcntBench: functionBench/popcntBenchmark.cpp 
	$(CC) $(STANDARD_FLAGS) $(OPTIMIZATION_FLAGS) $(CFLAGS) functionBench/popcntBenchmark.cpp -o popcntBench $(GOOGLE_HIGHWAY_INCLUDE) $(GOOGLE_BENCHMARK_INCLUDE)

logicalFunctionsBench: functionBench/logicalFunctionsBenchmark.cpp logicalFunctions.o 
	$(CC) $(STANDARD_FLAGS) $(OPTIMIZATION_FLAGS) functionBench/logicalFunctionsBenchmark.cpp logicalFunctions.o -o logicalBench $(GOOGLE_HIGHWAY_INCLUDE) $(GOOGLE_BENCHMARK_INCLUDE) 

# --------- Object Files ---------
mandelbrotScalar.o: mandelbrot/mandelbrotScalar.hpp mandelbrot/mandelbrotScalar.cpp mandelbrot/mandelbrotSettings.hpp
	$(CC) $(STANDARD_FLAGS) -O2 -fno-tree-vectorize -ffast-math -c mandelbrot/mandelbrotScalar.cpp -o mandelbrotScalar.o

mandelbrot.o: mandelbrot/mandelbrot.hpp mandelbrot/mandelbrot.cpp mandelbrot/mandelbrotSettings.hpp
	$(CC) $(STANDARD_FLAGS) $(OPTIMIZATION_FLAGS) -ffast-math -c mandelbrot/mandelbrot.cpp $(GOOGLE_HIGHWAY_INCLUDE) $(VCDEVEL_VC_INCLUDE) $(LIBSIMDPP_INCLUDE) $(PURE_SIMD_INCLUDE) $(CFLAGS)

nsimdMandelbrot.o: mandelbrot/nsimdMandelbrot.cpp mandelbrot/nsimdMandelbrot.hpp mandelbrot/mandelbrotSettings.hpp
	$(CC) $(STANDARD_FLAGS) $(OPTIMIZATION_FLAGS) -ffast-math -c mandelbrot/nsimdMandelbrot.cpp $(NSIMD_INCLUDE) $(CFLAGS) 

nsimdBaseMandelbrot.o: mandelbrot/nsimdBaseMandelbrot.cpp mandelbrot/nsimdBaseMandelbrot.hpp mandelbrot/mandelbrotSettings.hpp
	$(CC) $(STANDARD_FLAGS) $(OPTIMIZATION_FLAGS) -ffast-math -c mandelbrot/nsimdBaseMandelbrot.cpp $(NSIMD_INCLUDE) $(CFLAGS) 

simdeMandelbrot.o: mandelbrot/simdeMandelbrot.cpp mandelbrot/simdeMandelbrot.hpp mandelbrot/mandelbrotSettings.hpp
	$(CC) $(STANDARD_FLAGS) $(OPTIMIZATION_FLAGS) -ffast-math -fopenmp-simd -DSIMDE_ENABLE_OPENMP -c mandelbrot/simdeMandelbrot.cpp $(CFLAGS)

dotProduct.o: dotProduct/dotProduct.hpp dotProduct/dotProduct.cpp
	$(CC) $(STANDARD_FLAGS) $(OPTIMIZATION_FLAGS) -ffast-math -c dotProduct/dotProduct.cpp $(VCDEVEL_VC_INCLUDE) $(LIBSIMDPP_INCLUDE) $(PURE_SIMD_INCLUDE) $(CFLAGS)

dotProductHighway.o: dotProduct/dotProductHighway.hpp dotProduct/dotProductHighway.cpp
	$(CC) $(STANDARD_FLAGS) $(OPTIMIZATION_FLAGS) -ffast-math -c dotProduct/dotProductHighway.cpp $(GOOGLE_HIGHWAY_INCLUDE) $(CFLAGS)

populationCount.o: functionBench/populationCount.hpp functionBench/populationCount.cpp
	$(CC) $(STANDARD_FLAGS) $(OPTIMIZATION_FLAGS) -c functionBench/populationCount.cpp $(GOOGLE_HIGHWAY_INCLUDE) $(CFLAGS)

logicalFunctions.o: functionBench/logicalFunctions.hpp functionBench/logicalFunctions.cpp
	$(CC) $(STANDARD_FLAGS) $(OPTIMIZATION_FLAGS) -c functionBench/logicalFunctions.cpp $(GOOGLE_HIGHWAY_INCLUDE)

utils.o: utils/utils.hpp utils/utils.cpp
	$(CC) $(STANDARD_FLAGS) $(OPTIMIZATION_FLAGS) -ffast-math -c utils/utils.cpp

# ------------- Clean ------------
clean:
	rm -f  mandelBench dotBench dotTest mandelTest popcntReduceBench logicalBench popcntBench *.out *.o 
