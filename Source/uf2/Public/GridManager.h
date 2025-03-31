#pragma once

#include "GridCell.h"                                   //mod
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyUserWidget.h"
#include "GridManager.generated.h"


UCLASS()
class UF2_API AGridManager : public AActor
{
    GENERATED_BODY()

public:

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UUserWidget> ScrollWidgetClass;

    void SetUp();
    AGridManager();
    //
    AGridCell* RandomAvaiable();
    TArray<AGridCell*> FindPath(AGridCell* Startcell, AGridCell* Endcell);

protected:
    virtual void BeginPlay() override;

public:

    void PrintScrollBoxMessage(const FText& Message);

    virtual void Tick(float DeltaTime) override;
    TArray<AGridCell*> GetNeighborCells(AGridCell* Cell);

    UPROPERTY(EditAnywhere)
    float ObstacolPerc;

private:

    UMyUserWidget* ScrollWidgetInstance;

    AGridCell* map[25][25];                             //mod
    TArray<AGridCell*> ObstacleCell;
    //
    TArray<AGridCell*> AvCell;
    //

    UPROPERTY(EditAnywhere)
    TSubclassOf<class AGridCell> GridCellClass;

    UPROPERTY(EditAnywhere)
    int32 GridSize = 25;
    UPROPERTY(EditAnywhere)
    float CellSpacing = 100.0f;

    void CreateGrid();
    void SetMat();
    void GenObstacles();
    void SetMatToObs();

    bool IsConnected();
    TArray<AGridCell*>  SelectRandomCells();

    UPROPERTY(EditAnywhere, Category = "Materials")
    UMaterialInterface* Material1;                      //prato

    UPROPERTY(EditAnywhere, Category = "Materials")
    UMaterialInterface* Material2;                      //albero

    UPROPERTY(EditAnywhere, Category = "Materials")
    UMaterialInterface* Material3;                      //albero1

    UPROPERTY(EditAnywhere, Category = "Materials")
    UMaterialInterface* Material4;                      //montagna
};