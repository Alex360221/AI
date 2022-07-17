// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseProductBuilding.h"
#include <AICoursework/AI/AIEntity.h>
#include <AICoursework/AI/Tasks/BaseTask.h>
#include <AICoursework/MapController.h>
#include <AICoursework/AI/Items/Crate.h>
#include <AICoursework/AI/Items/Item.h>
#include <AICoursework/AI/Tasks/Genral/Task_PickUp.h>

// Sets default values
ABaseProductBuilding::ABaseProductBuilding(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, (TEXT("MeshComp")));
	mat = ObjectInitializer.CreateDefaultSubobject<UMaterialInterface>(this, (TEXT("Mat")));

	building_CurrentWorkers = 0;
	building_MaxWorkers = 1;
	building_CanWork = true;
	building_Center = FVector(0, 0, 50);
	crate_OffsetAmount = FVector(0, 20, 50);
}

// Called when the game starts or when spawned
void ABaseProductBuilding::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseProductBuilding::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	amounntoftasks = building_Tasks.Num();
}

bool ABaseProductBuilding::CanAIWorkHere(AAIEntity* ai)
{
	if (building_CurrentWorkers < building_MaxWorkers)
	{
		//can work here 
		building_CurrentWorkers += 1;
		building_Workers.Add(ai);
		ai->ai_jobRef = this;
		ai->ai_jobLocation = building_Center;
		return true;
	}
	return false;
}

void ABaseProductBuilding::CreateTasks()
{
}

bool ABaseProductBuilding::FindTask(AAIEntity* ai)
{
	if (building_Tasks.Num() > 0)
	{
		for (int i = 0; i < building_Tasks.Num();i++)
		{
			//check if task is atctive
			if (!building_Tasks[i]->task_Active)
			{
				building_Tasks[i]->task_Active = true;
				ai->ai_jobTask = building_Tasks[i];
				ai->ai_jobLocation = building_Tasks[i]->task_Location;
				return true;
			}		
		}
	}
	return false;
}

void ABaseProductBuilding::ExcuteTask(BaseTask* task)
{
}

void ABaseProductBuilding::SetBuildingArea(int minX, int maxX, int minY, int maxY, TArray<FVector2D> movementArea, FVector2D newbuildingCenter)
{
	//set up building area
	for (int x = minX; x <= maxX; x++)
	{
		for (int y = minY; y <= maxY; y++)
		{
			buildingArea.Add(FVector2D(x, y));
			buildingMovement.Add(FVector2D(x, y));
		}
	}
	//remove an area so ai can move into house
	for (int i = 0; i < movementArea.Num();i++)
	{
		buildingMovement.Remove(movementArea[i]);
	}
	
	buildingCenter = newbuildingCenter;
}

void ABaseProductBuilding::SetBuildingWorkingPositions(TArray<FVector2D> workingPositions)
{
	building_WorkingPositions = workingPositions;
}

TArray<FVector2D> ABaseProductBuilding::GetBuildingArea(int gridSize, FVector2D buildingPos)
{
	TArray<FVector2D> ra;
	for (int i = 0; i < buildingArea.Num();i++)
	{
		FVector2D pos = FVector2D(buildingArea[i].X * gridSize, buildingArea[i].Y * gridSize);
		pos += buildingPos;
		ra.Add(pos);
	}
	return ra;
}

TArray<FVector2D> ABaseProductBuilding::GetBuildingMovement(int gridSize, FVector2D buildingPos)
{
	TArray<FVector2D> ra;
	for (int i = 0; i < buildingMovement.Num();i++)
	{
		FVector2D pos = FVector2D(buildingMovement[i].X * gridSize, buildingMovement[i].Y * gridSize);
		pos += buildingPos;
		ra.Add(pos);
	}
	return ra;
}

void ABaseProductBuilding::SetBuildingCenter(int gridSize, FVector2D buildingPos)
{
	buildingCenter *= gridSize;
	buildingCenter += buildingPos;
	//updates the working positions
	for (int i = 0; i < building_WorkingPositions.Num();i++)
	{
		building_WorkingPositions[i] *= gridSize;
		building_WorkingPositions[i] += buildingPos;
	}
	building_Center = FVector(buildingCenter.X, buildingCenter.Y, 50);
	CreateTasks();	//final thing is to create the tasks!!
}

void ABaseProductBuilding::SpawnCrate(Item* item, FVector pos)
{
	if (item == nullptr) { return; }
	if (mapRef == nullptr) { return; }
	
	ACrate* newCrate = GetWorld()->SpawnActor<ACrate>(pos - crate_OffsetAmount, FRotator::ZeroRotator);
	if (newCrate != nullptr)
	{
		newCrate->SetCrate(item, pos);
		//need to store crate..
		mapRef->AddCrate(newCrate, pos);
	}

	mapRef->CreateGlobalTask(new Task_PickUp(pos));

}

