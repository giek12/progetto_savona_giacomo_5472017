#include "MyGameModeBase.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Actor.h"
#include "Math/UnrealMathUtility.h"
#include "Blueprint/UserWidget.h"
#include "MyHUD.h"
#include "GameFramework/PlayerController.h"
#include "GridCell.h"
#include "GridManager.h"
#include "BaseCharacter.h"
#include "Sniperf.h"

AMyGameModeBase::AMyGameModeBase()
{
    HUDClass = AMyHUD::StaticClass();
}

void AMyGameModeBase::BeginPlay()
{
    Super::BeginPlay();

    SniperClass = ASniperf::StaticClass();
    BrawlerClass = ABrawlerf::StaticClass();
    //presa delle classi (sniper, brawler)

    APlayerController* PC = GetWorld()->GetFirstPlayerController();
    if (PC)
    {
        PC->GetPawn()->Destroy();
        //rimozione del defaul_pawn creato dal player controller

        PC->bShowMouseCursor = true;
        PC->bEnableMouseOverEvents = true;
        PC->bEnableClickEvents = true;
        if (PC->InputComponent)
        {
            PC->InputComponent->BindAction("LeftClick", IE_Pressed, this, &AMyGameModeBase::HandleLeftClick);
        }
        //configurazione per rilevare il click e il mouse_over
    }

    GridM = Cast<AGridManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AGridManager::StaticClass()));
    //presa del gridmanager
}

void AMyGameModeBase::StartPlacing() {
    //funzione per avviare la fase di piazzamento delle unità

    SetUpPhase = false;
    PlacePhase = true;

    bool PlayerFirst = PlaceTroup();
    IsPlayerTurn = PlayerFirst;
    if (!PlayerFirst) {
        InGameCharacter.Add(PlaceSniper(false, NULL));
    }
}

void AMyGameModeBase::HandleLeftClick()
{
    APlayerController* PC = GetWorld()->GetFirstPlayerController();
    if (!PC) return;

    FHitResult Hit;
    PC->GetHitResultUnderCursor(ECC_Visibility, false, Hit);

    //presa del player_contoller e rilevamento del click

    if (Hit.bBlockingHit)
    {
        AGridCell* ClickedCell = Cast<AGridCell>(Hit.GetActor());
        //si prende la cella clickata (se è stata clickata una cella)

        if (ClickedCell)
        {

            ClickedCell->OnCellSelected();
            //chiamata alla funione della cella clickata, che gestisce il click

            //comportamento nella placce_phase
            if (PlacePhase) {
                if (ClickedCell->avaiable && !ClickedCell->HasACharacter) {
                    if (InGameCharacter.Num() <2) {
                        InGameCharacter.Add(PlaceSniper(true, ClickedCell));
                        if (InGameCharacter.Num() < 2) {
                            InGameCharacter.Add(PlaceSniper(false, NULL));
                        }
                        else {
                            InGameCharacter.Add(PlaceBrawler(false, NULL));
                        }
                    }
                    else if (InGameCharacter.Num() < 4) {
                        InGameCharacter.Add(PlaceBrawler(true, ClickedCell));
                        if (InGameCharacter.Num() < 4) {
                            InGameCharacter.Add(PlaceBrawler(false, NULL));

                        }
                    }
                    //blocco di codice per il piazzamentoo delle varie unità

                    if (InGameCharacter.Num() > 3) {
                        PlacePhase = false;
                        PlayPhase = true;
                        UE_LOG(LogTemp, Display, TEXT("avvio playphase"));
                        if (!IsPlayerTurn) AiTurn();
                        return;
                    }
                    //se sono state piazzate tutte le unità si passa alla prossima fase
                }
            }
            else if (PlayPhase) {
                if (ClickedCell->avaiable && !ClickedCell->HasACharacter) {
                    if (ClickedCell->fs) {
                    //se la cella clickata è una cella all'interno di un array per il movimento di un personaggio
                        for (ABaseCharacter* c : InGameCharacter) {
                            if (c->Moving()) {
                                c->MoveTo(ClickedCell);
                            }
                        }
                    }
                }
            }
            //blocco di codice per far partire il movimento

        }
    }
}

bool AMyGameModeBase::PlaceTroup() {
    MyHUD = Cast<AMyHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
    int StartC = FMath::RandRange(0, 1);
    if (StartC == 0) {      //inizia la cpu
        MyHUD->SetHUDMessage(TEXT("turno dell' AI"));
        return false;
    }
    else {
        MyHUD->SetHUDMessage(TEXT("turno del PLAYER"));
        return true;
    }
    //scelta di chi inizia
}

ABaseCharacter* AMyGameModeBase::PlaceSniper(bool BTP, AGridCell* SelCell){
    MyHUD = Cast<AMyHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
    if (!BTP) {
        MyHUD->SetHUDLPAIS(20);
        SelCell= GridM->RandomAvaiable();
    }else MyHUD->SetHUDLPHPS(20);
    //aggiornamento dei punti vita nel widget

    SelCell->HasACharacter = true;
    FVector location = SelCell->GetPosition();
    FRotator rotation(0.0f, 0.0f, 0.0f);
    ASniperf* NewSniper = GetWorld()->SpawnActor<ASniperf>(SniperClass, location, rotation);
    NewSniper->BelongToPlayer = BTP;
    NewSniper->SetName();
    NewSniper->GridM = GridM;
    NewSniper->CurrentCell = SelCell;
    SelCell->character = NewSniper;
    if (!BTP) {
        UMaterialInterface* Mats = LoadObject<UMaterialInterface>(nullptr, TEXT("/Game/materiali/aisniperm"));
        UMaterialInterface* Mats1 = LoadObject<UMaterialInterface>(nullptr, TEXT("/Game/materiali/aisnipertar"));

        if (Mats)
        {
            NewSniper->MaterialBase = Mats;
            NewSniper->SniperMesh->SetMaterial(0, NewSniper->MaterialBase);
        }
        if (Mats1)
        {
            NewSniper->MaterialTarget = Mats1;
        }
    }
    return NewSniper;

    //set delle impostazioni per lo sniper
}

ABaseCharacter* AMyGameModeBase::PlaceBrawler(bool BTP, AGridCell* SelCell) {
    MyHUD = Cast<AMyHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
    if (!BTP) {
        MyHUD->SetHUDLPAIB(40);
        SelCell = GridM->RandomAvaiable();
    }else MyHUD->SetHUDLPHPB(40);
    //aggiornamento dei punti vita nel widget

    SelCell->HasACharacter = true;
    FVector location = SelCell->GetPosition();
    FRotator rotation(0.0f, 0.0f, 0.0f);
    ABrawlerf* NewBrawler = GetWorld()->SpawnActor<ABrawlerf>(BrawlerClass, location, rotation);
    NewBrawler->BelongToPlayer = BTP;
    NewBrawler->SetName();
    NewBrawler->GridM = GridM;
    NewBrawler->CurrentCell = SelCell;
    SelCell->character = NewBrawler;
    if (!BTP) {
        UMaterialInterface* Mats = LoadObject<UMaterialInterface>(nullptr, TEXT("/Game/materiali/aibrawlerm"));
        UMaterialInterface* Mats1 = LoadObject<UMaterialInterface>(nullptr, TEXT("/Game/materiali/aisnipertar.aibrawlertar"));

        if (Mats)
        {
            NewBrawler->MaterialBase = Mats;
            NewBrawler->BrawlerMesh->SetMaterial(0, NewBrawler->MaterialBase);
        }
        if (Mats1)
        {
            NewBrawler->MaterialTarget = Mats1;
        }
    }
    return NewBrawler;

    //set delle impostazioni per il brawler
}

void AMyGameModeBase::ResetOthers(ABaseCharacter* caller) {

    if (ActiveCharacter == caller) return;
    else if (ActiveCharacter == nullptr) ActiveCharacter = caller;
    else {
        ActiveCharacter->DeClick();
        ActiveCharacter = caller;
    }
    //reset ad idol del vecchio active_character e aggionamento con il nuovo active_character
}

AGridCell* AMyGameModeBase::FindOptimalCell(ABaseCharacter* chara) {

    int ClosestEnemy = 10000;    
    TArray<AGridCell*> Percorsof;
    for (ABaseCharacter* IGC : InGameCharacter) {
        if (IGC->BelongToPlayer) {
            TArray<AGridCell*> Percorsot = GridM->FindPath(chara->CurrentCell, IGC->CurrentCell);
            if (Percorsot.Num() < ClosestEnemy && Percorsot.Num() > 0) {
                Percorsof = Percorsot;
                ClosestEnemy = Percorsof.Num();
            }
        }
    }

    //scelta del nemico più vicino (in termini di percorso da effettuare, e non di distanza "in linea d'aria")
    //per farlo si conta il numero di step da fare per arrivare a ogni nemico e si sceglie quello più vicino
    //se nessuno dei nemici è raggiungibile il personaggio si sposta in una cella casuale raggiungibile

    if (ClosestEnemy == 10000) {    //non c'è nessun percorso attuabile => movimento a caso
        chara->GetReachableCells();
        TArray<AGridCell*> ReacheableCellsSet = chara->ReachableCells;
        return ReacheableCellsSet[FMath::RandRange(0,ReacheableCellsSet.Num()-1)];
    }

    Percorsof.RemoveAt(0);  //si rimuove la cella di destinazione (quella in cui c'è il character)
    Percorsof.RemoveAt(Percorsof.Num()-1);  //si rimuove la cella corrente del character

    if (Percorsof.Num() == 0) { //se si è già attaccati ad un nemico
        return chara->CurrentCell;
    }

    chara->GetReachableCells();
    TArray<AGridCell*> ReacheableCellsSet = chara->ReachableCells;

    for (int i = 0; i < Percorsof.Num() - 1; i++) {
        if (ReacheableCellsSet.Contains(Percorsof[i])) {
            return Percorsof[i];
        }
    }
    //si prende la cella più distante che sia raggiungibile e nel percorso per arrivare al nemico più vicino
    return ReacheableCellsSet[FMath::RandRange(0, ReacheableCellsSet.Num() - 1)];
}

void AMyGameModeBase::AiTurn() {

    TArray<ABaseCharacter*> CharactersCopy = InGameCharacter;

    for (ABaseCharacter* IAC : CharactersCopy) {
    //per ogni personaggio dell'ai si trova la cella ottimale del movimento e lo si sposta li (prima di attaccere si aspetta la fine del movimento)
        if (!IAC->BelongToPlayer) {
            IAC->MoveWait(FindOptimalCell(IAC), [IAC, this]() {
                IAC->GetEnemiesInRange();

                //una volta arrivato a destinazione il personaggio controlla se ci sono nemici attaccabili e in caso li attacca
                if (IAC->HittableEnemies.Num() > 0) {
                    int Dmg = IAC->DamageOutput();
                    ABaseCharacter* Target = IAC->HittableEnemies[0];

                    Target->Damage(Dmg);
                    GridM->PrintScrollBoxMessage(FText::FromString(FString::Printf(TEXT("%s %s %d"), *IAC->Nome, *Target->CurrentCell->GetName(), Dmg)));
                    UE_LOG(LogTemp, Display, TEXT("%s %s %d"), *IAC->Nome, *Target->CurrentCell->GetName(), Dmg);

                    if (Target->AttRange > 1) {  // è uno sniper
                        int CAD = FMath::RandRange(1, 3);
                        IAC->Damage(CAD);
                        GridM->PrintScrollBoxMessage(FText::FromString(FString::Printf(TEXT("%s %s %d"), *Target->Nome, *IAC->CurrentCell->GetName(), CAD)));
                        UE_LOG(LogTemp, Display, TEXT("%s %s %d"), *Target->Nome, *IAC->CurrentCell->GetName(), CAD);
                    }
                    //danno da contrattacco
                }
                IAC->Done = true;
                //si setta il personaggio dell'ai come "ha esaurito le mosse"
                int AIChara = 0;
                int AICharaDone = 0;
                TArray<ABaseCharacter*> CharactersCopy = InGameCharacter;
                for (ABaseCharacter* IAC1 : CharactersCopy) {
                    if (!IAC1->BelongToPlayer) {
                        AIChara++;
                        if (IAC1->Done) {
                            AICharaDone++;
                        }
                    }
                }
                //si controlla se tutti e 2 hanno finito le azioni
                if (AIChara == AICharaDone) PassTurn();
                });
        }
    }
}

void AMyGameModeBase::PassTurn() {
    if (NoBodyWin) {
        if (IsPlayerTurn) {
            IsPlayerTurn = !IsPlayerTurn;

            MyHUD->SetHUDMessage(TEXT("turno dell'AI"));

            for (ABaseCharacter* chara : InGameCharacter) {
                if (!chara->BelongToPlayer) {
                    chara->MoveToken = true;
                    chara->AttackToken = true;
                    chara->Done = false;
                }
            }
            //passaggio del turno e reset dei characters
            AiTurn();
        }
        else {
            for (ABaseCharacter* chara : InGameCharacter) {
                if (chara->BelongToPlayer) {
                    chara->MoveToken = true;
                    chara->AttackToken = true;
                    chara->Done = false;
                }
            }
            MyHUD->SetHUDMessage(TEXT("turno del PLAYER"));
            IsPlayerTurn = !IsPlayerTurn;
            //passaggio del turno e reset dei characters
        }
    }
}

void AMyGameModeBase::ShouldPass() {
    for (ABaseCharacter* chara : InGameCharacter) {
        if (chara->BelongToPlayer && !chara->Done) return;
    }
    PassTurn();
    //controllo se tutti i character del player hanno esaurito le mosse
}

void AMyGameModeBase::DidSomeOneWin() {
    int NumCharPlayer = 0;
    int NumCharAi = 0;
    for (ABaseCharacter* chara : InGameCharacter) {
        if (chara->BelongToPlayer) NumCharPlayer++;
        else NumCharAi++;
    }
    if (NumCharPlayer == 0) {
        MyHUD->SetHUDMessage(TEXT("sconfitta"));
        UE_LOG(LogTemp, Warning, TEXT("Partità finita con vittoria dell' AI"));
        NoBodyWin = false;
    }
    else if (NumCharAi == 0) {
        MyHUD->SetHUDMessage(TEXT("vittoria"));
        UE_LOG(LogTemp, Warning, TEXT("Partità finita con vittoria del Player"));
        NoBodyWin = false;
    }
    //controllo fine partita e stampa messaggio
}