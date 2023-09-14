// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "Components/ArrowComponent.h"
#include "Components/StaticMeshComponent.h"
#include "WeaponEquipItemObject.h"


// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	RootComponent = SceneComponent;
	SetRootComponent(RootComponent);
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(RootComponent);
	StaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	StartPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("StartPoint"));
	StartPoint->SetupAttachment(StaticMesh);
	EndPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("EndPoint"));
	EndPoint->SetupAttachment(StaticMesh);

}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeapon::WeaponInitialize_Implementation(UWeaponEquipItemObject* Item)
{
	EquipItem = Cast<UWeaponEquipItemObject>(Item);
	if (EquipItem != nullptr)
	{
		StaticMesh->SetStaticMesh(EquipItem->StaticMesh);
		StaticMesh->SetRelativeTransform(EquipItem->StaticMeshTransform);
		StartPoint->SetRelativeTransform(EquipItem->StartPoint);
		EndPoint->SetRelativeTransform(EquipItem->EndPoint);
		CharacterAttackMontage = EquipItem->CharacterAttackMontage;
	}
}