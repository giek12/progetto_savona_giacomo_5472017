#include "BaseCharacter.h"
#include "GridCell.h"
#include "GridManager.h"
#include "MyHUD.h"
#include "MyGameModeBase.h"
#include "GameFramework/Controller.h"
#include "Kismet/GameplayStatics.h"
#include "Math/UnrealMathUtility.h"
#include "SfxActor.h"
#include "UObject/ConstructorHelpers.h"


//              CLASSE BASE DEIPERSONAGGI (BRAWLER, SNIPER)


ABaseCharacter::ABaseCharacter()
{
    PrimaryActorTick.bCanEverTick = true;

    static ConstructorHelpers::FObjectFinder<UMaterialInterface> Mats(TEXT("/Game/materiali/Movem"));
    //MATERIALE MESSO SULLE CELLE SU CUI CI SI PUò MUOVERE
    if (Mats.Succeeded())
    {
        Footm = Mats.Object;
    }

    static ConstructorHelpers::FObjectFinder<UMaterialInterface> Mats1(TEXT("/Game/materiali/Targetm"));
    if (Mats.Succeeded())
    {
        Targetm = Mats.Object;
    }
}

void ABaseCharacter::NotifyActorOnClicked(FKey ButtonPressed)       //RILEVA IL CLICK SU UN PERSONAGGIO
{
    AMyGameModeBase* GameMode = Cast<AMyGameModeBase>(GetWorld()->GetAuthGameMode());       //SI PRENDE LA GAMEMODE
    if (GameMode)
    {
        if (IsATarget) {
        //SE IL CLICK AVVIENE SU UN PERSONAGGIO CHE è ATTACCABILE DAL PERSONAGGIO ATTIVO (IL PERSONAGGIO ATTIVO è IL PERSONAGGIO CONTROLLATO DAL PLAYER CHE STA EFFETTUANDO UN AZIONE)
            
            if (GameMode->ActiveCharacter->AttackToken) {
            //SE IL PERSONAGGIO ATTIVO NON HA ANCORA ATTACCATO
                
                int DmgInput = GameMode->ActiveCharacter->DamageOutput();
                //SI CALCOLA IL DANNO CHE IL PERSONAGGIO ATTIVO INFLIGGE
                
                Damage(DmgInput);
                //SI ASSEGNA IL DANNO
                
                GridM->PrintScrollBoxMessage(FText::FromString(FString::Printf(TEXT("%s %s %d"), *GameMode->ActiveCharacter->Nome, *CurrentCell->GetName(), DmgInput)));
                //SI STAMPA NELLA SCROLLBOX IL LOG DELL'ATTACCO
                
                if (GameMode->ActiveCharacter->AttRange > 1) {
                //allora è uno sniper e bisogna infligere i danni da contrattacco    
                    
                    if (AttRange > 1) {
                    //SE IL PERSONAGGIO ATTACCATO HA ATTRANGE > 1 è UNO SNIPER, E NON HA BISOGNO DI CALCOLARE SE LO SNIPER CHE LO HA ATTACCATO è IN RANGE PER IL CONTRATTACCO
                        
                        int CAD = FMath::RandRange(1, 3);
                        //SI CALCOLA IL DANNO DA CONTRATTACCO
                        
                        GameMode->ActiveCharacter->Damage(CAD);
                        //SI INFLIGGE IL DANNO DA CONTRATTACCO

                        GridM->PrintScrollBoxMessage(FText::FromString(FString::Printf(TEXT("%s %s %d"), *Nome, *GameMode->ActiveCharacter->CurrentCell->GetName(), CAD)));
                        //SI STAMPA NELLA SCROLLBOX IL LOG DEL CONTRATTACCO
                    }
                    else {
                    //ALTRIMENTI LO SNIPER HA APPENA ATTACCATO UN BRAWLER, E IL DANNO DA CONTRATTACCO DEVE ESSERE INFLITTO SOLO SE è IN RANGE
                        
                        for (AGridCell* bc : GridM->GetNeighborCells(CurrentCell)) {
                        //SI CONSIDERA OGNI CELLA VICINO AL BRAWLER    
                            
                            if (bc->HasACharacter) {
                            //SE LA CELLA VICINA PRESA IN CONSIDERAZIONE HA UN CHARACTER
                                
                                if (bc->character == GameMode->ActiveCharacter) {
                                //E QUEL CHARACTER è L'ATTACCANTE ALLORA DOBBIAMO EFFETTUARE IL CONTRATTACCO

                                    int CAD1 = FMath::RandRange(1, 3);
                                    //SI CALCOLA IL DANNO DA CONTRATTACO

                                    GameMode->ActiveCharacter->Damage(CAD1);
                                    //SI INFLIGGE IL DANNO DA CONTRATTACCO

                                    GridM->PrintScrollBoxMessage(FText::FromString(FString::Printf(TEXT("%s %s %d"), *Nome, *GameMode->ActiveCharacter->CurrentCell->GetName(), CAD1)));
                                    //SI STAMPA NELLA SCROLLBOX IL LOG DEL CONTRATTACCO
                                }
                            }
                        }
                    }
                }
                GameMode->ActiveCharacter->AttackToken = false;
                //SI DISATTIVA LA POSSIBILITà DI ATTACCARE (IN QUESTO TURNO) AL CHARACTER ATTIVO

                GameMode->ActiveCharacter->Done = true;
                //SI IMPOSTA IL CHARACTER ATTIVO COME PRONTO PER PASSARE (NON PUò PIù FARE AZIONI)
                
                GameMode->ShouldPass();
                //SI CONTROLLA SE TUTTI I CHARACTER APPARTENENTI AL PLAYER HANNO FINITO LE AZIONI
                
                for (ABaseCharacter* bc : GameMode->ActiveCharacter->HittableEnemies) {
                //SI RESETTA IL MATERIALE DEI BERSAGLI (QUANDO GLI SI TOGLIE IL MIRINO DA SOPRA)

                    bc->ResetTarget();
                    //EFFETTIVO RESET DEL MATERIALE

                    bc->IsATarget = false;
                    //SI IMPOSTA IL CHARACTER CHE ERA UN BERSAGLIO COME NON PIù BERSAGLIO
                }
                return;
            }
            else {
                UE_LOG(LogTemp, Warning, TEXT("attacco già effettuato"));
                return;
            }
        }
        else {
            if (!BelongToPlayer) {
            //SI STA CERCANDO DI INTERAGIRE CON UN PERSONAGIO CHE NON APPARTIENE AL PLAYER (PER FARE AZIONI)

                UE_LOG(LogTemp, Warning, TEXT("questo personagio non ti appartiene"));
                return;
            }
            else if(!GameMode->IsPlayerTurn){
                //SI STA CERCANDO DI INTERAGIRE CON UN PERSONAGIO CHE APPARTIENE AL PLAYER (PER FARE AZIONI) MA NON è IL TURNO DEL PLAYER

                UE_LOG(LogTemp, Warning, TEXT("non e' il tuo turno"));
                return;
            }
            GameMode->ResetOthers(this);
            //SI RESETTANO GLI ALTRI CHARACTER (SI PONE ATTIVO QUESTO CARACTER PER EFFETTUARE UN' AZIONE) E IL VECCHIO CHARACTER ATTIVO TORNA IDOL
        }
    }
    Super::NotifyActorOnClicked(ButtonPressed);
    ClickCounter += 1;
    //SI AUMENTA IL CLICKCOUNTER CHE SERVE A CAPIRE CHE AZIONE SI VUOLE EFFETTUARE

    ClickCounter = ClickCounter % 3;
    if (ClickCounter == 0) {
    //ALLORA SIAMO IN IDOL

        for (ABaseCharacter* bc : HittableEnemies) {
        //SI RESETTANO TUTTI I BERSAGLI (DATO CHE PRIMA IL CHARACTER ERA PRONTO PER ATTACCARE)
            
            if (bc) {
                bc->ResetTarget();
                bc->IsATarget = false;
            }
        }
    }
    else if (ClickCounter == 1) {
    //ALLORA SIAMO NELLA SELEZIONE DELLA CELLA IN CUI MUOVERCI

        GetReachableCells();
        //SI PRENDONO LE CELLE RAGGIUNGIBILI (LA FUNZIONE RIEMPIE IL TARRAY "ReachableCells")

        for (AGridCell* cell : ReachableCells) {
        //SI CAMBIA IL MATERIALE DELLE CELLE RAGGIUNGIBILI PER INDICARE CHE CI SI PUò SPOSTARE IN QUELLA CELLA

            cell->SetMaterial(Footm);
            cell->fs = true;
            //SI IMPOSTA LA CELLA COME CELLA CON IL MATERIALE PER LO SPOSTAMENTO
        }
    }
    else {
    //SIAMO NELLA SELEZIONE DEL PERSONAGIO DA ATTACCARE

        GetEnemiesInRange();
        //SI PRENDONO I PERSONAGGI ATTACCABILI (LA FUNZIONE RIEMPIE IL TARRAY "HittableEnemies")

        for (AGridCell* cell : ReachableCells) {
            cell->SetMaterial(cell->MaterialBase);
            cell->fs = false;
        }
        for (ABaseCharacter* bc : HittableEnemies) {
            if (bc) {
                bc->SetTarget();
                bc->IsATarget = true;
            }
        }
    }
}

void ABaseCharacter::BeginPlay()
{
    Super::BeginPlay();

}

void ABaseCharacter::Tick(float DeltaTime)
{

    //IL CODICE IN TICK SI OCCUPA DI EFFETTUARE LO SPOSTAMENTO DEL CHARACTER IN UNA CELLA RAGGIUNGIBILE
    //("STEPS" è IL VETTORE CHE CONTIENE TUTTE LE CELLE CHE COMPONGONO IL PERCORSO DA EFFETTUARE) (RIEMPITO DALL'ALGORITMO A*)

    Super::Tick(DeltaTime);
    if (!MoveToken && Steps.Num() > 0) {

        CurrentPos = FMath::VInterpConstantTo(CurrentPos, Steps[0]->GetPosition(), DeltaTime, 200);
        SetActorLocation(CurrentPos);
        //MOVIMENTO FRAME PER FRAME

        if (FVector::Distance(CurrentPos, Steps[0]->GetPosition()) < 4) Steps.RemoveAt(0);
        //SE SI è RAGGIUNTA LA CELLA LA SI RIMUOVE

    }
    if (!MoveToken && Steps.Num() == 0 && MoveCompletedCallback)
    {
        MoveCompletedCallback();
        MoveCompletedCallback = nullptr;
        //RESETTIAMO IL CALLBACK
    }

}

FVector ABaseCharacter::GetCellWorldPosition(AGridCell* TargetCell)
{
    //QUESTA FUNZIONE SI OCCUPA DI PRENDERE LA POSIZIONE NEL MONDO DI GIOCO DI UNA CELLA (SERVE PER LO SPOSTAMENTO)

    if (TargetCell)
    {
        return TargetCell->GetActorLocation();
    }
    return FVector::ZeroVector;
}

int ABaseCharacter::DamageOutput() {
    //FUNZIONE PER CALCOLARE IL DANNO CHE IL CHARACTER INFLIGGE

    if (AttRange > 1) {
        Cast<ASfxActor>(UGameplayStatics::GetActorOfClass(GetWorld(), ASfxActor::StaticClass()))->PlayShot();
    }else Cast<ASfxActor>(UGameplayStatics::GetActorOfClass(GetWorld(), ASfxActor::StaticClass()))->PlayPunch();
    //BLOCCO DI CODICE PER FAR PARTIRE IL SUONO DELL'ATTACCO

    return FMath::RandRange(MinDamage, MaxDamage);
}

void ABaseCharacter::Damage(int Dmg) {
    Hp = Hp - Dmg;
    //SI DIMINUISCONO I PUNTI VITA

    if (AttRange > 1) {
    //è UNO SNIPER
        if (BelongToPlayer) {
            Cast<AMyHUD>(GetWorld()->GetFirstPlayerController()->GetHUD())->SetHUDLPHPS(Hp);
        }
        else Cast<AMyHUD>(GetWorld()->GetFirstPlayerController()->GetHUD())->SetHUDLPAIS(Hp);
        //SI AGGIORNANO I PUNTI VITA A SCHERMO

    }
    else
    {
    //è UN BRAWLER
        if (BelongToPlayer) {
            Cast<AMyHUD>(GetWorld()->GetFirstPlayerController()->GetHUD())->SetHUDLPHPB(Hp);
        }
        else Cast<AMyHUD>(GetWorld()->GetFirstPlayerController()->GetHUD())->SetHUDLPAIB(Hp);
        //SI AGGIORNANO I PUNTI VITA A SCHERMO

    }

    if (Hp <= 0) {
    //è MORTO 
        AMyGameModeBase* GameMode = Cast<AMyGameModeBase>(GetWorld()->GetAuthGameMode());
        if (GameMode) {
            GameMode->InGameCharacter.Remove(this);
            GameMode->ActiveCharacter->HittableEnemies.Remove(this);
            //SI RIMUOVE IL PERSONAGGIO DALL'ARRAY DEI PERSONAGGI IN GIOCO

            CurrentCell->HasACharacter = false;
            CurrentCell->character = nullptr;
            //SI RESETTANO LE "IMPOSTAZIONI" DELLE CELLE

            GameMode->ActiveCharacter->DeClick();
            //SI RESETTA IL PERSONAGGI ATTIVO (TORNA IDOL)

            GameMode->DidSomeOneWin();
            //SI CONTROLLA SE QUALCUNO HA VINTO

            Cast<ASfxActor>(UGameplayStatics::GetActorOfClass(GetWorld(), ASfxActor::StaticClass()))->PlayDeath();
            Destroy();
        }
    }
}

void ABaseCharacter::GetReachableCells(){
    ReachableCells.Reset();
    //SI SVUOTA IL VETTORE DELLE CELLE RAGGIUNGIBILI

    TQueue<AGridCell*> OpenSet;        
    // CODA PER LA BFS

    TMap<AGridCell*, int32> Distance;  
    // TIENE TRACCIA DELLA DISTANZ DALLA CELLA INIZIALE

    if (!CurrentCell) {
        UE_LOG(LogTemp, Warning, TEXT("cella di appartenenza non trovata"));
        return;
    }
    if (!GridM) return;

    OpenSet.Enqueue(CurrentCell);
    Distance.Add(CurrentCell, 0);
    ReachableCells.Add(CurrentCell);
    //PARTENZA DELLA BFS

    while (!OpenSet.IsEmpty())
    {
        AGridCell* CCell;
        OpenSet.Dequeue(CCell);
        //SI ESTRAE LA CELLA IN CODA

        int32 CurrentDistance = Distance[CCell];
        //SI PRENDE LA DISTANZA

        if (CurrentDistance >= MaxMove) continue; 
        //SE SUPERA IL RAGGIO DI MOVIMENTO IGNIORIAMO LA CELLA

        TArray<AGridCell*> Neighbors = GridM->GetNeighborCells(CCell);
        //PRENDIAMO LE CELLE VICINE

        for (AGridCell* Neighbor : Neighbors)
        {
            if (!Neighbor || !Neighbor->avaiable || Neighbor->HasACharacter) continue;
            //SALTIAMO LE CELLE NON RAGGIUNGIBILI

            if (!Distance.Contains(Neighbor))
            //SE NON è ANCORA STATA VISITATA
            {
                Distance.Add(Neighbor, CurrentDistance + 1);
                OpenSet.Enqueue(Neighbor);
                ReachableCells.Add(Neighbor);
            }
        }
    }
}

void ABaseCharacter::GetEnemiesInRange() {
    //BFS (COME REACHABLECELL)
    HittableEnemies.Reset();
    TQueue<AGridCell*> OpenSet;
    TMap<AGridCell*, int32> Distance;

    if (!CurrentCell) {
        UE_LOG(LogTemp, Display, TEXT("cella di appartenenza non trovata"));
        return;
    }
    if (!GridM) return;

    OpenSet.Enqueue(CurrentCell);
    Distance.Add(CurrentCell, 0);
    ReachableCells.Add(CurrentCell);

    while (!OpenSet.IsEmpty())
    {
        AGridCell* CCell;
        OpenSet.Dequeue(CCell);

        int32 CurrentDistance = Distance[CCell];

        if (CurrentDistance >= AttRange) continue;

        TArray<AGridCell*> Neighbors = GridM->GetNeighborCells(CCell);

        for (AGridCell* Neighbor : Neighbors)
        {
            if (!Neighbor) continue;

            if (!Distance.Contains(Neighbor))
            {
                Distance.Add(Neighbor, CurrentDistance + 1);
                OpenSet.Enqueue(Neighbor);
                if (Neighbor->HasACharacter) {
                    if (Neighbor->character->BelongToPlayer != BelongToPlayer) {
                        HittableEnemies.Add(Neighbor->character);
                    }
                }
            }
        }
    }
}

void ABaseCharacter::DeClick() {
    ClickCounter = 0;
    //VALORE IDOL

    for (AGridCell* cell : ReachableCells) {
        cell->SetMaterial(cell->MaterialBase);
        cell->fs = false;
    }
    for (ABaseCharacter* bc : HittableEnemies) {
        if (bc) {
            bc->ResetTarget();
            bc->IsATarget = false;
        }
    }
    //RESET DELLE CELLE E DEI BERSAGLI

    HittableEnemies.Reset();
}

bool ABaseCharacter::Moving() {
    if (ClickCounter == 1) return true;
    else return false;
}

void ABaseCharacter::MoveTo(AGridCell* cell) {
    MoveWait(cell, [this]() {
        CanAttack();
        if (Done) {
            Cast<AMyGameModeBase>(GetWorld()->GetAuthGameMode())->ShouldPass();
        }
    });
    //FUNZIONE CHE CHIAMA L'EFFETTIVA FUNZIONE DI MOVIMENTO E ALLA FINE CONTROLLA SE è POSSIBILE EFFETTUARE UN ATTACCO
    //IN CASO NON SI POSSA EFFETTUARE UN ATTACO ALLA FINE DEL MOVIMENTO IL PERSONAGGIO VIENE IMPOSTATO COME DONE (AZIONI FINITE)
}

void ABaseCharacter::SetTarget() {
    //FUNZIONE CHE APPLICA IL MATERIALE CON IL MIRINO AL PERSONAGGIO
    UE_LOG(LogTemp, Warning, TEXT("chiamata in classe base"));
}

void ABaseCharacter::ResetTarget() {
    //SI RESETTA IL MATERIALE
    UE_LOG(LogTemp, Warning, TEXT("chiamata in classe base"));
}

void ABaseCharacter::SetName() {
    //FUNZIONE PER IL RENAME DEL PERSONAGGIO
    UE_LOG(LogTemp, Warning, TEXT("chiamata in classe base"));
}

void ABaseCharacter::CanAttack() {
    GetEnemiesInRange();
    if (HittableEnemies.Num() == 0) {
        Done = true;
    }
    //CONTROLLO PER VEDERE SE è POSSIBILE EFFETTUARE UN ATTACCO
}

void ABaseCharacter::MoveWait(AGridCell* cell, TFunction<void()> OnMoveCompleted) {
    if (MoveToken && !Done) {
    //SE è POSSIBILE EFFETTUARE UN MOVIMENTO

        CurrentCell->HasACharacter = false;
        CurrentCell->character = nullptr;
        //SI RESETTANO LE "IMPOSTAZIONI" DELLA CELLA DI PERTENZA

        Steps = GridM->FindPath(CurrentCell, cell);
        //SI TROVA IL PERCORSO CON L'ALGORITMO A*

        TArray<AGridCell*> Temp = Steps;
        for (int i = 0; i < Steps.Num(); i++) {
            Steps[i] = Temp[Steps.Num() - (i + 1)];
        }
        //SI RIBALTA L'ARRAY (L'ALGORITMO A* RESTITUISCE I PASSI AL CONTRARIO (DALLA DESTINAZIONE ALLA PARTENZA))

        Steps.RemoveAt(0);
        //SI RIMUOVE LA CELLA DI PARTENZA

        DeClick();
        //SI FA TORNARE AD IDOL IL PERSONAGGIO CHE SI STA MUOVENDO

        GridM->PrintScrollBoxMessage(FText::FromString(FString::Printf(TEXT("%s %s -> %s"), *Nome, *CurrentCell->GetName(), *cell->GetName())));
        //SI STAMPA IL LOG RELATIVO AL MOVIMENTO
        
        CurrentPos = CurrentCell->GetPosition();
        //SI PRENDE LA POSIZIONE DELLA CELLA DI PARTENZA

        CurrentCell = cell;
        CurrentCell->HasACharacter = true;
        CurrentCell->character = this;
        //MODIFICA ALLE "IMPOSTAZIONI" DELLA NUOVA CELLA
        
        MoveToken = false;
        //SI FA PARTIRE LO SPOSTAMENTO

        MoveCompletedCallback = OnMoveCompleted;
        //CALLBACK
    }
    else UE_LOG(LogTemp, Warning, TEXT("hai già mosso questo character"));
}