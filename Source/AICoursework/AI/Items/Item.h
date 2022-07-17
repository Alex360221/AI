// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class AICOURSEWORK_API Item
{
public:
	Item();
	Item(int id, FString name, int amount, float weight, int type);

    int item_ID;
    FString item_Name;
    int item_Amount;
    float item_Weight;
    int item_Type;  //0 == baics // 1 == food

};
