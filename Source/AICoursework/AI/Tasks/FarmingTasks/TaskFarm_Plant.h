// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <AICoursework/AI/Tasks/BaseTask.h>

/**
 * 
 */
class AICOURSEWORK_API TaskFarm_Plant
    : public BaseTask
{
public:
	TaskFarm_Plant();
    TaskFarm_Plant(FVector taskPos);
    virtual void ExcuteSpecificTask(class AAIEntity* ai, class ABaseProductBuilding* building) override;
};
