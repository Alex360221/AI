// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseJob.h"

/**
 * 
 */
class AICOURSEWORK_API FishFarmerJob : public BaseJob
{
public:
	FishFarmerJob();
    virtual void Job(class AAIEntity* ai) override;
};
