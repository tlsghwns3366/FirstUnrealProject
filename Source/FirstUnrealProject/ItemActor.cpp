// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemActor.h"
#include "ItemObject.h"

// Sets default values
AItemActor::AItemActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
}

// Called when the game starts or when spawned
void AItemActor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AItemActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AItemActor::Iteminitialization(UItemObject* _Item)
{
	if (_Item != nullptr)
	{
		Item = _Item;
		StaticMesh->SetStaticMesh(Item->StaticMesh);
		StaticMesh->SetSimulatePhysics(true);
	}
}
