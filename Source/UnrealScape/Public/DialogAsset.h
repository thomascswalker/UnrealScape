// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StaticEntity.h"
#include "UObject/NoExportTypes.h"

#include "DialogAsset.generated.h"

UENUM()
enum class EDialogType : uint8
{
    Player,
    Npc,
    None,
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
    int GoToStep;
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

UCLASS(DefaultToInstanced, EditInlineNew, Blueprintable, BlueprintType)
class UNREALSCAPE_API UDialogAsset : public UObject
{
    GENERATED_BODY()
public:
    UPROPERTY(BlueprintReadOnly, meta = (ExposeOnSpawn = true))
    APlayerController* Player = nullptr;

    UPROPERTY(BlueprintReadOnly, meta = (ExposeOnSpawn = true))
    TScriptInterface<IInteractive> Npc = nullptr;

    UPROPERTY(BlueprintReadOnly)
    TMap<int, FDialog> Conversation;

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    void Construct();

    UFUNCTION(BlueprintCallable, Category = "Dialog")
    UDialogAsset* AddOneOption(const FString& Text, const EDialogType Type, int ThisStep, const FDialogOption& Option);

    UFUNCTION(BlueprintCallable, Category = "Dialog")
    UDialogAsset* AddTwoOptions(const FString& Text, int ThisStep, const FDialogOption& Option1,
                                const FDialogOption& Option2);

    UFUNCTION(BlueprintCallable, Category = "Dialog")
    UDialogAsset* AddThreeOptions(const FString& Text, int ThisStep, const FDialogOption& Option1,
                                  const FDialogOption& Option2, const FDialogOption& Option3);

    UFUNCTION(BlueprintCallable, Category = "Dialog")
    UDialogAsset* AddFourOptions(const FString& Text, int ThisStep, const FDialogOption& Option1,
                                 const FDialogOption& Option2, const FDialogOption& Option3,
                                 const FDialogOption& Option4);

    UFUNCTION(BlueprintCallable, Category = "Dialog")
    UDialogAsset* AddEnd(int ThisStep);

    UFUNCTION(BlueprintCallable, Category = "Dialog")
    UDialogAsset* AddPlayer(const FString& Text, int ThisStep, int GoToStep);

    UFUNCTION(BlueprintCallable, Category = "Dialog")
    UDialogAsset* AddNpc(const FString& Text, int ThisStep, int GoToStep);
};
