// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BlueprintActionDatabaseRegistrar.h"
#include "BlueprintNodeSpawner.h"
#include "CoreMinimal.h"
#include "EdGraph/EdGraphPin.h"
#include "K2Node.h"

#include "../UnrealScape/Public/Interactive.h"

#include "K2Node_SwitchFOption.generated.h"

/**
 * 
 */
UCLASS()
class BLUEPRINTNODES_API UK2Node_SwitchFOption : public UK2Node
{
    GENERATED_BODY()

public:
    // K2Node API
    UE_NODISCARD bool IsNodeSafeToIgnore() const override { return true; }
    void GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const override;
    UE_NODISCARD FText GetMenuCategory() const override;

    // EdGraphNode API
    void AllocateDefaultPins() override;
    UE_NODISCARD FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
};
