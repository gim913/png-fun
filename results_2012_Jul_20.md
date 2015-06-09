## LEGEND ##


### algorithms ###

  * snappy - http://code.google.com/p/snappy/
  * dash - http://code.google.com/p/data-shrinker/
  * lz4 - http://code.google.com/p/lz4/ http://fastcompression.blogspot.com/p/lz4.html
  * lzham - http://code.google.com/p/lzham/ level 4
    * lzhamp - lzham level 4, polar
    * lzhamd - lzham level 4, deterministic
    * lzhamdp - lzham level 4, deterministic + polar
    * lzham3 - lzham level 3
    * lzham0 - lzham level 0
    * lzham0dp - lzham level 0, deterministic + polar
  * bsc    - http://libbsc.com/ (without sort transform)
  * bscf   - libbsc, fastmode     (without sort transform)

### test files ###
  * _test3 xray_ - 860x683  xray image, grayscale (uncompressed data size: 588,063)
  * _test1 moar_ - 2048x2048 game graphic, but over half of the image is fully transparent (uncompressed data size: 16,779,264)
  * _test2 orb_  - 3000x2000 concept art image (uncompressed data size: 18,002,000)

### quick info ###

For time measurements every image was compressed / decompress 100 times, and aritmetic mean from **top 10** results were taken.

For more details got to [Main page](http://code.google.com/p/png-fun)


## LOAD ##

### Load Times ###

in milliseconds (faster is nicer)
| |		**lode-inflate**| | **snappy**| **dash**| **lz4**| **lode-inflate**| **lzhamd**| **lzham3**| **lzhamdp**| **lzhamp**| **lzham**| **lzham0dp**| **lzham0**| **bsc**| **bscf**|
|:|:-----------------|:|:----------|:--------|:-------|:----------------|:----------|:----------|:-----------|:----------|:---------|:------------|:----------|:-------|:--------|
| test3\_xray | 129              | | <font color='#00cc33'>28</font>| <font color='#008822'>34</font>| <font color='#005511'>51</font>| 129             | 164       | 167       | 169        | 208       | 171      | 225         | 225       | 910    | 1046    |
| test1\_moar | 1711             | | <font color='#00cc33'>596</font>| <font color='#008822'>855</font>| <font color='#005511'>879</font>| 1711            | 1875      | 1889      | 1553       | 1552      | 1878     | 2529        | 2039      | 13880  | 13862   |
| test2\_orb | 4339             | | <font color='#00cc33'>1206</font>| <font color='#008822'>1449</font>| <font color='#005511'>2630</font>| 4339            | 4728      | 4714      | 6183       | 6182      | 5772     | 9616        | 9617      | 41899  | 41906   |

### Load ratio (score 1) ###

Calculated as (compressedSize+uncompressedSize) / time taken for uncompression, in Mb/s (higher is better)

(Taking sum is probably better meassure, as compressed data must be read, and whole decompressed data must be created)

| |		**lode-inflate**| | **snappy**| **dash**| **lz4**| **lode-inflate**| **lzhamd**| **lzham3**| **lzhamdp**| **lzhamp**| **lzham**| **lzham0dp**| **lzham0**| **bsc**| **bscf**|
|:|:-----------------|:|:----------|:--------|:-------|:----------------|:----------|:----------|:-----------|:----------|:---------|:------------|:----------|:-------|:--------|
|test3\_xray (588063) | 5.49             | | <font color='#00cc33'>27.861</font>| <font color='#008822'>23.125</font>| <font color='#005511'>14.210</font>| 5.49            | 4.15      | 4.08      | 4.02       | 3.27      | 3.98     | 3.13        | 3.13      | 0.73   | 0.635   |
|test1\_moar (16779264) | 10.1             | | <font color='#00cc33'>31.171</font>| <font color='#008822'>21.108</font>| <font color='#005511'>19.878</font>| 10.1            | 9.06      | 9.0       | 10.9       | 10.9      | 9.05     | 6.8         | 8.43      | 1.21   | 1.21    |
|test2\_orb (18002000) | 5.16             | | <font color='#00cc33'>21.816</font>| <font color='#008822'>18.184</font>| <font color='#005511'>9.074</font>| 5.16            | 4.61      | 4.63      | 3.53       | 3.53      | 3.78     | 2.35        | 2.35      | 0.509  | 0.509   |

### Load ratio (score 2) ###

calculated as compressedSize / time taken for uncompression, Mb/s (higher is better)

| |		**lode-inflate**| | **snappy**| **dash**| **lz4**| **lzhamd**| **lzham3**| **lzhamdp**| **lzhamp**| **lzham**| **lzham0dp**| **lzham0**| **lode-inflate**| **bsc**| **bscf**|
|:|:-----------------|:|:----------|:--------|:-------|:----------|:----------|:-----------|:----------|:---------|:------------|:----------|:----------------|:-------|:--------|
|test3\_xray (588063) | 1.14             | | <font color='#00cc33'>20.029</font>| <font color='#008822'>16.495</font>| <font color='#005511'>10.996</font>| 3.42      | 3.36      | 3.32       | 2.7       | 3.28     | 2.49        | 2.49      | 1.14            | 0.616  | 0.536   |
|test1\_moar (16779264) | 0.704            | | <font color='#00cc33'>26.849</font>| <font color='#008822'>18.716</font>| <font color='#005511'>18.205</font>| 8.53      | 8.47      | 10.3       | 10.3      | 8.52     | 6.33        | 7.85      | 0.704           | 1.15   | 1.15    |
|test2\_orb (18002000) | 1.2              | | <font color='#00cc33'>14.236</font>| <font color='#008822'>11.848</font>| <font color='#005511'>6.528</font>| 3.63      | 3.64      | 2.78       | 2.78      | 2.97     | 1.79        | 1.79      | 1.2             | 0.41   | 0.41    |

## SAVE ##

### Save Times ###

As for me save time doesn't matter, cause you're doing conversion, on your side, so it doesn't matter whether it's fast or not

| |		**lode-deflate<sup>(1)</sup>**| | **dash**| **snappy**| **bscf**| **bsc**| **lz4**| **lode-deflate<sup>(1)</sup>**| **lzham0**| **lzham0dp**| **lzham3**| **lzhamd**| **lzhamdp**| **lzham**| **lzhamp**|
|:|:-------------------------------|:|:--------|:----------|:--------|:-------|:-------|:------------------------------|:----------|:------------|:----------|:----------|:-----------|:---------|:----------|
| test3\_xray | 4373                           | | <font color='#008822'>119</font>| <font color='#00cc33'>72</font>| <font color='#005511'>1356</font>| 2091   | 4326   | 4373                          | 3775      | 3776        | 13852     | 15005     | 15052      | 15032    | 15054     |
| test1\_moar | 53057                          | | <font color='#00cc33'>862</font>| <font color='#008822'>1050</font>| <font color='#005511'>16593</font>| 16635  | 47402  | 53057                         | 77713     | 84633       | 234404    | 314677    | 257264     | 314683   | 257187    |
| test2\_orb | 116451                         | | <font color='#00cc33'>3297</font>| <font color='#008822'>3416</font>| 76150   | <font color='#005511'>75767</font>| 156546 | 116451                        | 158169    | 158150      | 653729    | 675750    | 794105     | 677685   | 794240    |

**<sup>(1)</sup>** - if I'm not mistaken implementation of deflate in lodepng is memory-intensive, so you probably shouldn't compare this one, to others

### Save Data (compression ratio) ###

Actually 'saving ratio', that is 1 - 'compression ratio' (higher is better)

| |		**lode-deflate**| | **bscf**| **bsc**| **lzhamdp**| **lzhamp**| **lzham**| **lzhamd**| **lzham3**| **lode-deflate**| **lzham0dp**| **lzham0**| **lz4**| **snappy**| **dash**|
|:|:-----------------|:|:--------|:-------|:-----------|:----------|:---------|:----------|:----------|:----------------|:------------|:----------|:-------|:----------|:--------|
|test3\_xray (588063) | 0.769            | | <font color='#00cc33'>0.816</font>| <font color='#00cc33'>0.816</font>| <font color='#008822'>0.788</font>| <font color='#008822'>0.788</font>| <font color='#005511'>0.787</font>| <font color='#005511'>0.787</font>| 0.786     | 0.769           | 0.745       | 0.745     | 0.708  | 0.609     | 0.598   |
|test1\_moar (16779264) | 0.922            | | <font color='#00cc33'>0.949</font>| <font color='#00cc33'>0.949</font>| <font color='#008822'>0.938</font>| <font color='#008822'>0.938</font>| <font color='#005511'>0.938</font>| <font color='#005511'>0.938</font>| 0.938     | 0.922           | 0.926       | 0.926     | 0.908  | 0.839     | 0.872   |
|test2\_orb (18002000) | 0.675            | | <font color='#00cc33'>0.757</font>| <font color='#00cc33'>0.757</font>| 0.729      | 0.729     | <font color='#008822'>0.73</font>| <font color='#008822'>0.73</font>| <font color='#005511'>0.729</font>| 0.675           | 0.684       | 0.684     | 0.61   | 0.467     | 0.465   |


compressed size in bytes (smaller is better?)

| |		**lode-deflate**| | **bscf**| **bsc**| **lzhamdp**| **lzhamp**| **lzham**| **lzhamd**| **lzham3**| **lode-deflate**| **lzham0dp**| **lzham0**| **lz4**| **snappy**| **dash**|
|:|:-----------------|:|:--------|:-------|:-----------|:----------|:---------|:----------|:----------|:----------------|:------------|:----------|:-------|:----------|:--------|
|test3\_xray (0,588,063) | 0,135,863        | | <font color='#00cc33'>0,108,098</font>| <font color='#00cc33'>0,108,098</font>| <font color='#008822'>0,124,924</font>| <font color='#008822'>0,124,924</font>| <font color='#005511'>0,125,088</font>| <font color='#005511'>0,125,088</font>| 0,125,609 | 0,135,863       | 0,149,951   | 0,149,951 | 0,171,822 | 0,229,935 | 0,236,392 |
|test1\_moar (16,779,264) | 1,308,122        | | <font color='#00cc33'>0,855,806</font>| <font color='#00cc33'>0,855,806</font>| <font color='#008822'>1,039,000</font>| <font color='#008822'>1,039,000</font>| <font color='#005511'>1,040,544</font>| <font color='#005511'>1,040,544</font>| 1,043,703 | 1,308,122       | 1,240,627   | 1,240,627 | 1,541,804 | 2,700,863 | 2,144,374 |
|test2\_orb (18,002,000) | 5,846,486        | | <font color='#00cc33'>4,371,218</font>| <font color='#00cc33'>4,371,218</font>| 4,886,334  | 4,886,334 | <font color='#008822'>4,857,548</font>| <font color='#008822'>4,857,548</font>| <font color='#005511'>4,870,974</font>| 5,846,486       | 5,679,731   | 5,679,731 | 7,020,656 | 9,586,398 | 9,626,421 |