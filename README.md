# SIMD Library Evaluation
## Introduction
This repository contains benchmarks for some of the most popular SIMD libraries available. 

The code available here is part of the implementation used in my Bachelor's Thesis, "Evaluation of C++ SIMD Libraries," which can be found [here](https://www.mnm-team.org/pub/Fopras/rock23/). Please note that this repo only contains part of the code used, and some alterations have been made since then. 

This repo allows benchmarking of the following libraries and extensions:

- AVX2 (VC, Highway, Libsimdpp, PureSimd, NSIMD, SIMD Everywhere, Intrinsics, AutoVectorization)
- AVX512 (Highway, Libsimdpp, PureSimd, NSIMD, SIMD Everywhere, Intrinsics, AutoVectorization)
- SVE (Highway, PureSimd, SIMD Everywhere, Intrinsics, AutoVectorization)
- NEON (Highway, Libsimdpp, PureSimd, NSIMD, SIMD Everywhere, Intrinsics, AutoVectorization)

## Findings
### Mandelbrot Benchmark targeting AVX2
![Mandelbrot Benchmark AVX2](/figures/x86_Mandelbrot_Benchmark_AVX2.png)


### Mandelbrot Benchmark targeting AVX512
![Mandelbrot Benchmark AVX512](/figures/x86_Mandelbrot_Benchmark_AVX512.png)


### Mandelbrot Benchmark targeting SVE
![Mandelbrot Benchmark SVE](/figures/ARM_Mandelbrot_Benchmark_SVE.png)

### Mandelbrot Benchmark targeting NEON
![Mandelbrot Benchmark NEON](/figures/ARM_Mandelbrot_Benchmark_NEON.png)


## Recreating the Benchmarks
### Disclaimer: 
Please note that this repository contains code that was primarily developed for research use within our lab environment. It may not be easily portable or adaptable to other systems or environments. I recommend using this code as a reference or starting point to familiarize yourself with the functionalities and usage of specific libraries.

### Getting started
Install and build the following libraries.

- [Google Benchmark](https://github.com/google/benchmark)
- [Google Highway](https://github.com/google/highway)
- [Libsimdpp](https://github.com/p12tic/libsimdpp)
- [PureSimd](https://github.com/eatingtomatoes/pure_simd)
- [VC](https://github.com/VcDevel/Vc)
- [NSIMD](https://github.com/agenium-scale/nsimd)
- [SIMD Everywhere](https://github.com/simd-everywhere/simde)

You will have to adjust the include paths of the libraries in the Makefile (it is probably best to rewrite it completely).

## Build Benchmarks
On x86 Machines with AVX2
```
$ make AVX2
```

On x86 Machines with AVX512
```
$ make AVX512
```

On ARM machines with SVE
```
$ make SVE
```

On ARM machines with NEON
```
$ make NEON
```
