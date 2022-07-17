// Fill out your copyright notice in the Description page of Project Settings.


#include "RoadGravel.h"

// Sets default values
ARoadGravel::ARoadGravel(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	tile_Blank = ObjectInitializer.CreateDefaultSubobject<UInstancedStaticMeshComponent>(this, (TEXT("Tile_Blank")));
	static ConstructorHelpers::FObjectFinder<UStaticMesh>Mesh01(TEXT("StaticMesh'/Game/Models/Square.Square'"));
	UStaticMesh* assset = Mesh01.Object;
	tile_Blank->SetStaticMesh(assset);
	tile_Blank->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	tile_Blank->SetCastShadow(false);
}

// Called when the game starts or when spawned
void ARoadGravel::BeginPlay()
{
	Super::BeginPlay();
	tile_Blank->AddInstance(GetActorTransform());
}

// Called every frame
void ARoadGravel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

