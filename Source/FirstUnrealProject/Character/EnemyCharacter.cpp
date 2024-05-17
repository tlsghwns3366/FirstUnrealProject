// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "BrainComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "MainGameState.h"
#include "PlayerCharacter.h"
#include "ActorComponent/AttackSystemComponent.h"
#include "ActorComponent/CharacterStateComponent.h"
#include "ActorComponent/PlayerMessageComponent.h"
#include "ActorComponent/AggroComponent.h"
#include "Item/ItemActor.h"
#include "Item/EnemyInventoryComponent.h"
#include "Anim/EnemyAnimInstance.h"
#include "Controller/EnemyAiController.h"

// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	EnemyInventoryComponent = CreateDefaultSubobject<UEnemyInventoryComponent>(TEXT("EnemyInventoryComponent"));
	AggroComponent = CreateDefaultSubobject<UAggroComponent>(TEXT("AggroComponent"));


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

void AEnemyCharacter::AttackAiming()
{
	AttackSystemComponent->StrengthUp(1.f);
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
		EnemyController->Destroy();
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
	EnemyInventoryComponent->DropItem();
}

void AEnemyCharacter::SetState()
{
	float AggroCount = AggroComponent->AggroCount;
	ECustomCharacterState TempState = MyCharacterState;
	if (AggroCount >= AttackAggro)
	{
		if (AggroTarget != nullptr)
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
			case EEnemyType::E_PeaceMonster:
				MyCharacterState = ECustomCharacterState::E_Peace;
				break;
			default:
				break;
			}
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
		case EEnemyType::E_PeaceMonster:
			MyCharacterState = ECustomCharacterState::E_Peace;
			break;
		default:
			break;
		}
	}
	else
	{
		MyCharacterState = ECustomCharacterState::E_Peace;
	}
	if (TempState != MyCharacterState)
	{
		EnemyInventoryComponent->SetEnemyEquipItem(MyCharacterState);
		AggroComponent->AddAggro(0.f);
	}
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

void AEnemyCharacter::SetWeaponEnum(EWeaponEnum WeaponEnum)
{
	if (Anim != nullptr)
	{
		Anim->WeaponEnum = WeaponEnum;
	}
}

void AEnemyCharacter::SetAiming(bool Value)
{
	Anim->IsZoom = Value;
}
