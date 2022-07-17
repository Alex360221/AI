// Fill out your copyright notice in the Description page of Project Settings.


#include "Crate.h"
#include "Item.h"


// Sets default values
ACrate::ACrate(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	MeshComp = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, (TEXT("MeshComp")));
	mat = ObjectInitializer.CreateDefaultSubobject<UMaterialInterface>(this, (TEXT("Mat")));
	MeshComp_01 = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, (TEXT("MeshComp_01")));
	mat_01 = ObjectInitializer.CreateDefaultSubobject<UMaterialInterface>(this, (TEXT("Mat_01")));
	static ConstructorHelpers::FObjectFinder<UMaterialInterface>Mat(TEXT("Material'/Game/Models/Items/Material.Material'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh>Mesh02(TEXT("StaticMesh'/Game/Models/Items/Crate.Crate'"));
	UStaticMesh* assset = Mesh02.Object;
	MeshComp_01->SetStaticMesh(assset);
	MeshComp_01->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComp_01->SetCastShadow(false);
	MeshComp_01->SetMaterial(0, mat);
	SetRootComponent(MeshComp_01);

}

void ACrate::SetCrate(Item* item, FVector pos)
{
	crate_Item = item;
	crate_Location = pos;
	SetMesh(item->item_ID);
}

// Called when the game starts or when spawned
void ACrate::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACrate::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACrate::SetMesh(int id)
{

}

