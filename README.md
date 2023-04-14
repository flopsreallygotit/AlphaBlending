# AlphaBlending
For average .bmp enjoyers.

## Overview

The goal of this project is implementing Alpha Blending algorithm the most effecient way.

To maximize performance we are going to use SSE instructions.

> CPU: Intel Core i5 11300H
> Tests were conducted on 800 x 600 Background image and 236 x 127 Foreground image

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

Don't forget to change flags in Makefile. Now you can research the SSE instructions optimization by yourself!

## Overview

We want to mix two images in RGBA profile together. Each pixel will be calculated according to this expression:

$newColor_{red} = backgroundColor_{red} * \alpha + foregroundColor_{red} * (\alpha - 1)$ 

The same goes for green and blue channels. Alpha of new pixel will be the same as alpha of current background pixel.

## First approach

We can calculate each pixel iteratively. 

~~~C++
unsigned foregroundAlpha = foregroundColor >> 24;
unsigned newColor = backgroundColor & 0xFF000000;

for (int idx = 0; idx < 24; idx += 8)
{
    unsigned foregroundChannel = ((0xFF << idx) & foregroundColor) >> idx;
    unsigned backgroundChannel = ((0xFF << idx) & backgroundColor) >> idx;

    newColor += ((foregroundChannel * foregroundAlpha + 
                    backgroundChannel * (255 - foregroundAlpha)) / 255) << idx;
}
~~~

## Optimizing with SSE instructions

We can calculate 4 pixels at once using intrinsic functions. Intrinsic function I won't show the example of code, because it's a bit difficult to perceive. Better check it in mainUtils.cpp.

## Performance

Let's check the perfomance table below.

> Calculation Loops count: 10000
> Number of values for average getting: 5

| Version | Compilation flags | Time, seconds    | Speed Growth |
| ------  | :---------------: | :--------------: | :----------: |
| SLOW    | None              | 3.824 +- 0.005   | None         |
| SLOW    | -О3               | 0.936 +- 0.012   | 4.085        |
| SLOW    | -Оfast            | 0.96 +- 0.04     | 3.983        |
| SLOW    | -mavx512dq -Оfast | 0.938 +- 0.012   | 4.077        |
| FAST    | -mavx512dq -О3    | 0.152 +- 0.004   | 25.15        |
| FAST    | -mavx512dq -Ofast | 0.1503 +- 0.0027 | 25.442       |

Speed increased about ***25.5 times*** for slowest and fastest version and about ***6.5 times*** for slow and fast version with -Ofast compilation flag. This confirms the effectivness of SSE instructions.

Also there is negative growth in speed if we don't use optimization flags, because of ineffecient usage of SIMD registers. 

## Conclusion

SSE instructions allowed us to speed up the calculations *25.5 times*. However the amount of code and it's difficulty became bigger too.

It was too official. Time to fool around.

![image](https://user-images.githubusercontent.com/89828695/230796967-914cdcb5-ee75-4e77-8e00-35b4ebd6235b.png)
