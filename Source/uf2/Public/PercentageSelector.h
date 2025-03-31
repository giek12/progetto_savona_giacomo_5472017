// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PercentageSelector.generated.h"

class USlider;
class UButton;
class UTextBlock;
UCLASS()
class UF2_API UPercentageSelector : public UUserWidget
{
	GENERATED_BODY()

public:
    virtual void NativeConstruct() override;

protected:
    UPROPERTY(meta = (BindWidget))
    USlider* ObstacleSlider;

    UPROPERTY(meta = (BindWidget))
    UButton* ConfirmButton;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* SliderValueText;

    UFUNCTION()
    void OnSliderValueChanged(float Value);

    UFUNCTION()
    void OnConfirmButtonClicked();
	
};
