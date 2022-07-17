// Fill out your copyright notice in the Description page of Project Settings.


#include "Task_PickUp.h"
#include <AICoursework/MapController.h>
#include <AICoursework/AI/AIEntity.h>

Task_PickUp::Task_PickUp()
{
	task_Specific = true;
}

Task_PickUp::Task_PickUp(FVector taskPos)
{
	task_Location = taskPos;
	task_Specific = true;
}

void Task_PickUp::ExcuteTask(AAIEntity* ai)
{
	AMapController* mapRef = ai->mapRef;
	if (mapRef != nullptr)
	{		
		if (ai->ai_CurrentWeight < ai->ai_MaxWeight)
		{
			mapRef->RemoveCrate(ai, task_Location);
			mapRef->RemoveGlobalTask(this);
		}	
	}
}
