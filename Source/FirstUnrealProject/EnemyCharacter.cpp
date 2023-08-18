// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"
#include "EnemyAiController.h"
#include "EnemyInventoryComponent.h"
#include "EnemyStateActorComponent.h"	
#include "EnemyAnimInstance.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "BrainComponent.h"
#include "Components/CapsuleComponent.h"
#include "ItemActor.h"
#include "Components/WidgetComponent.h"
#include "DamageComponent.h"
#include "DamageType_FIre.h"
#include "DamageType_Physical.h"
#include "DamageType_Critical.h"
#include "Engine/DamageEvents.h"

// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	EnemyStateComponent = CreateDefaultSubobject<UEnemyStateActorComponent>(TEXT("EnemyStateActorCompenent"));
	EnemyInventoryComponent = CreateDefaultSubobject<UEnemyInventoryComponent>(TEXT("EnemyInventoryComponent"));
	static ConstructorHelpers::FObjectFinder< USkeletalMesh> SkeletalMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/ThirdPerson/Characters/Mannequins/Meshes/SKM_Quinn.SKM_Quinn'"));
	if (SkeletalMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SkeletalMesh.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -88.0f), FRotator(0.f, -90.f, 0.f));
	}
	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));

	WidgetComponent->SetupAttachment(GetMesh());
	WidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	AIControllerClass = AEnemyAIController::StaticClass();

}

// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	Anim = Cast<UEnemyAnimInstance>(GetMesh()->GetAnimInstance());
	if (Anim)
	{
		Anim->OnAttackHit.AddUObject(this, &AEnemyCharacter::OnHitActor);
		Anim->OnMontageEnded.AddDynamic(this, &AEnemyCharacter::OnAttackMontageEnded);
	}
}

// Called every frame
void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float AEnemyCharacter::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	if (EnemyStateComponent->IsDie)
	{
		Anim->IsDie = true;
		GetMesh()->SetSimulatePhysics(true); 
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, [&]() {
			Destroy();
			}, DestroyTime, false);
		AAIController* EnemyController = Cast<AAIController>(GetController());
		if (EnemyController)
		{
			EnemyController->GetBrainComponent()->StopLogic(FString("Die"));
		}
		UCapsuleComponent* EnemyCapsule = GetCapsuleComponent();
		if (EnemyCapsule)
		{
			EnemyCapsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
		DropItem();
	}
	return Damage;
}
void AEnemyCharacter::OnHitActor()
{
	FHitResult HitResult;
	FCollisionQueryParams Parems(NAME_None, false, this);

	float AttackRange = 100.f;
	float AttackRadius = 30.f;

	FVector Center = GetActorLocation();
	FVector Forward = Center + GetActorForwardVector() * AttackRange;

	bool Result = GetWorld()->SweepSingleByChannel
	(OUT HitResult,
		Center,
		Forward,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel1,
		FCollisionShape::MakeSphere(AttackRadius),
		Parems);

	float HalfHeight = AttackRange * 0.5f + AttackRadius;
	FQuat Rotation = FRotationMatrix::MakeFromZ(Forward).ToQuat();
	FColor DrawColor;
	if (Result && HitResult.GetActor())
	{
		AActor* HitActor = HitResult.GetActor();

		float RandomChance = FMath::RandRange(0.f, 100000.f);
		if (EnemyStateComponent->FinalState.CriticalChance > RandomChance / 100000.f)
		{
			TSubclassOf<UDamageType_Critical> DamageTypeClass = UDamageType_Critical::StaticClass();
			UGameplayStatics::ApplyDamage(HitActor, EnemyStateComponent->GetPhysicalDamage() * EnemyStateComponent->FinalState.CriticalDamage, GetController(), this, DamageTypeClass);
		}
		else {
			TSubclassOf<UDamageType_Physical> DamageTypeClass = UDamageType_Physical::StaticClass();
			UGameplayStatics::ApplyDamage(HitActor, EnemyStateComponent->GetPhysicalDamage(), GetController(), this, DamageTypeClass);
		}
		DrawColor = FColor::Green;
	}
	else
	{
		DrawColor = FColor::Red;
	}
	if (EnemyStateComponent->RestTime < 200.f)
		EnemyStateComponent->RestTime += 10.f;
	DrawDebugSphere(GetWorld(), Forward,AttackRadius,16,DrawColor,false,5.0f);
}

void AEnemyCharacter::DropItem()
{
	FVector ActorLocation = GetActorLocation();
	for (auto Enemyitem : EnemyInventoryComponent->ItemInventory)
	{
		ActorLocation += FVector(0.f, 0.f, 100.f);
		AItemActor* EItem = GetWorld()->SpawnActor<AItemActor>(EItem->StaticClass(),ActorLocation,FRotator::ZeroRotator);
		EItem->Iteminitialization(Enemyitem);
	}
}
