#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BaseCharacter.h"
#include "MyHUD.h"
#include "Sniperf.h"
#include "Brawlerf.h"
#include "MyGameModeBase.generated.h"

UCLASS()
class UF2_API AMyGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:

	void StartPlacing();

	bool ClickFlag;
	void ResetOthers(ABaseCharacter* caller);
	ABaseCharacter* ActiveCharacter = nullptr;
	TArray<ABaseCharacter*> InGameCharacter;
	bool IsPlayerTurn;
	void PassTurn();
	void ShouldPass();
	void DidSomeOneWin();
	AMyGameModeBase();

protected:
	virtual void BeginPlay() override;

	//		click update
	void HandleLeftClick();
	//		click update

private:

	bool NoBodyWin = true;
	AGridManager* GridM;

	bool PlaceTroup();
	ABaseCharacter* PlaceSniper(bool BTP, AGridCell* SelCell);
	ABaseCharacter* PlaceBrawler(bool BTP, AGridCell* SelCell);
	TSubclassOf<class ASniperf> SniperClass;
	TSubclassOf<class ABrawlerf> BrawlerClass;
	//fasi
	bool SetUpPhase = true;
	bool PlacePhase = false;
	bool PlayPhase = false;
	
	AGridCell* FindOptimalCell(ABaseCharacter* chara);

	void AiTurn();
	AMyHUD* MyHUD;

};
