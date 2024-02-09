// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "NpcCharacter.h"
#include "QuestComponent.generated.h"

UENUM(BlueprintType)
enum class EQuestType : uint8
{
	E_Location UMETA(DisplayName = "Location"),
	E_EnemyKill UMETA(DisplayName = "EnemyKill"),
	E_Interact UMETA(DisplayName = "Interact"),
	E_ItemCollect UMETA(DisplayName = "ItemCollect"),
};

USTRUCT()
struct FQuestObjective : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FText ObjectiveName;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FText ObjectiveDescription;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		EQuestType QuestType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float ObjectiveKill;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float ObjectiveItemCollect;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		bool ObjectiveInteract;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FVector ObjectiveLocationVector;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float CurrentKill;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float CurrentItemCollect;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		bool IsCompleted;
};

USTRUCT()
struct FQuestData : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FString QuestString;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FText QuestName;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FText QuestDescription;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TArray<struct FQuestObjective> QuestObjective;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int32 QuestStartTalkNumber;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int32 QuestEndTalkNumber;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TArray<struct FPlayerSelect> QuestSelectMenu;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float XPReward;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TArray<FString> ItemRewards;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TArray<FString> ClearQuest;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float MinLv;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		bool IsLoop;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FIRSTUNREALPROJECT_API UQuestComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UQuestComponent();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TArray<struct FQuestData> QuestInfo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UDataTable* QuestTable;

protected:
	// Called when the game startsQ
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SetQuestDataTable(FString string);
	int32 FindQuest(FString string);
	bool QuestCheck(int32 QuestIndex, AActor* Caller);
	void ObjectiveCheck(int32 QuestIndex);
	FString GetQuestTalk(int32 QuestIndex, int32* TalkIndex);
	FQuestData GetQuestData(int32 QuestIndex);
		
};
