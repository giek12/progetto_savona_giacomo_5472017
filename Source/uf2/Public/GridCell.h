#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GridCell.generated.h"

//		forward per il pointer al character
class ABaseCharacter;
//		forward per il pointer al character

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCellClicked, AGridCell*, ClickedCell);
//delegeto per il click

UCLASS()
class UF2_API AGridCell : public AActor
{
	GENERATED_BODY()

public:
	AGridCell();

	//		click update
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnCellClicked OnCellClicked;
	//		click update

	void SetMaterial(UMaterialInterface* Material);
	FVector GetPosition() const;

	UPROPERTY(EditAnywhere)
	int Row = -1;

	UPROPERTY(EditAnywhere)
	int Col = -1;

	UPROPERTY(EditAnywhere)
	bool avaiable = true;

	UPROPERTY(EditAnywhere)
	bool HasACharacter = false;

	ABaseCharacter* character;

	bool fs = false;	//è vero se questa cella ha i footprint

	float GCost = 0;		//costo per l'A*
	float HCost = 0;		//costo per l'A*
	AGridCell* Parent = nullptr;	//per l'A*

	//		click update
	UFUNCTION()
	void OnCellSelected();
	//		click update

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnBeginCursorOver(UPrimitiveComponent* TouchedComponent);

	UFUNCTION()
	void OnEndCursorOver(UPrimitiveComponent* TouchedComponent);

	UPROPERTY(EditAnywhere, Category = "Materials")
	UMaterialInterface* MaterialBase;

	UPROPERTY(EditAnywhere, Category = "Materials")
	UMaterialInterface* FootSelMat;

	UPROPERTY(EditAnywhere, Category = "Materials")
	UMaterialInterface* FootBaseMat;

	UPROPERTY(EditAnywhere)
	bool over = false;	//se se mouse_over


private:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* CellMesh;

	UPROPERTY(EditAnywhere, Category = "Materials")
	UMaterialInterface* MaterialSel;
};