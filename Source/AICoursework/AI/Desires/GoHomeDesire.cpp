// Fill out your copyright notice in the Description page of Project Settings.


#include "GoHomeDesire.h"
#include <AICoursework/AI/AIEntity.h>

GoHomeDesire::GoHomeDesire()
{
}

void GoHomeDesire::CalculateDesire(AAIEntity* ai)
{
	if (ai->ai_goingHome == true && ai->ai_distanceToHome != 0)
	{
		desireValue = 10.f;
	}
	else { desireValue = 0; }
}

void GoHomeDesire::ExcuteDesire(AAIEntity* ai)
{
	GLog->Log("Going Home.. " + FString::FromInt(ai->ai_distanceToHome));
	ai->ai_moveHome = true;
}
