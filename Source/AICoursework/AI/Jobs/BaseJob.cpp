// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseJob.h"
#include <AICoursework/AI/AIEntity.h>
#include <AICoursework/AI/Buildings/BaseProductBuilding.h>
#include <AICoursework/AI/Tasks/BaseTask.h>

BaseJob::BaseJob()
{

}

void BaseJob::Job(AAIEntity* ai)
{
}

void BaseJob::GoToJob(class AAIEntity* ai, FVector jobPos)
{
	GLog->Log("Go to Job");
	if (ai != nullptr)
	{
		if (ai->ai_distanceToJob > 5)
		{
			ai->ai_jobMove = true;
			ai->ai_jobLocation = jobPos;
		}
		else
		{
			ai->ai_jobMove = false;
			ai->pathFind = false;
			ai->ai_atJob = true;
			GLog->Log("at Job doing job!!");
		}
	}
}

void BaseJob::CollectItemsForJob()
{
}

void BaseJob::DoTask(AAIEntity* ai)
{
	//does the ai have a task?
	if (!ai->ai_hasTask)
	{
		//no task so find no task
		GLog->Log("Finding task!!");
		ai->ai_hasTask = ai->ai_jobRef->FindTask(ai);
	}
	else
	{
		//are we at the task?
		if (ai->ai_jobTask != nullptr)
		{
			ai->ai_distanceToJob = FVector::Distance(ai->GetActorLocation(), ai->ai_jobTask->task_Location);
			GLog->Log("Distance to task... " + FString::FromInt(ai->ai_distanceToJob));
			if (ai->ai_distanceToJob > 5)
			{
				//move to task location
				GoToJob(ai, ai->ai_jobTask->task_Location);
			}
			else
			{
				//at job so excute job
				ai->ai_jobMove = false;
				ai->pathFind = false;
				ai->ai_jobTask->RunTask(ai);
			}
		}	
	}
}
