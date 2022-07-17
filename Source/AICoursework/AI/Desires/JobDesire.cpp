// Fill out your copyright notice in the Description page of Project Settings.


#include "JobDesire.h"
#include <AICoursework/AI/AIEntity.h>
#include <AICoursework/AI/Jobs/BaseJob.h>
#include <AICoursework/AI/Buildings/BaseProductBuilding.h>

JobDesire::JobDesire()
{
}

void JobDesire::CalculateDesire(class AAIEntity* ai)
{
	if (ai->ai_job > 0)
	{
		//has ai got place to work?
		if (ai->ai_jobRef != nullptr)
		{
			if (ai->ai_canWork) 
			{ 
				if(ai->ai_jobRef->building_Tasks.Num() == 0)	//no tasks to do at job so job vaule decreasd
				{
					desireValue = 0.1f;
				}
				else
				{
					desireValue = 0.8f;
				}					
			}
			else { desireValue = 0.2f; }
		}
		else
		{
			//no place to work
			desireValue = 0.2f;
		}
	}
	else
	{
		//no job
		desireValue = 0.2f;
	}
}

void JobDesire::ExcuteDesire(AAIEntity* ai)
{
	GLog->Log("Doing Job");
	if (ai != nullptr)
	{
		ai->ai_jobBase->Job(ai);
	}
}
