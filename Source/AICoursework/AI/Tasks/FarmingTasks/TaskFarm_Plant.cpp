// Fill out your copyright notice in the Description page of Project Settings.


#include "TaskFarm_Plant.h"
#include <AICoursework/AI/AIEntity.h>
#include <AICoursework/AI/Buildings/FarmBuilding.h>
#include <AICoursework/AI/Buildings/BaseProductBuilding.h>

TaskFarm_Plant::TaskFarm_Plant()
{
	task_Specific = true;
}

TaskFarm_Plant::TaskFarm_Plant(FVector taskPos)
{
	task_Location = taskPos;
	//task_Index = index;

	task_Specific = true;
}

void TaskFarm_Plant::ExcuteSpecificTask(AAIEntity* ai, ABaseProductBuilding* building)
{
	GLog->Log("PLANTING!!!!!!!!!!!!!!!!!!!!");

	AFarmBuilding* farm = Cast<AFarmBuilding>(building);
	if (farm != nullptr)
	{
		ai->ai_jobTask = NULL;
		ai->ai_hasTask = false;
		farm->ExcuteTask(this);
	}

}

//void TaskFarm_Plant::ExcuteTask(AAIEntity* ai)
//{
//	//plant crop in feild
//}


