// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class AICOURSEWORK_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter(const FObjectInitializer& ObjectInitializer);
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
		UCameraComponent* CameraComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player")
		UStaticMeshComponent* MeshComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera Arm")
		USpringArmComponent* CameraArm;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForward(float value);
	void MoveRight(float value);
	void Scroll(float value);
	void ZoomIn(float value);
	void ZoomOut(float value);

	

	void SpaceBar();
	void LeftMouse();
	void RightMouse();
	void ToggleMap();
	void Test();

	void SpawnAIAtLocation(FVector Location);	//this will spawn a new ai

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	UFUNCTION(BlueprintCallable)
		void BuildingSected(int buildingType);	//this will spawn a building on the map
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera Arm")
		bool clickedAi;
	UFUNCTION(BlueprintCallable)
		int GetAiInventoryAmount(int index);
	UFUNCTION(BlueprintCallable)
		FString GetAiInevntoryName(int index);
	class AAIEntity* AiClicked;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera Arm")
		bool clickedBuilding;
	UFUNCTION(BlueprintCallable)
		int GetBuildingInventoryAmount(int index);
	UFUNCTION(BlueprintCallable)
		FString GetBuildingInevntoryName(int index);
	class AStorageBuilding* BuildingClicked;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera Arm")
		bool clickedHouse;
	UFUNCTION(BlueprintCallable)
		int GetHouseInventoryAmount(int index);
	UFUNCTION(BlueprintCallable)
		FString GetHouseInevntoryName(int index);
	class AHouseBuilding* HouseClicked;

private:
	float currentArmLength, maxArmLength, minArmLength;
	TArray<class AAIEntity*> AIArray;
	int currentBuilding;
	bool updateBuildingLocation;
	class AActor* spawnedBuilding;
	class AMapController* mapRef;
	FVector buildingPos;

	class AAIEntity* IsAIClicked();	//this will work out of an ai is under the mouser
	class AStorageBuilding* IsBuildingClicked();
	class AHouseBuilding* IsHouseCliked();
	void UpdateBuildingLocation();	//this will update the lcoation of the building
	void RemoveBuilding();	//this will remove this building
	bool clickOne;	//these are used for dragging scaleable placeables
	bool clickTwo;
	FVector2D pointOne;	//this will store the click points
	FVector2D pointTwo;


};
