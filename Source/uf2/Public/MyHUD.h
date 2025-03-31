// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MyHUD.generated.h"

/**
 * 
 */
UCLASS()
class UF2_API AMyHUD : public AHUD
{
	GENERATED_BODY()

public:

	virtual void DrawHUD() override;
	void SetHUDMessage(const FString& NewMessage);
	void SetHUDLPHPS(int LP);
	void SetHUDLPHPB(int LP);
	void SetHUDLPAIS(int LP);
	void SetHUDLPAIB(int LP);

private:
	FString HUDMessage = TEXT("prova");
	FString LPHPS = TEXT("Punti vita HP S: -");
	FString LPHPB = TEXT("Punti vita HP B: -");
	FString LPAIS = TEXT("Punti vita AI S: -");
	FString LPAIB = TEXT("Punti vita AI B: -");

protected:
	virtual void BeginPlay() override;

};
