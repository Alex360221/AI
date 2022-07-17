// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HouseBuilding.generated.h"

UCLASS()
class AICOURSEWORK_API AHouseBuilding : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHouseBuilding(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
		UStaticMeshComponent* MeshComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "House Details")
		int house_aiCount;
	int house_aiMax;
	class TArray<class Item*> house_Inventory;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	bool CanAILiveHere(class AAIEntity* ai);	//this will work out if an ai can live here
	TArray<FVector2D> GetHouseArea(int gridSize, FVector2D housePos, bool set);	//this will return the house area relative to the house location
	TArray<FVector2D> GetHouseMovement(int gridSize, FVector2D housePos);	//this will return the house movement relative to the house location
	void SetHouseCenter(int gridSize, FVector2D housePos);	//this will set the center of this house
	FVector2D GetHouseCenter() { return houseCenter; }	//this will get the house center relative the house location

	int CheckItemInInventory(int itemId);	//this will check to see if this item is in the inventory
	void AddItemToInventory(class Item* item);
	void RemoveItemFromInventory(class Item* item);
	void RemoveFood(class AAIEntity* ai, int amount);	//this will remove the food from the inventory
	void EatFood(class AAIEntity* ai);	//this will do the various checks then remove the food
	bool CheckEnoughFoodInHouse(int amount);	//checks if there is enough food to forfill the amount	
	TArray<FVector2D> GetArea() { return houseArea; }
	int GetAmountOfFood();	//this will work out how mcuh food is in the house
	int GetAmountOfFoodNeeded();	//this will work out how food this house needs for a while 


	bool AiCollectingFood;
private:
	TArray<class AAIEntity*> house_aiLivingHere;

	TArray<FVector2D> houseArea;
	TArray<FVector2D> houseMovement;
	FVector2D houseCenter;




};
