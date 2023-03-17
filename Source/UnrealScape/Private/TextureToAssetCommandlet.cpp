// Fill out your copyright notice in the Description page of Project Settings.


#include "TextureToAssetCommandlet.h"

DEFINE_LOG_CATEGORY(LogTextureToAsset);

bool UTextureToAssetCommandlet::ParseParams(const FString& InParams)
{
    TArray<FString> Tokens;
    TArray<FString> Params;
    TMap<FString, FString> ParamVals;

    ParseCommandLine(*InParams, Tokens, Params, ParamVals);

    return true;
}

int32 UTextureToAssetCommandlet::Main(const FString& Params)
{
    UE_LOG(LogTextureToAsset, Display, TEXT("Running Texture to Asset commandlet"));
    return 0;
}