// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileActor_Arrow.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SkinnedMeshComponent.h"

#include "Item/Weapon.h"
#include "Character/CustomCharacter.h"


AProjectileActor_Arrow::AProjectileActor_Arrow()
{
}

void AProjectileActor_Arrow::BeginPlay()
{
	Super::BeginPlay();
	HitBox->OnComponentBeginOverlap.AddDynamic(this, &AProjectileActor_Arrow::HitBoxOverlap);
	ActorDestroy();
}
void AProjectileActor_Arrow::HitBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != this && OwnerActor != OtherActor && OwnerActor)
	{
		if (USkinnedMeshComponent* HitBone = Cast<USkinnedMeshComponent>(OtherComp))
		{
			if (ACustomCharacter* Character = Cast<ACustomCharacter>(OtherActor))
				this->AttachToComponent(Character->GetMesh(), FAttachmentTransformRules::KeepWorldTransform, HitBone->GetBoneName(OtherBodyIndex));
		}
		else
		{
			this->AttachToActor(OtherActor, FAttachmentTransformRules(EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, true));
		}
		HitBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		StopArrow();

		if (SweepResult.GetActor() && Cast<ACustomCharacter>(SweepResult.GetActor()))
		{
			if (!IsDamage)
			{
				IsDamage = true;
				OnArrowHit.Execute(SweepResult.GetActor(), LastValue);
			}
		}
	}
}

void AProjectileActor_Arrow::SetArrow(FRotator ArrowRotator, float Value)
{
	LastValue = Value;
	float NormalizedValue = FMath::Clamp(Value / 100.f, 0.f, 1.f);
	float ArrowSpeed = FMath::Lerp(MinSpeed, MaxSpeed, NormalizedValue);
	ProjectileComponent->Velocity = ArrowRotator.Vector() * ArrowSpeed;
	ProjectileComponent->bRotationFollowsVelocity = true;
	ProjectileComponent->ProjectileGravityScale = Gravity;

}

void AProjectileActor_Arrow::SetHitBoxIgnore(AActor* IgnoreActor)
{
	HitBox->IgnoreActorWhenMoving(OwnerActor, true);
}

void AProjectileActor_Arrow::StopArrow()
{
	ProjectileComponent->StopMovementImmediately();
	ProjectileComponent->ProjectileGravityScale = 0;
}

void AProjectileActor_Arrow::Arrowinitialization(float SpeedValue, float DestroyTimeValue, float GravityValue)
{
	Gravity = GravityValue;
	ProjectileComponent->InitialSpeed = SpeedValue;
	ProjectileComponent->MaxSpeed = SpeedValue;
	ProjectileComponent->ProjectileGravityScale = Gravity;

}
