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

void ANpcCharacter::SetNpcSelectData()
{
	LastTalkSelectMenu.Empty();
	int32 TalkNumber = NpcInfo.DefaultTalkNumber;
	for (int32 Index = 0; Index < NpcInfo.TalkInfo[TalkNumber].PlayerSelect.Num(); Index++)
	{
		if (NpcInfo.TalkInfo[TalkNumber].PlayerSelect[Index].IsVisible)
			LastTalkSelectMenu.Add(NpcInfo.TalkInfo[TalkNumber].PlayerSelect[Index]);
	}
	FQuestData* QuestStartData = GetQuestStartData();
	FQuestData* QuestEndData = GetQuestEndData();
	if (QuestStartData != nullptr)
	{
		FPlayerSelect Select;
		Select.NpcMenu = NpcInfo.TalkInfo[QuestStartData->QuestStartTalkNumber].TalkName.ToString();
		Select.TalkType = ETalkType::E_QuestStart;
		LastTalkSelectMenu.Add(Select);
	}
	if (QuestEndData != nullptr)
	{
		FPlayerSelect Select;
		Select.NpcMenu = NpcInfo.TalkInfo[QuestEndData->QuestStartTalkNumber].TalkName.ToString();
		Select.TalkType = ETalkType::E_QuestEnd;
		LastTalkSelectMenu.Add(Select);
	}


}

void ANpcCharacter::PlayTalk(int32 TalkNumber)
{
	if (Player != nullptr)
	{
		FSystemMessage Message;
		int32 TalkIndex = 0;
		int32 TalkDescriptionIndex = 0;
		for (int32 Index = 0; Index < NpcInfo.TalkInfo.Num(); Index++)
		{
			if (TalkNumber == NpcInfo.TalkInfo[Index].TalkNumber)
			{
				TalkIndex = Index;
				TalkDescriptionIndex = NpcInfo.TalkInfo[Index].TalkIndex;
				break;
			}
		}
		Message.SystemName = NpcInfo.NpcName;
		Message.SystemMessage = NpcInfo.TalkInfo[TalkIndex].TalkDescription[TalkDescriptionIndex];
		Player->PlayerMessageComponent->AddMessage(Message);
		NpcInfo.TalkInfo[TalkIndex].TalkIndex++;
		if (NpcInfo.TalkInfo[TalkIndex].TalkIndex >= NpcInfo.TalkInfo[TalkIndex].TalkDescription.Num())
		{
			IsTalk = false;
			TalkDescriptionIndex = NpcInfo.TalkInfo[TalkIndex].TalkIndex = 0;
			GetWorld()->GetTimerManager().ClearTimer(TalkHandle);
			Player->PlayerMessageComponent->SetIsTalk(IsTalk);
			return;
		}
	}
}

void ANpcCharacter::PlayQuestTalk(int32 TalkNumber)
{
	if (Player != nullptr)
	{
		FSystemMessage Message;
		int32 TalkIndex = 0;
		int32 TalkDescriptionIndex = 0;
		for (int32 Index = 0; Index < NpcInfo.TalkInfo.Num(); Index++)
		{
			if (TalkNumber == NpcInfo.TalkInfo[Index].TalkNumber)
			{
				TalkIndex = Index;
				TalkDescriptionIndex = NpcInfo.TalkInfo[Index].TalkIndex;
				break;
			}
		}
		Message.SystemName = NpcInfo.NpcName;
		Message.SystemMessage = NpcInfo.TalkInfo[TalkIndex].TalkDescription[TalkDescriptionIndex];
		Player->PlayerMessageComponent->AddMessage(Message);
		NpcInfo.TalkInfo[TalkIndex].TalkIndex++;
		if (NpcInfo.TalkInfo[TalkIndex].TalkIndex >= NpcInfo.TalkInfo[TalkIndex].TalkDescription.Num())
		{
			IsTalk = false;
			TalkDescriptionIndex = NpcInfo.TalkInfo[TalkIndex].TalkIndex = 0;
			GetWorld()->GetTimerManager().ClearTimer(TalkHandle);
			Player->PlayerMessageComponent->SetIsTalk(IsTalk);

			Player->PlayerMessageComponent->SetNpcMenuInfo(&NpcInfo.TalkInfo[TalkNumber].PlayerSelect);
			if (&NpcInfo.TalkInfo[TalkNumber].PlayerSelect != nullptr)
				LastTalkSelectMenu = NpcInfo.TalkInfo[TalkNumber].PlayerSelect;
			return;
		}
	}
}

FQuestData* ANpcCharacter::GetQuestStartData()
{
	for (int32 Index = 0; Index < NpcInfo.QuestList.Num(); Index++)
	{
		FQuestData* QuestData = QuestComponent->GetQuestData(NpcInfo.QuestList[Index]);
		if (QuestComponent->QuestCheck(QuestData, Player))
		{
			return QuestData;
		}
	}
	return nullptr;
}

FQuestData* ANpcCharacter::GetQuestEndData()
{
	for (int32 Index = 0; Index < NpcInfo.QuestList.Num(); Index++)
	{
		FQuestData* QuestData = nullptr;
		FQuestData* CurrentQuestData = nullptr;
		QuestData = QuestComponent->GetQuestData(NpcInfo.QuestList[Index]);
		if(QuestData != nullptr)
			CurrentQuestData = Player->PlayerMessageComponent->GetCurrentQuest(QuestData->QuestString);
		if (CurrentQuestData != nullptr)
		{
			if (QuestComponent->ObjectiveCheck(CurrentQuestData))
			{
				return CurrentQuestData;
			}
		}
	}
	return nullptr;
}

bool ANpcCharacter::PlayQuest()
{
	IsTalk = true;
	Player->PlayerMessageComponent->SetIsTalk(IsTalk);
	FQuestData* QuestData = GetQuestStartData();
	Player->PlayerMessageComponent->SetNpcMenuInfo(nullptr);
	if(QuestData != nullptr)
	{
		int32 TempIndex = QuestData->QuestStartTalkNumber;
		if (NpcInfo.TalkInfo[TempIndex].TalkDescription.Num() == 1)
		{
			PlayQuestTalk(TempIndex);
		}
		else
		{
			PlayQuestTalk(TempIndex);
			GetWorld()->GetTimerManager().SetTimer(TalkHandle, [this, TempIndex]() {
				PlayQuestTalk(TempIndex);
				}, 2.f, true);
		}
		return true;
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
			SetNpcSelectData();
			Player->PlayerMessageComponent->SetNpcMenuInfo(&LastTalkSelectMenu);
			Player->SetTalkActor(this);
		}
		else if(!IsTalk)
		{
			int32 PlayerSelectNumber = Player->PlayerMessageComponent->PlayerSelectNumber;
			int32 TempIndex = NpcInfo.DefaultTalkNumber;
			switch (LastTalkSelectMenu[PlayerSelectNumber].TalkType)
			{
			case ETalkType::E_Talk:
				IsTalk = true;
				Player->PlayerMessageComponent->SetIsTalk(IsTalk);
				if(NpcInfo.TalkInfo[NpcInfo.DefaultTalkNumber].TalkDescription.Num() == 1)
				{
					PlayTalk(NpcInfo.DefaultTalkNumber);
				}
				else
				{
					PlayTalk(NpcInfo.DefaultTalkNumber);
					GetWorld()->GetTimerManager().SetTimer(TalkHandle, [this, TempIndex]() {
						PlayTalk(TempIndex);
						}, 2.f, true);
				}
				IsMenu = false;
				Player->PlayerMessageComponent->SetNpcMenuInfo(nullptr);
				Player->SetTalkActor(nullptr);
				break;
			case ETalkType::E_QuestStart:
				if (!PlayQuest())
				{
					IsMenu = false;
					Player->SetTalkActor(nullptr);
					Player->PlayerMessageComponent->SetNpcMenuInfo(nullptr);
				}
				break;
			case ETalkType::E_QuestEnd:
				Player->PlayerMessageComponent->RemoveQuest(*GetQuestEndData());
				IsMenu = false;
				Player->SetTalkActor(nullptr);
				Player->PlayerMessageComponent->SetNpcMenuInfo(nullptr);
				break;
			case ETalkType::E_Trade:
				IsMenu = false;
				Player->PlayerMessageComponent->SetNpcMenuInfo(nullptr);
				Player->SetTalkActor(nullptr);
				break;
			case ETalkType::E_QuestSelectYes:
				IsMenu = false;
				Player->PlayerMessageComponent->AddQuest(*GetQuestStartData());
				Player->PlayerMessageComponent->SetNpcMenuInfo(nullptr);
				Player->SetTalkActor(nullptr);
				break;
			case ETalkType::E_QuestSelectNo:
				IsMenu = false;
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
