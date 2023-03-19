// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Commandlets/Commandlet.h"
#include "TextureToAssetCommandlet.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogTextureToAsset, Log, All);

/**
 * 
 */
UCLASS()
class UNREALSCAPE_API UTextureToAssetCommandlet : public UCommandlet
{
	GENERATED_BODY()

	bool ParseParams(const FString& InParams);
    virtual int32 Main(const FString& Params) override;
};
