// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseProductBuilding.h"
#include "FarmBuilding.generated.h"

/**
 * 
 */
UCLASS()
class AICOURSEWORK_API AFarmBuilding : public ABaseProductBuilding
{
	GENERATED_BODY()
public:
	AFarmBuilding(const FObjectInitializer& ObjectInitializer);

	TArray<FVector2D> CreateFarmArea(FVector2D point1, FVector2D point2, int gridSize);	//this will create a farm area from point 1 to 2
	void SpawnFarmArea();

	virtual void CreateTasks() override;

	virtual void ExcuteTask(class BaseTask* task) override;	//this will excute the task

private:
	TArray<FVector2D> farmArea;

	class UInstancedStaticMeshComponent* tile_Tiled;
	
};
