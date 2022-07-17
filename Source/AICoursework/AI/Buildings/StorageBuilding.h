// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "StorageBuilding.generated.h"

UCLASS()
class AICOURSEWORK_API AStorageBuilding : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AStorageBuilding(const FObjectInitializer& ObjectInitializer);
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
		UStaticMeshComponent* MeshComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
		UMaterialInterface* mat;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	TArray<FVector2D> GetBuildingArea(int gridSize, FVector2D buildingPos, bool Set);
	TArray<FVector2D> GetMovementArea(int gridSize, FVector2D buildingPos);
	TArray<FVector2D> GetStoragePoints(int gridSize, FVector2D buildingPos);
	void SetBuildingArea(int minX, int maxX, int minY, int maxY, TArray<FVector2D> movementArea, FVector2D newbuildingCenter);

	void AddItem(class Item* newItem);	//this will add an item
	bool TransferFoodToAi(class AAIEntity* ai, int amount);	//this will tarnsfer food to the ai inventory

	TArray<FVector2D> GetArea() { return buildingArea; }
	TArray<class Item*> storageInventory;
private:
	TArray<FVector2D> buildingArea;
	TArray<FVector2D> buildingMovement;
	TArray<FVector2D> buildingStoragePoints;
	


	int CheckItemInInventory(int itemId);	//this will check if this item is in the iventory
};
