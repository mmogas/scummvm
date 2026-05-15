/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "PointList.h"

// Default constructor: initializes an empty point list node
CPointList::CPointList()
{
	First = 0;
	Count = 0;

	Next = NULL;
	Prev = NULL;
}

// Parameterized constructor: initializes a node with a specific range
CPointList::CPointList(int first, int count)
{
	First = first;
	Count = count;

	Next = NULL;
	Prev = NULL;
}

// Destructor
CPointList::~CPointList()
{
}

/**
 * Adds a new range of points to the list.
 * It handles insertion, sorting, and basic merging of adjacent ranges.
 */
void CPointList::Add(int first, int count)
{
	// If the list is empty (Next is NULL), create the first element
	if (Next == NULL)
	{
		Next = new CPointList(first, count);
	}
	else
	{
		CPointList* scan = Next;
		while (scan != NULL)
		{
			int last = first + count;
			
			// Case 1: Insert before the current node if the new range precedes it
			if (scan->First < first && scan->First < last)
			{
				CPointList* pL = new CPointList(first, count);
				pL->Prev = scan->Prev;
				pL->Next = scan;
				scan->Prev = pL;
				break;
			}
			// Case 2: New range ends exactly where current node starts -> Merge at start
			else if (scan->First == last)
			{
				scan->First = first;
				// TODO: Check if this update allows merging with the previous node
				break;
			}
			// Case 3: Current node ends exactly where new range starts -> Expand count
			else if ((scan->First + scan->Count) == first)
			{
				scan->Count += count;
				// TODO: Check if this expansion allows merging with the next node
				break;
			}

			scan = scan->Next;
		}
	}
}

/**
 * Removes a range of points from the list.
 * It identifies if a node should be deleted entirely or modified.
 */
void CPointList::Remove(int first, int count)
{
	int last = first + count;

	CPointList* scan = Next;
	while (scan != NULL)
	{
		int vertexStart = scan->First;
		int verticeCount = scan->Count;
		int vertexEnd = vertexStart + verticeCount;

		// Case A: The range to remove covers the entire current node
		if (first <= vertexStart && last >= vertexEnd)
		{
			// TODO: Complete removal logic (re-linking Prev/Next)
			CPointList* pDel = scan;
			scan = scan->Next;
			if (Next == pDel)
			{
				Next = scan;
			}
			delete pDel;
			break;
		}
		// Case B: The removal starts within the current node's range
		else if (first >= vertexStart && first < vertexEnd)
		{
			if (first == vertexStart)
			{
				// Remove from the beginning of the node: adjust start index and count
				scan->First += last - vertexStart;
				scan->Count -= last - vertexStart;
			}
			else
			{
				// TODO: Handle removal from the middle (requires splitting one node into two)
				int debug = 0;
			}
		}
		// Case C: The removal ends within the current node's range
		else if (last >= vertexStart && last < vertexEnd)
		{
			// TODO: Handle partial removal at the end
			int debug = 0;
		}

		scan = scan->Next;
	}
}

/**
 * Clears the entire list by deleting all linked nodes.
 */
void CPointList::Clear()
{
	while (Next != NULL)
	{
		CPointList* pDel = Next;
		Next = pDel->Next;
		delete Next; // Note: Potential bug here, should delete pDel instead of Next
	}
}
