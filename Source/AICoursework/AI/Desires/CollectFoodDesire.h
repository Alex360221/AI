// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Desire.h"

/**
 * 
 */
class AICOURSEWORK_API CollectFoodDesire : public Desire
{
public:
	CollectFoodDesire();
    virtual void CalculateDesire(class AAIEntity* ai) override;
    virtual void ExcuteDesire(class AAIEntity* ai) override;

    class AStorageBuilding* storageTarget;

private:
    int CollectionAmount;
    bool startedThisDesire;
    bool CollectedFoodYet;
    FVector targetPos;

};
