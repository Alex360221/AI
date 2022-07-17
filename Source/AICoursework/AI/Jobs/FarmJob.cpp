// Fill out your copyright notice in the Description page of Project Settings.


#include "FarmJob.h"
#include <AICoursework/AI/AIEntity.h>
#include <AICoursework/AI/Buildings/FarmBuilding.h>

FarmJob::FarmJob()
{
}

void FarmJob::Job(AAIEntity* ai)
{
	GLog->Log("Doing Job!!!!!!!!!!");
	//the jobs behaviour tree will be here
	if (ai != nullptr)
	{	 	
		//ai at job? 
		if (!ai->ai_atJob)
		{
			//no, travel to job
			GoToJob(ai, ai->ai_jobRef->building_Center);
		}
		else
		{
			//yes, find task and do task	
			GLog->Log("At job now do task");
			DoTask(ai);
			////does the ai have a task?
			//if (!ai->ai_hasTask)
			//{
			//	//no task so find no task
			//	ai->ai_hasTask = ai->ai_jobRef->FindTask(ai);
			//}

		}
	}
}
