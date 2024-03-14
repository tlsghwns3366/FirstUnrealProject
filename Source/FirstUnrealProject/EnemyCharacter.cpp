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
#include "PlayerCharacter.h"
#include "PlayerMessageComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

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
}

float AEnemyCharacter::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	if (MainStateComponent->IsDie)
	{
		EnemyDie();
		if (APlayerCharacter* Player = Cast<APlayerCharacter>(DamageCauser))
		{
			Player->PlayerMessageComponent->EnemyKillCount(EnemyName);
		}
	}
	return Damage;
}

void AEnemyCharacter::Attack()
{
	//UE_LOG(LogTemp, Log, TEXT("EnemyAttack"));
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
	/*
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [&]() {
		TArray<FName> SocketName = GetMesh()->GetAllSocketNames();
		int32 SocketSize = SocketName.Num();
		for (int32 i = 0; i < SocketSize; i++)
		{
			GetMesh()->PutRigidBodyToSleep(SocketName[i]);
			GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel1);
		}
		}, 5.0f, false);*/
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
	}
	EnemyInventoryComponent->ItemInventory.Empty();
}

void AEnemyCharacter::SetState()
{
	float AggroCount = MainStateComponent->AggroCount;
	ECustomCharacterState TempState = MyCharacterState;
	if (AggroCount >= AttackAggro)
	{
		switch (EnemyType)
		{
		case EEnemyType::E_DefendingMonster:
			MyCharacterState = ECustomCharacterState::E_Defense;
			break;
		case EEnemyType::E_AttackingMonster:
			MyCharacterState = ECustomCharacterState::E_Attack;
			break;
		case EEnemyType::E_RunAwayMonster:
			MyCharacterState = ECustomCharacterState::E_Runaway;
			break;
		default:
			break;
		}
	}
	else if (AggroCount >= AlertAggro)
	{
		switch (EnemyType)
		{
		case EEnemyType::E_DefendingMonster:
			MyCharacterState = ECustomCharacterState::E_Alert;
			break;
		case EEnemyType::E_AttackingMonster:
			MyCharacterState = ECustomCharacterState::E_Alert;
			break;
		case EEnemyType::E_RunAwayMonster:
			MyCharacterState = ECustomCharacterState::E_Runaway;
			break;
		default:
			break;
		}
	}
	else
	{
		switch (EnemyType)
		{
		case EEnemyType::E_DefendingMonster:
			MyCharacterState = ECustomCharacterState::E_Peace;
			break;
		case EEnemyType::E_AttackingMonster:
			MyCharacterState = ECustomCharacterState::E_Peace;
			break;
		case EEnemyType::E_RunAwayMonster:
			MyCharacterState = ECustomCharacterState::E_Peace;
			break;
		default:
			break;
		}
	}
	if (TempState != MyCharacterState)
		MainStateComponent->AddAggro(0);
}

void AEnemyCharacter::SetMovementSpeed(ECharacterMovementSpeedState State)
{
	switch (State)
	{
	case ECharacterMovementSpeedState::E_Walk:
		GetCharacterMovement()->MaxWalkSpeed = MainStateComponent->FinalState.WalkSpeed;
		break;
	case ECharacterMovementSpeedState::E_Alert:
		GetCharacterMovement()->MaxWalkSpeed = MainStateComponent->FinalState.AlertSpeed;
		break;
	case ECharacterMovementSpeedState::E_Run:
		GetCharacterMovement()->MaxWalkSpeed = MainStateComponent->FinalState.RunSpeed;
		break;
	}
}
