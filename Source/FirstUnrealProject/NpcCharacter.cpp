// Fill out your copyright notice in the Description page of Project Settings.


#include "NpcCharacter.h"
#include "Components/WidgetComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "InteractionUserWidget.h"
#include "NpcMessageComponent.h"
#include "PlayerCharacter.h"
#include "PlayerMessageComponent.h"
#include "QuestComponent.h"

ANpcCharacter::ANpcCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	QuestComponent = CreateDefaultSubobject<UQuestComponent>(TEXT("QuestComponent"));
	static ConstructorHelpers::FObjectFinder< USkeletalMesh> SkeletalMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/ThirdPerson/Characters/Mannequins/Meshes/SKM_Quinn.SKM_Quinn'"));
	if (SkeletalMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SkeletalMesh.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -88.0f), FRotator(0.f, -90.f, 0.f));
	}
	static ConstructorHelpers::FClassFinder<UInteractionUserWidget>BlueprintWidget(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/WBP_InteractionE.WBP_InteractionE_C'"));
	if (BlueprintWidget.Succeeded())
	{
		Widget = BlueprintWidget.Class;
	}
	InteractionWidget = CreateWidget<UInteractionUserWidget>(GetWorld(), Widget);

	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));
	WidgetComponent->SetupAttachment(GetMesh());
	WidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);

	NpcWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("NpcWidgetComponent"));
	NpcWidgetComponent->SetupAttachment(GetMesh());
	NpcWidgetComponent->SetWidgetSpace(EWidgetSpace::World);
	NpcWidgetComponent->SetRelativeLocation(FVector(0.f, 0.f, 200.f));

	InteractionWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("InteractionWidgetComponent"));
	InteractionWidgetComponent->SetupAttachment(GetMesh());
	InteractionWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	InteractionWidgetComponent->SetVisibility(false);

	QuestNum = -1;
}

void ANpcCharacter::BeginPlay()
{
	Super::BeginPlay();
	if (InteractionWidget != nullptr)
	{
		InteractionWidgetComponent->SetWidget(InteractionWidget);
	}
	SetNpcInfo();
	SetQuest();
}

void ANpcCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FVector WidgetLocation = NpcWidgetComponent->GetComponentLocation();
	FVector TargetLocation = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->GetCameraLocation();
	NpcWidgetComponent->SetWorldRotation(UKismetMathLibrary::FindLookAtRotation(WidgetLocation, TargetLocation));
}

float ANpcCharacter::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	return Damage;
}

void ANpcCharacter::NpcTalk()
{/*
	FSystemMessage Message = NpcMessageComponent->GetNpcMessage(Index);
	Player->PlayerMessageComponent->AddMessage(Message);
	Index++;
	if (NpcMessageComponent->GetMaxDescription() <= Index)
	{
		IsTalk = false;
		Index = 0;
		Player = nullptr;
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	}*/
}

void ANpcCharacter::QuestTalk()
{/*
	FSystemMessage Message = NpcMessageComponent->GetQuestMessage(QuestNum,Index);
	Player->PlayerMessageComponent->AddMessage(Message);
	Index++;
	if (NpcMessageComponent->GetMaxDescription() <= Index)
	{
		Player->PlayerMessageComponent->ShowQuestWidget();
		IsTalk = false;
		Index = 0;
		Player = nullptr;
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	}*/
}

void ANpcCharacter::SetNpcInfo()
{
	if (IsValid(NpcTable))
	{
		FNpcInfo* TempInfo = NpcTable->FindRow<FNpcInfo>(NpcId, FString(""));
		if(TempInfo != nullptr)
			NpcInfo = *TempInfo;
	}
}

void ANpcCharacter::SetQuest()
{
	if (QuestComponent != nullptr)
	{
		for (int32 Index = 0; Index < NpcInfo.QuestList.Num(); Index++)
		{
			QuestComponent->SetQuestDataTable(NpcInfo.QuestList[Index]);
		}
	}
}

bool ANpcCharacter::CheckQuest(int32 QuestIndex, FString QuestString)
{
	if (Player != nullptr)
	{
		return QuestComponent->QuestCheck(QuestIndex, QuestString, Player);
	}
	return false;
}

void ANpcCharacter::PlayTalk()
{
	int32 Range = NpcInfo.IdleDescription.Num();
	if (Range <= 0)
		return;
	FSystemMessage Message;
	Message.SystemName = NpcInfo.NpcName;
	Message.SystemMessage = NpcInfo.IdleDescription[FMath::RandRange(0, Range)];
	if (Player != nullptr)
	{
		Player->PlayerMessageComponent->AddMessage(Message);
	}
}

void ANpcCharacter::PlayQuestAndTalk(FString QuestString)
{
	if (QuestNum == -1)
		QuestNum = 0;
	int32 QuestIndex = QuestComponent->FindQuest(QuestString);
	FSystemMessage Message;
	Message.SystemName = NpcInfo.NpcName;
	Message.SystemMessage = QuestComponent->GetQuestTalk(QuestIndex, &QuestNum);
	if (Player != nullptr)
	{
		Player->PlayerMessageComponent->AddMessage(Message);
	}
}

void ANpcCharacter::OnInteract_Implementation(AActor* Caller)
{
	if (APlayerCharacter* CallPlayer = Cast<APlayerCharacter>(Caller))
	{
		if (!IsTalk)
		{
			//IsTalk = true;
			Player = CallPlayer;
			PlayTalk();
			/*
			Index = 0;
			NpcTalk();
			QuestNum = NpcMessageComponent->FindQuest(CallPlayer);
			if (QuestNum == -1.f)
			{
				GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ANpcCharacter::NpcTalk, 2.f, true);
			}
			else
			{
				Index = 0;
				GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ANpcCharacter::QuestTalk, 2.f, true);
			}
			*/
		}
	}
}

void ANpcCharacter::StartFocus_Implementation()
{
	InteractionWidgetComponent->SetVisibility(true);
}

void ANpcCharacter::EndFocus_Implementation()
{
	InteractionWidgetComponent->SetVisibility(false);
}
