// Fill out your copyright notice in the Description page of Project Settings.


#include "HouseBuilding.h"
#include <AICoursework/AI/AIEntity.h>
#include <AICoursework/AI/Items/Item.h>

// Sets default values
AHouseBuilding::AHouseBuilding(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//
	UMaterialInterface* mat = ObjectInitializer.CreateDefaultSubobject<UMaterialInterface>(this, (TEXT("House Mat")));
	static ConstructorHelpers::FObjectFinder<UMaterialInterface>Mat(TEXT("Material'/Game/Models/Houses/Material.Material'"));
	UMaterialInterface* material = Mat.Object;
	MeshComp = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, (TEXT("MeshComp")));
	static ConstructorHelpers::FObjectFinder<UStaticMesh>Mesh01(TEXT("StaticMesh'/Game/Models/Houses/Basic_House.Basic_House'"));
	UStaticMesh* assset = Mesh01.Object;
	MeshComp->SetStaticMesh(assset);
	MeshComp->SetMaterial(0, mat);
	SetRootComponent(MeshComp);
	SetActorEnableCollision(false);

	house_aiCount = 0;
	house_aiMax = 5;
	AiCollectingFood = false;
	//set up house area
	for (int x = -2; x <= 2; x++)
	{
		for (int y = -2; y <= 2; y++)
		{
			houseArea.Add(FVector2D(x, y));
			houseMovement.Add(FVector2D(x, y));
		}
	}
	//remove an area so ai can move into house
	houseMovement.Remove(FVector2D(-2, 0));
	houseMovement.Remove(FVector2D(-1, 0));
	houseMovement.Remove(FVector2D(0, 0));
	houseCenter = FVector2D(0, 0);
}

// Called when the game starts or when spawned
void AHouseBuilding::BeginPlay()
{
	Super::BeginPlay();
	SetActorTickEnabled(false);
}

// Called every frame
void AHouseBuilding::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool AHouseBuilding::CanAILiveHere(AAIEntity* ai)
{
	if (house_aiCount < house_aiMax)
	{
		//room to  live here so assign ai to this house
		house_aiCount += 1;
		house_aiLivingHere.Add(ai);
		ai->ai_home = FVector(houseCenter.X, houseCenter.Y,50);
		return true;
	}
	return false;
}

TArray<FVector2D> AHouseBuilding::GetHouseArea(int gridSize, FVector2D housePos, bool set)
{
	TArray<FVector2D> ra;
	for (int i = 0; i < houseArea.Num();i++)
	{
		FVector2D pos = FVector2D(houseArea[i].X * gridSize, houseArea[i].Y * gridSize);
		pos += housePos;
		ra.Add(pos);
	}
	if (set)
	{
		houseArea = ra;
	}
	return ra;
}

TArray<FVector2D> AHouseBuilding::GetHouseMovement(int gridSize, FVector2D housePos)
{
	TArray<FVector2D> ra;
	for (int i = 0; i < houseMovement.Num();i++)
	{
		FVector2D pos = FVector2D(houseMovement[i].X * gridSize, houseMovement[i].Y * gridSize);
		pos += housePos;
		ra.Add(pos);
	}
	return ra;
}

void AHouseBuilding::SetHouseCenter(int gridSize, FVector2D housePos)
{
	houseCenter * gridSize;
	houseCenter += housePos;
}

int AHouseBuilding::CheckItemInInventory(int itemId)
{
	//loop through all the items
	for (int i = 0; i < house_Inventory.Num();i++)
	{
		Item* checkItem = house_Inventory[i];
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

void AHouseBuilding::AddItemToInventory(Item* item)
{
	int index = CheckItemInInventory(item->item_ID);
	if (index >= 0)
	{
		house_Inventory[index]->item_Amount += item->item_Amount;
		//ai_CurrentWeight += item->item_Amount * item->item_Weight;
		GLog->Log("Item added to stack ai now has " + house_Inventory[index]->item_Name + FString::FromInt(house_Inventory[index]->item_Amount));
	}
	else
	{
		house_Inventory.Add(item);
		//ai_CurrentWeight += item->item_Amount * item->item_Weight;
		GLog->Log("New item added to inventory");
	}
}

void AHouseBuilding::RemoveItemFromInventory(Item* item)
{
}

void AHouseBuilding::RemoveFood(AAIEntity* ai, int amount)
{
	int currentFood = amount;
	//do we have food in house?
	if (house_Inventory.Num() > 0)
	{
		//loop through all the items in the inventory
		for (int i = 0; i < house_Inventory.Num(); i++)
		{
			//check if item is food
			if (house_Inventory[i]->item_Type == 1)
			{
				//enough of this item?
				if (house_Inventory[i]->item_Amount > currentFood)
				{
					//remove amount needed 
					house_Inventory[i]->item_Amount -= currentFood;
					ai->ai_hunger += (currentFood * 10);
					return;
				}
				else
				{
					//not enough food so remove item
					currentFood -= house_Inventory[i]->item_Amount;
					ai->ai_hunger += currentFood;
					RemoveItemFromInventory(house_Inventory[i]);
					i--;
				}
			}
		}
	}
}

void AHouseBuilding::EatFood(AAIEntity* ai)
{
	//do we have food in house?
	if (house_Inventory.Num() > 0)
	{
		//loop through all the items in the inventory
		for (int i = 0; i < house_Inventory.Num(); i++)
		{
			//check if item is food
			if (house_Inventory[i]->item_Type == 1)
			{

			}
		}
	}
	else
	{
		GLog->Log("Nothing in inventory");
	}
}

bool AHouseBuilding::CheckEnoughFoodInHouse(int amount)
{
	int currentFood = 0;
	//do we have food in house?
	if (house_Inventory.Num() > 0)
	{
		//loop through all the items in the inventory
		for (int i = 0; i < house_Inventory.Num(); i++)
		{
			//check if item is food
			if (house_Inventory[i]->item_Type == 1)
			{
				currentFood += house_Inventory[i]->item_Amount;
				GLog->Log("Found food " + FString::FromInt(currentFood) + "Amount Needed.." + FString::FromInt(amount));
			}
		}
		if (currentFood >= amount)
		{
			return true;
		}
	}
	return false;
}

int AHouseBuilding::GetAmountOfFood()
{
	int currentFood = 0;
	for (int i = 0; i < house_Inventory.Num(); i++)
	{
		//check if item is food
		if (house_Inventory[i]->item_Type == 1)
		{
			currentFood += house_Inventory[i]->item_Amount;
		}
	}
	return currentFood;
}

int AHouseBuilding::GetAmountOfFoodNeeded()
{
	return (house_aiLivingHere.Num() * (10 * 2));
}


