// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/NoExportTypes.h"
#include "USUtils.h"

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
    FText Text;

    UPROPERTY(BlueprintReadWrite)
    int GoToStep;
};

UCLASS(DefaultToInstanced, EditInlineNew, Blueprintable, BlueprintType, Meta = (ShowWorldContextPin))
class UNREALSCAPE_API UDialogAsset : public UObject
{
    GENERATED_BODY()
public:
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    void Communicate(int Step);

    UFUNCTION(BlueprintCallable, BlueprintPure)
    virtual UWorld* GetWorld() const override;

    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAddOneOption, FText, Text, const FDialogOption&, Option);
    UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Event Dispatchers")
    FAddOneOption AddOneOption;

    DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FAddTwoOptions, FText, Text, const FDialogOption&, Option1,
                                                   const FDialogOption&, Option2);
    UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Event Dispatchers")
    FAddTwoOptions AddTwoOptions;

    DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FAddThreeOptions, FText, Text, const FDialogOption&, Option1,
                                                  const FDialogOption&, Option2, const FDialogOption&, Option3);
    UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Event Dispatchers")
    FAddThreeOptions AddThreeOptions;

    DECLARE_DYNAMIC_MULTICAST_DELEGATE_FiveParams(FAddFourOptions, FText, Text, const FDialogOption&, Option1,
                                                  const FDialogOption&, Option2, const FDialogOption&, Option3,
                                                  const FDialogOption&, Option4);
    UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Event Dispatchers")
    FAddFourOptions AddFourOptions;

    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAddNpc, FText, Text, int, GoToStep);
    UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Event Dispatchers")
    FAddNpc AddNpc;

    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAddPlayer, FText, Text, int, GoToStep);
    UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Event Dispatchers")
    FAddPlayer AddPlayer;

    DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEndDialog);
    UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Event Dispatchers")
    FEndDialog EndDialog;
};
