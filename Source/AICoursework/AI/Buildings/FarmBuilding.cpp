// Fill out your copyright notice in the Description page of Project Settings.


#include "FarmBuilding.h"
#include <AICoursework/AI/Tasks/FarmingTasks/TaskFarm_Plant.h>

AFarmBuilding::AFarmBuilding(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	mat = ObjectInitializer.CreateDefaultSubobject<UMaterialInterface>(this, (TEXT("House Mat")));
	static ConstructorHelpers::FObjectFinder<UMaterialInterface>Mat(TEXT("Material'/Game/Models/Farm/TiledLandMat.TiledLandMat'"));
	UMaterialInterface* material = Mat.Object;
	tile_Tiled = ObjectInitializer.CreateDefaultSubobject<UInstancedStaticMeshComponent>(this, (TEXT("Tile_Tiled")));
	static ConstructorHelpers::FObjectFinder<UStaticMesh>Mesh02(TEXT("StaticMesh'/Game/Models/Farm/TiledLand.TiledLand'"));
	UStaticMesh* assset = Mesh02.Object;
	tile_Tiled->SetStaticMesh(assset);
	tile_Tiled->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	tile_Tiled->SetCastShadow(false);
	tile_Tiled->SetMaterial(0,mat);
	//SetRootComponent(MeshComp);

	//
}

TArray<FVector2D> AFarmBuilding::CreateFarmArea(FVector2D point1, FVector2D point2, int gridSize)
{
	building_Center = FVector(point1.X, point1.Y, 50);
	point1 = FVector2D(int(point1.X / gridSize), int(point1.Y / gridSize));
	point2 = FVector2D(int(point2.X / gridSize), int(point2.Y / gridSize));
	int lX, hX, lY, hY;
	//first task is to find the highest and lowest x and y positions
	if (point1.X > point2.X) { hX = point1.X; lX = point2.X; }
	else { hX = point2.X; lX = point1.X; }
	if (point1.Y > point2.Y) { hY = point1.Y; lY = point2.Y; }
	else { hY = point2.Y; lY = point1.Y; }

	farmArea.Empty();

	for (int x = lX; x <= hX;x++)
	{
		for (int y = lY; y <= hY;y++)
		{
			farmArea.Add(FVector2D(x * gridSize, y * gridSize));
		}
	}
	
	return farmArea;
}

void AFarmBuilding::SpawnFarmArea()
{
	for (int i = 0; i < farmArea.Num();i++)
	{
		//spawn a farm tile at this location
		FTransform pos;
		pos.SetLocation(FVector(farmArea[i].X,farmArea[i].Y,0));
		tile_Tiled->AddInstance(pos);
	}

	//now the farm area is created create tasks
	CreateTasks();

}

void AFarmBuilding::CreateTasks()
{
	GLog->Log("Creating Tasks");
	//should use the state of the field to deturimine what tasks are created

	for (int i = 0; i < farmArea.Num();i++)
	{
		TaskFarm_Plant* newTask = new TaskFarm_Plant(FVector(farmArea[i].X, farmArea[i].Y, 50));
		building_Tasks.Add(newTask);
	}
}

void AFarmBuilding::ExcuteTask(BaseTask* task)
{
	for (int i = 0; i < building_Tasks.Num(); i++)
	{
		if (building_Tasks[i] == task)
		{
			GLog->Log("REMOVING TASK!!!!!!!!!!!!!!!!!!!");
			building_Tasks.RemoveAt(i);
			GLog->Log("Tasks Left!!! " + FString::FromInt(building_Tasks.Num()));
			delete task;
			return;
		}
	}
}
