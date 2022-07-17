// Fill out your copyright notice in the Description page of Project Settings.


#include "Desire.h"
#include <AICoursework/AI/AIEntity.h>

Desire::Desire()
{
}

void Desire::CalculateDesire(class AAIEntity* ai)
{
}

void Desire::ExcuteDesire(AAIEntity* ai)
{
}

bool Desire::Comparator(Desire* des2)
{
	//used for sorting desires
	return desireValue > des2->desireValue;	//this will return true if des1 is less than des 2
}

