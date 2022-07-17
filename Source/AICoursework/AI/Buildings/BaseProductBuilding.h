// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseProductBuilding.generated.h"

UCLASS()
class AICOURSEWORK_API ABaseProductBuilding : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseProductBuilding(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
		UStaticMeshComponent* MeshComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
		UMaterialInterface* mat;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Building Details")
		int building_MaxWorkers;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Building Details")
		int building_CurrentWorkers;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Building Details")
		bool building_CanWork;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Building Details")
		FVector2D buildingCenter;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Building Details")
		TArray<FVector2D> building_WorkingPositions;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Building Details")
		int amounntoftasks;
	
	FVector building_Center;
	TArray<class BaseTask*> building_Tasks;	//an array of task for workers to do
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	bool CanAIWorkHere(class AAIEntity* ai);	//this will work out if an AI can work here - if so it will assign it to this building

	virtual void CreateTasks();	//this will create an array of tasks 

	bool FindTask(class AAIEntity* ai);	//this will work out if the ai can do a task for the job
	void SetMaxWorkers(int newMaxWorkers) { building_MaxWorkers = newMaxWorkers; }
	virtual void ExcuteTask(class BaseTask* task);	//this will excute the task
	void SetBuildingArea(int minX, int maxX, int minY, int maxY, TArray<FVector2D> movementArea, FVector2D newbuildingCenter);	//this will generate the building area, removing the movement area
	void SetBuildingWorkingPositions(TArray<FVector2D> workingPositions);	//this will set the building working postions
	TArray<FVector2D> GetBuildingArea(int gridSize, FVector2D buildingPos);	//this will return the house area relative to the house location
	TArray<FVector2D> GetBuildingMovement(int gridSize, FVector2D buildingPos);	//this will return the house movement relative to the house location
	void SetBuildingCenter(int gridSize, FVector2D buildingPos);	//this will set the center of this house
	FVector2D GetBuildingCenter() { return buildingCenter; }	//this will get the house center relative the house location
	TArray<FVector2D> GetWorkingPositions() { return building_WorkingPositions; }
	void SpawnCrate(class Item* item, FVector pos);
	class AMapController* mapRef;
	FVector crate_OffsetAmount;
private:
	TArray<class AAIEntity*> building_Workers;
	TArray<FVector2D> buildingArea;
	TArray<FVector2D> buildingMovement;
	//FVector2D buildingCenter;
	//TArray<FVector2D> building_WorkingPositions;
	

};
