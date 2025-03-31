#pragma once

#include "CoreMinimal.h"
#include "GridCell.h"
#include "GridManager.h"
#include "GameFramework/Actor.h"
#include "BaseCharacter.generated.h"

UCLASS()
class UF2_API ABaseCharacter : public AActor
{
	GENERATED_BODY()
	
public:	
	ABaseCharacter();

	virtual void NotifyActorOnClicked(FKey ButtonPressed) override; //click update

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int MaxMove;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool MeleeA;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int AttRange;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int MinDamage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int MaxDamage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Hp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool BelongToPlayer;
	UPROPERTY(EditAnywhere, Category = "Materials")
	UMaterialInterface* Footm;
	UPROPERTY(EditAnywhere, Category = "Materials")
	UMaterialInterface* Targetm;

	void DeClick();
	bool Done = false;

	FString Nome;

	AGridManager* GridM;

	void Damage(int Dmg);
	int DamageOutput();
	void GetEnemiesInRange();
	TArray<ABaseCharacter*> HittableEnemies;
	bool AttackToken = true;
	void GetReachableCells();
	TArray<AGridCell*> ReachableCells;
	bool MoveToken = true;
	void MoveWait(AGridCell* cell, TFunction<void()> OnMoveCompleted);

private:
	TFunction<void()> MoveCompletedCallback;		//wait update

	TArray<AGridCell*> Steps;
	int ClickCounter = 0;
	void CanAttack();
	// per il lerp
	FVector CurrentPos;
	// per il lerp

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void MoveTo(AGridCell* cell);
	bool Moving();
	FVector GetCellWorldPosition(class AGridCell* TargetCell);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
	AGridCell* CurrentCell;
	UPROPERTY(EditAnywhere, Category = "Materials")
	UMaterialInterface* MaterialBase;
	UPROPERTY(EditAnywhere, Category = "Materials")
	UMaterialInterface* MaterialTarget;
	virtual void SetTarget();
	virtual void ResetTarget();
	bool IsATarget = false;

	virtual void SetName();
};
