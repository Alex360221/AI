// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseJob.h"

/**
 * 
 */
class AICOURSEWORK_API FarmJob
    : public BaseJob
{
public:
	FarmJob();
    virtual void Job(class AAIEntity* ai) override;
};
