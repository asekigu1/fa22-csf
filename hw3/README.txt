Name: Aimi Sekiguchi
JHED: asekigu1

Part b:
For part (b), you’ll use the memory traces as well as your simulator to 
determine which cache configuration has the best overall effectiveness. 
You should take a variety of properties into account: hit rates, 
miss penalties, total cache size (including overhead), etc. 

In your README.txt, describe in detail what experiments you ran (and why!),
 what results you got (and how!), and what, in your opinion, 
 is the best cache configuration of them all.


///  START Experiment 1  ///
256 4 16 write-allocate write-back fifo < gcc.trace
Total loads: 318197
Total stores: 197486
Load hits: 314171
Load misses: 4026
Store hits: 188047
Store misses: 9439
Total cycles: 9831818
Hit Rate (%): 97.3889
Total Cache Size: 16384
Overhead cache size: 6144

256 4 16 write-allocate write-back lru  < gcc.trace
Total loads: 318197
Total stores: 197486
Load hits: 314798
Load misses: 3399
Store hits: 188250
Store misses: 9236
Total cycles: 9331848
Hit Rate (%): 97.5499
Total Cache Size: 16384
Overhead cache size: 6144

256 4 16 write-allocate write-through fifo  < gcc.trace
Total loads: 318197
Total stores: 197486
Load hits: 314171
Load misses: 4026
Store hits: 188047
Store misses: 9439
Total cycles: 25636818
Hit Rate (%): 97.3889
Total Cache Size: 16384
Overhead cache size: 5120

256 4 16 write-allocate write-through lru  < gcc.trace
Total loads: 318197
Total stores: 197486
Load hits: 314798
Load misses: 3399
Store hits: 188250
Store misses: 9236
Total cycles: 25305648
Hit Rate (%): 97.5499
Total Cache Size: 16384
Overhead cache size: 5120

256 4 16 no-write-allocate write-through fifo  < gcc.trace
Total loads: 318197
Total stores: 197486
Load hits: 311017
Load misses: 7180
Store hits: 163705
Store misses: 33781
Total cycles: 23095322
Hit Rate (%): 92.0569
Total Cache Size: 16384
Overhead cache size: 5120

256 4 16 no-write-allocate write-through lru  < gcc.trace
Total loads: 318197
Total stores: 197486
Load hits: 311613
Load misses: 6584
Store hits: 164819
Store misses: 32667
Total cycles: 22858632
Hit Rate (%): 92.3885
Total Cache Size: 16384
Overhead cache size: 5120
///  END Experiment 1  ///

There are 6 possible parameter configurations for write and store methods:
 1. write-allocate write-back fifo
 2. write-allocate write-back lru
 3. write-allocate write-through fifo
 4. write-allocate write-through lru
 5. no-write-allocate write-through fifo
 6. no-write-allocate write-through lru

Above, I ran all 6 confirgurations for 256 sets, 4 blocks, 16 bytes per block.
  write-allocate write-back uses a dirty tag, which increases the overhead cache 
 size compared to the other 4 configurations. However, I believe the tag size is
 trivial given that write to memory is 100x more cycles than writing to cache.

I noticed the hit rate for write-allocate is 97.39% (fifo) and 97.55%(lru), 
 whereas the hitrate for no-write-allocate is 92.06% (fifo) and 92.39%(lru).
 So lru has a slightly better hit rate than fifo (as expected, since we update the timestamp)
 and write-allocate increases the hit rate by 5% (as expected, because we 
 regularly update the cache.) 

The configurations in descending order of total cycles:
 config3: 25.6m cycles
 config4: 25.3m cycles
 config5: 23.1m cycles
 config6: 22.9m cycles
 config1: 9.8m cycles
 config2: 9.3m cycles
 We notice write-allocate write-back has significantly less cycles than the others,
 which is also great because they also had the highest hit rates. I believe the 5%
 difference in cycles is more significant than the 0.2% difference in hit rate
 between lru and fifo, so I conclude that configuration 2(write-allocate write-back lru) 
 is that best out of the 6.


/// START Experiment 2 ///
./csim 512 64 64 write-allocate write-back fifo < gcc.trace 
Total loads: 318197
Total stores: 197486
Load hits: 317204
Load misses: 993
Store hits: 195112
Store misses: 2374
Total cycles: 5899516
Hit Rate (%): 99.3471
Total Cache Size: 2097152
Overhead cache size: 196608

./csim 512 64 64 write-allocate write-back lru < gcc.trace 
Total loads: 318197
Total stores: 197486
Load hits: 317204
Load misses: 993
Store hits: 195112
Store misses: 2374
Total cycles: 5899516
Hit Rate (%): 99.3471
Total Cache Size: 2097152
Overhead cache size: 196608

./csim 512 64 64 write-allocate write-through fifo < gcc.trace 
Total loads: 318197
Total stores: 197486
Load hits: 317204
Load misses: 993
Store hits: 195112
Store misses: 2374
Total cycles: 25648116
Hit Rate (%): 99.3471
Total Cache Size: 2097152
Overhead cache size: 163840

./csim 512 64 64 write-allocate write-through lru < gcc.trace 
Total loads: 318197
Total stores: 197486
Load hits: 317204
Load misses: 993
Store hits: 195112
Store misses: 2374
Total cycles: 25648116
Hit Rate (%): 99.3471
Total Cache Size: 2097152
Overhead cache size: 163840

./csim 512 64 64 no-write-allocate write-through fifo < gcc.trace
Total loads: 318197
Total stores: 197486
Load hits: 315897
Load misses: 2300
Store hits: 173709
Store misses: 23777
Total cycles: 23918206
Hit Rate (%): 94.9432
Total Cache Size: 2097152
Overhead cache size: 163840

./csim 512 64 64 no-write-allocate write-through lru < gcc.trace
Total loads: 318197
Total stores: 197486
Load hits: 315897
Load misses: 2300
Store hits: 173709
Store misses: 23777
Total cycles: 23918206
Hit Rate (%): 94.9432
Total Cache Size: 2097152
Overhead cache size: 163840
/// END Experiment 2///

Above is experiemnt 2, run with 512 sets, 64 blocks, 64 bytes per block.
I noticed that the same trends follow for the configurations, however since 
the numbers are a lot bigger, the hit rate difference is trivial, which makes 
configuration 1 and 2 having less cycles more significant.

Now assuming configuration 2 is the best, I will test for different size,block,and 
byte sizes.

/// START Experiemnt 3///
aimi@Aimis-MacBook-Pro hw3 % ./csim 2048 1 16 write-allocate write-back lru < gcc.trace
Total loads: 318197
Total stores: 197486
Load hits: 313662
Load misses: 4535
Store hits: 188043
Store misses: 9443
Total cycles: 9670105
Hit Rate (%): 97.2894
Total Cache Size: 32768
Overhead cache size: 12288
aimi@Aimis-MacBook-Pro hw3 % ./csim 1024 1 16 write-allocate write-back lru < gcc.trace
Total loads: 318197
Total stores: 197486
Load hits: 312238
Load misses: 5959
Store hits: 187502
Store misses: 9984
Total cycles: 11111340
Hit Rate (%): 96.9084
Total Cache Size: 16384
Overhead cache size: 6144
aimi@Aimis-MacBook-Pro hw3 % ./csim 512 1 16 write-allocate write-back lru < gcc.trace
Total loads: 318197
Total stores: 197486
Load hits: 308018
Load misses: 10179
Store hits: 186206
Store misses: 11280
Total cycles: 14447824
Hit Rate (%): 95.8387
Total Cache Size: 8192
Overhead cache size: 3072
aimi@Aimis-MacBook-Pro hw3 % ./csim 256 1 16 write-allocate write-back lru < gcc.trace
Total loads: 318197
Total stores: 197486
Load hits: 298863
Load misses: 19334
Store hits: 185202
Store misses: 12284
Total cycles: 20280865
Hit Rate (%): 93.8687
Total Cache Size: 4096
Overhead cache size: 1536
aimi@Aimis-MacBook-Pro hw3 % ./csim 128 1 16 write-allocate write-back lru < gcc.trace
Total loads: 318197
Total stores: 197486
Load hits: 279902
Load misses: 38295
Store hits: 179498
Store misses: 17988
Total cycles: 33321000
Hit Rate (%): 89.0857
Total Cache Size: 2048
Overhead cache size: 768
aimi@Aimis-MacBook-Pro hw3 % ./csim 2048 16 16 write-allocate write-back lru < gcc.trace
Total loads: 318197
Total stores: 197486
Load hits: 315854
Load misses: 2343
Store hits: 188615
Store misses: 8871
Total cycles: 4999669
Hit Rate (%): 97.8254
Total Cache Size: 524288
Overhead cache size: 196608
aimi@Aimis-MacBook-Pro hw3 % ./csim 1024 8 16 write-allocate write-back lru < gcc.trace
Total loads: 318197
Total stores: 197486
Load hits: 315755
Load misses: 2442
Store hits: 188603
Store misses: 8883
Total cycles: 5947958
Hit Rate (%): 97.8039
Total Cache Size: 131072
Overhead cache size: 49152
aimi@Aimis-MacBook-Pro hw3 % ./csim 512 4 16 write-allocate write-back lru < gcc.trace
Total loads: 318197
Total stores: 197486
Load hits: 315211
Load misses: 2986
Store hits: 188468
Store misses: 9018
Total cycles: 8595679
Hit Rate (%): 97.6722
Total Cache Size: 32768
Overhead cache size: 12288
aimi@Aimis-MacBook-Pro hw3 % ./csim 256 4 16 write-allocate write-back lru < gcc.trace
Total loads: 318197
Total stores: 197486
Load hits: 314798
Load misses: 3399
Store hits: 188250
Store misses: 9236
Total cycles: 9331848
Hit Rate (%): 97.5499
Total Cache Size: 16384
Overhead cache size: 6144
aimi@Aimis-MacBook-Pro hw3 % ./csim 128 4 16 write-allocate write-back lru < gcc.trace
Total loads: 318197
Total stores: 197486
Load hits: 314007
Load misses: 4190
Store hits: 188015
Store misses: 9471
Total cycles: 10058022
Hit Rate (%): 97.3509
Total Cache Size: 8192
Overhead cache size: 3072
aimi@Aimis-MacBook-Pro hw3 % ./csim 1 64 16 write-allocate write-back lru < gcc.trace
Total loads: 318197
Total stores: 197486
Load hits: 300027
Load misses: 18170
Store hits: 184063
Store misses: 13423
Total cycles: 19929690
Hit Rate (%): 93.8736
Total Cache Size: 1024
Overhead cache size: 384
aimi@Aimis-MacBook-Pro hw3 % ./csim 1 128 16 write-allocate write-back lru < gcc.trace
Total loads: 318197
Total stores: 197486
Load hits: 310493
Load misses: 7704
Store hits: 186857
Store misses: 10629
Total cycles: 12636550
Hit Rate (%): 96.4449
Total Cache Size: 2048
Overhead cache size: 768
aimi@Aimis-MacBook-Pro hw3 % ./csim 1 256 16 write-allocate write-back lru < gcc.trace
Total loads: 318197
Total stores: 197486
Load hits: 313123
Load misses: 5074
Store hits: 187653
Store misses: 9833
Total cycles: 10787576
Hit Rate (%): 97.1093
Total Cache Size: 4096
Overhead cache size: 1536
aimi@Aimis-MacBook-Pro hw3 % ./csim 1 512 16 write-allocate write-back lru < gcc.trace
Total loads: 318197
Total stores: 197486
Load hits: 314275
Load misses: 3922
Store hits: 188096
Store misses: 9390
Total cycles: 9867571
Hit Rate (%): 97.4186
Total Cache Size: 8192
Overhead cache size: 3072
/// END Experiemnt 3///

From the results I conclude that 2048 16 16 write-allocate write-back lru
 is the best configuration. So set associative is the best.
