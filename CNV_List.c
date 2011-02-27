/*
 * =====================================================================================
 *
 *       Filename:  CNV_List.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  02/26/2011 03:04:26 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */

#include <stdio.h>
#include <assert.h>

#include "CNV_List.h"
#include "CNV_Error.h"
#include "CNV_Types.h"


//===============================
// Constructors and Destructors
//===============================

// create a new size list
CNV_SizeList* CNV_SizeListAlloc(void)
{
    CNV_SizeList* sizeList = (CNV_SizeList*) malloc(sizeof(CNV_SizeList));
    if (sizeList == NULL)
        CNV_ErrSys("ERROR: Not enough memory for a size list.\n");

    sizeList->count = 0;
    sizeList->first = NULL;
    sizeList->last  = NULL;

    return sizeList;
}

// create a new size list node
CNV_SizeListNode* CNV_SizeListNodeAlloc(size_t sizeData)
{
    CNV_SizeListNode* newNode = (CNV_SizeListNode*) malloc(sizeof(CNV_SizeListNode));
    if (newNode == NULL)
        CNV_ErrSys("ERROR: Not enough memory for a node of a size data list.\n");

    newNode->sizeData = sizeData;
    newNode->prev     = NULL;
    newNode->next     = NULL;

    return newNode;
}

// free a size list
void CNV_SizeListFree(CNV_SizeList* sizeList)
{
    assert(sizeList != NULL);

    // node to be deleted
    CNV_SizeListNode* toBeDelNode = sizeList->first;
    while (toBeDelNode != NULL)
    {
        // next node to be deleted
        CNV_SizeListNode* nextNode = toBeDelNode->next;
        free(toBeDelNode);
        toBeDelNode = nextNode;
    }

    // free the list head
    free(sizeList);
}


//===============================
// Non-constant methods
//===============================

// add a new size data to the end of a size list
int CNV_SizeListPush(CNV_SizeList* sizeList, CNV_SizeListNode* newNode)
{
    assert(sizeList != NULL && newNode != NULL);

    CNV_SizeListNode* lastNode = sizeList->last;
    if (sizeList->count == 0)                   // if the list is empty the new node is also the first node in the list
    {
        sizeList->first = newNode;
        newNode->prev   = NULL;
    }
    else                                       // if the list is not empty attach the new node at the end of the list
    {
        lastNode->next = newNode;
        newNode->prev  = lastNode;
    }

    newNode->next  = NULL;
    sizeList->last = newNode;
    ++(sizeList->count);

    return CNV_OK;
}

// add a new size data to the end of a size list
int CNV_SizeListUnshift(CNV_SizeList* sizeList, CNV_SizeListNode* newNode)
{
    assert(sizeList != NULL && newNode != NULL);

    CNV_SizeListNode* firstNode = sizeList->first;
    if (sizeList->count == 0)                 // if the list is empty the new node is also the last node in the list
    {
        sizeList->last = newNode;
        newNode->next  = NULL;
    }
    else                                     // if the list is not empty attach the new node at the beginning of the list
    {
        firstNode->prev = newNode;
        newNode->next   = firstNode;
    }

    newNode->prev   = NULL;
    sizeList->first = newNode;
    ++(sizeList->count);

    return CNV_OK;
}

// delete the elements in a size data list according to the indecies in another size data list
int CNV_SizeListDelFromList(CNV_SizeList* sizeList, const CNV_SizeList* delIndecies, CNV_SizeList* deletedElements)
{
    assert(sizeList != NULL && sizeList->count > 0 && delIndecies != NULL && delIndecies->count > 0);
    
    size_t currIndex = sizeList->count - 1;            // current index in the size data list
    CNV_SizeListNode* currNode = sizeList->last;       // current node with index in currIndex variable
    CNV_SizeListNode* delIndex = delIndecies->last;    // the index of next to-be-deleted node

    while (delIndex != NULL)
    {
        assert(currIndex >= delIndex->sizeData);

        // go to next to-be-deleted node
        while (currIndex > delIndex->sizeData)
        {
            --currIndex;
            currNode = currNode->prev;
        }

        // check if the node is the first one in the list
        if (currNode != sizeList->first)
            (currNode->prev)->next = currNode->next;
        else
        {
            sizeList->first = currNode->next;
            (currNode->next)->prev = NULL;
        }

        // check if the node is the last one in the list
        if (currNode != sizeList->last)
            (currNode->next)->prev = currNode->prev;
        else
        {
            sizeList->last = currNode->prev;
            (currNode->prev)->next = NULL;
        }

        // prepare to delete the node
        CNV_SizeListNode* toBeDelNode = currNode;

        --currIndex;
        currNode = currNode->prev;
        delIndex = delIndex->prev;

        // move the node to another list or free it directly
        if (deletedElements != NULL)
            CNV_SizeListUnshift(deletedElements, toBeDelNode);
        else
            free(toBeDelNode);
    }

    // update the number of elements in the size list
    sizeList->count -= delIndecies->count;
    return CNV_OK;
}
