// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Node.h"

/**
 * 
 */
class AICOURSEWORK_API PathFinding
{
public:
	PathFinding();
    int gridSize;
    int mapSize;
    TArray<FVector2D> FindPath(FVector2D startPos, FVector2D targetPos, class AMapController* mapRef);  //this will find the path to the target lcoation
private:
    TArray<Node*> openSet;      //stores the open nodes
    TArray<Node*> closedSet;    //stores the closed nodes
    TMap<FVector2D, Node*> pathFindingMapParent;    //this stores the nodes parents
    Node* currentNode;


    TArray<FVector2D> RetracePath(Node* startNode, Node* targetNode);   //this retraces the path
    TArray<Node*> GetNeighbours(Node* node, AMapController* mapRef);	//this work out the neighbouring nodes
    bool ClosedArrayContain(Node* node);    //this will check to see if the node is contain in the clsoed array
    bool OpenArrayContian(Node* node);  //this will check if the node is contained int the open array
    int GetDistance(Node* nodeA, Node* nodeB, class AMapController* mapRef);    //this works out the disatcne between the nodes
    void SetParent(Node* neighbourNode, Node* parentNode);  //this updates the nodes parent
};
