// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Desire.h"

/**
 * 
 */
class AICOURSEWORK_API WanderDesire
    : public Desire
{
public:
	WanderDesire();
    virtual void CalculateDesire(class AAIEntity* ai) override;
    virtual void ExcuteDesire(class AAIEntity* ai) override;
};
