// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PlayerAnimInstance.h"
#include "CharacterStateComponent.h"
#include "PlayerInventoryComponent.h"
#include "Kismet/GameplayStatics.h"
#include "EnemyCharacter.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "ItemActor.h"
#include "ItemObject.h"
#include "DamageType_FIre.h"
#include "DamageType_Physical.h"
#include "DamageType_Critical.h"
#include "Engine/DamageEvents.h"


// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SkeletalMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/ThirdPerson/Characters/Mannequins/Meshes/SKM_Quinn.SKM_Quinn'"));
	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstance(TEXT("/Script/Engine.AnimBlueprint'/Game/Animation/Player/ABP_Player.ABP_Player_C'"));

	PlayerStateComponent = CreateDefaultSubobject<UCharacterStateComponent>(TEXT("PlayerStateComponent"));
	PlayerInventoryComponent = CreateDefaultSubobject<UPlayerInventoryComponent>(TEXT("PlayerInventoryComponent"));
	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));

	AIPerceptionStimuliSourceComponent = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("AIPerceptionStimuliSourceComponent"));
	if (SkeletalMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SkeletalMesh.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -88.0f), FRotator(0.f, -90.f, 0.f));
	}


	Scene->SetupAttachment(RootComponent);
	GetMesh()->SetupAttachment(Scene);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));

	SpringArm->SetupAttachment(GetCapsuleComponent());
	Camera->SetupAttachment(SpringArm);

	SpringArm->TargetArmLength = 600.f;
	SpringArm->SetRelativeRotation(FRotator(-35.f, 0.f, 0.f));
	SpringArm->SocketOffset = FVector(0.f, 50.f, 75.f);

	SpringArm->bUsePawnControlRotation = true;


	GetCharacterMovement()->MaxWalkSpeed = 500;
	
	if (AnimInstance.Succeeded())
	{
		GetMesh()->SetAnimClass(AnimInstance.Class);
	}
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	Anim = Cast<UPlayerAnimInstance>(GetMesh()->GetAnimInstance());
	if (Anim)
	{
		Anim->OnAttackHit.AddUObject(this, &APlayerCharacter::OnHitActor);
		Anim->OnMontageEnded.AddDynamic(this, &APlayerCharacter::OnAttackMontageEnded);
		Anim->OnPlayMontageNotifyBegin.AddDynamic(this, &APlayerCharacter::OnNotifyBeginRecieved);
	}
	
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
float APlayerCharacter::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	return Damage;
}

void APlayerCharacter::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	Anim->IsAttack = false;
}

void APlayerCharacter::OnNotifyBeginRecieved(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload)
{
	AttackIndex--;
	if (AttackIndex < 0)
	{
		Anim->Montage_Stop(0.35f, Anim->AttackMontage);
		AttackIndex = 0;
	}

}

void APlayerCharacter::Attack()
{
	IsAttacking = true;
	if (IsValid(Anim))
	{
		if (!Anim->Montage_IsPlaying(Anim->AttackMontage))
		{
			Anim->PlayMontage();
		}
		else
		{
			AttackIndex = 1;
		}
		//UE_LOG(LogTemp, Log, TEXT("Attack"));
	}
}

void APlayerCharacter::OnHitActor()
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
		if (PlayerStateComponent->FinalState.CriticalChance > RandomChance / 100000.f)
		{
			TSubclassOf<UDamageType_Critical> DamageTypeClass = UDamageType_Critical::StaticClass();
			UGameplayStatics::ApplyDamage(HitActor, PlayerStateComponent->GetPhysicalDamage() * PlayerStateComponent->FinalState.CriticalDamage, GetController(), this, DamageTypeClass);
		}
		else {
			TSubclassOf<UDamageType_Physical> DamageTypeClass = UDamageType_Physical::StaticClass();
			UGameplayStatics::ApplyDamage(HitActor, PlayerStateComponent->GetPhysicalDamage(), GetController(), this, DamageTypeClass);
		}
		DrawColor = FColor::Green;
	}
	else
	{
		DrawColor = FColor::Red;
	}
	DrawDebugSphere(GetWorld(), Forward, AttackRadius, 16, DrawColor, false, 5.0f);
}


void APlayerCharacter::Interaction()
{
	float Range = 50.f;
	FHitResult HitResult;
	FVector Center = GetActorLocation();
	FVector Forward = Center + GetActorForwardVector() * Range;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	bool Result = GetWorld()->SweepSingleByChannel
	(OUT HitResult,
		Center,
		Forward,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel3,
		FCollisionShape::MakeSphere(Range),
		Params
	);
	FColor DrawColor;
	if (Result)
	{
		auto RootItem = Cast<AItemActor>(HitResult.GetActor());
		if (RootItem)
		{
			RootItem->AddInventory(PlayerInventoryComponent);
		}
		DrawColor = FColor::Green;
	}
	else
	{
		DrawColor = FColor::Red;
	}
	DrawDebugSphere(GetWorld(), Forward, Range, 32, DrawColor, false, 5.0f);
}

void APlayerCharacter::SetIsRunTrue()
{
	Cast<UPlayerAnimInstance>(GetMesh()->GetAnimInstance())->SetIsRunTrue();
	GetCharacterMovement()->MaxWalkSpeed = PlayerStateComponent->FinalState.RunSpeed;
	PlayerStateComponent->CurrentSpeed = PlayerStateComponent->FinalState.RunSpeed;
}

void APlayerCharacter::SetIsRunFalse()
{
	Cast<UPlayerAnimInstance>(GetMesh()->GetAnimInstance())->SetIsRunFalse();
	GetCharacterMovement()->MaxWalkSpeed = PlayerStateComponent->FinalState.WalkSpeed;
	PlayerStateComponent->CurrentSpeed = PlayerStateComponent->FinalState.WalkSpeed;
}

void APlayerCharacter::UseItem(UItemObject* Item)
{
	if (Item)
	{
		Item->OnUse(this);
	}
}
