// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "MyUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class UF2_API UMyUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeConstruct() override;

	virtual bool Initialize() override;
	UFUNCTION(BlueprintCallable, Category = "Chat")
	void AddMessage(const FText& Message);

	UFUNCTION()
	void OnButtonClicked(); // Funzione chiamata quando il pulsante viene premuto

protected:

	UPROPERTY(meta = (BindWidget))
	UScrollBox* ScrollBoxWidget;

	UPROPERTY(meta = (BindWidget))
	UButton* MyButton; // Pulsante UI
	
private:
	UPROPERTY()
	TArray<UTextBlock*> MessageWidgets; // Salva i messaggi aggiunti

	const int32 MaxMessages = 15; // Numero massimo di messaggi visibili

};
