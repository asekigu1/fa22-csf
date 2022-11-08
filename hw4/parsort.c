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

void merge(int64_t *arr, size_t begin, size_t mid, size_t end, int64_t *temparr);
void merge_sort(int64_t *arr, size_t begin, size_t end, size_t threshold);
int do_child_work(int64_t *arr, size_t begin, size_t end, size_t threshold);


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

int cmpvals(const void *p1, const void *p2){
  return ( *(int64_t*)p1 - *(int64_t*)p2 );
}

void merge_sort(int64_t *arr, size_t begin, size_t end, size_t threshold) {
  // TODO: implement
  size_t numElements = end - begin;
  if ((numElements) < threshold ) {
    qsort(arr, numElements, sizeof(int64_t), cmpvals);
  } else {
    pid_t pid = fork();
    size_t mid = (begin+end)/2;
    if (pid == -1) {
      // fork failed to start a new process
      // handle the error and exit
      fprintf(stderr, "Error: fork failed to start a new process\n");
      exit(1);
    } else if (pid == 0) {
      int retcode = do_child_work(arr, begin, mid, threshold);
      exit(retcode);
      // everything past here is now unreachable in the child
    } else {
      // parent process
      begin = mid + 1;
      merge_sort(arr, begin, end, threshold);
    }
    
    int wstatus;
    // blocks until the process indentified by pid_to_wait_for completes
    pid_t actual_pid = waitpid(pid, &wstatus, 0);
    if (actual_pid == -1) {
      // handle waitpid failure
      fprintf(stderr, "Error: waitpid failure\n");
      exit(1); // exit with 1 is going to terminate the entire process
    }

    if (!WIFEXITED(wstatus)) {
      // subprocess crashed, was interrupted, or did not exit normally
      // handle as error
      fprintf(stderr, "Error: subprocess crashed, was interrupted, or did not exit normally\n");
      exit(1);
    }
    if (WEXITSTATUS(wstatus) != 0) {
      // subprocess returned a non-zero exit code
      // if following standard UNIX conventions, this is also an error
      fprintf(stderr, "Error: subprocess returned a non-zero exit code\n");
      exit(1);
    }
    // if pid is not 0, we are in the parent process
    // WARNING, if the child process path can get here, things will quickly break very badly

    int64_t arr[end-begin]; //clear temparr
    int64_t *temparr;
    temparr = arr;
    merge(arr, begin, mid, end, temparr);

    for (size_t i = 0; i < end; i++) {
      //update original array
      arr[i] = temparr[i];
    }
  }
  
}



int do_child_work(int64_t *arr, size_t begin, size_t end, size_t threshold) {
  // this is now in the child process
  merge_sort(arr, begin, end, threshold);
  return 1;
  // TODO: a child process not exiting normally, or exiting with a non-zero exit code?
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
    fprintf(stderr, "Error: invalid threshold value\n");
    return 1;
  }
    

  // TODO: open the file
  int fd = open(filename, O_RDWR);
  if (fd < 0) {
    // file couldn't be opened: handle error and exit
    fprintf(stderr, "Error: file couldn't be opened\n");
    return 1;
  }

  // TODO: use fstat to determine the size of the file
  struct stat statbuf;
  int rc = fstat(fd, &statbuf);
  if (rc != 0) {
    // handle fstat error and exit
    fprintf(stderr, "Error: fstat did not execute corretly\n");
    return 1;
  }
  size_t file_size_in_bytes = statbuf.st_size;

  // TODO: map the file into memory using mmap
  int64_t *data = mmap(NULL, file_size_in_bytes, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  if (data == MAP_FAILED) {
    // handle mmap error and exit
    fprintf(stderr, "Error: mmap did not execute correctly\n");
    return 1;
  }
  // *data now behaves like a standard array of int64_t. Be careful though! Going off the end
  // of the array will silently extend the file, which can rapidly lead to disk space
  // depletion!

  // TODO: sort the data!
  size_t numElements = file_size_in_bytes/sizeof(int64_t);
  merge_sort(data, 0, numElements, threshold);

  // TODO: unmap and close the file
  int retVal = munmap(data, file_size_in_bytes);
  if (retVal == -1) {
    fprintf(stderr, "Error: failure to munmap the file data\n");
    return 1;
  }
  retVal = close(fd);
  if (retVal == -1) {
    fprintf(stderr, "Error: failure to close the file\n");
    return 1;
  }

  // TODO: exit with a 0 exit code if sort was successful
  return 0;
}
