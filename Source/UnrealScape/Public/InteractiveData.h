#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "UObject/Interface.h"

#include "InteractiveData.generated.h"

#define DEFAULT_INTERACT_DISTANCE 87.5f

USTRUCT(BlueprintType, Blueprintable)
struct FInteractOption : public FTableRowBase
{
public:
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Interaction")
    FString Name;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Interaction")
    bool bUseInteractionDistance = true;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Interaction")
    bool bVisible = true;
};

USTRUCT(BlueprintType)
struct FInteractRequest
{
public:
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Interaction")
    TObjectPtr<AActor> Player;

    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Interaction")
    FInteractOption Option;
};

UENUM(BlueprintType)
enum class EEntityType : uint8
{
    Object UMETA(DisplayName = "Object"),
    Npc UMETA(DisplayName = "Npc"),
    Enemy UMETA(DisplayName = "Enemy"),
    Item UMETA(DisplayName = "Item"),
};

USTRUCT(BlueprintType, Blueprintable)
struct FEntityData : public FTableRowBase
{
public:
    GENERATED_BODY()

    // Properties
    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    float InteractDistance = DEFAULT_INTERACT_DISTANCE;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    FString Name = "Object";

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    EEntityType Type = EEntityType::Object;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    TArray<FInteractOption> Options;
};