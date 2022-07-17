// Fill out your copyright notice in the Description page of Project Settings.


#include "EatDesire.h"
#include <AICoursework/AI/AIEntity.h>
#include <AICoursework/AI/Buildings/HouseBuilding.h>

EatDesire::EatDesire()
{
}

void EatDesire::CalculateDesire(AAIEntity* ai)
{
	float dis = (ai->ai_distanceToHome / 10);
	if (dis == 0) { dis = 1; }	//this done to make the distance to home doesn't equal 0, if not error

	if (ai->ai_hunger < (ai->ai_hungerMax / 2))	//only think about food when at half hunger
	{
		if (ai->ai_hunger < 15)
		{
			desireValue = 0.9f;
		}
		else
		{
			desireValue = 1 * ((ai->ai_hungerMax - ai->ai_hunger) / dis);
		}
	}
	else
	{
		desireValue = 0.1f;
	}
}

void EatDesire::ExcuteDesire(AAIEntity* ai)
{
	//check ai is at home 
	if (ai->ai_distanceToHome == 0)
	{
		if (ai->ai_HouseRef == nullptr) { return; }
		//calcauate how much food we need
		int foodNeeded = ai->FoodNeedToEat();
		//check we have food
		bool eat = ai->ai_HouseRef->CheckEnoughFoodInHouse(foodNeeded);
		if (eat)
		{
			GLog->Log("Eating Food");
			//ai->ai_hunger = ai->ai_hungerMax;
			ai->ai_HouseRef->RemoveFood(ai, foodNeeded);
		}
		else
		{
			//no food in house
			//check ai for food
			//if ai has food move amount needed, move to house
			GLog->Log("No Food in house so transfer look in ai for food");
			ai->TransferItemTypeToHouse(1, foodNeeded);
		}
		
	}
	else
	{
		GLog->Log("Need to go home");
		ai->ai_moveHome = true;
	}
}

