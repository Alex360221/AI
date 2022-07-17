// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class AICOURSEWORK_API Desire
{
public:
	Desire();

    //the value used by the computer
    float desireValue;

    //this sets the desire value in the child classes (doeas nothing here)
    virtual void CalculateDesire(class AAIEntity* ai);

    //this Excutes the desire value in the child classes (doeas nothing here)
    virtual void ExcuteDesire(class AAIEntity* ai);
    
    //comparator
    bool Comparator(Desire* des2);
};
