// Fill out your copyright notice in the Description page of Project Settings.

#include "DialogAsset.h"

UDialogAsset::UDialogAsset(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}

void UDialogAsset::AddPlayerDialog(const FString& Text, int Stage)
{
    FDialog Dialog;
    Dialog.Text = Text;
    Dialog.Type = EDialogType::Player;
    Conversation.Add(Stage, Dialog);
}

void UDialogAsset::AddNpcDialog(const FString& Text, int Stage)
{
    FDialog Dialog;
    Dialog.Text = Text;
    Dialog.Type = EDialogType::Npc;
    Conversation.Add(Stage, Dialog);
}

void UDialogAsset::AddTwoOptions(const FString& Text, int Stage, const FDialogOption& Option1,
                                 const FDialogOption& Option2)
{
    FDialog Dialog;
    Dialog.Text = Text;
    Dialog.Type = EDialogType::Options;
    Dialog.Options.Add(Option1);
    Dialog.Options.Add(Option2);
    Conversation.Add(Stage, Dialog);
}

void UDialogAsset::AddThreeOptions(const FString& Text, int Stage, const FDialogOption& Option1,
                                   const FDialogOption& Option2, const FDialogOption& Option3)
{
    FDialog Dialog;
    Dialog.Text = Text;
    Dialog.Type = EDialogType::Options;
    Dialog.Options.Add(Option1);
    Dialog.Options.Add(Option2);
    Dialog.Options.Add(Option3);
    Conversation.Add(Stage, Dialog);
}

void UDialogAsset::AddFourOptions(const FString& Text, int Stage, const FDialogOption& Option1,
                                  const FDialogOption& Option2, const FDialogOption& Option3,
                                  const FDialogOption& Option4)
{
    FDialog Dialog;
    Dialog.Text = Text;
    Dialog.Type = EDialogType::Options;
    Dialog.Options.Add(Option1);
    Dialog.Options.Add(Option2);
    Dialog.Options.Add(Option3);
    Dialog.Options.Add(Option4);
    Conversation.Add(Stage, Dialog);
}

void UDialogAsset::AddEnd(int Stage)
{
    FDialog Dialog;
    Dialog.Type = EDialogType::End;
    Conversation.Add(Stage, Dialog);
}