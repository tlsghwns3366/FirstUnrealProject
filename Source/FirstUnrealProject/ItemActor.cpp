// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemActor.h"
#include "ItemObject.h"
#include "PlayerInventoryComponent.h"
// Sets default values
AItemActor::AItemActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	static ConstructorHelpers::FObjectFinder<UMaterialInstance> FoundMaterial(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/SMT/HighLight/M_HighLightM_Inst.M_HighLightM_Inst'"));
	if (FoundMaterial.Succeeded())
	{
		StaticMesh->SetOverlayMaterial(FoundMaterial.Object);
	}
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

void AItemActor::AddInventory(UPlayerInventoryComponent* Inventory)
{
	if(Inventory->AddItem(Item))
		Destroy();
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
