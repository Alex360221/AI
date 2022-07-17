// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Desire.h"

/**
 * 
 */
class AICOURSEWORK_API PickUpDesire : public Desire
{
public:
	PickUpDesire();
    void GoToJob(class AAIEntity* ai, FVector jobPos);
    virtual void CalculateDesire(class AAIEntity* ai) override;
    virtual void ExcuteDesire(class AAIEntity* ai) override;

private:
    class BaseTask* closetTask;
};
