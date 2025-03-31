#include "GridManager.h"
#include "GridCell.h"
#include "Engine/World.h"
#include "MyUserWidget.h"
#include "Math/UnrealMathUtility.h"
// fixing ue_log

AGridManager::AGridManager()
{
    PrimaryActorTick.bCanEverTick = true;
    static ConstructorHelpers::FObjectFinder<UMaterialInterface> Mat1(TEXT("/Game/materiali/pratom"));
    if (Mat1.Succeeded())
    {
        Material1 = Mat1.Object;
    }
    //materiale base del prato

    static ConstructorHelpers::FObjectFinder<UMaterialInterface> Mat2(TEXT("/Game/materiali/alberom"));
    if (Mat2.Succeeded())
    {
        Material2 = Mat2.Object;
    }
    //materiale albero

    static ConstructorHelpers::FObjectFinder<UMaterialInterface> Mat3(TEXT("/Game/materiali/pinom"));
    if (Mat2.Succeeded())
    {
        Material3 = Mat3.Object;
    }
    //materiale albero1

    static ConstructorHelpers::FObjectFinder<UMaterialInterface> Mat4(TEXT("/Game/materiali/montagnam"));
    if (Mat2.Succeeded())
    {
        Material4 = Mat4.Object;
    }
    //materiale montagna
}

void AGridManager::BeginPlay()
{
    Super::BeginPlay();

    if (ScrollWidgetClass)
    {
        // Crea il widget come UMyScrollWidget
        ScrollWidgetInstance = CreateWidget<UMyUserWidget>(GetWorld(), ScrollWidgetClass);
        if (ScrollWidgetInstance)
        {
            ScrollWidgetInstance->SetPositionInViewport(FVector2D(0.0f, 0.0f), true);
            ScrollWidgetInstance->AddToViewport();
        }
    }
}

void AGridManager::PrintScrollBoxMessage(const FText& Message) {
    ScrollWidgetInstance->AddMessage(Message);
    //funzione di interfaccia per aggiungere un messaggio alla scrollbox
}

void AGridManager::SetUp() {
    CreateGrid();
    //creazione della griglia
    SetMat();
    //set dei materiali nelle celle
    AvCell = SelectRandomCells();
    //presa delle celle libere
    SetMatToObs();
    //set dei materiali degli ostacoli
}

void AGridManager::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AGridManager::CreateGrid()
{
    if (!GridCellClass) return;
    const char StartLetter = 'A';
    //variabile di partenza per la nomenclatura delle celle

    for (int32 Row = 0; Row < GridSize; ++Row)
    {
        for (int32 Col = 0; Col < GridSize; ++Col)
        {
            FVector Location(Row * CellSpacing, Col * CellSpacing, 0.0f);
            FRotator Rotation(0.0f, 0.0f, 0.0f);
            AGridCell* NewCell = GetWorld()->SpawnActor<AGridCell>(GridCellClass, Location, Rotation);
            //blocco di codice per lo spawn delle celle



            if (NewCell)
            {
                FString CellName = FString::Printf(TEXT("%c%d"), StartLetter + Row, 25-Col);
                NewCell->Rename(*CellName);
                //rinominazione delle celle

                map[Row][Col] = NewCell;
                NewCell->Row = Row;
                NewCell->Col = Col;
                //mappature delle celle
            }
        }
    }
}

void AGridManager::SetMat() {
    for (int32 Row = 0; Row < GridSize; ++Row) {
        for (int32 Col = 0; Col < GridSize; ++Col) {
            map[Row][Col]->SetMaterial(Material1);
            map[Row][Col]->MaterialBase = Material1;
        }
    }
    //set dei materiali (a tutti viene applicato il prato di base)
}

void AGridManager::GenObstacles() {
    int32 NumObstacles = FMath::RoundToInt(GridSize * GridSize * ObstacolPerc);
    //calcolo del numero di ostacoli

    TArray<AGridCell*> AvailableCells;
    for (int32 Row = 0; Row < GridSize; ++Row)
    {
        for (int32 Col = 0; Col < GridSize; ++Col)
        {
            if (map[Row][Col])  // Verifica che la cella esista
            {
                AvailableCells.Add(map[Row][Col]);
            }
        }
    }

    // Mischiamo l'array
    int32 NumCells = AvailableCells.Num();
    for (int32 i = NumCells - 1; i > 0; --i)
    {
        int32 j = FMath::RandRange(0, i);
        AvailableCells.Swap(i, j);
    }

    for (int i = 0; i < NumObstacles; ++i) {
        ObstacleCell.Add(AvailableCells[i]);
    }

}

void AGridManager::SetMatToObs() {
    int32 ObstacolSelector;

    for (AGridCell* Obs : ObstacleCell) {
        ObstacolSelector = FMath::RandRange(1, 3);
        switch (ObstacolSelector) {
        case 1: Obs->SetMaterial(Material2);
            Obs->MaterialBase = Material2;
            break;
        case 2: Obs->SetMaterial(Material3);
            Obs->MaterialBase = Material3;
            break;
        default: Obs->SetMaterial(Material4);
            Obs->MaterialBase = Material2;
        }
    }
}

bool AGridManager::IsConnected() {

    if (ObstacleCell.Num() == GridSize * GridSize) return true;     //debugger di un "problema"

    int32 MaxIterations = GridSize * GridSize;
    int32 Iterations = 0;
    TSet<AGridCell*> Visited;
    TArray<AGridCell*> Queue;

    // Troviamo una cella libera per iniziare il controllo
    AGridCell* StartCell = nullptr;
    for (int32 Row = 0; Row < GridSize; ++Row)
    {
        for (int32 Col = 0; Col < GridSize; ++Col)
        {
            if (map[Row][Col] && !ObstacleCell.Contains(map[Row][Col]))
            {
                StartCell = map[Row][Col];
                break;
            }
        }
        if (StartCell) break;
    }

    if (!StartCell) return false; // Non ci sono celle libere

    // Avviamo il BFS
    Queue.Add(StartCell);
    Visited.Add(StartCell);

    while (Queue.Num() > 0)
    {
        Iterations++;
        if (Iterations > MaxIterations) return false;
        AGridCell* Current = Queue[0];
        Queue.RemoveAt(0);

        // Otteniamo le celle adiacenti
        TArray<AGridCell*> Neighbors = GetNeighborCells(Current);
        for (AGridCell* Neighbor : Neighbors)
        {
            if (Neighbor && !Visited.Contains(Neighbor) && !ObstacleCell.Contains(Neighbor))
            {
                Visited.Add(Neighbor);
                Queue.Add(Neighbor);
            }
        }
    }

    // La griglia è connessa se tutte le celle libere sono state visitate
    int32 FreeCells = (GridSize * GridSize) - ObstacleCell.Num();
    return Visited.Num() == FreeCells;
}

TArray<AGridCell*> AGridManager::GetNeighborCells(AGridCell* Cell)
{
    TArray<AGridCell*> Neighbors;

    int32 Row, Col;
    Row = Cell->Row;
    Col = Cell->Col;

    {
        if (Row > 0) Neighbors.Add(map[Row - 1][Col]); // Sopra
        if (Row < GridSize - 1) Neighbors.Add(map[Row + 1][Col]); // Sotto
        if (Col > 0) Neighbors.Add(map[Row][Col - 1]); // Sinistra
        if (Col < GridSize - 1) Neighbors.Add(map[Row][Col + 1]); // Destra
    }

    return Neighbors;
}

TArray<AGridCell*> AGridManager::SelectRandomCells() {
    int32 NumObstacles = FMath::RoundToInt(GridSize * GridSize * ObstacolPerc);

    // Lista delle celle disponibili
    TArray<AGridCell*> AvailableCells;
    for (int32 Row = 0; Row < GridSize; ++Row)
    {
        for (int32 Col = 0; Col < GridSize; ++Col)
        {
            if (map[Row][Col])
            {
                AvailableCells.Add(map[Row][Col]);
            }
        }
    }

    // Mischiamo l'array per ottenere celle casuali
    int32 NumCells = AvailableCells.Num();
    for (int32 i = NumCells - 1; i > 0; --i)
    {
        int32 j = FMath::RandRange(0, i);
        AvailableCells.Swap(i, j);
    }

    for (int i = 0; i < NumObstacles; i++) {
        for (AGridCell* Cell : AvailableCells) {

            ObstacleCell.Add(Cell);
            Cell->avaiable = false;

            if (!IsConnected()) {
                Cell->avaiable = true;
                ObstacleCell.Remove(Cell);
            }
            else {
                AvailableCells.Remove(Cell);
                break;
            }
        }
    }
    //set delle celle con ostacoli
    return AvailableCells;
}

AGridCell* AGridManager::RandomAvaiable() {
    
    TArray<AGridCell*> IC;
    IC = AvCell;
    for (AGridCell* HC : AvCell) {
        if (HC->HasACharacter) {
            IC.Remove(HC);
        }
    }
    if (IC.Num() <= 0) {
        UE_LOG(LogTemp, Display, TEXT("no more room"));
        return map[0][0];
        //safe close function call
    }
    return IC[FMath::RandRange(0, IC.Num() - 1)];
}

TArray<AGridCell*> AGridManager::FindPath(AGridCell* Startcell, AGridCell* Endcell)
{
    TArray<AGridCell*> OpenSet;
    TSet<AGridCell*> ClosedSet;

    Startcell->GCost = 0;
    Startcell->HCost = FVector2D::Distance(FVector2D(Startcell->GetPosition().X, Startcell->GetPosition().Y), FVector2D(Endcell->GetPosition().X, Endcell->GetPosition().Y));
    Startcell->Parent = nullptr; // Assicura che il nodo iniziale non abbia un genitore
    OpenSet.Add(Startcell);

    while (OpenSet.Num() > 0)
    {
        // Ordina l'OpenSet per il costo F più basso
        OpenSet.Sort([](const AGridCell& A, const AGridCell& B) {
            return (A.GCost + A.HCost) < (B.GCost + B.HCost);
            });

        AGridCell* CurrentNode = OpenSet[0];
        OpenSet.RemoveAt(0);
        ClosedSet.Add(CurrentNode);

        // Se abbiamo raggiunto la destinazione, ricostruiamo il percorso
        if (CurrentNode == Endcell)
        {
            TArray<AGridCell*> Path;
            while (CurrentNode)
            {
                Path.Add(CurrentNode);
                CurrentNode = CurrentNode->Parent;
            }
            return Path;
        }

        // Esplora i vicini
        TArray<AGridCell*> Neighbors = GetNeighborCells(CurrentNode);

        for (AGridCell* Neighbor : Neighbors)
        {
            if (!Neighbor->avaiable || ClosedSet.Contains(Neighbor))
                continue;

            float NewGCost = CurrentNode->GCost + 1; // Costo di movimento standard

            if (!OpenSet.Contains(Neighbor) || NewGCost < Neighbor->GCost)
            {
                Neighbor->GCost = NewGCost;
                Neighbor->HCost = FVector2D::Distance(FVector2D(Neighbor->GetPosition().X, Neighbor->GetPosition().Y), FVector2D(Endcell->GetPosition().X, Endcell->GetPosition().Y));
                Neighbor->Parent = CurrentNode;

                if (!OpenSet.Contains(Neighbor))
                    OpenSet.Add(Neighbor);
            }
        }
    }

    return TArray<AGridCell*>(); // Nessun percorso trovato
}