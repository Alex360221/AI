// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MapController.generated.h"

UCLASS()
class AICOURSEWORK_API AMapController : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMapController(const FObjectInitializer& ObjectInitializer);
	int gridSize;
	int mapSize;
	
		

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void SpawnDeafultBuildings();

	AActor* SpawnBuilding(int buildingType); //this will spawn the building
	bool CanBuild(class AActor* building, FVector2D pos);	//this will check weather it can build here
	void BuildBuilding(class AActor* building, int buildingType, FVector2D pos);	//this will build the building
	void BuildFarm(class AFarmBuilding* farm, FVector2D point1, FVector2D point2);	//this will build a farm placeable
	void BuildProductionBuilding(class ABaseProductBuilding* building, int buildingType, FVector2D pos);	//this will build the building
	void ClearBuidlingPoses() { buildingPoses.Empty(); }//this will remove the building poses
	bool FindAIHome(class AAIEntity* ai);	//this will find a home for the ai and assign it to the house
	bool FindAIJob(class AAIEntity* ai, int jobType);	//this will find if there is palce of work for this ai
	bool CanAIWork(class ABaseProductBuilding* jobBuilding);	//this will return weather the ai can work
	bool CanWanderHere(FVector2D nodePos);	//this wlill check if the wander location is vaild
	//A* pathfinding
	void ToggleAIMovementMap();	//this will spawn or despawn tiles to show the ai movement map
	
	void CreateDeafaultPathFindingMap();	//this creates a deafult map for the nodes
	class Node* GetNode(FVector2D nodePos) { return pathFindingMapDeafult.FindRef(nodePos); }	//gets the node at this pos
	bool IsNodeWalkable(FVector2D nodePos) { return pathFindingBlocking.Contains(nodePos); }	//checks if the node is in the blockign array
	float GetNodeMovementCost(FVector2D nodePos);	//this will return the movement cost of this node

	//crates
	void AddCrate(class ACrate* newCrate, FVector pos);	//this will add a crate
	void RemoveCrate(class AAIEntity* ai, FVector pos);	//this will remove a crate
	//golbal tasks
	class BaseTask* GetTask(FVector aiPos, FVector2D tilePos);	//this will work out the disatance to the task if thee is no task will return high number
	void CreateGlobalTask(class BaseTask* task);	//this adds a golbal task
	void RemoveGlobalTask(class BaseTask* task);	//this will remove a task
	FVector2D GetClosestStoragePoint(FVector aiPos);	//this will work out the cloest storage point to the player
	class AStorageBuilding* GetStorageBuilding(FVector2D pos);
	class AStorageBuilding* GetStorageBuildingAtPos(FVector2D pos);
	class AHouseBuilding* GetHouseBuildingAtPos(FVector2D pos);
	FVector2D ClosestStorageWithFood(FVector aiPos, class CollectFoodDesire* desire);	//this will find the closest storage place with food 
	
private:
	TArray<class AHouseBuilding*> buildings_House;
	TArray<class AFarmBuilding*> buildings_Farm;
	TArray<class AFishingHunt*> buildings_FishingHunt;
	TArray<class AStorageBuilding*> buildings_Storage;
	TMap<FVector, class BaseTask*> globalTasks;
	TMap<FVector, class ACrate*> crates;

	TArray<FVector2D> buildingPoses;
	TMap<FVector2D, int> mapData;
	TMap<FVector2D, AStorageBuilding*> building_StoragePoints;
	
	bool aiMovementMapShow;
	class UInstancedStaticMeshComponent* tile_Blank;
	class UInstancedStaticMeshComponent* tile_Block;
	class UInstancedStaticMeshComponent* tile_Path;

	
	//A* pathfinding
	TMap<FVector2D, class Node*> pathFindingMapDeafult;
	TArray<FVector2D> pathFindingBlocking;
	TMap<FVector2D, float> pathFindingMovementCosts;
	

};
