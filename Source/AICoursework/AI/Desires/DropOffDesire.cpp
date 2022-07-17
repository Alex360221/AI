// Fill out your copyright notice in the Description page of Project Settings.


#include "DropOffDesire.h"
#include <AICoursework/AI/AIEntity.h>
#include <AICoursework/MapController.h>

DropOffDesire::DropOffDesire()
{
}

void DropOffDesire::CalculateDesire(AAIEntity* ai)
{
	//looking at how much weight the ai has
	//and hwo close to the drop off the ai has
	//and max weight
	if (ai == nullptr) { return; }
	if (ai->ai_CurrentWeight != 0 && ai->ai_Inventory.Num() != 0)
	{
		//distance to closest sotrage point
		FVector aiPos = ai->GetActorLocation();
		targetPoint = ai->mapRef->GetClosestStoragePoint(aiPos);
		float dis = FVector2D::Distance(targetPoint, FVector2D(aiPos.X, aiPos.Y));
		float weightleft = ai->ai_MaxWeight - (ai->ai_MaxWeight - ai->ai_CurrentWeight);
		desireValue = 5 * (weightleft / dis);
	}
	else{ desireValue = 0.0f; }	
}

void DropOffDesire::ExcuteDesire(AAIEntity* ai)
{
	GLog->Log("HAVE DESIRE TO DROP OFF ITEMS!!!!!!!!!!!!!!!!!!!!!!!!!");
	FVector aiPos = ai->GetActorLocation();
	float dis = FVector2D::Distance(targetPoint, FVector2D(aiPos.X, aiPos.Y));
	if (dis > ai->mapRef->gridSize)
	{
		//move to target
		ai->ai_moveHome = false;
		ai->ai_jobMove = false;
		ai->ai_MoveToLocation = true;
		ai->ai_TargetLocation = FVector(targetPoint.X, targetPoint.Y, 50);
	}
	else
	{
		//drop off item
		ai->ai_MoveToLocation = false;
		GLog->Log("Drop Off items!!!!!!!");
		ai->EmtpyInventory(ai->mapRef->GetStorageBuilding(targetPoint));
	}
}

