// Fill out your copyright notice in the Description page of Project Settings.
#include "PlayerCharacter.h"

#include "GameFramework/PlayerController.h"
#include "Components/CapsuleComponent.h"
#include <AICoursework/AI/AIEntity.h>
#include <AICoursework/AI/Buildings/HouseBuilding.h>
#include <AICoursework/AI/Buildings/FarmBuilding.h>
#include "MapController.h"
#include <AICoursework/AI/Items/Item.h>
#include <AICoursework/AI/Buildings/StorageBuilding.h>

// Sets default values
APlayerCharacter::APlayerCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	currentArmLength = 750;
	maxArmLength = 1500;
	minArmLength = 350;

	GetCapsuleComponent()->InitCapsuleSize(20.f, 40.0f);

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera Arm"));
	CameraArm->SetupAttachment(GetCapsuleComponent());
	CameraArm->TargetArmLength = currentArmLength;
	CameraArm->SetRelativeRotation(FRotator(-70, 0, 0));//sets the angle

	CameraComp->SetupAttachment(CameraArm);

	currentBuilding = 0;
	updateBuildingLocation = false;

	clickOne = false;
	clickTwo = false;
	clickedAi = false;
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	mapRef = GetWorld()->SpawnActor<AMapController>(FVector::ZeroVector, FRotator::ZeroRotator);

	for (int i = 0; i < 5; i++)
	{
		SpawnAIAtLocation(FVector(0, -300, 50));
	}	
}

void APlayerCharacter::MoveForward(float vaule)
{
	if ((Controller) && (vaule != 0.0f))
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), vaule);
	}
}

void APlayerCharacter::MoveRight(float vaule)
{
	if ((Controller) && (vaule != 0.0f))
	{
		AddMovementInput(GetActorRightVector(), vaule);
	}
}

void APlayerCharacter::Scroll(float value)
{
	if (value > 0) { ZoomIn(value); }
	else if (value < 0) { ZoomOut(value); }
}

void APlayerCharacter::ZoomIn(float value)
{
	//GLog->Log("Zoom in" + FString::FromInt(value));
	if (currentArmLength + (value * 10) >= minArmLength)
	{
		currentArmLength += (value * 10);
		CameraArm->TargetArmLength = currentArmLength;
	}
}

void APlayerCharacter::ZoomOut(float value)
{
	//GLog->Log("Zoom out" + FString::FromInt(value));
	if (currentArmLength + (value*10) <= maxArmLength)
	{
		currentArmLength += (value * 10);
		CameraArm->TargetArmLength = currentArmLength;
	}
}

void APlayerCharacter::SpaceBar()
{
	//SpawnAIAtLocation(GetActorLocation());
}

void APlayerCharacter::LeftMouse()
{
	//check if click on ai
	AiClicked = IsAIClicked();
	if (AiClicked != nullptr)
	{
		clickedAi = true;
		GLog->Log("Clciked on Ai");
	}
	else
	{
		clickedAi = false;
		//GLog->Log("Clciked on Ai");
	}
	//check if building is clicked
	BuildingClicked = IsBuildingClicked();
	if (BuildingClicked != nullptr)
	{
		clickedBuilding = true;
		GLog->Log("Clciked on Building");
	}
	else
	{
		clickedBuilding = false;
	}
	//check if house is clicked
	HouseClicked = IsHouseCliked();
	if (HouseClicked != nullptr)
	{
		clickedHouse = true;
		GLog->Log("Clciked on House");
	}
	else
	{
		clickedHouse = false;
	}

	if (spawnedBuilding == nullptr) { return; }
	AFarmBuilding* farm = Cast<AFarmBuilding>(spawnedBuilding);
	if (farm != nullptr)
	{
		//farm needs two clicks
		if (clickOne == false)
		{
			//place first point...
			clickOne = true;
			pointOne = FVector2D(buildingPos.X, buildingPos.Y);
			return;
		}
		if (clickTwo == false)
		{
			//place second point..
			clickTwo = true;
			pointTwo = FVector2D(buildingPos.X, buildingPos.Y);
			//spawned second point now reset it
			if (mapRef != nullptr) { mapRef->BuildFarm(farm, pointOne, pointTwo); }
			clickOne = false;
			clickTwo= false;
			if (mapRef != nullptr) { spawnedBuilding = mapRef->SpawnBuilding(currentBuilding); }	//this respawns another building
			return;
		}
	}

	ABaseProductBuilding* building = Cast<ABaseProductBuilding>(spawnedBuilding);
	if (building != nullptr)
	{
		//building is of type base production so needs building
		if (mapRef != nullptr) { mapRef->BuildProductionBuilding(building, currentBuilding, FVector2D(buildingPos.X, buildingPos.Y)); }
		if (mapRef != nullptr) { spawnedBuilding = mapRef->SpawnBuilding(currentBuilding); }	//this respawns another building
		return;
	}

	if (spawnedBuilding != nullptr)
	{
		if (mapRef != nullptr) { mapRef->BuildBuilding(spawnedBuilding, currentBuilding, FVector2D(buildingPos.X, buildingPos.Y)); }
		if (mapRef != nullptr) { spawnedBuilding = mapRef->SpawnBuilding(currentBuilding); }	//this respawns another building
		return;
	}
}

void APlayerCharacter::RightMouse()
{
	if (spawnedBuilding != nullptr)
	{
		GLog->Log("Remove building!!!!");
		currentBuilding = 0;
		updateBuildingLocation = false;
		RemoveBuilding();
	}
}

void APlayerCharacter::ToggleMap()
{
	mapRef->ToggleAIMovementMap();
}

void APlayerCharacter::Test()
{
}

void APlayerCharacter::SpawnAIAtLocation(FVector Location)
{
	FVector newLocation = FVector(Location.X, Location.Y, 50);
	FRotator rotation = FRotator::ZeroRotator;
	AAIEntity* newAI = GetWorld()->SpawnActor<AAIEntity>(newLocation, rotation);
	if (newAI != nullptr)
	{
		AIArray.Add(newAI);
		GLog->Log("Spawned new AI");
		//can now set ai varibles and stuff
		newAI->mapRef = mapRef;
	}
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (updateBuildingLocation)
	{
		UpdateBuildingLocation();
	}
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAction("SpaceBar", IE_Pressed,this, &APlayerCharacter::SpaceBar);
	PlayerInputComponent->BindAction("LeftMouse", IE_Pressed,this, &APlayerCharacter::LeftMouse);
	PlayerInputComponent->BindAction("RightMouse", IE_Pressed,this, &APlayerCharacter::RightMouse);
	PlayerInputComponent->BindAction("KKey", IE_Pressed,this, &APlayerCharacter::ToggleMap);

	InputComponent->BindAxis("MoveX", this, &APlayerCharacter::MoveForward);
	InputComponent->BindAxis("MoveY", this, &APlayerCharacter::MoveRight);
	InputComponent->BindAxis("Scroll", this, &APlayerCharacter::Scroll);

}

void APlayerCharacter::BuildingSected(int buildingType)
{
	if (currentBuilding != buildingType)
	{
		//this will spawn this new building
		RemoveBuilding();	//removes the last building
		GLog->Log("Spawn Building!!!!");
		currentBuilding = buildingType;
		updateBuildingLocation = true;
		if (mapRef != nullptr) { spawnedBuilding = mapRef->SpawnBuilding(buildingType); }
		//SpawnBuilding(buildingType);
	}
	else
	{
		//this will remove this building
		GLog->Log("Remove building!!!!");
		currentBuilding = 0;
		updateBuildingLocation = false;
		RemoveBuilding();
	}
}

int APlayerCharacter::GetAiInventoryAmount(int index)
{
	if (AiClicked != nullptr)
	{
		if (AiClicked->ai_Inventory.IsValidIndex(index))
		{
			return AiClicked->ai_Inventory[index]->item_Amount;
		}
	}
	return 0;
}

FString APlayerCharacter::GetAiInevntoryName(int index)
{
	if (AiClicked != nullptr)
	{
		if (AiClicked->ai_Inventory.IsValidIndex(index))
		{			
			return AiClicked->ai_Inventory[index]->item_Name;
		}
	}
	return FString();
}

int APlayerCharacter::GetBuildingInventoryAmount(int index)
{
	//GLog->Log("Working123dd");
	if (BuildingClicked != nullptr)
	{
		//GLog->Log("Working");
		if (BuildingClicked->storageInventory.IsValidIndex(index))
		{
			return BuildingClicked->storageInventory[index]->item_Amount;
		}
	}
	return 0;
}

FString APlayerCharacter::GetBuildingInevntoryName(int index)
{
	if (BuildingClicked != nullptr)
	{
		if (BuildingClicked->storageInventory.IsValidIndex(index))
		{
			return BuildingClicked->storageInventory[index]->item_Name;
		}
	}
	return FString();
}

int APlayerCharacter::GetHouseInventoryAmount(int index)
{
	if (HouseClicked != nullptr)
	{
		//GLog->Log("Working");
		if (HouseClicked->house_Inventory.IsValidIndex(index))
		{
			return HouseClicked->house_Inventory[index]->item_Amount;
		}
	}
	return 0;
}

FString APlayerCharacter::GetHouseInevntoryName(int index)
{
	if (HouseClicked != nullptr)
	{
		if (HouseClicked->house_Inventory.IsValidIndex(index))
		{
			return HouseClicked->house_Inventory[index]->item_Name;
		}
	}
	return FString();
}

AAIEntity* APlayerCharacter::IsAIClicked()
{
	APlayerController* PC = Cast<APlayerController>(GetController());
	//UWorld* const World = GetWorld();
	if (PC)
	{
		FVector Location, Direction;
		PC->DeprojectMousePositionToWorld(Location, Direction);	//mouse location
		FVector end = Location + Direction * 10000;
		FHitResult hitResult;
		if (GetWorld()->LineTraceSingleByChannel(hitResult, Location, end, ECC_Visibility))
		{			
			int gridSize = 0;
			if (mapRef != nullptr)
			{
				gridSize = mapRef->gridSize;
				FVector2D clickedGrid = FVector2D(int(hitResult.ImpactPoint.X / gridSize) * gridSize, int(hitResult.ImpactPoint.Y / gridSize) * gridSize);
				for (int i = 0; i < AIArray.Num();i++)
				{
					AAIEntity* checkAI = AIArray[i];
					if (checkAI != nullptr)
					{
						FVector AIPos = checkAI->GetActorLocation();
						FVector2D aiPos = FVector2D(int(AIPos.X / gridSize) * gridSize, int(AIPos.Y / gridSize) * gridSize);
						if (clickedGrid == aiPos)
						{
							return checkAI;
						}
					}
				}
			}		
		}
	}
	return nullptr;
}

AStorageBuilding* APlayerCharacter::IsBuildingClicked()
{
	APlayerController* PC = Cast<APlayerController>(GetController());
	//UWorld* const World = GetWorld();
	if (PC)
	{
		FVector Location, Direction;
		PC->DeprojectMousePositionToWorld(Location, Direction);	//mouse location
		FVector end = Location + Direction * 10000;
		FHitResult hitResult;
		if (GetWorld()->LineTraceSingleByChannel(hitResult, Location, end, ECC_Visibility))
		{
			int gridSize = 0;
			if (mapRef != nullptr)
			{
				gridSize = mapRef->gridSize;
				FVector2D clickedGrid = FVector2D(int(hitResult.ImpactPoint.X / gridSize) * gridSize, int(hitResult.ImpactPoint.Y / gridSize) * gridSize);
				return mapRef->GetStorageBuildingAtPos(clickedGrid);				
			}
		}
	}
	return nullptr;
}

AHouseBuilding* APlayerCharacter::IsHouseCliked()
{
	APlayerController* PC = Cast<APlayerController>(GetController());
	//UWorld* const World = GetWorld();
	if (PC)
	{
		FVector Location, Direction;
		PC->DeprojectMousePositionToWorld(Location, Direction);	//mouse location
		FVector end = Location + Direction * 10000;
		FHitResult hitResult;
		if (GetWorld()->LineTraceSingleByChannel(hitResult, Location, end, ECC_Visibility))
		{
			int gridSize = 0;
			if (mapRef != nullptr)
			{
				gridSize = mapRef->gridSize;
				FVector2D clickedGrid = FVector2D(int(hitResult.ImpactPoint.X / gridSize) * gridSize, int(hitResult.ImpactPoint.Y / gridSize) * gridSize);
				return mapRef->GetHouseBuildingAtPos(clickedGrid);
			}
		}
	}
	return nullptr;
}

void APlayerCharacter::UpdateBuildingLocation()
{
	APlayerController* PC = Cast<APlayerController>(GetController());
	//UWorld* const World = GetWorld();
	if (PC)
	{
		FVector Location, Direction;		
		PC->DeprojectMousePositionToWorld(Location, Direction);	//mouse location
		FVector end = Location + Direction * 10000;
		FHitResult hitResult;
		if (GetWorld()->LineTraceSingleByChannel(hitResult, Location, end, ECC_Visibility))
		{
			if (spawnedBuilding != nullptr)
			{
				int gridSize = 0;
				if (mapRef != nullptr) 
				{ 
					gridSize = mapRef->gridSize; 
					buildingPos = FVector(int(hitResult.ImpactPoint.X / gridSize) * gridSize, int(hitResult.ImpactPoint.Y / gridSize) * gridSize, 0);
					spawnedBuilding->SetActorLocation(buildingPos);
					//check weather it can build here
					mapRef->CanBuild(spawnedBuilding,FVector2D(buildingPos.X, buildingPos.Y));
				}
			}
		}
	}
}

void APlayerCharacter::RemoveBuilding()
{
	if (spawnedBuilding != nullptr)
	{
		mapRef->ClearBuidlingPoses();
		spawnedBuilding->Destroy();
		spawnedBuilding = nullptr;
	}
}

