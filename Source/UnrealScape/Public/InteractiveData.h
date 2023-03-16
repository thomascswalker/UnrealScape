#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "UObject/Interface.h"

#include "InteractiveData.generated.h"

#define INTERACT_SAME_TILE 12.5f;
#define INTERACT_ONE_TILE 50.f + INTERACT_SAME_TILE;
#define INTERACT_TWO_TILES 100.f + INTERACT_SAME_TILE;

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

    FInteractOption(FString InName = "", bool UseInteractionDistance = true)
    {
        Name = InName;
        bUseInteractionDistance = UseInteractionDistance;
        bVisible = true;
    }
    static FInteractOption Pickup() { return FInteractOption("Pickup"); }
    static FInteractOption Examine() { return FInteractOption("Examine", false); }
    static FInteractOption Use() { return FInteractOption("Use", false); }
    static FInteractOption Drop() { return FInteractOption("Drop", false); }
    static FInteractOption Equip() { return FInteractOption("Equip", false); }
    static FInteractOption Wield() { return FInteractOption("Wield", false); }
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
    float InteractDistance = INTERACT_ONE_TILE;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    FString Name = "Object";

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    EEntityType Type = EEntityType::Object;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    TArray<FInteractOption> Options;
};