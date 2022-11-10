Name: Aimi Sekiguchi
JHED: asekigu1
Contributions: main(), parallel mergesort implementation

Name: Ryad Taleb
JHED: rtaleb1
Contributions: Mergesort() original implementation, debugging


REPORT
TODO: add your report according to the instructions in the
"Experiments and analysis" section of the assignment description.

Threshold: 2097152 
real    0m0.985s
user    0m0.942s
sys     0m0.032s

Threshold: 1048576
real    0m0.634s
user    0m1.076s
sys     0m0.074s

Threshold: 524288
real    0m0.318s
user    0m0.765s
sys     0m0.090s

Threshold: 262144
real    0m0.247s
user    0m0.800s
sys     0m0.122s

Threshold: 131072
real    0m0.229s
user    0m0.909s
sys     0m0.180s

Threshold: 65536
real    0m0.214s
user    0m0.921s
sys     0m0.218s

Threshold: 32768
real    0m0.200s
user    0m0.896s
sys     0m0.329s

Threshold: 16384
real    0m0.186s
user    0m0.929s
sys     0m0.461s

fork() is called twice to recursively call merge_sort(), for 
the first and second half of the array. Each recursive call is done 
in a separate child process. They can run in parallel because the 
two halves of the array are completely independent. This allows 
the OS Kernel to schedule the recursive computations in parallel. 

As we can see from our test results, as we decrease the threshold,
 the program speed increased. This suggests the recursive calls 
 are successfully executing in parallel. However, looking at the decrease percentages:

From threshold 2097152 to 1048576 (50% decrease), the real time decreases from 0.985s to 0.634s (about 36% decrease).
Similarly, from threshold 1048576 to 524288 (50% decrease), the real time decreases from 0.634s to 0.318s (about 50% decrease).
Going from threshold 524288 to 262144 (50% decrease), the real time decreases from 0.318s to 0.247s (about 23% decrease).
Going from threshold 262144 to 131072 (50% decrease), the real time decreases from 0.247s to 0.229s (about 7% decrease).
Going from threshold 131072 to 65536 (50% decrease), the real time decreases from 0.229s to 0.214s (about 7% decrease).
Going from threshold 65536 to 32768 (50% decrease), the real time decreases from 0.214s to 0.200s  (about 7% decrease).
Going from threshold 32768 to 16384 (50% decrease), the real time decreases from 0.200s to 0.186s (about 7% decrease).

We see that the decrease rate plateaus after the fourth threshold value. 
This could be due all the available CPU cores being used for parallel 
processes. Then, even if more processes could hypothetically be run in 
parallel, the Kernel must still execute the process after the previous 
execution completes and a slot opens. Therefore, in real life, we reach 
diminishing returns after the threshold hits below 262144.