
#include "Memory.h"

static char* pointerToFreeMemory = (char*)0x20010000;
static int firstCellAdress = 0x20010000;
static int lastCellAdress = 0x20017FFF;
// static int memorySize = 32767;
static int remainingMemory = 32767;

void* allocMemory( int size )
{
	if( remainingMemory < size )
	{
		return 0;
	}
	void* p = pointerToFreeMemory;

	pointerToFreeMemory += size;
	remainingMemory -= size;
	return p;
}

void* allocTable(int tabsize, int typesize )
{
	if( remainingMemory < tabsize*typesize )
	{
		return 0;
	}

	void* p = pointerToFreeMemory;

	pointerToFreeMemory += tabsize*typesize;
	remainingMemory -= tabsize*typesize;
	return p;
}

short freeMemory( void* adress, int size )
{
	if( adress < firstCellAdress || adress > lastCellAdress )
	{
		return 1;
	}
	if( adress + size > lastCellAdress )
	{
		return 2;
	}
	int* cellToRead = adress + size;
	int* cellToWrite = adress;
	int i;
	for( i = 0; i < pointerToFreeMemory - (char*)adress - size; i ++)
	{
		*cellToWrite = *cellToRead;
		cellToWrite++;
		cellToRead++;
	}
	remainingMemory += size;
	return 0;
}

short freeTable( void* adress, int tabsize, int typesize )
{
	return freeMemory( adress, tabsize*typesize );
}


void resetMemory( )
{
	pointerToFreeMemory = (char*)firstCellAdress;
	remainingMemory = 32767;
}
