// Fill out your copyright notice in the Description page of Project Settings.


#include "StorageBuilding.h"
#include <AICoursework/AI/Items/Item.h>
#include <AICoursework/AI/AIEntity.h>

// Sets default values
AStorageBuilding::AStorageBuilding(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	MeshComp = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, (TEXT("MeshComp")));
	mat = ObjectInitializer.CreateDefaultSubobject<UMaterialInterface>(this, (TEXT("Mat")));
	static ConstructorHelpers::FObjectFinder<UMaterialInterface>Mat(TEXT("Material'/Game/Models/Storage/Material.Material'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh>Mesh02(TEXT("StaticMesh'/Game/Models/Storage/Barn.Barn'"));
	UStaticMesh* assset = Mesh02.Object;
	MeshComp->SetStaticMesh(assset);
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComp->SetCastShadow(false);
	MeshComp->SetMaterial(0, mat);


	TArray<FVector2D> removalArea;
	removalArea.Add(FVector2D(-5, -2));
	removalArea.Add(FVector2D(-5, -1));
	removalArea.Add(FVector2D(-5, 0));
	removalArea.Add(FVector2D(-5, 1));
	removalArea.Add(FVector2D(-5, 2));
	removalArea.Add(FVector2D(5, -2));
	removalArea.Add(FVector2D(5, -1));
	removalArea.Add(FVector2D(5, 0));
	removalArea.Add(FVector2D(5, 1));
	removalArea.Add(FVector2D(5, 2));
	SetBuildingArea(-5, 5, -2, 2, removalArea, FVector2D(0, 0));
	buildingStoragePoints.Add(FVector2D(-5, 0));
	buildingStoragePoints.Add(FVector2D(5, 0));
}

// Called when the game starts or when spawned
void AStorageBuilding::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AStorageBuilding::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

TArray<FVector2D> AStorageBuilding::GetBuildingArea(int gridSize, FVector2D buildingPos, bool Set)
{
	TArray<FVector2D> ra;
	for (int i = 0; i < buildingArea.Num();i++)
	{
		FVector2D pos = FVector2D(buildingArea[i].X * gridSize, buildingArea[i].Y * gridSize);
		pos += buildingPos;
		ra.Add(pos);
	}
	if (Set) { buildingArea = ra; }
	return ra;
}

TArray<FVector2D> AStorageBuilding::GetMovementArea(int gridSize, FVector2D buildingPos)
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

TArray<FVector2D> AStorageBuilding::GetStoragePoints(int gridSize, FVector2D buildingPos)
{
	TArray<FVector2D> ra;
	for (int i = 0; i < buildingStoragePoints.Num();i++)
	{
		FVector2D pos = FVector2D(buildingStoragePoints[i].X * gridSize, buildingStoragePoints[i].Y * gridSize);
		pos += buildingPos;
		ra.Add(pos);
	}
	return ra;
}

void AStorageBuilding::SetBuildingArea(int minX, int maxX, int minY, int maxY, TArray<FVector2D> movementArea, FVector2D newbuildingCenter)
{
	for (int x = minX; x <= maxX; x++)
	{
		for (int y = minY; y <= maxY; y++)
		{
			buildingArea.Add(FVector2D(x, y));
			if (!movementArea.Contains(FVector2D(x, y)))
			{
				buildingMovement.Add(FVector2D(x, y));
			}		
		}
	}
}

void AStorageBuilding::AddItem(Item* newItem)
{
	int index = CheckItemInInventory(newItem->item_ID);
	if (index >= 0)
	{
		storageInventory[index]->item_Amount += newItem->item_Amount;
		//ai_CurrentWeight += item->item_Amount * item->item_Weight;
		GLog->Log("Item added to stack ai now has " + storageInventory[index]->item_Name + FString::FromInt(storageInventory[index]->item_Amount));
	}
	else
	{
		storageInventory.Add(newItem);
		//ai_CurrentWeight += item->item_Amount * item->item_Weight;
		GLog->Log("New item added to inventory");
	}
}

bool AStorageBuilding::TransferFoodToAi(AAIEntity* ai, int amount)
{
	int currentFood = amount;
	//do we have food in house?
	if (storageInventory.Num() > 0)
	{
		//loop through all the items in the inventory
		for (int i = 0; i < storageInventory.Num(); i++)
		{
			//check if item is food
			if (storageInventory[i]->item_Type == 1)
			{
				//check if the amount of food in this stack will forefill amount
				if (storageInventory[i]->item_Amount >= currentFood)
				{
					//enough food so trasnfer food to ai
					ai->AddItemToInventory(
						new Item(storageInventory[i]->item_ID, storageInventory[i]->item_Name, currentFood, storageInventory[i]->item_Weight, storageInventory[i]->item_Type));
					storageInventory[i]->item_Amount -= currentFood;
					GLog->Log("TRANSFERED FOOD!! Amount Transfered = " + FString::FromInt(amount) + "Amount Left" + FString::FromInt(storageInventory[i]->item_Amount));
					return true;
				}
				else
				{
					//transfer what we can					
					ai->AddItemToInventory(
						new Item(storageInventory[i]->item_ID, storageInventory[i]->item_Name, storageInventory[i]->item_Amount, storageInventory[i]->item_Weight, storageInventory[i]->item_Type));
					currentFood -= storageInventory[i]->item_Amount;
					//as we've tranfered all of the stack remove this item from the ineventory
					storageInventory.RemoveAt(i);
					i--;	//i has to go back one as the array would have been resised
				}				
			}
		}
	}
	return false;
}

int AStorageBuilding::CheckItemInInventory(int itemId)
{
	//loop through all the items
	for (int i = 0; i < storageInventory.Num();i++)
	{
		Item* checkItem = storageInventory[i];
		if (checkItem != nullptr)	//check item is vaild
		{
			if (checkItem->item_ID == itemId)	//check item ids
			{
				return i;		//itemid matches so return this index
			}
		}
	}
	return -1;
}



