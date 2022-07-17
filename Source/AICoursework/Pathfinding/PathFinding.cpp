// Fill out your copyright notice in the Description page of Project Settings.


#include "PathFinding.h"
#include <AICoursework/MapController.h>
#include <AICoursework/Pathfinding/Node.h>

PathFinding::PathFinding()
{
}

TArray<FVector2D> PathFinding::FindPath(FVector2D startPos, FVector2D targetPos, AMapController* mapRef)
{
	//start pos and target pos need to the psotions of the nodes 
	gridSize = mapRef->gridSize;
	mapSize = mapRef->mapSize;
	//then we set the start and target nodes
	Node* startNode = mapRef->GetNode(startPos);	//start node
	Node* targetNode = mapRef->GetNode(targetPos);	//end node
	openSet.Add(startNode);	//adds the start node to the open set array
	if(startNode == nullptr || targetNode == nullptr) { return TArray<FVector2D>(); }
	while (openSet.Num() > 0)	//this will loop through the open set as long as their is something in openSet
	{
		currentNode = openSet[0];	//this sets the current node to the firat value in the open set array, first pass it will be the start node
		int currentNodeIndex = 0;	//this is needed to remove current node from the open set later
		for (int i = 0; i < openSet.Num();i++)	//this will loop through the open set array
		{
			if (openSet[i] != nullptr && currentNode != nullptr)
			{
				//checks if the node in open set has a lower f cost than the current node or 
			//if the f costs are the same check that the open set h cost is less than the current node h cost
				if (mapRef->IsNodeWalkable(openSet[i]->GetNodePos()) == false)
				{
					if (openSet[i]->GetFCost() < currentNode->GetFCost() || openSet[i]->GetFCost() == currentNode->GetFCost() && openSet[i]->GetHCost() < currentNode->GetHCost())
					{
						currentNode = openSet[i];	//this will update the current node to the new lower fcost node
						currentNodeIndex = i;		//this then sets the node index to i so it can be removed later on 
					}
				}	
			}	
		}
		openSet.RemoveAt(currentNodeIndex);	//this removes the node at the index from  the open set as it has the lowest f cost or hcost on the pass through the loop
		closedSet.Add(currentNode);		//then as its the lowest cost it's added to the closed set so its not edited later

		//this checks if the current node is our target node if so we have reached the end of the path
		if (currentNode->GetNodePos() == targetNode->GetNodePos())
		{//we have reached the end of thbe path so return the retraced path
			return RetracePath(startNode, targetNode);
		}
		TArray<Node*> neighbouringNodes = GetNeighbours(currentNode, mapRef);	//this gets an array of the neigbouring nodes

		for (int i = 0; i < neighbouringNodes.Num();i++)
		{
			if (neighbouringNodes[i] != nullptr)
			{//would add here if the node is walkable | check if that node is already in the closed set
				if (mapRef->IsNodeWalkable(neighbouringNodes[i]->GetNodePos())) { continue; }

				if (ClosedArrayContain(neighbouringNodes[i]) == true) { continue; }

				int newMovementCost = currentNode->GetGCost() + GetDistance(currentNode, neighbouringNodes[i], mapRef);
				if (newMovementCost < neighbouringNodes[i]->GetGCost() || !OpenArrayContian(neighbouringNodes[i]))
				{//if the new movement cost is less than the neighbouring node g cost or isn't int he open array then work out the new cost for this node
					neighbouringNodes[i]->SetGCost(newMovementCost);		//GCost is the distance from current node to neigbouring node
					neighbouringNodes[i]->SetHCost(GetDistance(neighbouringNodes[i], targetNode, mapRef));		//HCost is the distance from neigbouring node to target node
					neighbouringNodes[i]->SetFCost(neighbouringNodes[i]->GetGCost() + neighbouringNodes[i]->GetHCost());
					SetParent(neighbouringNodes[i], currentNode);	//we then set the aprent i n the parent map to the current node

					if (!OpenArrayContian(neighbouringNodes[i]))	//check to see if the neighbouring node isn't int he open set array
					{
						openSet.Add(neighbouringNodes[i]);	//if it's not in then add it to the open set
					}
				}
			}
		}
	}
	//this would mean the path find failed as we have to return something, we create a blank FVector 2d array
	return TArray<FVector2D>();
}

TArray<FVector2D> PathFinding::RetracePath(Node* startNode, Node* targetNode)
{
	TArray<FVector2D> pathArray; //creates path array
	Node* localCurrentNode = targetNode;	//sets the current node to the target node as this function works backwards
	pathArray.Add(localCurrentNode->GetNodePos());	//adds the current node to the array
	//GLog->Log("retracing Start!!");
	//checks to see if the current node == sart ndoe if so the path has been retrace if not continue loop
	while (localCurrentNode->GetNodePos() != startNode->GetNodePos())
	{
		//GLog->Log("retracing 1 loop!!");
		localCurrentNode = pathFindingMapParent.FindRef(localCurrentNode->GetNodePos());	//sets the curent node equal to the parent ndoe
		if (localCurrentNode != nullptr) 
		{ 
			pathArray.Add(localCurrentNode->GetNodePos()); 
		}	//adds that mdoe locationt to the path}		
	//	GLog->Log("retracing 2 loop!!");
	}
	//GLog->Log("retracing mid!!");
	if (pathArray.Contains(startNode->GetNodePos()) == false)	//checks the start node is within the array
	{
		pathArray.EmplaceAt(pathArray.Num(), startNode->GetNodePos());	//if its not, add it to the array
	}
	//GLog->Log("retracing end!!");
	return pathArray;
}

TArray<Node*> PathFinding::GetNeighbours(Node* node, AMapController* mapRef)
{
	int lmap = (mapSize * gridSize) * -1;
	int hmap = (mapSize * gridSize);

	TArray<Node*> neighbours;	//creates a array of nodes to store the neighbouring nodes
	for (int x = -1; x <= 1; x++)
	{
		for (int y = -1; y <= 1; y++)
		{
			bool dontCheck = false;
			if (x == 0 && y == 0)	//checks the x and y don't = 0 as that would be the nodes pos and no need to check there
			{
				dontCheck = true;
			}
			if (dontCheck == false)
			{
				FVector2D neighbourNode = node->GetNodePos();
				//adds the x and y to the nodes pos
				neighbourNode.X += (x * gridSize);
				neighbourNode.Y += (y * gridSize);
				if (x > lmap && x < hmap && y > lmap && y < hmap)
				{
					if (mapRef->IsNodeWalkable(neighbourNode) == false)
					{
						neighbours.Add(mapRef->GetNode(neighbourNode));	//then gets the new node
					}
					//else{ GLog->Log("CAN'T MOVE TO THIS NODE !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"); }
					//not added as neigbhouring node can't be walked
				}			
			}
		}
	}
	return neighbours;
}

bool PathFinding::ClosedArrayContain(Node* node)
{
	for (int i = 0; i < closedSet.Num(); i++)	//loop through the closed set array
	{
		if (node->GetNodePos() == closedSet[i]->GetNodePos())	//check to see if the node is in the array
		{
			return true;
		}
	}
	return false;
}

bool PathFinding::OpenArrayContian(Node* node)
{
	for (int i = 0; i < openSet.Num(); i++)	//loop through the closed set array
	{
		if (node->GetNodePos() == openSet[i]->GetNodePos())	//check to see if the node is in the array
		{
			return true;
		}
	}
	return false;
}

int PathFinding::GetDistance(Node* nodeA, Node* nodeB, AMapController* mapRef)
{
	FVector2D distance = nodeA->GetNodePos() - nodeB->GetNodePos();		//this set the x distance 
	//if (distance.X < 0) { distance.X *= -1; }					//this makes sure it's a postive
	//if (distance.Y < 0) { distance.Y *= -1; }

	float tileMoveCost = mapRef->GetNodeMovementCost(nodeA->GetNodePos());	 //this should get the tile cost

	//might need this
	//if (distanceX > distanceY)		//if distance x is more than disatnce y then return the following
	//{
	//	return 14 * distanceY + tileMoveCost * (distanceX - distanceY);		//this returns the cost for the distance
	//}
	//return 14 * distanceX + tileMoveCost * (distanceY - distanceX);		//returns the same cost but x and y and switch in the forumal to avoid negative numbers
	return distance.Distance(nodeA->GetNodePos(),nodeB->GetNodePos()) + tileMoveCost;
}

void PathFinding::SetParent(Node* neighbourNode, Node* parentNode)
{
	//GLog->Log("add parent");
	FVector2D key = neighbourNode->GetNodePos();
	if (pathFindingMapParent.Contains(key))	//is the key contained within the map
	{
		pathFindingMapParent[{key}] = parentNode;	//if so set that node with that key to the new parent node
	}
	pathFindingMapParent.Add(key, parentNode);	//if not add a new elment to the array with the key parent node
	//GLog->Log("Done add parent");
}
