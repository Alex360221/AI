// Fill out your copyright notice in the Description page of Project Settings.


#include "PickUpDesire.h"
#include <AICoursework/AI/AIEntity.h>
#include <AICoursework/AI/Tasks/BaseTask.h>
#include <AICoursework/MapController.h>

PickUpDesire::PickUpDesire()
{
}

void PickUpDesire::GoToJob(class AAIEntity* ai, FVector jobPos)
{
	GLog->Log("Go to Job");
	if (ai != nullptr)
	{
		//if (ai->ai_distanceToJob > 5)
		//{
			ai->ai_jobMove = true;
			ai->ai_jobLocation = jobPos;
		//}
		/*else
		{
			ai->ai_jobMove = false;
			ai->pathFind = false;
			ai->ai_atJob = true;
			GLog->Log("at Job doing job!!");
		}*/
	}
}

void PickUpDesire::CalculateDesire(AAIEntity* ai)
{
	//is ai close to pickup task
	//get distance to pickup task
	 BaseTask* task = ai->DistanceToPickUpTask();
	 if (task != nullptr && !task->task_Active)
	 {
		 if (ai->ai_CurrentWeight < ai->ai_MaxWeight)
		 {
			 //work out distance to task
			 float dis = FVector::Distance(ai->GetActorLocation(), task->task_Location);
			 desireValue = 50 * (1 / dis);
			 closetTask = task;
		 }
		 else
		 {
			 desireValue = 0;
		 }		 
	 }
	 else
	 {
		 desireValue = 0;
	 }	 
}

void PickUpDesire::ExcuteDesire(AAIEntity* ai)
{
	GLog->Log("DESIRE TO PICK UP SOEMTHING!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
	//check we are are next to the object
	if (closetTask != nullptr)
	{
		float dis = FVector::Distance(ai->GetActorLocation(), closetTask->task_Location);
		if (dis < ai->mapRef->gridSize)
		{
			//pick up object...
			closetTask->ExcuteTask(ai);
		}
		else
		{
			GLog->Log("Needs to move closer to object");
			GoToJob(ai,closetTask->task_Location);
		}
	}
	

}


