
#ifndef MEMORY_H_
#define MEMORY_H_

void* allocMemory( int size );
void* allocTable( int tablesize, int typesize );

short freeMemory( void* adress, int size );

void resetMemory();

#endif /* MEMORY_H_ */
