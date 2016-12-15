#ifndef CALLTREE_H_
#define CALLTREE_H_

#include "Memory.h"

typedef void(*CallPtr)();

typedef struct _CallTree
{
	CallPtr* funTree;
	uint8_t curFun;
	uint8_t max_size;
} CallTree;

//CallTree* AllocCallTree(uint8_t treeSize);
//void FreeCallTree(CallTree* ptree);

#define CALLTREE_ALLOC(ptree, maxsize) ptree = allocMemory(sizeof(CallTree)); \
	ptree->funTree = allocMemory(maxsize*sizeof(CallPtr)); \
	ptree->max_size = maxsize; \
	ptree->curFun = 0;

#define CALLTREE_FREE(ptree) freeMemory(ptree->funTree, \
		ptree->max_size * sizeof(CallPtr)); \
		freeMemory(ptree, sizeof(CallTree));

#define CALLTREE_ASCEND(fun, ptree) (ptree)->funTree[ptree->curFun] \
	= (fun); ((ptree)->curFun) += 1;

#define CALLTREE_DESCEND(ptree) if(ptree->curFun > 0) \
		{ \
			((ptree)->curFun) -= 1; \
			((ptree)->funTree[(ptree)->curFun])(); \
		}

#endif
