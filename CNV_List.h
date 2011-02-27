/*
 * =====================================================================================
 *
 *       Filename:  CNV_List.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  02/26/2011 02:43:14 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */

#ifndef  CNV_LIST_H
#define  CNV_LIST_H

#include <stdlib.h>

//===============================
// Type and constant definition
//===============================

// type define of the head of a size data list
typedef struct sizeListHead CNV_SizeList;

// type define of a node of a size data list
typedef struct sizeListNode CNV_SizeListNode;

// list head of a size list
struct sizeListHead
{
    size_t count;               // number of elements in the list
    CNV_SizeListNode* first;    // first node in the list
    CNV_SizeListNode* last;     // last node in the list
};

// list node to store size related data (coordinates, index, count, etc.)
struct sizeListNode
{
    size_t sizeData;           // size data
    CNV_SizeListNode* prev;    // previous node
    CNV_SizeListNode* next;    // next node
};


//===============================
// Constructors and Destructors
//===============================

// create a new size list
CNV_SizeList* CNV_SizeListAlloc(void);

// create a new size list node
CNV_SizeListNode* CNV_SizeListNodeAlloc(size_t sizeData);

// free a size list
void CNV_SizeListFree(CNV_SizeList* sizeList);


//===============================
// Non-constant methods
//===============================

// add a new node to the end of a size list
int CNV_SizeListPush(CNV_SizeList* sizeList, CNV_SizeListNode* newNode);

// add a new node to the beginning of a size list
int CNV_SizeListUnshift(CNV_SizeList* sizeList, CNV_SizeListNode* newNode);

// delete the elements in a size data list according to the indecies in another size data list
int CNV_SizeListDelFromList(CNV_SizeList* sizeList, const CNV_SizeList* delIndecies, CNV_SizeList* deletedElements);

#endif  /*CNV_LIST_H*/
