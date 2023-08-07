// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PlayerAnimInstance.h"
#include "PlayerActorComponent.h"
#include "PlayerInventoryComponent.h"
#include "Kismet/GameplayStatics.h"
#include "EnemyCharacter.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "ItemActor.h"
#include "DamageComponent.h"
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
	static ConstructorHelpers::FClassFinder<UAnimInstance> Anim(TEXT("/Script/Engine.AnimBlueprint'/Game/Animation/Player/ABP_Player.ABP_Player_C'"));

	PlayerComponent = CreateDefaultSubobject<UPlayerActorComponent>(TEXT("PlayerComponent"));
	PlayerInventoryComponent = CreateDefaultSubobject<UPlayerInventoryComponent>(TEXT("PlayerInventoryComponent"));
	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	DamageComponent = CreateDefaultSubobject<UDamageComponent>(TEXT("DamageComponent"));

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
	
	if (Anim.Succeeded())
	{
		GetMesh()->SetAnimClass(Anim.Class);
	}
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	Animinstance = Cast<UPlayerAnimInstance>(GetMesh()->GetAnimInstance());
	if (Animinstance)
	{
		Animinstance->OnAttackHit.AddUObject(this, &APlayerCharacter::OnHit);
		Animinstance->OnMontageEnded.AddDynamic(this, &APlayerCharacter::OnAttackMontageEnded); 
		Animinstance->OnPlayMontageNotifyBegin.AddDynamic(this, &APlayerCharacter::OnNotifyBeginRecieved);
	}
	
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("UpDown"), this, &APlayerCharacter::KeyUpDown);
	PlayerInputComponent->BindAxis(TEXT("LeftRight"), this, &APlayerCharacter::KeyLeftRight);
	PlayerInputComponent->BindAxis(TEXT("LookLeftRight"), this, &APlayerCharacter::LookLeftRight);
	PlayerInputComponent->BindAxis(TEXT("LookUpDown"), this, &APlayerCharacter::LookUpDown);

	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &APlayerCharacter::Jump);


	PlayerInputComponent->BindAction(TEXT("PlayerRun"), EInputEvent::IE_Pressed, this, &APlayerCharacter::SetIsRunTrue);
	PlayerInputComponent->BindAction(TEXT("PlayerRun"), EInputEvent::IE_Released, this, &APlayerCharacter::SetIsRunFalse);

	PlayerInputComponent->BindAction(TEXT("LeftMouseClick"), EInputEvent::IE_Released, this, &APlayerCharacter::Attack);

	PlayerInputComponent->BindAction(TEXT("Interaction"), EInputEvent::IE_Pressed, this, &APlayerCharacter::Interaction);

}

float APlayerCharacter::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	if (IDamageInterface* DamageInterface = Cast<IDamageInterface>(DamageEvent.DamageTypeClass->GetDefaultObject<UDamageType>()))
	{
			DamageInterface->SetAttackType(DamageComponent, Damage);
	}
	return Damage;
}

void APlayerCharacter::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	//UE_LOG(LogTemp, Log, TEXT("AttackFalse"));
	IsAttacking = false;
	Animinstance->IsAttack = false;
}

void APlayerCharacter::OnNotifyBeginRecieved(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload)
{
	AttackIndex--;
	if (AttackIndex < 0)
	{
		Animinstance->Montage_Stop(0.35f, Animinstance->AttackMontage);
		AttackIndex = 0;
	}

}

void APlayerCharacter::Attack()
{
	IsAttacking = true;

	if (IsValid(Animinstance))
	{
		if (!Animinstance->Montage_IsPlaying(Animinstance->AttackMontage))
		{
			Animinstance->PlayMontage();
		}
		else
		{
			AttackIndex = 1;
		}
		//UE_LOG(LogTemp, Log, TEXT("Attack"));
	}
}

void APlayerCharacter::Interaction()
{
	//UE_LOG(LogTemp, Log, TEXT("%f, %f, %f"), GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z);
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

void APlayerCharacter::OnHit()
{
	//UE_LOG(LogTemp, Log, TEXT("OnHit"));
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
		if (PlayerComponent->CriticalChance > RandomChance / 100000.f)
		{
			TSubclassOf<UDamageType_Critical> DamageTypeClass = UDamageType_Critical::StaticClass();
			UGameplayStatics::ApplyDamage(HitActor, PlayerComponent->GetPhysicalDamage() * PlayerComponent->CriticalDamage, GetController(), this, DamageTypeClass);
		}
		else {
			TSubclassOf<UDamageType_Physical> DamageTypeClass = UDamageType_Physical::StaticClass();
			UGameplayStatics::ApplyDamage(HitActor, PlayerComponent->GetPhysicalDamage(), GetController(), this, DamageTypeClass);
		}
		DrawColor = FColor::Green;
	}
	else
	{
		DrawColor = FColor::Red;
	}
	DrawDebugSphere(GetWorld(), Forward, AttackRadius, 16, DrawColor, false, 5.0f);
}

void APlayerCharacter::KeyUpDown(float value)
{
	AddMovementInput(GetActorForwardVector(), value);
}

void APlayerCharacter::KeyLeftRight(float value)
{
	AddMovementInput(GetActorRightVector(), value);
}

void APlayerCharacter::LookLeftRight(float value)
{
	AddControllerYawInput(value);
}

void APlayerCharacter::LookUpDown(float value)
{
	AddControllerPitchInput(value);
}

void APlayerCharacter::SetIsRunTrue()
{
	Cast<UPlayerAnimInstance>(GetMesh()->GetAnimInstance())->SetIsRunTrue();
}

void APlayerCharacter::SetIsRunFalse()
{
	Cast<UPlayerAnimInstance>(GetMesh()->GetAnimInstance())->SetIsRunFalse();
}
