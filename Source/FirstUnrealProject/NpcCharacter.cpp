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

}

void ANpcCharacter::BeginPlay()
{
	Super::BeginPlay();
	if (InteractionWidget != nullptr)
	{
		InteractionWidgetComponent->SetWidget(InteractionWidget);
	}
	SetNpcInfo();
	SetQuestData();
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

void ANpcCharacter::SetQuestData()
{
	if (QuestComponent != nullptr)
	{
		for (int32 Index = 0; Index < NpcInfo.QuestList.Num(); Index++)
		{
			QuestComponent->SetQuestDataTable(NpcInfo.QuestList[Index]);
		}
	}
}


void ANpcCharacter::PlayTalk(int32 TalkNumber)
{
	FSystemMessage Message;
	Message.SystemName = NpcInfo.NpcName;
	int32 TalkIndex = 0;
	int32 TalkDescriptionIndex = 0;
	for (int32 Index = 0; Index < NpcInfo.TalkInfo.Num(); Index++)
	{
		if (TalkNumber == NpcInfo.TalkInfo[Index].TalkNumber)
		{
			TalkIndex = Index;
			TalkDescriptionIndex = NpcInfo.TalkInfo[Index].TalkIndex;
			if (NpcInfo.TalkInfo[TalkIndex].TalkIndex >= NpcInfo.TalkInfo[TalkIndex].TalkDescription.Num())
			{
				TalkDescriptionIndex = NpcInfo.TalkInfo[TalkIndex].TalkIndex = 0;
				GetWorld()->GetTimerManager().ClearTimer(TalkHandle);
				IsTalk = false;
				return;
			}
			break;
		}
	}
	Message.SystemMessage = NpcInfo.TalkInfo[TalkIndex].TalkDescription[TalkDescriptionIndex];
	if (Player != nullptr)
	{
		Player->PlayerMessageComponent->AddMessage(Message);
		NpcInfo.TalkInfo[TalkIndex].TalkIndex++;
	}
}

bool ANpcCharacter::PlayQuest()
{
	for (int32 Index = 0; Index < NpcInfo.QuestList.Num(); Index++)
	{
		QuestIndex = QuestComponent->FindQuest(NpcInfo.QuestList[Index]);
		if (QuestComponent->QuestCheck(QuestIndex, Player))
		{
			int32 TempIndex = QuestComponent->QuestInfo[QuestIndex].QuestStartTalkNumber;
			PlayTalk(TempIndex);
			GetWorld()->GetTimerManager().SetTimer(TalkHandle, [this,TempIndex]() {
				PlayTalk(TempIndex);
				}, 2.f, true);
			Player->PlayerMessageComponent->SetNpcMenuInfo(&QuestComponent->QuestInfo[QuestIndex].QuestSelectMenu);
			if(&QuestComponent->QuestInfo[QuestIndex].QuestSelectMenu != nullptr)
				LastTalkSelectMenu = QuestComponent->QuestInfo[QuestIndex].QuestSelectMenu;
			return true;
		}
	}
	return false;
}

void ANpcCharacter::OnInteract_Implementation(AActor* Caller)
{
	if (APlayerCharacter* CallPlayer = Cast<APlayerCharacter>(Caller))
	{
		if (!IsMenu)
		{
			IsMenu = true;
			Player = CallPlayer;
			Player->SetTempAction(2);
			Player->PlayerMessageComponent->SetNpcMenuInfo(&NpcInfo.TalkInfo[NpcInfo.DefaultTalkNumber].PlayerSelect);
			LastTalkSelectMenu = NpcInfo.TalkInfo[NpcInfo.DefaultTalkNumber].PlayerSelect;
			Player->SetTalkActor(this);

			/*
				GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ANpcCharacter::QuestTalk, 2.f, true);
			*/
		}
		else if(!IsTalk)
		{
			int32 PlayerSelectNumber = Player->PlayerMessageComponent->PlayerSelectNumber;
			int32 TempIndex = NpcInfo.DefaultTalkNumber;
			IsTalk = true;
			switch (LastTalkSelectMenu[PlayerSelectNumber].TalkType)
			{
			case ETalkType::E_Talk:
				PlayTalk(NpcInfo.DefaultTalkNumber);
				GetWorld()->GetTimerManager().SetTimer(TalkHandle, [this,TempIndex]() {
					PlayTalk(TempIndex);
					}, 2.f, true);
				IsMenu = false;
				Player->PlayerMessageComponent->SetNpcMenuInfo(nullptr);
				Player->SetTalkActor(nullptr);
				break;
			case ETalkType::E_Quest:
				if (!PlayQuest())
				{
					IsMenu = false;
					IsTalk = false;
					Player->PlayerMessageComponent->SetNpcMenuInfo(nullptr);
					Player->SetTalkActor(nullptr);
				}
				break;
			case ETalkType::E_Trade:
				IsMenu = false;
				IsTalk = false;
				Player->PlayerMessageComponent->SetNpcMenuInfo(nullptr);
				Player->SetTalkActor(nullptr);
				break;
			case ETalkType::E_QuestSelectYes:
				IsMenu = false;
				IsTalk = false;
				Player->PlayerMessageComponent->AddQuest(QuestComponent->GetQuestData(QuestIndex));
				Player->PlayerMessageComponent->SetNpcMenuInfo(nullptr);
				Player->SetTalkActor(nullptr);
				break;
			case ETalkType::E_QuestSelectNo:
				IsMenu = false;
				IsTalk = false;
				Player->PlayerMessageComponent->SetNpcMenuInfo(nullptr);
				Player->SetTalkActor(nullptr);
				break;
			}
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
