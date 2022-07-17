// Fill out your copyright notice in the Description page of Project Settings.


#include "CollectFoodDesire.h"
#include <AICoursework/AI/AIEntity.h>
#include <AICoursework/AI/Buildings/HouseBuilding.h>
#include <AICoursework/AI/Buildings/StorageBuilding.h>
#include <AICoursework/MapController.h>

CollectFoodDesire::CollectFoodDesire()
{
	CollectionAmount = 0;
	startedThisDesire = false;
	CollectedFoodYet = false;
}

void CollectFoodDesire::CalculateDesire(AAIEntity* ai)
{
	if (startedThisDesire == false)
	{
		if (ai == nullptr || ai->ai_HouseRef == nullptr) { return; }
		//only update the desire rates for this desire if the desire is finshed
		//how much food is in the house?
		//how much food do we need?
		int houseFood = ai->ai_HouseRef->GetAmountOfFood();
		int FoodNeeded = ai->ai_HouseRef->GetAmountOfFoodNeeded();
		CollectionAmount = FoodNeeded - houseFood;
		if (CollectionAmount > 10)
		{
			//GLog->Log("Working out collect food Desire");
			if (ai->ai_HouseRef->AiCollectingFood == false)
			{
				ai->ai_HouseRef->AiCollectingFood = true;
				desireValue = 1000.0f;	//really high desire as it's very important
				return;
			}			
		}
		desireValue = 0;
	}	
}

void CollectFoodDesire::ExcuteDesire(AAIEntity* ai)
{	
	startedThisDesire = true;

	if (CollectedFoodYet == false)
	{
		//go to nearest storage palce with food
		//check if we already have target
		if (storageTarget == nullptr)
		{
			//need to get target
			FVector2D pos = ai->mapRef->ClosestStorageWithFood(ai->GetActorLocation(), this);
			if (storageTarget != nullptr)
			{
				targetPos = FVector(pos.X, pos.Y, 50);

				ai->ai_TargetLocation = targetPos;
				ai->ai_MoveToLocation = true;
				ai->ai_jobMove = false;
			}			
		}
		if (storageTarget != nullptr)
		{
			//are we at the target yet?
			float dis = FVector::Dist(ai->GetActorLocation(), targetPos);
			if (dis < 5)
			{
				//at targert Pick up food
				GLog->Log("At Barn Pick up food");
				CollectedFoodYet = storageTarget->TransferFoodToAi(ai, CollectionAmount);				
			}
			else
			{
				GLog->Log("Moving to barn");
			}
		}
		else
		{
			//no food anywhere
			GLog->Log("No Food!!!!!!");
			startedThisDesire = false;
			desireValue = 0;
			ai->ai_MoveToLocation = false;
			ai->ai_HouseRef->AiCollectingFood = false;
			ai->ai_jobMove = false;
		}	
	}
	else
	{
		GLog->Log("Got Food!!!!!!!");
		GLog->Log("Going home to drop off food");

		//are we at home yet?
		if (ai->ai_distanceToHome < 5)
		{
			//at home drop off items
			GLog->Log("home drop off food");
			ai->TransferItemTypeToHouse(1, CollectionAmount);			
			ai->ai_MoveToLocation = false;
			ai->ai_HouseRef->AiCollectingFood = false;
			startedThisDesire = false;
			desireValue = 0;
		}
		else
		{
			targetPos = ai->ai_home;
			ai->ai_TargetLocation = targetPos;
			ai->ai_MoveToLocation = true;
			ai->ai_jobMove = false;
		}

	}

}

