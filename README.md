# AlphaBlending
For average .bmp enjoyers.

## Overview

The goal of this project is implementing Alpha Blending algorithm the most effecient way.

To maximize performance we are going to use AVX2 instructions.

> CPU: Intel Core i5 11300H

## Quick Start

First clone repo:
```
git clone "https://github.com/flopsreallygotit/AlphaBlending.git"
```
Then switch to repo directory and type this:
```
make
./main
```
Good job! Now you can research the AVX instructions optimization by yourself.

## Alpha blending in a nutshell 

We want to mix two images in RGBA profile together. Each pixel will be calculated according to this formula:

$newColor_{red} = backgroundColor_{red} * \alpha + foregroundColor_{red} * (\alpha - 1)$ 

The same goes for green and blue channels.

## First approach

We can calculate each pixel iteratively.
This means that for one cycle of drawing we will make approximately **foregroundResolution operations**. But I will add one internal cycle, to calculate one point 100 times. This is going to *lower the impact* of calculations not related to alpha blending.

## Optimizing with AVX2 instructions

We can calculate 4 pixels at once with AVX2 instrucions.
This method will do about **foregroundResolution / 4 operations** in one calculation cycle.

## Performance

| Version | Compilation flags   | Time, seconds | Speed Growth |
| ------  | :-----------------: | :-----------: | :----------: |
| SLOW    | None                | 0.175772      | None         |
| SLOW    | -О3                 | 0.042878      | 4.099        |
| SLOW    | -Оfast              | 0.042472      | 4.139        |
| SLOW    | -mavx -mavx2 -Оfast | 0.046457      | 3.784        |
| FAST    | -mavx -mavx2 -О3    | 0.008710      | 20.180       |
| FAST    | -mavx -mavx2 -Ofast | 0.004815      | 36.505       |

As we can see, speed incresed **~36 times**. This confirms the effectivness of AVX instructions.

Also there is negative growth in speed if we don't use optimization flags or we're using AVX instructions on SLOW version, because of ineffecient usage of SIMD registers. 

## Conclusion

AVX instructions allowed us to speed up the calculations *36 times*. However the amount of code and it's difficulty became bigger too.

It was too official. Time to fool around.

![image](https://user-images.githubusercontent.com/89828695/230796967-914cdcb5-ee75-4e77-8e00-35b4ebd6235b.png)
