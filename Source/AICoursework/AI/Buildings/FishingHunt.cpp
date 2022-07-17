// Fill out your copyright notice in the Description page of Project Settings.


#include "FishingHunt.h"
#include <AICoursework/AI/Tasks/BaseTask.h>
#include <AICoursework/AI/Tasks/FishingTasks/Task_Fishing.h>
#include <AICoursework/AI/Items/Item.h>

// Sets default values
AFishingHunt::AFishingHunt(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UMaterialInterface>Mat(TEXT("Material'/Game/Models/Houses/Material2.Material2'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh>Mesh02(TEXT("StaticMesh'/Game/Models/Houses/Basic_House.Basic_House'"));
	UStaticMesh* assset = Mesh02.Object;
	MeshComp->SetStaticMesh(assset);
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComp->SetCastShadow(false);
	MeshComp->SetMaterial(0, mat);

	TArray<FVector2D> removalArea;
	removalArea.Add(FVector2D(-2, 3));
	removalArea.Add(FVector2D(-1, 3));
	removalArea.Add(FVector2D(0, 3));
	removalArea.Add(FVector2D(1, 3));
	removalArea.Add(FVector2D(2, 3));
	SetBuildingArea(-2,2,-2,3,removalArea,FVector2D(0,3));
	SetBuildingWorkingPositions(removalArea);	//for now just using removal area
	SetMaxWorkers(5);	//sets the maxium amount of workers

	//realtive variables
	fish_CurrentAmount = 0;
	fish_MaxAmountPerCrate = 25;
}

void AFishingHunt::CreateTasks()
{
	GLog->Log("Creating Tasks");
	//should use the state of the field to deturimine what tasks are created
	TArray <FVector2D> poses = GetWorkingPositions();
	for (int i = 0; i < poses.Num();i++)
	{
		Task_Fishing* newTask = new Task_Fishing(FVector(poses[i].X, poses[i].Y,50));
		building_Tasks.Add(newTask);
	}
}

void AFishingHunt::ExcuteTask(BaseTask* task)
{
	//fishing tasks will be removed from the 0 pos and put at the end of the queue
	FVector FishingSpot;
	for (int i = 0; i < building_Tasks.Num(); i++)
	{
		if (building_Tasks[i] == task)
		{
			FishingSpot = building_Tasks[i]->task_Location;		
			AddFish(FishingSpot);
			building_Tasks.RemoveAt(i);
			delete task;

			Task_Fishing* newTask = new Task_Fishing(FishingSpot);
			building_Tasks.Add(newTask);
			return;
		}
	}
}

// Called when the game starts or when spawned
void AFishingHunt::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFishingHunt::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFishingHunt::AddFish(FVector taskPos)
{
	fish_CurrentAmount += 5;
	if (fish_CurrentAmount >= fish_MaxAmountPerCrate)
	{
		SpawnCrate(new Item(0, "Fish", fish_MaxAmountPerCrate, 1.0f, 1), taskPos);
		fish_CurrentAmount -= fish_MaxAmountPerCrate;
	}
}

