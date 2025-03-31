// Fill out your copyright notice in the Description page of Project Settings.


#include "MyUserWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "MyGameModeBase.h"


void UMyUserWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // Associa la funzione al pulsante
    if (MyButton)
    {
        MyButton->OnClicked.AddDynamic(this, &UMyUserWidget::OnButtonClicked);
    }
}

bool UMyUserWidget::Initialize()
{
    bool Success = Super::Initialize();
    if (!Success) return false;

    return true;
}

void UMyUserWidget::AddMessage(const FText& Message)
{
    if (!ScrollBoxWidget) return;

    // Crea un nuovo TextBlock per il messaggio
    UTextBlock* NewTextBlock = NewObject<UTextBlock>(this);
    if (NewTextBlock)
    {
        NewTextBlock->SetText(Message);
        ScrollBoxWidget->AddChild(NewTextBlock);
        MessageWidgets.Add(NewTextBlock);
    }

    // Se abbiamo superato il numero massimo di messaggi, rimuoviamo il più vecchio
    if (MessageWidgets.Num() > MaxMessages)
    {
        UTextBlock* OldestMessage = MessageWidgets[0]; // Primo messaggio
        ScrollBoxWidget->RemoveChild(OldestMessage); // Rimuove dalla ScrollBox
        MessageWidgets.RemoveAt(0); // Rimuove dalla lista
    }

    // Auto-scroll alla fine per vedere sempre il messaggio più recente
    ScrollBoxWidget->ScrollToEnd();
}

void UMyUserWidget::OnButtonClicked()
{
    AMyGameModeBase* GameMode = Cast<AMyGameModeBase>(GetWorld()->GetAuthGameMode());
    if (GameMode->IsPlayerTurn) {
        GameMode->PassTurn();
    }else AddMessage(FText::FromString("non e' il tuo turno!"));
}