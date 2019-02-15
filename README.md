## News ##

### 2012 Aug 18 ###

I've added results of different versions of lzo, you can see them [here](https://github.com/gim913/png-fun/wiki/results_lzo_2012_Aug_18).

Based on that I've added lzoc1 to the results, you can see updated version [here](https://github.com/gim913/png-fun/wiki/results_2012_Aug_18).

### 2012 Aug 15 ###

**m^2** on encode.ru forum, pointed out there's something wrong with the results.
To be more precise, LZ4 had worse decompression speed than snappy. The reason for that is that instead of using original decompression routine, I've used my own, which as you can see by comparing results in worst case was **3.7 times SLOWER** than original LZ4 implementation!

I've updated the results, and updated "summary below"

## Overview ##

This project is something I wanted to do some time ago.

Main idea was to change ZLIB's compression/decompression used in PNGs to different algorithms and
see what's gonna happen.

I've even tried it once (at the beginning of 2011), but I was trying to do that using libpng, which was somewhat painful.

Some time ago, [Noel Llopis](https://twitter.com/noel_llopis/status/220919076652974080) tweeted about [lodepng](http://lodev.org/lodepng) (actually, I've seen RT via [Glenn Fiedler](https://twitter.com/gafferongames)).
I think I've header about **LodePNG** some time ago, but must've forgotten about it.
So I've taken a look at it, and it's seems it's once of those pieces made by developer for developers.

It allows exchanging both whole ZLIB or only inflate/deflate methods.
I've actually decided to do the second thing.

The only thing you actually need to do is to write your own lodepng\_custom\_inflate and/or lodepng\_custom\_deflate.


So I've created small app suitable for doing the test.
Source code of test app is available in the repo.
Please, keep in mind, this is PoC-like code, so it's more hacky-do-the-job-way than elegant way.

I've added few known, and lesser known algorithms
  * snappy - http://code.google.com/p/snappy/
  * dash   - http://code.google.com/p/data-shrinker/
  * lz4    - http://code.google.com/p/lz4/ http://fastcompression.blogspot.com/p/lz4.html
  * lzham  - http://code.google.com/p/lzham/ level 4
  * bsc    - http://libbsc.com/ (without sort transform)

snappy, data-shrinker, bsc were compiled with -O2, lzham with -O3 (accidentally),
I'm not sure if lz4 had O2 or Os, I hope the first.

Algorithms were allowed to use only one thread (I think bsc is able to use more).

So skipping the PNG part this is actually comparison of few compression/decompression algorithms.

## Tests ##

To obtain some sensible times, I've patched lodepng a bit, to call two additional functions
`lodepng_timer_start` and `lodepng_timer_stop`. If someone will be interested I can push diff to the repo.

I've used only 3 input files, compression and decompression was run 100 times.
Arithmetic mean was calculated from **top 10** results
(I think this is fair thing to do, to minimize system influence,
> If you think that's not right way, make your own tests :])

Results of **Save Time** (read: compression time), shouldn't be treated too strictly, as some algorithms
allow estimation of the size of output data, while some othere's don't, so there might be additional
penalty on memory allocation.

## Results ##

**NEW** results are [here](https://github.com/gim913/png-fun/wiki/results_2012_Aug_18).
Previous old results are [here](https://github.com/gim913/png-fun/wiki/).

Now what (I think) these results tells us?

  * if your goal is **decompression speed**, and you don't care much about compression speed, go with **LZ4** (hc variant)
    * also LZ4 decompressor can be realy, realy easily written
  * if you care about both **load time** and **save time**, go for snappy (but keep in mind compression ratio is quite low)
    * so for example it compressed 18Mb to 9.1Mb (when zlib shrinked it to 5.5), but the loading is almost 3 times that of zlib
  * the same goes for dash, but (sorry to say) it's source code looks like PoC (and seems OOB/overflows are possible), so I'd probably NOT use it in real world software
  * if you'd like **better-than-zlib ratio**, with comparable load-times go for LZHAM deterministic parsing variant
  * and there is BSC, keeping in mind, that it has the **best compression ratio**, it's kinda surprising, that it actually has **quite good compression time**, OTOH, decompression time is **terribly awful**
    * seems like a good candidate for compressing stuff downloaded from internet, it compresses quite fast, and for end-user, if the file has already been downloaded, decompression time shouldn't really matter
