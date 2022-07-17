// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class AICOURSEWORK_API Node
{
public:
	Node();
    void SetGCost(int newGCost) { gCost = newGCost; }
    void SetHCost(int newHCost) { hCost = newHCost; }
    void SetFCost(int newFCost) { fCost = newFCost; }
    void SetNodePos(FVector2D newPos) { pos = newPos; }
    void SetParent(Node* newParent) { parent = newParent; }


    int GetGCost() { return gCost; }
    int GetHCost() { return hCost; }
    int GetFCost() { return fCost; }
    FVector2D GetNodePos() { return pos; }
    Node* GetParent() { return parent; }

private:
    int gCost;
    int hCost;
    int fCost;
    FVector2D pos;
    Node* parent;

};
