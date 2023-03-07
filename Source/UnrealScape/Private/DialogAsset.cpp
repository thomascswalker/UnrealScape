// Fill out your copyright notice in the Description page of Project Settings.

#include "DialogAsset.h"

void UDialogAsset::Communicate_Implementation(int Step)
{
    INFO(L"Default construction. Implement this!");
}

UWorld* UDialogAsset::GetWorld() const
{
    return GetOuter()->GetWorld();
}
