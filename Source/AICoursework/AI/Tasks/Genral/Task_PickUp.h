// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <AICoursework/AI/Tasks/BaseTask.h>

/**
 * 
 */
class AICOURSEWORK_API Task_PickUp : public BaseTask
{
public:
	Task_PickUp();
    Task_PickUp(FVector taskPos);
   // virtual void ExcuteSpecificTask(class AAIEntity* ai, class ABaseProductBuilding* building) override;
    virtual void ExcuteTask(class AAIEntity* ai) override; 
private:
};
