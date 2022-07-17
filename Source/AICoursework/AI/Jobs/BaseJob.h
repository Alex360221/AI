// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class AICOURSEWORK_API BaseJob
{
public:
	BaseJob();
    FVector job_Location;
    //this sets the desire value in the child classes (doeas nothing here)
    virtual void Job(class AAIEntity* ai);

    void GoToJob(class AAIEntity* ai, FVector jobPos);
    void CollectItemsForJob();
    void DoTask(class AAIEntity* ai);   //this will do a task for the ai or find one
};
