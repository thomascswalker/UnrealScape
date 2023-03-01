// Fill out your copyright notice in the Description page of Project Settings.

#include "DialogAsset.h"

void UDialogAsset::Communicate_Implementation(int Step)
{
    INFO(L"Default construction. Implement this!");
}

bool UDialogAsset::PlayerHasCondition()
{
    APlayerController* Controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (!Controller)
    {
        return false;
    }
    else
    {
        return true;
    }
}