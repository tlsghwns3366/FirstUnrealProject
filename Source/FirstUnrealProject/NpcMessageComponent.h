// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/DataTable.h"
#include "ItemObject.h"
#include "NpcMessageComponent.generated.h"

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
		FText ObjectiveLocation;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FText ObjectiveInteract;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		bool IsCompleted;
};

USTRUCT()
struct FQuestData : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FText QuestName;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FText QuestDescription;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TArray<FString>  QuestNpcDescription;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TArray<struct FQuestObjective> QuestObjective;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float LvMax;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float XPReward;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TArray<FString> ItemRewards;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		bool IsLoop;
};
USTRUCT()
struct FNpcInfo : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FName NpcName;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TArray<FString> NpcDescription;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FString> QuestList;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FIRSTUNREALPROJECT_API UNpcMessageComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UNpcMessageComponent();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FNpcInfo NpcInfo;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TArray<struct FQuestData> QuestInfo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UDataTable* TalkTable;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UDataTable* QuestTable;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	void SetDataTable();
	struct FNpcMessage GetNpcMessage(int32 Index);
	struct FNpcMessage GetQuestMessage(int32 QuestIndex, int32 DescriptionIndex);
	int32 GetMaxDescription();
	int32 FindQuest(class APlayerCharacter* Player);
};
