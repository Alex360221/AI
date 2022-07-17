// Fill out your copyright notice in the Description page of Project Settings.


#include "MapController.h"
#include <AICoursework/AI/Buildings/BaseProductBuilding.h>
#include <AICoursework/AI/Buildings/HouseBuilding.h>
#include <AICoursework/AI/Buildings/FarmBuilding.h>
#include <AICoursework/AI/Buildings/FishingHunt.h>
#include <AICoursework/AI/Buildings/StorageBuilding.h>
#include <AICoursework/AI/BuildingsExtra/RoadGravel.h>
#include <AICoursework/Pathfinding/Node.h>
#include <AICoursework/Pathfinding/PathFinding.h>
#include <AICoursework/AI/AIEntity.h>
#include <AICoursework/AI/Items/Crate.h>
#include <AICoursework/AI/Items/Item.h>

#include <AICoursework/AI/Jobs/FarmJob.h>
#include <AICoursework/AI/Jobs/FishFarmerJob.h>
#include <AICoursework/AI/Tasks/BaseTask.h>

#include <AICoursework/AI/Desires/CollectFoodDesire.h>

#include <Engine/Classes/Kismet/KismetMathLibrary.h>

// Sets default values
AMapController::AMapController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	gridSize = 50;
	mapSize = 500;

	aiMovementMapShow = false;

	tile_Blank = ObjectInitializer.CreateDefaultSubobject<UInstancedStaticMeshComponent>(this, (TEXT("Tile_Blank")));
	static ConstructorHelpers::FObjectFinder<UStaticMesh>Mesh01(TEXT("StaticMesh'/Game/Models/Square.Square'"));
	UStaticMesh* assset = Mesh01.Object;
	tile_Blank->SetStaticMesh(assset);
	tile_Blank->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	tile_Blank->SetCastShadow(false);

	tile_Block = ObjectInitializer.CreateDefaultSubobject<UInstancedStaticMeshComponent>(this, (TEXT("Tile_Block")));
	static ConstructorHelpers::FObjectFinder<UStaticMesh>Mesh02(TEXT("StaticMesh'/Game/Models/Square1.Square1'"));
	assset = Mesh02.Object;
	tile_Block->SetStaticMesh(assset);
	tile_Block->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	tile_Block->SetCastShadow(false);

	tile_Path = ObjectInitializer.CreateDefaultSubobject<UInstancedStaticMeshComponent>(this, (TEXT("Tile_Path")));
	static ConstructorHelpers::FObjectFinder<UStaticMesh>Mesh03(TEXT("StaticMesh'/Game/Models/Square2.Square2'"));
	assset = Mesh03.Object;
	tile_Path->SetStaticMesh(assset);
	tile_Path->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	tile_Path->SetCastShadow(false);
}

// Called when the game starts or when spawned
void AMapController::BeginPlay()
{
	Super::BeginPlay();
	CreateDeafaultPathFindingMap();	//this will create the pat finding map
	SpawnDeafultBuildings();
}

// Called every frame
void AMapController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMapController::SpawnDeafultBuildings()
{
	AActor* building = SpawnBuilding(5);
	BuildBuilding(building, 5, FVector2D(0, 0));	
	//fill storage building with some food
	buildings_Storage[0]->AddItem(new Item(0, "Fish", 200, 1.0f, 1));
}

AActor* AMapController::SpawnBuilding(int buildingType)
{
	FVector location = FVector::ZeroVector;
	FRotator rotation = FRotator::ZeroRotator;
	switch (buildingType)
	{
	case 1:
	{
		AHouseBuilding* house = GetWorld()->SpawnActor<AHouseBuilding>(location, rotation);
		buildingPoses = house->GetHouseArea(gridSize, FVector2D(0, 0),false);
		return house;
		GLog->Log("Hosue Spawned!!!");
		break;
	}
	case 2:
	{
		AFarmBuilding* farm = GetWorld()->SpawnActor<AFarmBuilding>(location, rotation);
		buildingPoses.Empty();
		return farm;
		break;
	}
	case 3:
	{
		ARoadGravel* road = GetWorld()->SpawnActor<ARoadGravel>(location, rotation);
		buildingPoses.Add(FVector2D(location.X, location.Y));
		return road;
		break;
	}
	case 4:
	{
		AFishingHunt* fishery = GetWorld()->SpawnActor<AFishingHunt>(location, rotation);
		buildingPoses = fishery->GetBuildingArea(gridSize, FVector2D(0, 0));
		return fishery;
		break;
	}
	case 5:
	{
		AStorageBuilding* storage = GetWorld()->SpawnActor<AStorageBuilding>(location, rotation);
		buildingPoses = storage->GetBuildingArea(gridSize, FVector2D(0, 0),false);
		return storage;
		break;
	}
	}
	return nullptr;
}

bool AMapController::CanBuild(class AActor* building, FVector2D pos)
{
	if (buildingPoses.Num() > 0)
	{
		for (int i = 0; i < buildingPoses.Num(); i++)
		{
			if (mapData.Contains((buildingPoses[i] + pos)))
			{
				building->SetActorHiddenInGame(true);
				return false; //can't build here as already have building here
			}			
		}
		building->SetActorHiddenInGame(false);
		return true;
	}
	if (mapData.Contains(pos))
	{
		building->SetActorHiddenInGame(true);
		return false; //can't build here as already have building here
	}
	building->SetActorHiddenInGame(false);
	return true;
}

void AMapController::BuildBuilding(AActor* building, int buildingType, FVector2D pos)
{
	if (CanBuild(building,pos))
	{
		switch (buildingType)
		{
		case 1:
		{
			AHouseBuilding* newHouse = Cast<AHouseBuilding>(building);
			if (newHouse != nullptr)
			{
				buildings_House.Add(newHouse);
				TArray<FVector2D> houseArea = newHouse->GetHouseArea(gridSize, pos,true);
				for (int i = 0; i < houseArea.Num();i++)
				{
					mapData.Add(houseArea[i], 1);	//this adds the building the mapData array
				}
				TArray<FVector2D> houseMovement = newHouse->GetHouseMovement(gridSize, pos);
				for (int i = 0; i < houseMovement.Num();i++)
				{
					pathFindingBlocking.Add(houseMovement[i]);	//this adds the building the mapData array
				}
				newHouse->SetHouseCenter(gridSize, pos);
				//also add the house to the path finding movement array
				GLog->Log("Hosue Built!!!");
			}

			break;
		}
		case 3:
		{
			ARoadGravel* road = Cast<ARoadGravel>(building);
			if (road != nullptr)
			{
				mapData.Add(pos, 3);	//this adds the new building ot the map data array
				pathFindingMovementCosts.Emplace(pos, 30);
				GLog->Log("Road Built!!!");
			}
			break;
		}
		case 5:
		{
			AStorageBuilding* newBuilding = Cast<AStorageBuilding>(building);
			if (newBuilding != nullptr)
			{
				buildings_Storage.Add(newBuilding);
				TArray<FVector2D> newBuidlingPoses = newBuilding->GetBuildingArea(gridSize, pos,true);			
				for (int i = 0; i < newBuidlingPoses.Num();i++)
				{
					mapData.Add(newBuidlingPoses[i], 5);	//this adds the ne building ot the map data array
					//pathFindingMovementCosts.Add(buildingArea[i], 500);	//high cost to move through farm tiles as we don't want them to move through unless they have to
				}
				TArray<FVector2D> buildingMovement = newBuilding->GetMovementArea(gridSize, pos);
				for (int i = 0; i < buildingMovement.Num();i++)
				{
					pathFindingBlocking.Add(buildingMovement[i]);	//this adds the building the mapData array
				}
				TArray<FVector2D> buildingPoints = newBuilding->GetStoragePoints(gridSize, pos);
				for (int i = 0; i < buildingPoints.Num();i++)
				{
					building_StoragePoints.Add(buildingPoints[i],newBuilding);	//this adds the storage points 
				}				
			}		
			break;
		}
		}
	}	
}

void AMapController::BuildFarm(AFarmBuilding* farm, FVector2D point1, FVector2D point2)
{
	if (farm != nullptr)
	{
		TArray<FVector2D> farmArea = farm->CreateFarmArea(point1, point2, gridSize);
		//check its vaild

		farm->SpawnFarmArea();
		buildings_Farm.Add(farm);
		for (int i = 0; i < farmArea.Num();i++)
		{			
			mapData.Add(farmArea[i], 2);	//this adds the ne building ot the map data array
			pathFindingMovementCosts.Add(farmArea[i], 500);	//high cost to move through farm tiles as we don't want them to move through unless they have to
		}	
		GLog->Log("Farm Built!!!");
	}
}

void AMapController::BuildProductionBuilding(ABaseProductBuilding* building, int buildingType, FVector2D pos)
{
	ABaseProductBuilding* newbuilding = Cast<ABaseProductBuilding>(building);
	if (newbuilding != nullptr)
	{		
		switch (buildingType)
		{
		case 4:
			AFishingHunt* fishHunt = Cast<AFishingHunt>(building);
			buildings_FishingHunt.Add(fishHunt);
			break;
		}
		TArray<FVector2D> buildingArea = newbuilding->GetBuildingArea(gridSize, pos);
		for (int i = 0; i < buildingArea.Num();i++)
		{
			mapData.Add(buildingArea[i], buildingType);	//this adds the building the mapData array
		}
		TArray<FVector2D> buildingMovement = newbuilding->GetBuildingMovement(gridSize, pos);
		for (int i = 0; i < buildingMovement.Num();i++)
		{
			pathFindingBlocking.Add(buildingMovement[i]);	//this adds the building the mapData array
		}
		newbuilding->mapRef = this;
		newbuilding->SetBuildingCenter(gridSize, pos);
		//also add the house to the path finding movement array
		GLog->Log("Hosue Built!!!");
	}
}

bool AMapController::FindAIHome(AAIEntity* ai)
{
	//shearch through homes to see if theres space in the homes
	for (int i = 0; i < buildings_House.Num();i++)
	{
		if (buildings_House[i] != nullptr)
		{
			if (buildings_House[i]->CanAILiveHere(ai))
			{
				ai->ai_HouseRef = buildings_House[i];
				return true;
			}		
		}
	}
	return false;
}

bool AMapController::FindAIJob(AAIEntity* ai, int jobType)
{
	switch(jobType)
	{
	case 1:
		for (int i = 0; i < buildings_Farm.Num();i++)
		{
			if (buildings_Farm[i] != nullptr)
			{
				if(buildings_Farm[i]->CanAIWorkHere(ai))
				{ 
					ai->ai_jobBase = new FarmJob();
					return true;
				}
			}
		}		
		break;
	case 2:
		GLog->Log(FString::FromInt(buildings_FishingHunt.Num()));
		for (int i = 0; i < buildings_FishingHunt.Num();i++)
		{
			if (buildings_FishingHunt[i] != nullptr)
			{
				if (buildings_FishingHunt[i]->CanAIWorkHere(ai))
				{
					ai->ai_jobBase = new FishFarmerJob();
					return true;
				}
			}
		}
		break;
	}
	return false;
}

bool AMapController::CanAIWork(ABaseProductBuilding* jobBuilding)
{
	return jobBuilding->building_CanWork;
}

bool AMapController::CanWanderHere(FVector2D nodePos)
{
	nodePos = FVector2D(int(nodePos.X / gridSize) * gridSize, int(nodePos.Y / gridSize) * gridSize);
	if (mapData.Contains(nodePos) || pathFindingBlocking.Contains(nodePos))
	{
		return false;
	}
	return true;
}

void AMapController::ToggleAIMovementMap()
{
	if (aiMovementMapShow == false)
	{
		//spawn tiles
		TArray<FVector2D> lala;
		pathFindingMapDeafult.GenerateKeyArray(lala);
		for (int i = 0; i < lala.Num();i++)
		{
			if (pathFindingBlocking.Contains(lala[i]))
			{
				FTransform pos;
				pos.SetLocation(FVector(lala[i].X, lala[i].Y,50));
				tile_Block->AddInstance(pos);
			}
		}
	}
	else
	{
		//remove tiles
	}
}

void AMapController::CreateDeafaultPathFindingMap()
{
	for (int x = (mapSize * -1); x <= mapSize;x++)
	{
		for (int y = (mapSize * -1); y <= mapSize;y++)
		{
			Node* newNode = new Node;
			FVector2D nodePos = FVector2D(x * gridSize, y * gridSize);
			newNode->SetNodePos(nodePos);
			pathFindingMapDeafult.Add(nodePos, newNode);
		}
	}
}

float AMapController::GetNodeMovementCost(FVector2D nodePos)
{
	if (pathFindingMovementCosts.Contains(nodePos))
	{
		return pathFindingMovementCosts.FindRef(nodePos);
	}
	return 50.f;// 50 as thats the length of the tile
}

void AMapController::AddCrate(ACrate* newCrate, FVector pos)
{
	crates.Add(pos, newCrate);
	//GLog->Log("Crate adeddededededededededededededededed");
}

void AMapController::RemoveCrate(class AAIEntity* ai, FVector pos)
{
	ACrate* crate = crates.FindRef(pos);
	if (crate != nullptr && ai != nullptr)
	{
		ai->AddItemToInventory(crate->crate_Item);
		crate->Destroy();
		crates.Remove(pos);
	}
}

class BaseTask* AMapController::GetTask(FVector aiPos, FVector2D tilePos)
{
	//GLog->Log("Player pos ..." + FString::FromInt(aiPos.X) + " " + FString::FromInt(aiPos.Y));
	//get the tile location
	tilePos *= gridSize;
	FVector2D aiTilePos = FVector2D(int(aiPos.X / gridSize) * gridSize, int(aiPos.Y / gridSize) * gridSize);
	//GLog->Log("Player pos ..." + FString::FromInt(aiPos.X) + " " + FString::FromInt(aiPos.Y));
	FVector tileLocation = FVector(aiTilePos.X + tilePos.X, aiTilePos.Y + tilePos.Y, 50);

	//GLog->Log("Player pos ..." + FString::FromInt(aiTilePos.X) + " " + FString::FromInt(aiTilePos.Y));
	//GLog->Log("tileLocation ..." + FString::FromInt(tileLocation.X) + " " + FString::FromInt(tileLocation.Y));

	//GLog->Log("Number of global taks.. " + FString::FromInt(globalTasks.Num()));
	//if (globalTasks.Contains(tileLocation)) { GLog->Log("task foudn!!!"); }
	//else { GLog->Log("no task !!!"); }
	return globalTasks.FindRef(tileLocation);
}

void AMapController::CreateGlobalTask(BaseTask* task)
{
	globalTasks.Add(task->task_Location, task);
}

void AMapController::RemoveGlobalTask(BaseTask* task)
{
	if (task != nullptr)
	{
		globalTasks.Remove(task->task_Location);
	}	
}

FVector2D AMapController::GetClosestStoragePoint(FVector aiPos)
{
	float lowestDis = 1000000;
	FVector2D lPoint;
	TArray<FVector2D> points;
	building_StoragePoints.GenerateKeyArray(points);
	for (int i = 0; i < points.Num();i++)
	{
		float newDis = FVector::Dist(FVector(points[i].X, points[i].Y, aiPos.Z), aiPos);
		if (newDis < lowestDis) 
		{ 
			lowestDis = newDis;
			lPoint = points[i];
		}
	}
	return lPoint;
}

AStorageBuilding* AMapController::GetStorageBuilding(FVector2D pos)
{
	return building_StoragePoints.FindRef(pos); 
}

AStorageBuilding* AMapController::GetStorageBuildingAtPos(FVector2D pos)
{
	//	//building at this pos
	//	//find correct storage
	for (int i = 0; i < buildings_Storage.Num();i++)
	{
		GLog->Log("Looking for building!!!!");
		AStorageBuilding* building = buildings_Storage[i];
		if (building != nullptr)
		{
			TArray<FVector2D> buildingPlaces = building->GetArea();
			for (int p = 0; p < buildingPlaces.Num();p++)
			{				
				if (buildingPlaces[p] == pos) { return buildings_Storage[i]; }
			}
		}
	}
	return nullptr;
}

AHouseBuilding* AMapController::GetHouseBuildingAtPos(FVector2D pos)
{
	//	//building at this pos
	//	//find correct storage
	for (int i = 0; i < buildings_House.Num();i++)
	{
		GLog->Log("Looking for building!!!!");
		AHouseBuilding* building = buildings_House[i];
		if (building != nullptr)
		{
			TArray<FVector2D> buildingPlaces = building->GetArea();
			for (int p = 0; p < buildingPlaces.Num();p++)
			{
				if (buildingPlaces[p] == pos) { return buildings_House[i]; }
			}
		}
	}
	return nullptr;
}

FVector2D AMapController::ClosestStorageWithFood(FVector aiPos, class CollectFoodDesire* desire)
{
	float lowestDis = 1000000;
	FVector2D lPoint;
	TArray<FVector2D> points;
	building_StoragePoints.GenerateKeyArray(points);
	for (int i = 0; i < points.Num();i++)
	{
		//check if storage place has food
		AStorageBuilding* building = building_StoragePoints.FindRef(points[i]);
		if (building != nullptr)
		{
			//check if building has food inside
			for (int f = 0; f < building->storageInventory.Num(); f++)
			{
				if (building->storageInventory[f]->item_Type == 1)
				{
					f = building->storageInventory.Num();
					float newDis = FVector::Dist(FVector(points[i].X, points[i].Y, aiPos.Z), aiPos);
					if (newDis < lowestDis)
					{
						lowestDis = newDis;
						lPoint = points[i];
					}
				}
			}
		}	
	}
	if (building_StoragePoints.Contains(lPoint))
	{
		desire->storageTarget = building_StoragePoints.FindRef(lPoint);
	}
	return lPoint;
}

