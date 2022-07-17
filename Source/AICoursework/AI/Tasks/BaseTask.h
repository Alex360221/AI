// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class AICOURSEWORK_API BaseTask
{
public:
	BaseTask();
   // BaseTask(FVector taskPos);
    FVector task_Location;
    bool task_Active;
   // int task_Index;
    bool task_Specific;
    void RunTask(class AAIEntity* ai);
    virtual void ExcuteTask(class AAIEntity* ai);
    virtual void ExcuteSpecificTask(class AAIEntity* ai , class ABaseProductBuilding* building);

};
