// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameEntity.h"
#include "UObject/NoExportTypes.h"

#include "DialogAsset.generated.h"

UENUM()
enum class EDialogType : uint8
{
    Player,
    Npc,
    Options,
    End
};

USTRUCT(BlueprintType)
struct FDialogOption
{
public:
    GENERATED_BODY();

    UPROPERTY(BlueprintReadWrite)
    FString Text;

    UPROPERTY(BlueprintReadWrite)
    int GoToStage;
};

USTRUCT(BlueprintType)
struct FDialog
{
public:
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite)
    FString Text;

    UPROPERTY(BlueprintReadWrite)
    EDialogType Type;

    UPROPERTY(BlueprintReadWrite)
    TArray<FDialogOption> Options;
};

UCLASS()
class UNREALSCAPE_API UDialogAsset : public UObject
{
    GENERATED_BODY()
public:
    UPROPERTY(BlueprintReadOnly, meta = (ExposeOnSpawn = true))
    APlayerController* Player = nullptr;

    UPROPERTY(BlueprintReadOnly, meta = (ExposeOnSpawn = true))
    AGameEntity* Npc = nullptr;

    UPROPERTY(BlueprintReadOnly)
    TMap<int, FDialog> Conversation;

    UDialogAsset(const FObjectInitializer& ObjectInitializer);

    UFUNCTION(BlueprintCallable, Category = "Dialog")
    void AddPlayerDialog(const FString& Text, int Stage);

    UFUNCTION(BlueprintCallable, Category = "Dialog")
    void AddNpcDialog(const FString& Text, int Stage);

    UFUNCTION(BlueprintCallable, Category = "Dialog")
    void AddTwoOptions(const FString& Text, int Stage, const FDialogOption& Option1, const FDialogOption& Option2);

    UFUNCTION(BlueprintCallable, Category = "Dialog")
    void AddThreeOptions(const FString& Text, int Stage, const FDialogOption& Option1, const FDialogOption& Option2,
                         const FDialogOption& Option3);

    UFUNCTION(BlueprintCallable, Category = "Dialog")
    void AddFourOptions(const FString& Text, int Stage, const FDialogOption& Option1, const FDialogOption& Option2,
                        const FDialogOption& Option3, const FDialogOption& Option4);

    UFUNCTION(BlueprintCallable, Category = "Dialog")
    void AddEnd(int Stage);
};
