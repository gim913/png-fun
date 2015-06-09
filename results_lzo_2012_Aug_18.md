## LEGEND ##

Different lzo variants

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
| |		**lode-inflate**| | **lzoc1**| **lzob1**| **lzob5**| **lzof**| **lzoc5**| **lzo**| **lzoa**| **lzoc9**| **lzoy**| **lzob9**| **lzox**| **lode-inflate**|
|:|:-----------------|:|:---------|:---------|:---------|:--------|:---------|:-------|:--------|:---------|:--------|:---------|:--------|:----------------|
| test3\_xray | 161              | | <font color='#00cc33'>36</font>| 46       | <font color='#00cc33'>36</font>| 37      | 49       | <font color='#00cc33'>36</font>| 41      | 38       | 40      | 73       | 66      | 161             |
| test1\_moar | 1700             | | 623      | 618      | 638      | 631     | <font color='#00cc33'>508</font>| <font color='#005511'>606</font>| <font color='#008822'>604</font>| 874      | 654     | 707      | 652     | 1700            |
| test2\_orb | 4320             | | <font color='#008822'>953</font>| <font color='#00cc33'>939</font>| <font color='#005511'>1001</font>| 1006    | 1009     | 1151   | 1057    | 1023     | 1099    | 1007     | 1081    | 4320            |

### Load ratio (score 1) ###

Calculated as (compressedSize+uncompressedSize) / time taken for uncompression, in Mb/s (higher is better)

(Taking sum is probably better meassure, as compressed data must be read, and whole decompressed data must be created)

| |		**lode-inflate**| | **lzoc1**| **lzob5**| **lzob1**| **lzoa**| **lzof**| **lzoc5**| **lzo**| **lzoc9**| **lzoy**| **lzox**| **lzob9**| **lode-inflate**|
|:|:-----------------|:|:---------|:---------|:---------|:--------|:--------|:---------|:-------|:---------|:--------|:--------|:---------|:----------------|
|test3\_xray (588063) | 4.4              | | <font color='#005511'>21.818</font>| <font color='#008822'>21.845</font>| 17.1     | 19.8    | 21.4    | 16.0     | <font color='#00cc33'>22.610</font>| 20.2     | 19.8    | 12.0    | 10.5     | 4.4             |
|test1\_moar (16779264) | 10.1             | | 28.9     | 28.2     | 29.1     | <font color='#008822'>30.271</font>| 28.7    | <font color='#00cc33'>35.449</font>| <font color='#005511'>30.190</font>| 20.4     | 27.7    | 27.8    | 25.3     | 10.1            |
|test2\_orb (18002000) | 5.18             | | <font color='#008822'>27.080</font>| <font color='#005511'>26.184</font>| <font color='#00cc33'>27.407</font>| 25.6    | 26.0    | 26.0     | 23.6   | 24.9     | 24.1    | 24.2    | 25.3     | 5.18            |

### Load ratio (score 2) ###

calculated as compressedSize / time taken for uncompression, Mb/s (higher is better)

| |		**lode-inflate**| | **lzoc1**| **lzob1**| **lzob5**| **lzof**| **lzoc5**| **lzo**| **lzoa**| **lzoc9**| **lzoy**| **lzob9**| **lzox**| **lode-inflate**|
|:|:-----------------|:|:---------|:---------|:---------|:--------|:---------|:-------|:--------|:---------|:--------|:---------|:--------|:----------------|
|test3\_xray (588063) | 0.912            | | <font color='#00cc33'>15.578</font>| 12.2     | <font color='#00cc33'>15.578</font>| <font color='#008822'>15.157</font>| 11.4     | <font color='#00cc33'>15.578</font>| 13.7    | <font color='#005511'>14.758</font>| 14.0    | 7.68     | 8.5     | 0.912           |
|test1\_moar (16779264) | 0.709            | | 25.7     | 25.9     | 25.1     | 25.4    | <font color='#00cc33'>31.500</font>| <font color='#005511'>26.406</font>| <font color='#008822'>26.493</font>| 18.3     | 24.5    | 22.6     | 24.5    | 0.709           |
|test2\_orb (18002000) | 1.21             | | <font color='#008822'>18.015</font>| <font color='#00cc33'>18.283</font>| <font color='#005511'>17.151</font>| 17.1    | 17.0     | 14.9   | 16.2    | 16.8     | 15.6    | 17.0     | 15.9    | 1.21            |

## SAVE ##

### Save Times ###

As for me save time doesn't matter, cause you're doing conversion, on your side, so it doesn't matter whether it's fast or not

| |		**lode-deflate<sup>(1)</sup>**| | **lzox**| **lzoy**| **lzo**| **lzoa**| **lzoc1**| **lzob1**| **lzof**| **lzob5**| **lzoc5**| **lzob9**| **lzoc9**| **lode-deflate<sup>(1)</sup>**|
|:|:-------------------------------|:|:--------|:--------|:-------|:--------|:---------|:---------|:--------|:---------|:---------|:---------|:---------|:------------------------------|
| test3\_xray | 5415                           | | <font color='#008822'>74</font>| <font color='#005511'>75</font>| <font color='#00cc33'>67</font>| 114     | 78       | 103      | 81      | 156      | 160      | 373      | 191      | 5415                          |
| test1\_moar | 52699                          | | <font color='#00cc33'>436</font>| <font color='#008822'>549</font>| 959    | <font color='#005511'>818</font>| 846      | 840      | 1088    | 1126     | 1152     | 2487     | 2531     | 52699                         |
| test2\_orb | 115824                         | | <font color='#00cc33'>1358</font>| <font color='#008822'>1374</font>| 2420   | <font color='#005511'>2165</font>| 2399     | 2374     | 2512    | 3636     | 3746     | 5139     | 5213     | 115824                        |

**<sup>(1)</sup>** - if I'm not mistaken implementation of deflate in lodepng is memory-intensive, so you probably shouldn't compare this one, to others

### Save Data (compression ratio) ###

Actually 'saving ratio', that is 1 - 'compression ratio' (higher is better)

| |		**lode-deflate**| | **lode-deflate**| **lzoc9**| **lzob9**| **lzob1**| **lzoc1**| **lzoc5**| **lzob5**| **lzox**| **lzof**| **lzoy**| **lzoa**| **lzo**|
|:|:-----------------|:|:----------------|:---------|:---------|:---------|:---------|:---------|:---------|:--------|:--------|:--------|:--------|:-------|
|test3\_xray (588063) | <font color='#00cc33'>0.769</font>| | <font color='#00cc33'>0.769</font>| <font color='#008822'>0.63</font>| <font color='#005511'>0.628</font>| 0.601    | 0.599    | 0.598    | 0.598    | 0.591   | 0.591   | 0.588   | 0.549   | 0.549  |
|test1\_moar (16779264) | <font color='#00cc33'>0.922</font>| | <font color='#00cc33'>0.922</font>| <font color='#005511'>0.884</font>| <font color='#008822'>0.884</font>| 0.875    | 0.874    | 0.875    | 0.875    | 0.869   | 0.869   | 0.868   | 0.857   | 0.857  |
|test2\_orb (18002000) | <font color='#00cc33'>0.675</font>| | <font color='#00cc33'>0.675</font>| <font color='#008822'>0.517</font>| <font color='#005511'>0.515</font>| 0.501    | 0.497    | 0.474    | 0.473    | 0.474   | 0.474   | 0.457   | 0.421   | 0.42   |


compressed size in bytes (smaller is better?)

| |		**lode-deflate**| | **lode-deflate**| **lzoc9**| **lzob9**| **lzob1**| **lzoc1**| **lzoc5**| **lzob5**| **lzox**| **lzof**| **lzoy**| **lzoa**| **lzo**|
|:|:-----------------|:|:----------------|:---------|:---------|:---------|:---------|:---------|:---------|:--------|:--------|:--------|:--------|:-------|
|test3\_xray (0,588,063) | <font color='#00cc33'>0,135,863</font>| | <font color='#00cc33'>0,135,863</font>| <font color='#008822'>0,217,582</font>| <font color='#005511'>0,218,537</font>| 0,234,715 | 0,235,547 | 0,236,257 | 0,236,564 | 0,240,381 | 0,240,506 | 0,242,281 | 0,265,183 | 0,265,436 |
|test1\_moar (16,779,264) | <font color='#00cc33'>1,308,122</font>| | <font color='#00cc33'>1,308,122</font>| <font color='#005511'>1,950,819</font>| <font color='#008822'>1,946,034</font>| 2,094,814 | 2,112,601 | 2,103,701 | 2,097,425 | 2,201,245 | 2,196,593 | 2,219,903 | 2,392,650 | 2,404,810 |
|test2\_orb (18,002,000) | <font color='#00cc33'>5,846,486</font>| | <font color='#00cc33'>5,846,486</font>| <font color='#008822'>8,701,600</font>| <font color='#005511'>8,722,459</font>| 8,982,988 | 9,058,539 | 9,471,744 | 9,481,522 | 9,461,295 | 9,471,117 | 9,781,392 | 10,424,808 | 10,444,381 |
