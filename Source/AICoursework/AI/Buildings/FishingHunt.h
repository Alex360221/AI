// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseProductBuilding.h"

#include "FishingHunt.generated.h"

UCLASS()
class AICOURSEWORK_API AFishingHunt : public ABaseProductBuilding
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFishingHunt(const FObjectInitializer& ObjectInitializer);

	virtual void CreateTasks() override;	//this will create an array of tasks 
	virtual void ExcuteTask(class BaseTask* task) override;	//this will excute the task

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
private:
	int fish_CurrentAmount;
	int fish_MaxAmountPerCrate;
	void AddFish(FVector taskPos);


};
