// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnActor.h"
#include "Components/BoxComponent.h"
#include "EnemyCharacter.h"

// Sets default values
ASpawnActor::ASpawnActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	BoxComponent = CreateDefaultSubobject<UBoxComponent>("BoxComponent");
	//FVector Vector = BoxComponent->GetUnscaledBoxExtent();
	
}

// Called when the game starts or when spawned
void ASpawnActor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ASpawnActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

FVector ASpawnActor::GetRandomPosition()
{
	FVector Origin;
	FVector BoxExtent;
	GetActorBounds(false, Origin, BoxExtent);

	FVector Vector = FVector(FMath::FRandRange(BoxExtent.X - BoxExtent.X * 2, BoxExtent.X), FMath::FRandRange(BoxExtent.Y - BoxExtent.Y * 2, BoxExtent.Y), Origin.Z);
	Origin += Vector;
	TArray<FHitResult> HitResult;
	FCollisionQueryParams Params;
	FVector Start = Origin + FVector(0.f, 0.f, 400.f);
	FVector End = Origin + FVector(0.f, 0.f, -400.f);
	Params.AddIgnoredActor(this);
	bool Result = GetWorld()->LineTraceMultiByChannel(
		HitResult,
		Start,
		End,
		ECollisionChannel::ECC_GameTraceChannel3
	);
	if (Result)
	{
		for (FHitResult Hit : HitResult)
		{
			if (!Cast<AEnemyCharacter>(Hit.GetActor()))
			{
				return Hit.GetActor()->GetActorLocation();
			}
		}
		DrawDebugLine(GetWorld(), Start,End, FColor::Blue, true);
	}
	return FVector::ZeroVector;
}

