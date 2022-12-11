Name: Aimi Sekiguchi
JHED: asekigu1

Name: Ryad Taleb
JHED: rtaleb1

Contributions:
We mostly implemented and debugged synchronously.

Synchronization report:
We manipulate pthreads using pthread_mutex_init and
pthread_mutex_destroy, for MessageQueue, Room, and 
the Server. This allows us to lock certain functions
in these classes, ensuring that only one client 
call is executing at a time, and not interfering 
with any variables while executing.

We use a semaphore in MessageQueue, and use the 
functions sem_init, sem_destroy, sem_post, and 
sem_timedwait. Semaphore is only used in MessageQueue
because in MessageQueue we need to be able to notify
if messages are available for send. Rooms and 
servers do not need to be regularly checked for new
elements. The semaphore allows the code to sleep 
until there is a new message, and then wake up 
when a message is available.

We synchronize using Guard g(lock) instead of explicitly calling
pthread_mutex_lock, as specified in the instructions. This help
avoid deadlocks. Below are the instances we lock, and why.

MessageQueue:
For MessageQueue enqeue() and dequeue(), we mostly just followed 
the TODO insturctions. Since multiple senders can send messages 
at once, there can be simultaneaous enqueue() calls caused by different
senders. Therefore, it is important to first lock the thread, and 
then to avoid deadlock, we call sem_post.

dequeue() is also a critical section becase it pops the queue,
and we must ensure the message is not lost with simultaneous calls.
Therefore we lock the thread before popping the queue.

Room:
add_member(), remove_member(), are both critical sections because
we are modifying the members list, therefore we must lock it before 
modification to ensure only one edit happens at a time, and that the
list updates properly.

for broadcast_message(), although the function itself doesn't modify 
the users, we are iterating through all its members. Therefore it is 
a critical section because we do not want other edits of the list while 
we are iterating. So we also lock the thread at the beginning of this function.

Server:
In handle_client_requests(), we create a new thread for each new client,
as instructed.

In find_or_create_room(), we are inserting a new room if it doesn't
exist. Therefore, to only have one client editing m_rooms, it is neccesary
to lock.