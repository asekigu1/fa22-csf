Name: Aimi Sekiguchi
JHED: asekigu1

Name: Ryad Taleb
JHED: rtaleb1

Contributions:
We mostly implemented and debugged synchronously.

Synchronization report:
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