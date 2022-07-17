// Fill out your copyright notice in the Description page of Project Settings.


#include "Task_Fishing.h"
#include <AICoursework/AI/Buildings/FishingHunt.h>
#include <AICoursework/AI/AIEntity.h>

Task_Fishing::Task_Fishing()
{
	task_Specific = true;
}

Task_Fishing::Task_Fishing(FVector taskPos)
{
	task_Location = taskPos;

	task_Specific = true;
}

void Task_Fishing::ExcuteSpecificTask(AAIEntity* ai, ABaseProductBuilding* building)
{
	GLog->Log("FISHING!!!!!!!!!!!!!!!!!!!!");

	AFishingHunt* fish_farm = Cast<AFishingHunt>(building);
	if (fish_farm != nullptr)
	{
		ai->ai_jobTask = NULL;
		ai->ai_hasTask = false;
		fish_farm->ExcuteTask(this);
	}
}

