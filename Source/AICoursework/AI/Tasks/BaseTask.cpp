// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseTask.h"
#include <AICoursework/AI/AIEntity.h>

BaseTask::BaseTask()
{
	task_Location = FVector(0, 0, 50);
	task_Active = false;
	//task_Index = 0;
	task_Specific = false;
}

void BaseTask::RunTask(AAIEntity* ai)
{
	if (ai != nullptr)
	{
		if (ai->ai_jobTask != nullptr)
		{
			if (ai->ai_jobTask->task_Specific)
			{
				ai->ai_jobTask->ExcuteSpecificTask(ai, ai->ai_jobRef);
			}
			else
			{
				ai->ai_jobTask->ExcuteTask(ai);
			}
		}
	}
}

void BaseTask::ExcuteTask(AAIEntity* ai)
{
}

void BaseTask::ExcuteSpecificTask(AAIEntity* ai, ABaseProductBuilding* building)
{
}

