// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AIEntity.generated.h"


UCLASS()
class AICOURSEWORK_API AAIEntity : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAIEntity(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
		UStaticMeshComponent* MeshComp;
	UPROPERTY(EditAnywhere, Category = "AI Stats")
		FVector velocity;
	UPROPERTY(EditAnywhere, Category = "AI Stats")
		float speed;
	UPROPERTY(EditAnywhere, Category = "AI Stats")
		int ai_state;
	UPROPERTY(EditAnywhere, Category = "AI Stats")
		int ai_job;
	UPROPERTY(EditAnywhere, Category = "AI Stats")
		bool ai_canWork;
	UPROPERTY(EditAnywhere, Category = "AI Stats")
		bool ai_jobMove;
	UPROPERTY(EditAnywhere, Category = "AI Stats")
		FVector ai_jobLocation;
	UPROPERTY(EditAnywhere, Category = "AI Stats")
		bool ai_atJob;
	UPROPERTY(EditAnywhere, Category = "AI Stats")
		bool ai_hasTask;
	UPROPERTY(EditAnywhere, Category = "AI Stats")
		int ai_distanceToJob;
	UPROPERTY(EditAnywhere, Category = "AI Stats")
		FVector ai_home;
	UPROPERTY(EditAnywhere, Category = "AI Stats")
		bool ai_hasHome;
	UPROPERTY(EditAnywhere, Category = "AI Stats")
		bool ai_moveHome;
	UPROPERTY(EditAnywhere, Category = "AI Stats")
		int ai_distanceToHome;
	UPROPERTY(EditAnywhere, Category = "AI Stats")
		bool ai_goingHome;
	UPROPERTY(EditAnywhere, Category = "AI Stats")
		int ai_hunger;
	UPROPERTY(EditAnywhere, Category = "AI Stats")
		int ai_hungerMax;
	UPROPERTY(EditAnywhere, Category = "AI Stats")
		int ai_rest;
	UPROPERTY(EditAnywhere, Category = "AI Stats")
		int ai_restMax;
	UPROPERTY(EditAnywhere, Category = "AI Stats")
		FVector ai_wanderLocation;
	UPROPERTY(EditAnywhere, Category = "AI Stats")
		bool ai_goingWondering;
	UPROPERTY(EditAnywhere, Category = "AI Stats")
		bool ai_MoveToLocation;
	UPROPERTY(EditAnywhere, Category = "AI Stats")
		FVector ai_TargetLocation;
	UPROPERTY(EditAnywhere, Category = "AI Stats")
		float ai_CurrentWeight;

	class AMapController* mapRef;
	class AHouseBuilding* ai_HouseRef;
	class ABaseProductBuilding* ai_jobRef;
	class BaseJob* ai_jobBase;
	class BaseTask* ai_jobTask;
	bool pathFind;
	float ai_MaxWeight;

	TArray<class Item*> ai_Inventory;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//public fucntions
	class BaseTask* DistanceToPickUpTask();
	void AddItemToInventory(class Item* item);	//this will add the item to the inventory
	int CheckItemInInventory(int itemId);	//this will check if this item is in the iventory
	void EmtpyInventory(class AStorageBuilding* storage);	//this will empty the inventory
	void RemoveItemFromInventory(class Item* item);	//this will remove thsi item from the ineventory
	int FoodNeedToEat();	//this will work out how much food is needed to eat
	bool AiHasEnoughFoodToEat(int amount);	//this will work out if the ai has enough food it it's enventory to eat
	void TransferItemTypeToHouse(int itemType, int amount);	//this will transfer an item type to the house

private:
	TArray<class Desire*> desireArray;
	float timer;
	float aiMovementTimer;

	class Desire* d_wanderDesire;

	void CreateDesire();	//creates the list of desires
	void UpdateDesire();	//this will update the list of desires
	void UpdateAIStats();	//this will update all the stats every tick, for example hunger etc
	void GoHome();	//this will move the ai home and work out the distances
	void Wander();	//this will move the ai to the wandering location
	void GoToLocation();	//this will move the ai to a location
	void GoJobLocation();	//this will move the ai to a location chosen by the job
	void FindPath();	//this will find a path for the ai
	void GoToLocation(FVector pos);	//this will move the ai to this location
	bool CheckDesire(class Desire* desire1, class Desire* desire2);	//this will compare to see if the desires are the same
	TArray<FVector2D> path;
	FVector2D pathTargetPos;	//this stores the psotion which the ai is mving towards
	FVector2D pathLastTargetPos;	//this stores the psotion which the ai is mving towards
	
	bool VaildMoveLocation(FVector2D pos);	//this checks if the move pos is clear to move too
};
