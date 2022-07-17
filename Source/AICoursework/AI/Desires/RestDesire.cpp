// Fill out your copyright notice in the Description page of Project Settings.


#include "RestDesire.h"
#include <AICoursework/AI/AIEntity.h>

RestDesire::RestDesire()
{
}

void RestDesire::CalculateDesire(AAIEntity* ai)
{
	float dis = (ai->ai_distanceToHome / 10);
	if (dis == 0) { dis = 1; }	//this done to make the distance to home doesn't equal 0, if not error

	if (ai->ai_rest < (ai->ai_restMax / 2)) 	//only think about rest when at half rest
	{
		if (ai->ai_rest < 15)
		{
			desireValue = 0.9f;
		}
		else
		{
			desireValue = 1 * ((ai->ai_restMax - ai->ai_rest) / dis);
		}
	}
	else
	{
		desireValue = 0.1f;
	}
	
}

void RestDesire::ExcuteDesire(AAIEntity* ai)
{
	//check ai is at home 
	if (ai->ai_distanceToHome == 0)
	{
		GLog->Log("Resting");
		ai->ai_rest = ai->ai_restMax;
	}
	else
	{
		GLog->Log("Need to go home");
		ai->ai_moveHome = true;
	}
}

