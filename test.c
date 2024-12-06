#include <stdio.h>
#include <pthread.h>

const long NT = 12;

void* task(void* thread_id)
{
  long tnumber = (long) thread_id; 
  printf("Hello World from thread %ld\n",1+tnumber);
}

int main(int argc,char** argv)
{
  int success;
  long i;
  pthread_t threads[NT];

  for(i=0; i<NT; ++i) {
    success = pthread_create(&threads[i],NULL,task,(void*)i);
    if (success != 0) {
      printf("ERROR: Unable to create worker thread %ld successfully\n",i);
      return 1;
    }
  }
  for(i=0; i<NT; ++i) {
    pthread_join(threads[i],NULL);
  }
  return 0;
}