// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyCharacter.h"
#include "InteractionInterface.h"
#include "NpcCharacter.generated.h"

UENUM(BlueprintType)
enum class ETalkType : uint8
{
	E_Talk UMETA(DisplayName = "Talk"),
	E_QuestStart UMETA(DisplayName = "QuestStart"),
	E_QuestEnd UMETA(DisplayName = "QuestEnd"),
	E_Trade UMETA(DisplayName = "Trade"),
	E_QuestSelectYes UMETA(DisplayName = "QuestSelectYes"),
	E_QuestSelectNo UMETA(DisplayName = "QuestSelectNo"),
};

USTRUCT()
struct FPlayerSelect : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FString NpcMenu;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		ETalkType TalkType;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		bool IsVisible = true;
};

USTRUCT()
struct FTalkInfo : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int32 TalkNumber;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FName TalkName;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TArray<FString> TalkDescription;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int32 TalkIndex;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<struct FPlayerSelect> PlayerSelect;
};


USTRUCT()
struct FNpcInfo : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FName NpcId;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FName NpcName;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TArray<struct FTalkInfo> TalkInfo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 DefaultTalkNumber;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FString> QuestList;
};

/**
 * 
 */
UCLASS()
class FIRSTUNREALPROJECT_API ANpcCharacter : public AEnemyCharacter, public IInteractionInterface
{
	GENERATED_BODY()
public:
	ANpcCharacter();
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		class UWidgetComponent* NpcWidgetComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UWidgetComponent* InteractionWidgetComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class UUserWidget> Widget;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UInteractionUserWidget* InteractionWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UQuestComponent* QuestComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class APlayerCharacter* Player;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName NpcId;
	UPROPERTY(EditAnywhere)
		FNpcInfo NpcInfo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<struct FPlayerSelect> LastTalkSelectMenu;


	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		bool IsTalk;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		bool IsMenu;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UDataTable* NpcTable;

	FTimerHandle TalkHandle;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual float TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	void SetNpcInfo();

	void SetQuestData();
	void SetNpcSelectData();

	void PlayTalk(int32 TalkNumber);
	void PlayQuestTalk(int32 TalkNumber);
	struct FQuestData* GetQuestStartData();
	struct FQuestData* GetQuestEndData();
	bool PlayQuest();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
		void OnInteract(AActor* Caller);
	virtual void OnInteract_Implementation(AActor* Caller);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
		void StartFocus();
	virtual void StartFocus_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
		void EndFocus();
	virtual void EndFocus_Implementation();
};
