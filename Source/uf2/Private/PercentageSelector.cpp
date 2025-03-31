// Fill out your copyright notice in the Description page of Project Settings.


#include "PercentageSelector.h"
#include "Components/Slider.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "GridManager.h"
#include "MyGameModeBase.h"

void UPercentageSelector::NativeConstruct()
{
    Super::NativeConstruct();

    // Associa l'evento di cambiamento del valore dello slider
    if (ObstacleSlider)
    {
        ObstacleSlider->OnValueChanged.AddDynamic(this, &UPercentageSelector::OnSliderValueChanged);
    }

    // Associa l'evento al pulsante di conferma
    if (ConfirmButton)
    {
        ConfirmButton->OnClicked.AddDynamic(this, &UPercentageSelector::OnConfirmButtonClicked);
    }

    // Imposta il testo iniziale con il valore corrente dello slider
    if (ObstacleSlider && SliderValueText)
    {
        OnSliderValueChanged(ObstacleSlider->GetValue());
    }
}

void UPercentageSelector::OnSliderValueChanged(float Value)
{
    // Il valore dello slider è tra 0 e 1; moltiplichiamo per 100 per ottenere la percentuale
    if (SliderValueText)
    {
        SliderValueText->SetText(FText::FromString(FString::Printf(TEXT("%.0f%%"), Value * 100.0f)));
    }
}

void UPercentageSelector::OnConfirmButtonClicked()
{
    // Ottieni il valore della percentuale dallo slider
    float Percentage = 0.0f;
    if (ObstacleSlider)
    {
        // Supponendo che il valore dello slider sia normalizzato tra 0 e 1
        Percentage = ObstacleSlider->GetValue() * 100.0f;
    }

    // Trova il GridManager nella scena
    AGridManager* GridManager = Cast<AGridManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AGridManager::StaticClass()));
    if (GridManager)
    {   
        if (Percentage != 0) {
            GridManager->ObstacolPerc = Percentage / 100.f;
        }
        else
        {
            GridManager->ObstacolPerc = 0.0f;
        }
        GridManager->SetUp();
    }

    Cast<AMyGameModeBase>(GetWorld()->GetAuthGameMode())->StartPlacing();

    // Rimuovi il widget dalla viewport
    RemoveFromParent();
}
