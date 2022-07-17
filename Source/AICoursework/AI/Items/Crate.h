// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Crate.generated.h"

UCLASS()
class AICOURSEWORK_API ACrate : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACrate(const FObjectInitializer& ObjectInitializer);

	class Item* crate_Item;
	FVector crate_Location;
	void SetCrate(class Item* item, FVector pos);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	void SetMesh(int id);
		UStaticMeshComponent* MeshComp;
		UMaterialInterface* mat;
		UStaticMeshComponent* MeshComp_01;
		UMaterialInterface* mat_01;

};
