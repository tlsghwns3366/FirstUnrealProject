// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Weapon_Arrow.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"

#include "ProjectileActor_Arrow.h"

AWeapon_Arrow::AWeapon_Arrow()
{
	ProjectilePath = CreateDefaultSubobject<USplineComponent>(TEXT("ProjectilePath"));
	IsEquip = false;
}
void AWeapon_Arrow::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
		SetArrowPosition();
}

void AWeapon_Arrow::SetOwnerWeapon(AWeapon* Weapon)
{
	OwnerWeapon = Weapon;
}

void AWeapon_Arrow::SetArrowPosition()
{
	if (OwnerWeapon)
	{
		FVector BowLocation = OwnerWeapon->SceneComponent->GetComponentLocation();
		BowLocation.Z += 2.f;
		FVector ArrowLocation = SceneComponent->GetComponentLocation();
		BowLocation -= ArrowLocation;
		ArrowRotator = UKismetMathLibrary::MakeRotFromX(BowLocation.GetSafeNormal());
		StaticMesh->SetWorldRotation(ArrowRotator);
	}
}

void AWeapon_Arrow::ShowProjectilePath(float Value)
{
	DeleteSpline();
	float NormalizedValue = FMath::Clamp(Value / 100.f, 0.f, 1.f);
	float ArrowSpeed = FMath::Lerp(MinSpeed, MaxSpeed, NormalizedValue);
	FPredictProjectilePathParams PathParams;
	PathParams.StartLocation = OwnerWeapon->SceneComponent->GetComponentLocation();
	PathParams.StartLocation.Z += 2.f;
	PathParams.LaunchVelocity = ArrowRotator.Vector() * ArrowSpeed;
	PathParams.MaxSimTime = 3.f;
	PathParams.SimFrequency = 30.f;
	PathParams.DrawDebugType = EDrawDebugTrace::None;
	PathParams.DrawDebugTime = 3.f;
	

	FPredictProjectilePathResult PathResult;
	UGameplayStatics::PredictProjectilePath(GetWorld(), PathParams, PathResult);

	int32 Index = 0;
	for (auto PathPosition : PathResult.PathData)
	{
		ProjectilePath->AddSplinePointAtIndex(PathPosition.Location, Index, ESplineCoordinateSpace::Local, true);
		Index++;
	}
	for (int32 i = 0; i < ProjectilePath->GetNumberOfSplinePoints(); i++)
	{
		USplineMeshComponent* SplineMeshComponent = NewObject<USplineMeshComponent>(this, USplineMeshComponent::StaticClass());
		SplineMeshComponent->SetStaticMesh(SplineMesh);

		SplineMeshComponent->SetMobility(EComponentMobility::Movable);
		SplineMeshComponent->CreationMethod = EComponentCreationMethod::UserConstructionScript;
		SplineMeshComponent->RegisterComponentWithWorld(GetWorld());

		SplineMeshComponent->AttachToComponent(ProjectilePath, FAttachmentTransformRules::KeepRelativeTransform);
		SplineMeshComponent->SetStartScale(FVector2D(0.01f, 0.01f));
		SplineMeshComponent->SetEndScale(FVector2D(0.01f, 0.01f));
		SplineMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		FVector StartPosition = ProjectilePath->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::Local);
		FVector StartTangent = ProjectilePath->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::Local);
		FVector EndPosition = ProjectilePath->GetLocationAtSplinePoint(i + 1, ESplineCoordinateSpace::Local);
		FVector EndTangent = ProjectilePath->GetLocationAtSplinePoint(i + 1, ESplineCoordinateSpace::Local);
		SplineMeshComponent->SetStartAndEnd(StartPosition, StartTangent, EndPosition, EndTangent);

		SplineMeshComponent->SetMaterial(0, SplineMaterial);
		ProjectilePathMesh.Add(SplineMeshComponent);
	}
}

void AWeapon_Arrow::DeleteSpline()
{
	if (ProjectilePath)
	{
		ProjectilePath->ClearSplinePoints();
		for (auto Mesh : ProjectilePathMesh)
		{
			Mesh->DestroyComponent();
		}
		ProjectilePathMesh.Empty();
	}
}
