// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CustomCharacter.h"
#include "InteractionInterface.h"
#include "NpcCharacter.generated.h"

UENUM(BlueprintType)
enum class ETalkType : uint8
{
	E_Talk UMETA(DisplayName = "Talk"),
	E_Quest UMETA(DisplayName = "Quest"),
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
class FIRSTUNREALPROJECT_API ANpcCharacter : public ACustomCharacter, public IInteractionInterface
{
	GENERATED_BODY()
public:
	ANpcCharacter();
public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		class UWidgetComponent* WidgetComponent;
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
	UPROPERTY(EditAnywhere)
		int32 QuestIndex;
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

	void NpcTalk();
	void QuestTalk();
	void SetNpcInfo();

	void SetQuestData();

	void PlayTalk(int32 TalkNumber);
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
