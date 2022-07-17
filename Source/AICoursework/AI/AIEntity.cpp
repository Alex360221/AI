// Fill out your copyright notice in the Description page of Project Settings.


#include "AIEntity.h"
#include "Components/StaticMeshComponent.h"
#include <AICoursework/MapController.h>
#include <AICoursework/AI/Jobs/BaseJob.h>
#include <AICoursework/AI/Desires/Desire.h>
#include <AICoursework/AI/Desires/JobDesire.h>
#include <AICoursework/AI/Desires/WanderDesire.h>
#include <AICoursework/AI/Desires/EatDesire.h>
#include <AICoursework/AI/Desires/RestDesire.h>
#include <AICoursework/AI/Desires/GoHomeDesire.h>
#include <AICoursework/AI/Desires/PickUpDesire.h>
#include <AICoursework/AI/Desires/DropOffDesire.h>
#include <AICoursework/AI/Desires/CollectFoodDesire.h>
#include <AICoursework/Pathfinding/PathFinding.h>
#include <AICoursework/AI/Items/Item.h>
#include <AICoursework/AI/Tasks/BaseTask.h>
#include <AICoursework/AI/Buildings/StorageBuilding.h>
#include <AICoursework/AI/Buildings/HouseBuilding.h>

// Sets default values
AAIEntity::AAIEntity(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	MeshComp = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, (TEXT("MeshComp")));
	static ConstructorHelpers::FObjectFinder<UStaticMesh>Mesh01(TEXT("StaticMesh'/Game/Models/BasicCube.BasicCube'"));
	UStaticMesh* assset = Mesh01.Object;
	MeshComp->SetStaticMesh(assset);
	SetRootComponent(MeshComp);
	SetActorEnableCollision(false);

	timer = 0;
	velocity = FVector(0, 0, 0);
	speed = 70;
	//AI brain stuff
	ai_state = 0;
	ai_job = 2;
	ai_canWork = false;
	ai_jobMove = false;
	ai_jobLocation = FVector(0, 0, 50);
	ai_distanceToJob = 0;
	ai_atJob = false;
	ai_home = FVector(0, 0, 50);
	ai_hasHome = false;
	ai_moveHome = false;
	ai_distanceToHome = 10;
	ai_goingHome = false;
	ai_hungerMax = 100;
	ai_hunger = ai_hungerMax;
	ai_restMax = 100;
	ai_rest = ai_restMax;
	ai_wanderLocation = FVector(0,0,50);
	ai_goingWondering = false;
	ai_MaxWeight = 100.f;
	ai_CurrentWeight = 0.0f;

	pathFind = false;
}

// Called when the game starts or when spawned
void AAIEntity::BeginPlay()
{
	Super::BeginPlay();
	CreateDesire();
}

// Called every frame
void AAIEntity::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	timer += DeltaTime;
	aiMovementTimer += DeltaTime;

	if (timer >= 1.0f) {
		//actions that should happen every second;
		timer = 0.0f;
		UpdateAIStats();
		UpdateDesire();
		if (pathFind)
		{
			FindPath();
		}
	}
	if (aiMovementTimer >= 0.1)
	{
		if (ai_moveHome == true)
		{//move the ai to home
			GoHome();
		}
		if (ai_jobMove == true)
		{//move ai to location chosen by job
			GoJobLocation();
		}
		if (ai_MoveToLocation == true)
		{//move ai to location
			GoToLocation();
		}
		if (CheckDesire(desireArray[0], d_wanderDesire))
		{//move the ai to wondering pos
			Wander();
		}
	}	

	SetActorLocation(GetActorLocation() + (velocity * DeltaTime));
}

class BaseTask* AAIEntity::DistanceToPickUpTask()
{
	//find the closest pick up within 3 tiles of ai
	//this loop will start next to the player and work outwards
	float shortestDis = 1000;
	//GLog->Log("Player pos ..." + FString::FromInt(GetActorLocation().X) + " " + FString::FromInt(GetActorLocation().Y));
	BaseTask* shortestTask = mapRef->GetTask(GetActorLocation(), FVector2D(0, 0));
	//as we know we've just checked the ai pos if this returns a task and isn't active we can return this task
	if (shortestTask != nullptr && !shortestTask->task_Active) { return shortestTask; }
	for (int z = 1; z < 4;z++)
	{		
		for (int x = z * -1; x <= z;x++)
		{
			for (int y = z * -1; y <= z;y++)
			{
				BaseTask* task = mapRef->GetTask(GetActorLocation(), FVector2D(x, y));
				if (task != nullptr && !task->task_Active)
				{
					//vaild task work out distance to task
					return task;	//no need to calcuate distance to task as we are shearching from the middle so the task will be the closest
				}
			}
		}
		//if (shortestDis != 1000) { return shortestDis; }//if not 1000, then a task has been found
	}
	return NULL;
}

void AAIEntity::AddItemToInventory(Item* item)
{
	//check if item type is already in inventory
	if (item == nullptr) { return; }
	int index = CheckItemInInventory(item->item_ID);
	if (index >= 0)
	{
		ai_Inventory[index]->item_Amount += item->item_Amount;
		ai_CurrentWeight += item->item_Amount * item->item_Weight;
		//GLog->Log("Item added to stack ai now has " + ai_Inventory[index]->item_Name + FString::FromInt(ai_Inventory[index]->item_Amount));
	}
	else
	{
		ai_Inventory.Add(item);
		ai_CurrentWeight += item->item_Amount * item->item_Weight;
		//GLog->Log("New item added to inventory");
	}
}

int AAIEntity::CheckItemInInventory(int itemId)
{
	//loop through all the items
	for (int i = 0; i < ai_Inventory.Num();i++)
	{
		Item* checkItem = ai_Inventory[i];
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

void AAIEntity::EmtpyInventory(class AStorageBuilding* storage)
{
	if (storage == nullptr) { return; }
	for (int i = 0; i < ai_Inventory.Num();i++)
	{		
		storage->AddItem(ai_Inventory[i]);
		ai_CurrentWeight -= ai_Inventory[i]->item_Weight * ai_Inventory[i]->item_Amount;
		ai_Inventory.RemoveAt(i);
	}
}

void AAIEntity::RemoveItemFromInventory(Item* item)
{
	ai_Inventory.Remove(item);
}

int AAIEntity::FoodNeedToEat()
{
	int hunger = ai_hungerMax - ai_hunger;
	return int(hunger / 10);
}

bool AAIEntity::AiHasEnoughFoodToEat(int amount)
{
	int currentFood = 0;
	//do we have food in house?
	if (ai_Inventory.Num() > 0)
	{
		//loop through all the items in the inventory
		for (int i = 0; i < ai_Inventory.Num(); i++)
		{
			//check if item is food
			if (ai_Inventory[i]->item_Type == 1)
			{
				currentFood += ai_Inventory[i]->item_Amount;
			}
		}
		if (currentFood >= amount)
		{
			return true;
		}
	}
	return false;
}

void AAIEntity::TransferItemTypeToHouse(int itemType, int amount)
{
	//loop through all the items in the inventory
	for (int i = 0; i < ai_Inventory.Num(); i++)
	{
		//check if item is food
		if (ai_Inventory[i]->item_Type == 1)
		{
			//check if there is enough of this type of food to forfill amount			
			Item* currentItem = ai_Inventory[i];
			if (currentItem->item_Amount >= amount)
			{
				//remove part of this item
				ai_HouseRef->AddItemToInventory(new Item(currentItem->item_ID, currentItem->item_Name, amount, amount * 1.0f, currentItem->item_Type));
				ai_Inventory[i]->item_Amount -= amount;
				ai_Inventory[i]->item_Weight -= ai_Inventory[i]->item_Amount * 1;
			}
			else
			{
				//if not remove all this item 
				ai_HouseRef->AddItemToInventory(ai_Inventory[i]);
				//adjust amount
				amount -= ai_Inventory[i]->item_Amount;
				//now remove item
				RemoveItemFromInventory(ai_Inventory[i]);
			}		
		}
	}
}

void AAIEntity::CreateDesire()
{
	desireArray.Add(new JobDesire());
	d_wanderDesire = new WanderDesire();
	desireArray.Add(d_wanderDesire);
	desireArray.Add(new EatDesire());
	desireArray.Add(new RestDesire());
	desireArray.Add(new GoHomeDesire());
	desireArray.Add(new PickUpDesire());
	desireArray.Add(new DropOffDesire());
	desireArray.Add(new CollectFoodDesire());
}

void AAIEntity::UpdateDesire()
{//updates all the desires
	ai_distanceToHome = FVector::Distance(GetActorLocation(), ai_home);
	for (int i = 0; i < desireArray.Num();i++)
	{
		Desire* currentDes = desireArray[i];
		if (currentDes != nullptr)
		{
			currentDes->CalculateDesire(this);
		}
	}
	//GLog->Log("Desire Length " + FString::FromInt(desireArray.Num()));
	//sort desires
	TArray<Desire*> sortedArray;
	for (int i = 0; i < desireArray.Num();i++)	//loops through the array that needs sorting
	{		
		bool added = false;
		for (int s = 0; s < sortedArray.Num();s++)	//loops through the sorted array
		{
			Desire* currentSortedDes = sortedArray[s];
			if (desireArray[i]->Comparator(currentSortedDes))
			{//replace value with new value
				sortedArray.EmplaceAt(s,desireArray[i]);
				added = true;
				s = sortedArray.Num();	//as added s can be set to vaule which makes it leave the loop
			}
		}
		if (i == 0) 
		{ 
			added = true;
			sortedArray.Add(desireArray[0]); 			
		}//first element needs to be manul assigned
		if (added == false) { sortedArray.Add(desireArray[i]); }
	}
	desireArray = sortedArray;
	//GLog->Log("Desire Length after " + FString::FromInt(desireArray.Num()));
	//excute top desire
	desireArray[0]->ExcuteDesire(this);
}

void AAIEntity::UpdateAIStats()
{
	ai_hunger--;
	ai_rest--;
	
	if (ai_hasHome == false)
	{
		ai_hasHome = mapRef->FindAIHome(this);
	}
	if (ai_jobRef == nullptr)
	{
		//doesn't have job assigned 
		mapRef->FindAIJob(this, ai_job);
	}
	else
	{
		ai_canWork = mapRef->CanAIWork(ai_jobRef);
		ai_distanceToJob = FVector::Distance(GetActorLocation(), ai_jobLocation);	//update distance
	}
	float newWeight = 0;
	for (int i = 0; i < ai_Inventory.Num();i++)
	{
		newWeight += (ai_Inventory[i]->item_Amount * 1);//ai_Inventory[i]->item_Weight);
	}
	ai_CurrentWeight = newWeight;
	ai_distanceToHome = FVector::Distance(GetActorLocation(), ai_home);
}

void AAIEntity::GoHome()
{
	if (ai_hasHome)
	{
		ai_atJob = false;	//no longer at job
		GoToLocation(ai_home);
		ai_distanceToHome = FVector::Distance(GetActorLocation(), ai_home);
		//GLog->Log("Distance ... " + FString::FromInt(ai_distanceToHome));
		if (ai_distanceToHome <= 0)
		{
			//at home
			ai_distanceToHome = 0;
			ai_goingHome = false;
			ai_moveHome = false;
		}
	}	
}

void AAIEntity::Wander()
{
	FVector aiPos = GetActorLocation();
	//check distance to wonder location
	float distanceToWonderPos = FVector::Distance(aiPos, ai_wanderLocation);
	if (distanceToWonderPos < 2)
	{//create new pos
		//GLog->Log("Create new pos");
		ai_wanderLocation.X = FMath::RandRange(ai_home.X - 500, ai_home.X + 500);
		ai_wanderLocation.Y = FMath::RandRange(ai_home.Y - 500, ai_home.Y + 500);
	}
	if (mapRef->CanWanderHere(FVector2D(ai_wanderLocation.X, ai_wanderLocation.Y)) == false)
	{
		ai_wanderLocation = aiPos;	//set the wander location to the current location so it won't move to unvaild location, will get new wander location on next pass
	}
	//move to that location
	GoToLocation(ai_wanderLocation);
}

void AAIEntity::GoToLocation()
{
	GoToLocation(ai_TargetLocation);
	float dis = FVector::Distance(GetActorLocation(), ai_TargetLocation);
	//GLog->Log("Distance ... " + FString::FromInt(ai_distanceToHome));
	if (dis <= 0)
	{
		ai_MoveToLocation = false;
	}
}

void AAIEntity::GoJobLocation()
{
	GoToLocation(ai_jobLocation);
	ai_distanceToJob = FVector::Distance(GetActorLocation(), ai_jobLocation);
	//GLog->Log("Distance ... " + FString::FromInt(ai_distanceToJob));
	if (ai_distanceToJob <= 0)
	{
		//at home
		ai_distanceToJob = 0;
		ai_jobMove = false;
	}
}

void AAIEntity::FindPath()
{
	//GLog->Log("UPDATING PATH!!!!!!!");
	PathFinding* APathFinding = new PathFinding;
	float gridSize = mapRef->gridSize;
	FVector aiPos = GetActorLocation();
	FVector2D startNode = FVector2D(int(aiPos.X / gridSize) * gridSize, int(aiPos.Y / gridSize) * gridSize);
	FVector2D TargetNode = FVector2D(int(pathTargetPos.X / gridSize) * gridSize, int(pathTargetPos.Y / gridSize) * gridSize);
	//GLog->Log("Looking for path!!!");
	if (VaildMoveLocation(pathTargetPos))
	{
		path = APathFinding->FindPath(startNode, TargetNode, mapRef);
	}	
	else
	{
		path = TArray<FVector2D>();	//target pos not vaild so return blank array
	}
}

void AAIEntity::GoToLocation(FVector pos)
{
	FVector aiPos = GetActorLocation();
	pathTargetPos = FVector2D(pos.X, pos.Y);
	//only find a new path if the target pos is different
	if (pathLastTargetPos != pathTargetPos)
	{
		pathFind = true;
		pathLastTargetPos = pathTargetPos;
	}
	if (path.Num() == 0)
	{
		FindPath();		
		pathFind = true;
		pathLastTargetPos = pathTargetPos;
	}
	//find out direction to pos
	//FVector target = pos - aiPos;
	FVector2D pathNodePos;
	FVector target;
	if (path.Num() == 0) 
	{ 
		velocity = FVector::ZeroVector;
		return; 
	}
	if (path.Num() > 2)
	{
		pathNodePos = path[path.Num() - 2];
		if (aiPos.X > pathNodePos.X -1 && aiPos.X < pathNodePos.X +1 && aiPos.Y > pathNodePos.Y - 1 && aiPos.Y < pathNodePos.Y + 1)
		{//if at node pos then remove this node
			path.RemoveAt(path.Num() - 1);
		}
		if (path.IsValidIndex(path.Num() - 2))
		{
			pathNodePos = path[path.Num() - 2];
			target = FVector(pathNodePos.X, pathNodePos.Y, 50) - aiPos;
			//target
			target.Normalize();
			velocity = target * speed;
			return;
		}	
	}
	else if (path.Num() >= 0)
	{
		pathNodePos = path[0];
		if (aiPos.X > pathNodePos.X - 1 && aiPos.X < pathNodePos.X + 1 && aiPos.Y > pathNodePos.Y - 1 && aiPos.Y < pathNodePos.Y + 1)
		{
			//if at node pos then remove this node
			path.Empty();
			pathFind = false;
			ai_wanderLocation = aiPos;
			velocity = FVector::ZeroVector;
			return;
		}
		if (path.IsValidIndex(0))
		{
			pathNodePos = path[0];
			target = FVector(pathNodePos.X, pathNodePos.Y, 50) - aiPos;
			target.Normalize();
			velocity = target * speed;
			return;
		}
	}	
	velocity = FVector::ZeroVector;
	return;
}

bool AAIEntity::CheckDesire(Desire* desire1, Desire* desire2)
{
	if (desire1 == desire2)
	{
		return true;
	}
	return false;
}

bool AAIEntity::VaildMoveLocation(FVector2D pos)
{
	float gridSize = mapRef->gridSize;
	FVector aiPos = GetActorLocation();
	FVector2D TargetNode = FVector2D(int(pos.X / gridSize) * gridSize, int(pos.Y / gridSize) * gridSize);
	if (mapRef->IsNodeWalkable(TargetNode))
	{
		return false;
	}
	return true;
}

