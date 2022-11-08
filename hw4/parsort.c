#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <assert.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

void merge(int64_t *arr, size_t begin, size_t mid, size_t end, int64_t *temparr) {
  // TODO: implement
  size_t leftIndex = 0;
  size_t rightIndex = mid+1;
  size_t tempIndex = 0;

  while ((leftIndex < mid) && (rightIndex < end)) {
    if (arr[leftIndex] <= arr[rightIndex]) {
      temparr[tempIndex] = arr[leftIndex];
      leftIndex++;
    } else {
      temparr[tempIndex] = arr[rightIndex];
      rightIndex++;
    }
    tempIndex++;
  }

  while (leftIndex < mid) {
    temparr[tempIndex] = arr[leftIndex];
    leftIndex++;
    tempIndex++;
  }

  while (rightIndex < end) {
    temparr[tempIndex] = arr[rightIndex];
    rightIndex++;
    tempIndex++;
  }
}

cmpvals(const void *p1, const void *p2){
  return cmp(*(const int64_t **) p1, *(const int64_t **) p2);
}

void merge_sort(int64_t *arr, size_t begin, size_t end, size_t threshold) {
  // TODO: implement
  if ((end-begin) < threshold ) {
    qsort(arr, end-begin, sizeof(int64_t*), cmpvals);
  } else {
    size_t newBeg = 0;
    size_t newEnd = 0;
    pid_t pid = fork();
    if (pid == -1) {
        // fork failed to start a new process
        // handle the error and exit
    } else if (pid == 0) {
      // this is now in the child process
      newBeg = (begin+end)/2 + 1;
      newEnd = end;
    } else {
      // parent process
      newBeg = begin;
      newEnd = (begin+end)/2;
    }
    merge_sort(arr, newBeg, newEnd, threshold);

    if (pid == 0) {
      int retcode = do_child_work();
      exit(retcode);
      // everything past here is now unreachable in the child
    }
    // if pid is not 0, we are in the parent process
    // WARNING, if the child process path can get here, things will quickly break very badly

    int64_t *temparr = new int64_t[end-begin]; //clear temparr
    merge(arr, begin, newEnd, end, temparr);

    for (size_t i = 0; i < end; i++) {
      //update original array
      arr[i] = temparr[i];
    }
  }
  
}

int main(int argc, char **argv) {
  // check for correct number of command line arguments
  if (argc != 3) {
    fprintf(stderr, "Usage: %s <filename> <sequential threshold>\n", argv[0]);
    return 1;
  }

  // process command line arguments
  const char *filename = argv[1];
  char *end;
  size_t threshold = (size_t) strtoul(argv[2], &end, 10);
  if (end != argv[2] + strlen(argv[2])) {
    /* TODO: report an error (threshold value is invalid) */;

  }
    

  // TODO: open the file
  int fd = open(filename, O_RDWR);
  if (fd < 0) {
    // file couldn't be opened: handle error and exit
  }

  // TODO: use fstat to determine the size of the file
  struct stat statbuf;
  int rc = fstat(fd, &statbuf);
  if (rc != 0) {
      // handle fstat error and exit
  }
  size_t file_size_in_bytes = statbuf.st_size;

  // TODO: map the file into memory using mmap
  int64_t *data = mmap(NULL, file_size_in_bytes, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0)
  if (data == MAP_FAILED) {
      // handle mmap error and exit
  }
  // *data now behaves like a standard array of int64_t. Be careful though! Going off the end
  // of the array will silently extend the file, which can rapidly lead to disk space
  // depletion!

  // TODO: sort the data!

  // TODO: unmap and close the file

  // TODO: exit with a 0 exit code if sort was successful
}
