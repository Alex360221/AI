// Fill out your copyright notice in the Description page of Project Settings.


#include "WanderDesire.h"
#include <AICoursework/AI/AIEntity.h>

WanderDesire::WanderDesire()
{
}

void WanderDesire::CalculateDesire(class AAIEntity* ai)
{
	if (ai->ai_job == 0 || ai->ai_canWork == false)
	{
		desireValue = 0.8f;
	}
	else
	{
		desireValue = 0.2f;
	}	
}

void WanderDesire::ExcuteDesire(AAIEntity* ai)
{
	GLog->Log("Wondering!");
	//ai->ai_goingWondering = true;
}
