// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"
#include "EnemyAiController.h"
#include "EnemyInventoryComponent.h"
#include "EnemyAnimInstance.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "BrainComponent.h"
#include "Components/CapsuleComponent.h"
#include "ItemActor.h"
#include "Components/WidgetComponent.h"
#include "Engine/DamageEvents.h"
#include "AttackSystemComponent.h"
#include "MainGameState.h"
#include "CharacterStateComponent.h"

// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
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
		Anim->OnMontageEnded.AddDynamic(this, &AEnemyCharacter::OnAttackMontageEnded);
	}
	MainState = Cast<AMainGameState>(GetWorld()->GetGameState());
}

// Called every frame
void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	float GameSpeed = MainState->GameSpeed;
	if (RestTime < 200.f && Anim->GetRest())
		RestTime += FMath::Lerp(0, 200.f, 0.001f * GameSpeed);
	if (Anim->GetRest())
	{
		if (RestTime > 0)
			RestTime -= FMath::Lerp(0, 200.f, 0.01f * GameSpeed);
	}
}

float AEnemyCharacter::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	if (MainStateComponent->IsDie)
	{
		EnemyDie();
	}
	return Damage;
}

void AEnemyCharacter::Attack()
{
	IsAttacking = true;
	AttackSystemComponent->Attack();
}


void AEnemyCharacter::EnemyDie()
{
	TArray<AActor*> AttachedActors;
	this->GetAttachedActors(AttachedActors);
	int32 AttachedSize = AttachedActors.Num();
	for (int32 i = 0; i < AttachedSize; i++)
	{
		FDetachmentTransformRules DetachRules(EDetachmentRule::KeepWorld, false);
		AttachedActors[i]->DetachFromActor(DetachRules);
		AttachedActors[i]->Destroy();
	}
	Anim->IsDie = true;
	GetMesh()->SetSimulatePhysics(true);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [&]() {
		TArray<FName> SocketName = GetMesh()->GetAllSocketNames();
		int32 SocketSize = SocketName.Num();
		for (int32 i = 0; i < SocketSize; i++)
		{
			GetMesh()->PutRigidBodyToSleep(SocketName[i]);
			GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel1);
		}
		}, 5.0f, false);

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

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [&]() {
		Destroy();
		}, DestroyTime, false);
}

void AEnemyCharacter::DropItem()
{
	FVector ActorLocation = GetActorLocation();
	for (auto Enemyitem : EnemyInventoryComponent->ItemInventory)
	{
		ActorLocation += FVector(0.f, 0.f, 100.f);
		AItemActor* EItem = GetWorld()->SpawnActor<AItemActor>(EItem->StaticClass(), ActorLocation, FRotator::ZeroRotator);
		EItem->Iteminitialization(Enemyitem);
		EnemyInventoryComponent->RemoveItem(Enemyitem);
	}
}
