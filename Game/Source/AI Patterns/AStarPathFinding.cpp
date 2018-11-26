#include "GamePCH.h"

Pathfinder::Pathfinder(Level* pTilemap)
{
	m_pTilemap = pTilemap;

	m_NumNodes = LEVEL_TILE_DIMENSIONS.x *  LEVEL_TILE_DIMENSIONS.y;

	m_Nodes = new PathNode[m_NumNodes];
	m_OpenNodes = new int[m_NumNodes];

	m_MapWidth = LEVEL_TILE_DIMENSIONS.x;
	m_MapHeight = LEVEL_TILE_DIMENSIONS.y;

	Reset();
}

Pathfinder::~Pathfinder()
{
	delete[] m_Nodes;
	delete[] m_OpenNodes;
}

void Pathfinder::Reset()
{
	m_NumOpen = 0;

	for (int i = 0; i < m_NumNodes; i++)
	{
		m_Nodes[i].parentNodeIndex = -1;
		m_Nodes[i].status = PathNode::Unchecked;

		m_Nodes[i].f = 0;
		m_Nodes[i].g = FLT_MAX; // Set G to be highest cost possible, so any comparison will be better.
		m_Nodes[i].h = -1; // -1 indicates the heuristic hasn't been calculated yet.
	}
}

bool Pathfinder::FindPath(int sx, int sy, int ex, int ey)
{
	// Reset the pathfinder.
	Reset();

	// Get the starting tile index and the destination tile index.
	int startIndex = CalculateNodeIndex(sx, sy);
	int endIndex = CalculateNodeIndex(ex, ey);

	// Set starting node cost to zero, then add it to the open list to start the process.
	m_Nodes[startIndex].f = 0;
	m_Nodes[startIndex].g = 0;
	m_Nodes[startIndex].h = 0;
	AddToOpen(startIndex);

	while (true)
	{
		// Find the lowest F and remove it from the open list.
		int fIndex = m_OpenNodes[FindNodeIndexWithLowestFInOpen()];
		RemoveFromOpen(fIndex);

		// If we found the end node, we're done.
		if (fIndex == endIndex)
			return true;

		// Mark the node as closed.
		m_Nodes[fIndex].status = PathNode::Closed;

		// Add neighbours to open list.
		AddNeighboursToOpenList(fIndex, endIndex);

		// If we're out of nodes to check, then we're done without finding the end node.
		if (m_NumOpen == 0)
			return false;
	}

	// This shouldn't happen, we should either find the destination or run out of nodes in the open list.
	assert(false);
	return false;
}

int Pathfinder::GetPath(int* path, int maxdistance, int ex, int ey)
{
	int nodeIndex = CalculateNodeIndex(ex, ey);

	int length = 0;
	while (true)
	{
		if (path != nullptr && length >= maxdistance)
			return -1; // Path didn't fit in size.

		if (path != nullptr) // If no path array is passed in, just get the length.
			path[length] = nodeIndex;
		length++;

		nodeIndex = m_Nodes[nodeIndex].parentNodeIndex;

		if (nodeIndex == -1)
			return length;
	}

	// There should be no way to get here.
	assert(false);
	return -1;
}

void Pathfinder::AddToOpen(int nodeindex)
{
	assert(m_Nodes[nodeindex].status != PathNode::Closed);

	// If the node isn't already open, then add it to the list.
	if (m_Nodes[nodeindex].status != PathNode::Open)
	{
		m_OpenNodes[m_NumOpen] = nodeindex;
		m_NumOpen++;
		m_Nodes[nodeindex].status = PathNode::Open;
	}
}

void Pathfinder::RemoveFromOpen(int nodeindex)
{
	// Remove the node from the open list, since we don't care about order, replace the node we're removing with the last node in list.
	for (int i = 0; i < m_NumOpen; i++)
	{
		if (m_OpenNodes[i] == nodeindex)
		{
			m_NumOpen--;
			m_OpenNodes[i] = m_OpenNodes[m_NumOpen];
			return;
		}
	}


}

int Pathfinder::FindNodeIndexWithLowestFInOpen()
{
	float lowestF = FLT_MAX;
	int index;

	// Loop through the nodes in the open list, then find and return the node with the lowest F score.
	for (int i = 0; i < m_NumOpen; i++)
	{
		if (m_Nodes[m_OpenNodes[i]].f < lowestF)
		{
			lowestF = m_Nodes[m_OpenNodes[i]].f;
			index = i;
		}
	}

	assert(lowestF != FLT_MAX);
	return index;

}

int Pathfinder::CalculateNodeIndex(int tx, int ty)
{

	assert(tx >= 0 && tx < m_MapWidth &&
		ty >= 0 && ty < m_MapHeight);

	// Calculate the node index based on the tiles x/y.
	return (tx + ty * m_MapWidth);
}

int Pathfinder::CheckIfNodeIsClearAndReturnNodeIndex(int tx, int ty)
{
	// If the node is out of bounds, return -1 (an invalid tile index).
	if (tx < 0 || tx >= m_MapWidth || ty < 0 || ty >= m_MapHeight)
		return -1;

	int index = CalculateNodeIndex(tx, ty);

	// If the node is already closed, return -1 (an invalid tile index).
	if (m_Nodes[index].status == PathNode::Closed)
		return -1;

	// If the node can't be walked on, return -1 (an invalid tile index).
	if (!m_pTilemap->GetTileAtPosition(tx, ty)->IsWalkable())
		return -1;

	return index;
	// Return a valid tile index.

}

void Pathfinder::AddNeighboursToOpenList(int nodeIndex, int endNodeIndex)
{
	// Calculate the tile x/y based on the nodeIndex.
	int x = nodeIndex % m_MapWidth;
	int y = nodeIndex / m_MapWidth;

	// Fill an array with the four neighbour tile indices. (use CheckIfNodeIsClearAndReturnNodeIndex() for each to see if it's valid).
	int nodestoadd[4];
	nodestoadd[0] = CheckIfNodeIsClearAndReturnNodeIndex(x + 1, y);
	nodestoadd[1] = CheckIfNodeIsClearAndReturnNodeIndex(x - 1, y);
	nodestoadd[2] = CheckIfNodeIsClearAndReturnNodeIndex(x, y + 1);
	nodestoadd[3] = CheckIfNodeIsClearAndReturnNodeIndex(x, y - 1);

	// Loop through the array.
	for (int i = 0; i < 4; i++)
	{
		// Check if the node to add has a valid node index.
		if (nodestoadd[i] != -1)
		{
			int cost = 1; // Assume a travel cost of 1 for each tile.

			// Add the node to the open list.
			AddToOpen(nodestoadd[i]);

			// If the cost to get there from here (new G) is less than the previous cost (old G) to get there, then overwrite the values.
			if (m_Nodes[nodeIndex].g + cost < m_Nodes[nodestoadd[i]].g)
			{
				// Set the parent node.
				m_Nodes[nodestoadd[i]].parentNodeIndex = nodeIndex;

				// Set the new cost to travel to that node.
				m_Nodes[nodestoadd[i]].g = m_Nodes[nodeIndex].g + cost;

				// If we haven't already calculated the heuristic, calculate it.
				m_Nodes[nodestoadd[i]].h = CalculateH(nodestoadd[i], endNodeIndex);

				// Calculate the final value.
				m_Nodes[nodestoadd[i]].f = m_Nodes[nodestoadd[i]].g + m_Nodes[nodestoadd[i]].h;
			}
		}
	}
}

float Pathfinder::CalculateH(int nodeIndex1, int nodeIndex2)
{
	// Calculate the h score using the Manhatten distance (absolute diff in x + absolute diff in y).
	return (float)abs(nodeIndex1 % m_MapWidth - nodeIndex2 % m_MapWidth) + abs(nodeIndex1 / m_MapHeight - nodeIndex2 / m_MapHeight);
}